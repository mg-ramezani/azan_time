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
#include "calcpraye.h"
#include <string.h>

int main(int argc, char** argv)
{
    double times[AzanTimesCount];
    giveMeAllTime(times);
    for (int i = 0; i < AzanTimesCount; ++i)
    {
        std::cout << TimeName[i] << " : " << AzanTime::float_time_to_time24(times[i]).c_str() << std::endl;
    }

    if (argc < 2)
    {
        fprintf(stderr,
             "USAGE: \n\t%s -c CITY_NAME -o OSTAN_NAME\n\t[PRINT NAMES]%s -a\n",
             __FILE__,
             __FILE__);
    }
    else
    {
        if (strcmp(argv[1], "-a") == 0)
        {
            char* tmp = static_cast<char*>(malloc(50));
            for (unsigned i = 0; i < CITY_ARRAY_LENGTH; ++i)
            {
                if (strcmp(tmp, cityName[i][0]) != 0)
                {
                    std::cout << "OSTAN]: " << cityName[i][0] << std::endl;
                    strncpy(tmp, cityName[i][0], 50);
                }
                std::cout << "\tCITY ]: " << cityName[i][1] << std::endl;
            }
            free(tmp);
        }
    }
    return 0;
}
