/**
 *    ||          ____  _ __
 * +------+      / __ )(_) /_______________ _____  ___
 * | 0xBC |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * +------+    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *  ||  ||    /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * Crazyflie control firmware
 *
 * Copyright (C) 2011-2016 Bitcraze AB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, in version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * sensors_stock.c - Crazyflie stock sensor acquisition function
 */
#include "sensors.h"
#include "imu.h"

#define IMU_RATE RATE_500_HZ

void sensorsInit(void)
{
    imu6Init();
}

bool sensorsTest(void)
{
    bool pass = true;

    pass &= imu6Test();

    return pass;
}

void sensorsAcquire(sensorData_t *sensors, const uint32_t tick)
{
    if (RATE_DO_EXECUTE(IMU_RATE, tick)) {
        imu9Read(&sensors->gyro, &sensors->acc, &sensors->mag);
    }
}

bool sensorsAreCalibrated()
{
    Axis3f dummyData;
    imu6Read(&dummyData, &dummyData);
    return imu6IsCalibrated();
}
