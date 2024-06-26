/*
 * Copyright (C) 2010 The Android Open Source Project
 * Copyright (C) 2012-2014, 2016-2021 The Linux Foundation. All rights reserved.
 *
 * Not a Contribution, Apache license notifications and license are
 * retained for attribution purposes only.

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
* Changes from Qualcomm Innovation Center are provided under the following license:
*
* Copyright (c) 2022-2023 Qualcomm Innovation Center, Inc. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
*
*    * Redistributions of source code must retain the above copyright
*      notice, this list of conditions and the following disclaimer.
*
*    * Redistributions in binary form must reproduce the above
*      copyright notice, this list of conditions and the following
*      disclaimer in the documentation and/or other materials provided
*      with the distribution.
*
*    * Neither the name of Qualcomm Innovation Center, Inc. nor the names of its
*      contributors may be used to endorse or promote products derived
*      from this software without specific prior written permission.
*
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
* GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
* IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
* OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef ANDROID_IQSERVICE_H
#define ANDROID_IQSERVICE_H

#include <stdint.h>
#include <sys/types.h>
#include <utils/Errors.h>
#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/IBinder.h>
#include <IQClient.h>
#include <IQHDMIClient.h>


namespace qService {
// ----------------------------------------------------------------------------
// clang-format off

class IQService : public android::IInterface
{
public:
    DECLARE_META_INTERFACE(QService);
    enum {
      COMMAND_LIST_START = android::IBinder::FIRST_CALL_TRANSACTION,
      GET_PANEL_BRIGHTNESS = 2,                // Provides ability to get the panel brightness
      SET_PANEL_BRIGHTNESS = 3,                // Provides ability to set the panel brightness
      CONNECT_HWC_CLIENT = 4,                  // Connect to qservice
      SCREEN_REFRESH = 5,                      // Refresh screen through SF invalidate
      GET_DISPLAY_VISIBLE_REGION = 11,         // Get the visibleRegion for dpy
      SET_SECONDARY_DISPLAY_STATUS = 12,       // Sets secondary display status
      SET_MAX_PIPES_PER_MIXER = 13,            // Set max pipes per mixer for MDPComp
      DYNAMIC_DEBUG = 15,                      // Enable more logging on the fly
      SET_IDLE_TIMEOUT = 16,                   // Set idle timeout for GPU fallback
      CONFIGURE_DYN_REFRESH_RATE = 18,         //  Enable/Disable/Set refresh rate dynamically
      CONTROL_PARTIAL_UPDATE = 19,             // Provides ability to enable/disable partial update
      TOGGLE_SCREEN_UPDATES = 20,              // Provides ability to set the panel brightness
      SET_FRAME_DUMP_CONFIG = 21,              // Provides ability to set the frame dump config
      CONNECT_HDMI_CLIENT = 23,                // Connect HDMI CEC HAL Client
      QDCM_SVC_CMDS = 24,                      // request QDCM services.
      SET_ACTIVE_CONFIG = 25,                  // Set a specified display config
      GET_ACTIVE_CONFIG = 26,                  // Get the current config index
      GET_CONFIG_COUNT = 27,                   // Get the number of supported display configs
      GET_DISPLAY_ATTRIBUTES_FOR_CONFIG = 28,  // Get attr for specified config
      SET_DISPLAY_MODE = 29,                   // Set display mode to command or video mode
      SET_CAMERA_STATUS = 30,                  // To notify display when camera is on and off
      MIN_HDCP_ENCRYPTION_LEVEL_CHANGED = 31,
      GET_BW_TRANSACTION_STATUS = 32,   // Client can query BW transaction status.
      SET_LAYER_MIXER_RESOLUTION = 33,  // Enables client to set layer mixer resolution.
      SET_COLOR_MODE = 34,              // Overrides the QDCM mode on the display
      SET_COLOR_MODE_BY_ID = 36,        // Overrides the QDCM mode using the given mode ID
      GET_COMPOSER_STATUS = 37,  // Get composer init status-true if primary display init is done
      SET_QSYNC_MODE = 38,       // Set qsync mode. 0 - (none)disable qsync, 1 - continuous mode.
      SET_COLOR_MODE_WITH_RENDER_INTENT = 39,  // Overrides the QDCM mode with render intent
      SET_IDLE_PC = 40,                        // Enable/disable Idle power collapse
      SET_DPPS_AD4_ROI_CONFIG = 41,            // Set ad4 roi config for debug
      SET_DSI_CLK = 42,                        // Set DSI Clk.
      GET_DSI_CLK = 43,                        // Get DSI Clk.
      GET_SUPPORTED_DSI_CLK = 44,              // Get supported DSI Clk.
      SET_COLOR_MODE_FROM_CLIENT = 45,         // Overrides the QDCM mode using the given mode ID
      SET_FRAME_TRIGGER_MODE = 46,             // Set frame trigger mode for debug
      SET_PANEL_LUMINANCE = 47,                // Set Panel Luminance attributes.
      SET_BRIGHTNESS_SCALE = 48,               // Set brightness scale ratio
      SET_COLOR_SAMPLING_ENABLED = 49,         // Toggle the collection of display color stats
      SET_VSYNC_STATE = 50,                    // Enable/disable Vsync
      NOTIFY_TUI_TRANSITION = 51,              // Notify TUI transition prepare/start/stop
      GET_DISPLAY_PORT_ID = 52,                // Get the port id for a given display
      SET_NOISE_PLUGIN_OVERRIDE = 53,          // Override NoisePlugIn parameters
      SET_DIMMING_ENABLE = 54,                 // Set display dimming enablement
      SET_DIMMING_MIN_BL = 55,                 // Set display dimming minimal backlight value
      DUMP_CODE_COVERAGE = 56,                 // Dump the code coverage data for userspace projects
      UPDATE_TRANSFER_TIME = 57,               // Update transfer time
      SET_JITTER_CONFIG = 58,                  // Watchdog TE Jitter Configuration
      RETRIEVE_DEMURATN_FILES = 59,            // Retrieve DemuraTn files from TVM
      SET_DEMURA_STATE = 60,                   // Enable/disable demura feature
      SET_DEMURA_CONFIG = 61,                  // Set the demura configuration index
      SET_BPP_MODE = 62,                       // Set Panel bpp to 24bpp or 30bpp
      COMMAND_LIST_END = 400,
    };

    enum {
        END = 0,
        START,
    };

    enum {
        DEBUG_ALL,
        DEBUG_MDPCOMP,
        DEBUG_VSYNC,
        DEBUG_VD,
        DEBUG_PIPE_LIFECYCLE,
        DEBUG_DRIVER_CONFIG,
        DEBUG_ROTATOR,
        DEBUG_QDCM,
        DEBUG_SCALAR,
        DEBUG_CLIENT,
        DEBUG_DISPLAY,
        DEBUG_IWE,
        DEBUG_WB_USAGE,
        DEBUG_MAX_VAL = DEBUG_WB_USAGE, // Used to check each bit of the debug command paramater.
        // Update DEBUG_MAX_VAL when adding new debug tag.
    };

    enum {
        PREF_POST_PROCESSING,
        PREF_PARTIAL_UPDATE,
        ENABLE_PARTIAL_UPDATE,
    };

    enum {
        QSYNC_MODE_NONE,
        QSYNC_MODE_CONTINUOUS,
        QSYNC_MODE_ONESHOT,    // Not supported
    };

    enum {
        TUI_TRANSITION_PREPARE,
        TUI_TRANSITION_START,
        TUI_TRANSITION_END,
    };

    // Register a HWC client that can be notified
    // This client is generic and is intended to get
    // dispatches of all events calling into QService
    virtual void connect(const android::sp<qClient::IQClient>& client) = 0;
    // Register an HDMI client. This client gets notification of HDMI events
    // such as plug/unplug and CEC messages
    virtual void connect(const android::sp<qClient::IQHDMIClient>& client) = 0;
    // Generic function to dispatch binder commands
    // The type of command decides how the data is parceled
    virtual android::status_t dispatch(uint32_t command,
            const android::Parcel* inParcel,
            android::Parcel* outParcel) = 0;
};

// ----------------------------------------------------------------------------

class BnQService : public android::BnInterface<IQService>
{
public:
    virtual android::status_t onTransact( uint32_t code,
                                          const android::Parcel& data,
                                          android::Parcel* reply,
                                          uint32_t flags = 0);
};

// ----------------------------------------------------------------------------
// clang-format on
}; // namespace qService

#endif // ANDROID_IQSERVICE_H
