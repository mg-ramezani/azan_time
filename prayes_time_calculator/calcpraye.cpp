#include "calcpraye.h"

#include <algorithm>
#include <iostream>

#include "cordinates.h"

Azan::CalculateAzan::CalculateAzan(cordinate cor)
    : lat_long(cor)
{
}

Azan::CalculateAzan::CalculateAzan(double lat, double lon)
    : lat_long(std::make_pair(lat, lon))
{
}

Azan::CalculateAzan::CalculateAzan(const std::string& name)
    : lat_long({})
{
    set_new_cordinates(name);
}

void Azan::CalculateAzan::set_calculation_method(const AzanTime::CalculationMethod method)
{
    prayer_calculator.set_calc_method(method);
}

const vec_string& Azan::CalculateAzan::get_prayer_times()
{
    ///TODO: Use std::vector instead of raw array

    date = std::time(nullptr);
    time_zone = AzanTime::get_effective_timezone(date);

    double __tmp[azan_times_count]{};
    prayer_calculator.get_prayer_times(
         date, lat_long.first, lat_long.second, time_zone, __tmp);

    prayer_times.clear();

    for (size_t i{}; i < azan_times_count; ++i)
    {
        prayer_times.emplace_back(AzanTime::float_time_to_time24(__tmp[i]));
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
        const auto n{city_names[i][0] + ":" + city_names[i][1]};
        if (n == name)
        {
            const auto lat{std::atof(city_names[i][2].c_str())};
            const auto lon{std::atof(city_names[i][3].c_str())};

            set_new_cordinates(lat, lon);

            return;
        }
    }

    throw std::out_of_range(name + ": not found.");
}
