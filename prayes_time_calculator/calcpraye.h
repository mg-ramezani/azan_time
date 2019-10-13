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

#include "prayes_calculator.h"
#include "cordinates.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/* Azan Time */
static const char* TimeName[] {
    "Fajr",
    "Sunrise",
    "Dhuhr",
    "Asr",
    "Sunset",
    "Maghrib",
    "Isha",
};

constexpr unsigned short int AzanTimesCount = AzanTime::TimesCount;

void giveMeAllTime(double azan_time[])
{
    static class AzanTime prayer_time;
    time_t date = time(nullptr);

    constexpr double latitude  = 35.6892;
    constexpr double longitude = 51.3890;
    const     double timezone  = AzanTime::get_effective_timezone(date);

    prayer_time.get_prayer_times(date, latitude, longitude,
        timezone, azan_time);
}
