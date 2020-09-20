/*
 * EMS-ESP - https://github.com/proddy/EMS-ESP
 * Copyright 2019  Paul Derbyshire
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EMSESP_COMMAND_H_
#define EMSESP_COMMAND_H_

#include <Arduino.h>
#include <ArduinoJson.h>

#include <string>
#include <vector>
#include <functional>

#include "console.h"

#include <uuid/log.h>

using uuid::console::Shell;

namespace emsesp {

using cmdfunction_p      = std::function<bool(const char * data, const int8_t id)>;
using cmdfunction_json_p = std::function<bool(const char * data, const int8_t id, JsonObject & output)>;

class Command {
  public:
    struct CmdFunction {
        uint8_t                     device_type_; // DeviceType::
        const __FlashStringHelper * cmd_;
        cmdfunction_p               cmdfunction_;
        cmdfunction_json_p          cmdfunction_json_;

        CmdFunction(const uint8_t device_type, const __FlashStringHelper * cmd, cmdfunction_p cmdfunction, cmdfunction_json_p cmdfunction_json)
            : device_type_(device_type)
            , cmd_(cmd)
            , cmdfunction_(cmdfunction)
            , cmdfunction_json_(cmdfunction_json) {
        }
    };

    static std::vector<CmdFunction> commands() {
        return cmdfunctions_;
    }

    static bool call(const uint8_t device_type, const char * cmd, const char * value, const int8_t id, JsonObject & output);
    static void add(const uint8_t device_type, const uint8_t device_id, const __FlashStringHelper * cmd, cmdfunction_p cb);
    static void add_with_json(const uint8_t device_type, const __FlashStringHelper * cmd, cmdfunction_json_p cb);
    static void show_all(uuid::console::Shell & shell);
    static void show(uuid::console::Shell & shell);
    static void add_context_commands(unsigned int context);
    static bool find(const uint8_t device_type, const char * cmd);

    static std::vector<CmdFunction> cmdfunctions_; // list of commands

  private:
    static uuid::log::Logger logger_;

    static void    show(uuid::console::Shell & shell, uint8_t device_type);
    static uint8_t context_2_device_type(unsigned int context);
};

} // namespace emsesp

#endif