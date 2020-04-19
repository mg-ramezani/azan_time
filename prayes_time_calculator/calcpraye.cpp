#include "calcpraye.h"

#include <algorithm>
#include <iostream>

#include "cordinates.h"

Azan::CalculateAzan::CalculateAzan(cordinate cor)
    : lat_long(cor)
    , prayer_times(azan_times_count)
    , date(std::time(nullptr))
    , time_zone(AzanTime::get_effective_timezone(date))
{
}

Azan::CalculateAzan::CalculateAzan(double lat, double lon)
    : lat_long(std::make_pair(lat, lon))
    , prayer_times(azan_times_count)
    , date(std::time(nullptr))
    , time_zone(AzanTime::get_effective_timezone(date))
{
}

Azan::CalculateAzan::CalculateAzan(const std::string& name)
    : lat_long({})
    , prayer_times(azan_times_count)
    , date(std::time(nullptr))
    , time_zone(AzanTime::get_effective_timezone(date))
{
    set_new_cordinates(name);
}

const vec_string& Azan::CalculateAzan::get_prayer_times()
{
    ///TODO: Use std::vector instead of raw array

    double __tmp[azan_times_count]{};
    prayer_calculator.get_prayer_times(
         date, lat_long.first, lat_long.second, time_zone, __tmp);

    for (size_t i{}; i < azan_times_count; ++i)
    {
        prayer_times.at(i) = AzanTime::float_time_to_time24(__tmp[i]);
    }

    return prayer_times;
}

void Azan::CalculateAzan::print_available_cordinate()
{
    for (size_t i{}; i < CITY_ARRAY_LENGTH; ++i)
    {
        std::cout << "[" << city_names[i][0] << ":";
        std::cout << city_names[i][1] << "] ";
        std::cout << city_names[i][2] << ",";
        std::cout << city_names[i][3] << std::endl;
    }
}

const vec_strings& Azan::CalculateAzan::get_availabel_cordinate() const
{
    return city_names;
}

size_t Azan::CalculateAzan::get_length_of_availabel_cordinate() const
{
    return CITY_ARRAY_LENGTH;
}

cordinate Azan::CalculateAzan::get_current_cordinate() const
{
    return lat_long;
}

double Azan::CalculateAzan::get_current_latitude() const
{
    return lat_long.first;
}

double Azan::CalculateAzan::get_current_longitude() const
{
    return lat_long.second;
}

void Azan::CalculateAzan::set_new_cordinates(cordinate cor)
{
    lat_long = cor;
}

void Azan::CalculateAzan::set_new_cordinates(double lat, double lon)
{
    lat_long = std::make_pair(lat, lon);
}

void Azan::CalculateAzan::set_new_cordinates(const std::string& name)
{
    for (size_t i{}; i < CITY_ARRAY_LENGTH; ++i)
    {
        if (city_names[i][0] + ":" + city_names[i][1] == name)
        {
            const double lat{std::atof(city_names[i][2].c_str())};
            const double lon{std::atof(city_names[i][3].c_str())};

            set_new_cordinates(lat, lon);

            break;
        }
    }

    throw std::out_of_range(name + ": not found.");
}
