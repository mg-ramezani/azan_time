#include "prayes_calculator.h"

#include <time.h>

AzanTime::AzanTime(AzanTime::CalculationMethod __calc_method, AzanTime::JuristicMethod __asr_juristic, AzanTime::AdjustingMethod __adjust_high_lats, double __dhuhr_minutes)
    : calc_method(__calc_method)
    , asr_juristic(__asr_juristic)
    , adjust_high_lats(__adjust_high_lats)
    , dhuhr_minutes(__dhuhr_minutes)
    , latitude(double())
    , longitude(double())
    , timezone(double())
    , julian_date(double())
{
    method_params[Jafari] = MethodConfig(16.0, false, 4.0, false, 14.0); // Jafari
    method_params[Karachi] = MethodConfig(18.0, true, 0.0, false, 18.0); // Karachi
    method_params[ISNA] = MethodConfig(15.0, true, 0.0, false, 15.0); // ISNA
    method_params[MWL] = MethodConfig(18.0, true, 0.0, false, 17.0); // MWL
    method_params[Makkah] = MethodConfig(19.0, true, 0.0, true, 90.0); // Makkah
    method_params[Egypt] = MethodConfig(19.5, true, 0.0, false, 17.5); // Egypt
    method_params[Custom] = MethodConfig(18.0, true, 0.0, false, 17.0); // Custom
}

void AzanTime::get_prayer_times_t(unsigned year, unsigned month, unsigned day, double lat, double lon, double timez, double t[])
{
    this->latitude = lat;
    this->longitude = lon;
    this->timezone = timez;
    this->julian_date = get_julian_date(year, month, day) -
                        this->longitude / static_cast<double>(15 * 24);
    compute_day_times(t);
}

void AzanTime::get_prayer_times(time_t date, const double &latitude, const double &longitude, const double &timezone, double times[])
{
    tm *t = localtime(&date);
    get_prayer_times_t(static_cast<unsigned>(1900 + t->tm_year), static_cast<unsigned>(t->tm_mon + 1), static_cast<unsigned>(t->tm_mday), latitude, longitude, timezone, times);
}

void AzanTime::set_calc_method(const AzanTime::CalculationMethod &method_id)
{
    calc_method = method_id;
}

void AzanTime::set_asr_method(const AzanTime::JuristicMethod &method_id)
{
    asr_juristic = method_id;
}

void AzanTime::set_high_lats_adjust_method(const AzanTime::AdjustingMethod &method_id)
{
    adjust_high_lats = method_id;
}

void AzanTime::set_fajr_angle(const double &angle)
{
    method_params[Custom].fajr_angle = angle;
    calc_method = Custom;
}

void AzanTime::set_maghrib_angle(const double &angle)
{
    method_params[Custom].maghrib_is_minutes = false;
    method_params[Custom].maghrib_value = angle;
    calc_method = Custom;
}

void AzanTime::set_isha_angle(const double &angle)
{
    method_params[Custom].isha_is_minutes = false;
    method_params[Custom].isha_value = angle;
    calc_method = Custom;
}

void AzanTime::set_dhuhr_minutes(const double &minutes)
{
    dhuhr_minutes = minutes;
}

void AzanTime::set_maghrib_minutes(const double &minutes)
{
    method_params[Custom].maghrib_is_minutes = true;
    method_params[Custom].maghrib_value = minutes;
    calc_method = Custom;
}

void AzanTime::set_isha_minutes(const double &minutes)
{
    method_params[Custom].isha_is_minutes = true;
    method_params[Custom].isha_value = minutes;
    calc_method = Custom;
}

void AzanTime::get_float_time_parts(double time, int &hours, int &minutes)
{
    time = fix_hour(time + 0.5 / 60); // add 0.5 minutes to round
    hours = static_cast<int>(floor(time));
    minutes = static_cast<int>(floor((time - hours) * 60));
}

std::string AzanTime::float_time_to_time24(double time)
{
    if (std::isnan(time))
        return std::string();
    int hours, minutes;
    get_float_time_parts(time, hours, minutes);
    return two_digits_format(hours) + ':' +
         two_digits_format(minutes);
}

std::string AzanTime::float_time_to_time12(double time, bool no_suffix)
{
    if (std::isnan(time))
        return std::string();
    int hours, minutes;
    get_float_time_parts(time, hours, minutes);
    const char *suffix = hours >= 12 ? " PM" : " AM";
    hours = (hours + 12 - 1) % 12 + 1;
    return int_to_string(hours) + ':' +
         two_digits_format(minutes) + (no_suffix ? "" : suffix);
}

std::string AzanTime::float_time_to_time12ns(double time)
{
    return float_time_to_time12(time, true);
}

double AzanTime::get_effective_timezone(time_t local_time)
{
    tm *tmp = localtime(&local_time);
    tmp->tm_isdst = 0;
    time_t local = mktime(tmp);
    tmp = gmtime(&local_time);
    tmp->tm_isdst = 0;
    time_t gmt = mktime(tmp);
    return (local - gmt) / 3600.0;
}

double AzanTime::get_effective_timezone(const int &year, const int &month, const int &day)
{
    tm date{};
    date.tm_year = year - 1900;
    date.tm_mon = month - 1;
    date.tm_mday = day;
    date.tm_isdst = -1; // determine it yourself from system
    time_t local = mktime(&date); // seconds since midnight Jan 1, 1970
    return get_effective_timezone(local);
}

AzanTime::DoublePair AzanTime::sun_position(double jd)
{
    const double d = jd - 2451545.0;
    const double g = fix_angle(357.529 + 0.98560028 * d);
    const double q = fix_angle(280.459 + 0.98564736 * d);
    const double l =
         fix_angle(q + 1.915 * dsin(g) + 0.020 * dsin(2 * g));

    // double r = 1.00014 - 0.01671 * dcos(g) - 0.00014 * dcos(2 * g);
    const double e = 23.439 - 0.00000036 * d;

    const double dd = darcsin(dsin(e) * dsin(l));
    double ra = darctan2(dcos(e) * dsin(l), dcos(l)) / 15.0;
    ra = fix_hour(ra);
    const double eq_t = q / 15.0 - ra;

    return DoublePair(dd, eq_t);
}

double AzanTime::equation_of_time(const double &jd)
{
    return sun_position(jd).second;
}

double AzanTime::sun_declination(const double &jd)
{
    return sun_position(jd).first;
}

double AzanTime::compute_mid_day(const double &_t)
{
    double t = equation_of_time(julian_date + _t);
    return fix_hour(12 - t);
}

double AzanTime::compute_time(const double &g, const double &t)
{
    double d = sun_declination(julian_date + t);
    double z = compute_mid_day(t);
    double v = 1.0 / 15.0 *
         darccos((-dsin(g) - dsin(d) * dsin(latitude)) /
              (dcos(d) * dcos(latitude)));
    return (z + (g > 90.0 ? -v : v));
}

double AzanTime::compute_asr(const int &step, const double &t) // Shafii: step=1, Hanafi: step=2
{
    double d = sun_declination(julian_date + t);
    double g = -darccot(step + dtan(fabs(latitude - d)));
    return compute_time(g, t);
}

void AzanTime::compute_times(double times[])
{
    day_portion(times);

    times[Fajr] =
         compute_time(180.0 - method_params[calc_method].fajr_angle,
              times[Fajr]);
    times[Sunrise] = compute_time(180.0 - 0.833, times[Sunrise]);
    times[Dhuhr] = compute_mid_day(times[Dhuhr]);
    times[Asr] = compute_asr(1 + asr_juristic, times[Asr]);
    times[Sunset] = compute_time(0.833, times[Sunset]);
    times[Maghrib] =
         compute_time(method_params[calc_method].maghrib_value,
              times[Maghrib]);
    times[Isha] =
         compute_time(method_params[calc_method].isha_value,
              times[Isha]);
}

void AzanTime::compute_day_times(double times[])
{
    constexpr double default_times[] = {5, 6, 12, 13, 18, 18, 18}; // default times
    for (int i = 0; i < TimesCount; ++i)
        times[i] = default_times[i];

    for (int i = 0; i < NUM_ITERATIONS; ++i)
        compute_times(times);

    adjust_times(times);
}

void AzanTime::adjust_times(double times[])
{
    for (int i = 0; i < TimesCount; ++i)
        times[i] += timezone - longitude / 15.0;
    times[Dhuhr] += dhuhr_minutes / 60.0; // Dhuhr
    if (method_params[calc_method].maghrib_is_minutes) // Maghrib
        times[Maghrib] = times[Sunset] +
             method_params[calc_method].maghrib_value / 60.0;

    if (method_params[calc_method].isha_is_minutes) // Isha
        times[Isha] = times[Maghrib] +
             method_params[calc_method].isha_value / 60.0;

    if (adjust_high_lats != None)
        adjust_high_lat_times(times);
}

void AzanTime::adjust_high_lat_times(double times[])
{
    double night_time = time_diff(times[Sunset], times[Sunrise]); // sunset to sunrise

    // Adjust Fajr
    const double fajr_diff =
         night_portion(method_params[calc_method].fajr_angle) *
         night_time;

    if (std::isnan(times[Fajr]) ||
         time_diff(times[Fajr], times[Sunrise]) > fajr_diff)
        times[Fajr] = times[Sunrise] - fajr_diff;

    // Adjust Isha
    const double isha_angle =
         method_params[calc_method].isha_is_minutes ? 18.0 :
                                                      method_params[calc_method].isha_value;
    const double isha_diff = night_portion(isha_angle) * night_time;
    if (std::isnan(times[Isha]) ||
         time_diff(times[Sunset], times[Isha]) > isha_diff)
        times[Isha] = times[Sunset] + isha_diff;

    // Adjust Maghrib
    const double maghrib_angle =
         method_params[calc_method].maghrib_is_minutes ? 4.0 :
                                                         method_params[calc_method].maghrib_value;
    const double maghrib_diff =
         night_portion(maghrib_angle) * night_time;
    if (std::isnan(times[Maghrib]) ||
         time_diff(times[Sunset], times[Maghrib]) > maghrib_diff)
        times[Maghrib] = times[Sunset] + maghrib_diff;
}

double AzanTime::night_portion(double angle)
{
    switch (adjust_high_lats)
    {
        case AngleBased:
            return angle / 60.0;
        case MidNight:
            return 1.0 / 2.0;
        case OneSeventh:
            return 1.0 / 7.0;
        default:
            // Just to return something!
            // In original library nothing was returned
            // Maybe I should throw an exception
            // It must be impossible to reach here
            return 0;
    }
}

void AzanTime::day_portion(double times[])
{
    for (int i = 0; i < TimesCount; ++i)
        times[i] /= 24.0;
}

double AzanTime::time_diff(const double &time1, const double &time2)
{
    return fix_hour(time2 - time1);
}

std::string AzanTime::int_to_string(const int &num)
{
    char tmp[16];
    tmp[0] = '\0';
    sprintf(tmp, "%d", num);
    return std::string(tmp);
}

std::string AzanTime::two_digits_format(const int &num)
{
    char tmp[16];
    tmp[0] = '\0';
    sprintf(tmp, "%2.2d", num);
    return std::string(tmp);
}

double AzanTime::get_julian_date(unsigned &year, unsigned &month, unsigned &day)
{
    if (month <= 2)
    {
        year -= 1;
        month += 12;
    }

    double a = floor(year / 100.0);
    double b = 2 - a + floor(a / 4.0);

    return (floor(365.25 * (year + 4716)) +
         floor(30.6001 * (month + 1)) + day + b - 1524.5);
}

double AzanTime::calc_julian_date(const int &year, const int &month, const int &day)
{
    constexpr double j1970 = 2440588.0;
    tm date{};

    date.tm_year = year - 1900;
    date.tm_mon = month - 1;
    date.tm_mday = day;
    date.tm_isdst = -1; // determine it yourself from system

    time_t ms = mktime(&date); // seconds since midnight Jan 1, 1970
    double days = floor(ms / static_cast<double>(60 * 60 * 24));
    return j1970 + days - 0.5;
}

double AzanTime::dsin(const double &d)
{
    return sin(deg2rad(d));
}

double AzanTime::dcos(const double &d)
{
    return cos(deg2rad(d));
}

double AzanTime::dtan(const double &d)
{
    return tan(deg2rad(d));
}

double AzanTime::darcsin(const double &x)
{
    return rad2deg(asin(x));
}

double AzanTime::darccos(const double &x)
{
    return rad2deg(acos(x));
}

double AzanTime::darctan(const double &x)
{
    return rad2deg(atan(x));
}

double AzanTime::darctan2(const double &y, const double &x)
{
    return rad2deg(atan2(y, x));
}

double AzanTime::darccot(const double &x)
{
    return rad2deg(atan(1.0 / x));
}

constexpr double AzanTime::deg2rad(const double &d)
{
    return d * M_PI / 180.0;
}

constexpr double AzanTime::rad2deg(const double &r)
{
    return r * 180.0 / M_PI;
}

double AzanTime::fix_angle(double a)
{
    a = a - 360.0 * floor(a / 360.0);
    a = a < 0.0 ? a + 360.0 : a;
    return a;
}

double AzanTime::fix_hour(double a)
{
    a = a - 24.0 * floor(a / 24.0);
    a = a < 0.0 ? a + 24.0 : a;
    return a;
}

AzanTime::MethodConfig::MethodConfig(double fajr_angle, bool maghrib_is_minutes, double maghrib_value, bool isha_is_minutes, double isha_value)

    : fajr_angle(fajr_angle)
    , maghrib_value(maghrib_value)
    , isha_value(isha_value)
    , maghrib_is_minutes(maghrib_is_minutes)
    , isha_is_minutes(isha_is_minutes)
{
}
