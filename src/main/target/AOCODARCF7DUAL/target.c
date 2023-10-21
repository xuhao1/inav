/*
 * This file is part of INAV.
 *
 * INAV is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * INAV is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with INAV.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>

#include "platform.h"

#include "drivers/bus.h"
#include "drivers/io.h"
#include "drivers/pwm_mapping.h"
#include "drivers/timer.h"
#include "drivers/pinio.h"
#include "drivers/sensor.h"

BUSDEV_REGISTER_SPI_TAG(busdev_mpu6000,     DEVHW_MPU6000,      MPU6000_SPI_BUS,    MPU6000_CS_PIN,     NONE, 0,   DEVFLAGS_NONE,  IMU_MPU6000_ALIGN);
BUSDEV_REGISTER_SPI_TAG(busdev_bmi270,      DEVHW_BMI270,       BMI270_SPI_BUS,     BMI270_CS_PIN,      NONE,  0,   DEVFLAGS_NONE,  IMU_BMI270_ALIGN);

timerHardware_t timerHardware[] = {
    DEF_TIM(TIM1, CH3, PA10, TIM_USE_PPM, 0, 0),    //PPM

    DEF_TIM(TIM3, CH1, PC6, TIM_USE_OUTPUT_AUTO, 0, 0),        // S1   D(2, 2, 7)
    DEF_TIM(TIM3, CH2, PC7, TIM_USE_OUTPUT_AUTO, 0, 0),        // S2   D(2, 3, 7)
    DEF_TIM(TIM3, CH3, PC8, TIM_USE_OUTPUT_AUTO, 0, 0),        // S3   D(2, 4, 7)
    DEF_TIM(TIM3, CH4, PC9, TIM_USE_OUTPUT_AUTO, 0, 0),        // S4   D(2, 7, 7)

    DEF_TIM(TIM8, CH2N, PB0, TIM_USE_OUTPUT_AUTO, 0, 0),        // S5   D(1, 7, 5)
    DEF_TIM(TIM8, CH3N, PB1, TIM_USE_OUTPUT_AUTO, 0, 0),        // S6   D(1, 2, 5)
    DEF_TIM(TIM4, CH1, PB6, TIM_USE_OUTPUT_AUTO, 0, 0),        // S7   D(1, 0, 2)
    DEF_TIM(TIM4, CH2, PB7, TIM_USE_OUTPUT_AUTO, 0, 0),        // S8   D(1, 3, 2)

    DEF_TIM(TIM2, CH1, PA0, TIM_USE_LED, 0, 0),    // LED_TRIP
};

const int timerHardwareCount = sizeof(timerHardware) / sizeof(timerHardware[0]);
