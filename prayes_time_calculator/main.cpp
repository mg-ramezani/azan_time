/*
 *  In The Name Of God.
 * <main.cpp>
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
#include <iostream>
#include <ctime>
#include "prayes_calculator.h"

static const char* TimeName[]
{
    "Fajr",
    "Sunrise",
    "Dhuhr",
    "Asr",
    "Sunset",
    "Maghrib",
    "Isha",
};

int main (void)
{
    PrayerTimes prayer_times;
    double times[PrayerTimes::TimesCount];
    time_t date = std::time(nullptr);
    double latitude  = 35.6892;
    double longitude = 51.3890;
    double timezone  = PrayerTimes::get_effective_timezone(date);

    prayer_times.get_prayer_times(date, latitude, longitude, timezone, times);

    for(int i{}; i < PrayerTimes::TimesCount; ++i)
        std::cout << TimeName[i] << " : " <<
            PrayerTimes::float_time_to_time24(times[i]).c_str() << std::endl;

    return 0;
}
