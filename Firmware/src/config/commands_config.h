#pragma once

// Standard imports
#include <stdint.h>
#include <unordered_map>
#include <functional>
#include <initializer_list>

// Third-party imports
#include <libriccore/commands/commandhandler.h>
#include <librnp/rnp_packet.h>

// Internal imports
#include "config/forward_decl.h"
#include "commands/commands.h"

namespace Commands
{
    enum class ID : uint8_t
    {
        NoCommand = 0,
        Free_Ram = 250,
        BlackBoxTelemetry = 123,
    };

    inline std::initializer_list<ID> defaultEnabledCommands = {ID::Free_Ram, ID::BlackBoxTelemetry};

    inline std::unordered_map<ID, std::function<void(ForwardDecl_SystemClass &, const RnpPacketSerialized &)>> command_map{
        {ID::Free_Ram, FreeRamCommand},
        {ID::BlackBoxTelemetry, BlackBoxTelemetryCommand}};

};