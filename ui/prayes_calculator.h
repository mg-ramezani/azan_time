#ifndef PRAYES_TIME_CALCULATOR
#define PRAYES_TIME_CALCULATOR

#include <cstdio>
#include <cmath>
#include <string>

/* -------------------- AzanTime Class --------------------- */

class AzanTime
{
public:
	enum
	{
		VERSION_MAJOR = 1,
		VERSION_MINOR = 0,
	};

	// Calculation Methods
	enum CalculationMethod
	{
        Jafari, // Ithna Ashari
        Karachi, // University of Islamic Sciences, Karachi
        ISNA, // Islamic Society of North America (ISNA)
        MWL, // Muslim World League (MWL)
        Makkah, // Umm al-Qura, Makkah
        Egypt, // Egyptian General Authority of Survey
        Custom, // Custom Setting

		CalculationMethodsCount
	};

	// Juristic Methods
	enum JuristicMethod
	{
        Shafii, // Shafii (standard)
        Hanafi, // Hanafi
	};

	// Adjusting Methods for Higher Latitudes
	enum AdjustingMethod
	{
        None, // No adjustment
        MidNight, // middle of night
        OneSeventh, // 1/7th of night
        AngleBased, // angle/60th of night
	};

	// Time IDs
	enum TimeID
	{
		Fajr,
		Sunrise,
		Dhuhr,
		Asr,
		Sunset,
		Maghrib,
		Isha,

		TimesCount
	};

    /* -------------------- Interface Functions -------------------- */

    AzanTime(CalculationMethod __calc_method = Jafari,
         JuristicMethod __asr_juristic = Shafii,
         AdjustingMethod __adjust_high_lats = MidNight,
         double __dhuhr_minutes = double());

    /* return prayer times for a given date */
    void get_prayer_times(unsigned year, unsigned month, unsigned day, double _latitude, double _longitude, double _timezone, double times[]);

	/* return prayer times for a given date */
    void get_prayer_times(time_t date, const double& latitude, const double& longitude, const double& timezone, double times[]);

	/* set the calculation method  */
    void set_calc_method(const CalculationMethod& method_id);

	/* set the juristic method for Asr */
    void set_asr_method(const JuristicMethod& method_id);

	/* set adjusting method for higher latitudes */
    void set_high_lats_adjust_method(const AdjustingMethod& method_id);

	/* set the angle for calculating Fajr */
    void set_fajr_angle(const double& angle);

	/* set the angle for calculating Maghrib */
    void set_maghrib_angle(const double& angle);

	/* set the angle for calculating Isha */
    void set_isha_angle(const double& angle);

	/* set the minutes after mid-day for calculating Dhuhr */
    void set_dhuhr_minutes(const double& minutes);

	/* set the minutes after Sunset for calculating Maghrib */
    void set_maghrib_minutes(const double& minutes);

	/* set the minutes after Maghrib for calculating Isha */
    void set_isha_minutes(const double& minutes);

	/* get hours and minutes parts of a float time */
    static void get_float_time_parts(double time, int& hours, int& minutes);

	/* convert float hours to 24h format */
    static std::string float_time_to_time24(double time);

	/* convert float hours to 12h format */
    static std::string float_time_to_time12(double time, bool no_suffix = false);

	/* convert float hours to 12h format with no suffix */
    static std::string float_time_to_time12ns(double time);

    /* ---------------------- Time-Zone Functions ----------------------- */

	/* compute local time-zone for a specific date */
    static double get_effective_timezone(time_t local_time);

	/* compute local time-zone for a specific date */
    static double get_effective_timezone(const int& year,
         const int& month,
         const int& day);

private:
    /* ------------------- Calc Method Parameters -------------------- */
	struct MethodConfig
	{
        MethodConfig(void) = default;

        MethodConfig(double fajr_angle,
             bool maghrib_is_minutes,
             double maghrib_value,
             bool isha_is_minutes,
             double isha_value);

        double fajr_angle;
        double maghrib_value; // angle or minutes
        double isha_value; // angle or minutes
        bool maghrib_is_minutes;
        bool isha_is_minutes;
	};

    /* ---------------------- Calculation Functions ----------------------- */

	/* References: */
	/* http://www.ummah.net/astronomy/saltime   */
	/* http://aa.usno.navy.mil/faq/docs/SunApprox.html */

	typedef std::pair<double, double> DoublePair;

	/* compute declination angle of sun and equation of time */
    DoublePair sun_position(double jd);

	/* compute equation of time */
    double equation_of_time(const double& jd);

	/* compute declination angle of sun */
    double sun_declination(const double& jd);

	/* compute mid-day (Dhuhr, Zawal) time */
    double compute_mid_day(const double& _t);

	/* compute time for a given angle G */
    double compute_time(const double& g, const double& t);

	/* compute the time of Asr */
    double compute_asr(const int& step, const double& t);

    /* ---------------------- Compute Prayer Times ----------------------- */

	// array parameters must be at least of size TimesCount

	/* compute prayer times at given julian date */
    void compute_times(double times[]);

	/* compute prayer times at given julian date */
    void compute_day_times(double times[]);

	/* adjust times in a prayer time array */
    void adjust_times(double times[]);

	/* adjust Fajr, Isha and Maghrib for locations in higher latitudes */
    void adjust_high_lat_times(double times[]);

	/* the night portion used for adjusting times in higher latitudes */
    double night_portion(double angle);

	/* convert hours to day portions  */
    void day_portion(double times[]);

    /* ---------------------- Misc Functions ----------------------- */

	/* compute the difference between two times  */
    static double time_diff(const double& time1, const double& time2);

    static std::string int_to_string(const int& num);

	/* add a leading 0 if necessary */
    static std::string two_digits_format(const int& num);

    /* ---------------------- Julian Date Functions ----------------------- */

	/* calculate julian date from a calendar date */
    double get_julian_date(unsigned& year, unsigned& month, unsigned& day);

	/* convert a calendar date to julian date (second method) */
    double calc_julian_date(const int& year, const int& month, const int& day);

    /* ---------------------- Trigonometric Functions ----------------------- */

	/* degree sin */
    static double dsin(const double& d);

	/* degree cos */
    static double dcos(const double& d);

	/* degree tan */
    static double dtan(const double& d);

	/* degree arcsin */
    static double darcsin(const double& x);

	/* degree arccos */
    static double darccos(const double& x);

	/* degree arctan */
    static double darctan(const double& x);

	/* degree arctan2 */
    static double darctan2(const double& y, const double& x);

	/* degree arccot */
    static double darccot(const double& x);

	/* degree to radian */
    static constexpr double deg2rad(const double& d);

	/* radian to degree */
    static constexpr double rad2deg(const double& r);

	/* range reduce angle in degrees. */
    static double fix_angle(double a);

	/* range reduce hours to 0..23 */
    static double fix_hour(double a);

private:
    /* ---------------------- Private Variables -------------------- */

	MethodConfig method_params[CalculationMethodsCount];

    CalculationMethod calc_method; // caculation method
    JuristicMethod asr_juristic; // Juristic method for Asr
    AdjustingMethod adjust_high_lats; // adjusting method for higher latitudes
    double dhuhr_minutes; // minutes after mid-day for Dhuhr

	double latitude;
	double longitude;
	double timezone;
	double julian_date;

    /* --------------------- Technical Settings -------------------- */

    static const int NUM_ITERATIONS = 1; // number of iterations needed to compute times
};
#endif
