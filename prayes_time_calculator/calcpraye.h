/*
 *  In The Name Of God.
 * <calcpraye.cpp>
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * @MG_Ramezani. 2019 10 9
 * g1999raemzani@gmail.com
 */

#ifndef CALCPRAYE_HEADER
#define CALCPRAYE_HEADER

#include <stdlib.h>
#include <stdio.h>
#include <utility>
#include <string>
#include <vector>
#include <ctime>

#include "prayes_calculator.h"
#include "noncopyable.hpp"
#include "types.h"

namespace Azan
{
constexpr auto azan_times_count{AzanTime::TimesCount};

class CalculateAzan : private noncopyable
{
private:
    cordinate lat_long;
    vec_string prayer_times;

    time_t date;
    double time_zone;

    AzanTime prayer_calculator;

public:
    CalculateAzan(cordinate cor);
    CalculateAzan(void) = default;
    explicit CalculateAzan(double lat, double lon);
    CalculateAzan(const std::string& name);

    const vec_string& get_prayer_times(void);
    void print_available_cordinate(void);
    const vec_strings& get_availabel_cordinate(void) const;
    size_t get_length_of_availabel_cordinate(void) const;

    cordinate get_current_cordinate(void) const;

    double get_current_latitude(void) const;
    double get_current_longitude(void) const;

    void set_new_cordinates(cordinate cor);
    void set_new_cordinates(double lat, double lon);
    void set_new_cordinates(const std::string& name);
};
} // namespace Azan

#endif
