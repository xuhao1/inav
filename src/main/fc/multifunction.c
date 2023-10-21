/*
 * This file is part of INAV Project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU General Public License Version 3, as described below:
 *
 * This file is free software: you may copy, redistribute and/or modify
 * it under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */

#include "platform.h"
#include "build/debug.h"
#include "drivers/time.h"

#ifdef USE_MULTI_FUNCTIONS

#include "fc/fc_core.h"
#include "fc/multifunction.h"
#include "fc/rc_modes.h"
#include "fc/runtime_config.h"

#include "io/osd.h"
#include "navigation/navigation.h"

multi_function_e selectedItem = MULTI_FUNC_NONE;
uint8_t multiFunctionFlags;
bool nextItemIsAvailable = false;

static void multiFunctionApply(multi_function_e selectedItem)
{
    switch (selectedItem) {
    case MULTI_FUNC_NONE:
        break;
    case MULTI_FUNC_1:  // redisplay current warnings
        osdResetWarningFlags();
        break;
    case MULTI_FUNC_2:  // control manual emergency landing
        checkManualEmergencyLandingControl(ARMING_FLAG(ARMED));
        break;
    case MULTI_FUNC_3:  // toggle Safehome suspend
#if defined(USE_SAFE_HOME)
        if (navConfig()->general.flags.safehome_usage_mode != SAFEHOME_USAGE_OFF) {
            MULTI_FUNC_FLAG(MF_SUSPEND_SAFEHOMES) ? MULTI_FUNC_FLAG_DISABLE(MF_SUSPEND_SAFEHOMES) : MULTI_FUNC_FLAG_ENABLE(MF_SUSPEND_SAFEHOMES);
        }
#endif
        break;
    case MULTI_FUNC_4:  // toggle RTH Trackback suspend
        if (navConfig()->general.flags.rth_trackback_mode != RTH_TRACKBACK_OFF) {
            MULTI_FUNC_FLAG(MF_SUSPEND_TRACKBACK) ? MULTI_FUNC_FLAG_DISABLE(MF_SUSPEND_TRACKBACK) : MULTI_FUNC_FLAG_ENABLE(MF_SUSPEND_TRACKBACK);
        }
        break;
    case MULTI_FUNC_5:
#ifdef USE_DSHOT
        if (STATE(MULTIROTOR)) {    // toggle Turtle mode
            MULTI_FUNC_FLAG(MF_TURTLE_MODE) ? MULTI_FUNC_FLAG_DISABLE(MF_TURTLE_MODE) : MULTI_FUNC_FLAG_ENABLE(MF_TURTLE_MODE);
        }
#endif
        break;
    case MULTI_FUNC_6:  // emergency ARM
        if (!ARMING_FLAG(ARMED)) {
            emergencyArmingUpdate(true, true);
        }
    case MULTI_FUNC_END:
        break;
    }
}

bool isNextMultifunctionItemAvailable(void)
{
    return nextItemIsAvailable;
}

void setMultifunctionSelection(multi_function_e item)
{
    selectedItem = item == MULTI_FUNC_END ? MULTI_FUNC_1 : item;
    nextItemIsAvailable = false;
}

multi_function_e multiFunctionSelection(void)
{
    static timeMs_t startTimer;
    static timeMs_t selectTimer;
    static bool toggle = true;
    const timeMs_t currentTime = millis();

    if (IS_RC_MODE_ACTIVE(BOXMULTIFUNCTION)) {
        if (selectTimer) {
            if (currentTime - selectTimer > 3000) {     // 3s selection duration to activate selected function
                multiFunctionApply(selectedItem);
                selectTimer = 0;
                selectedItem = MULTI_FUNC_NONE;
                nextItemIsAvailable = false;
            }
        } else if (toggle) {
            if (selectedItem == MULTI_FUNC_NONE) {
                selectedItem++;
            } else {
                selectTimer = currentTime;
                nextItemIsAvailable = true;
            }
        }
        startTimer = currentTime;
        toggle = false;
    } else if (startTimer) {
        if (!toggle && selectTimer) {
            setMultifunctionSelection(++selectedItem);
        }
        if (currentTime - startTimer > 4000) {      // 4s reset delay after mode deselected
            startTimer = 0;
            selectedItem = MULTI_FUNC_NONE;
        }
        selectTimer = 0;
        toggle = true;
    }

    return selectedItem;
}
#endif
