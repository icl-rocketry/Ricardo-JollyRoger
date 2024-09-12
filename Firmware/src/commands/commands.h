/**
 * @file commands.h
 * @author Kiran de Silva (kd619@ic.ac.uk)
 * @brief Contains signatures of all commands in the system. Note there is no requirement ot have all the command signatures defined in a single file, just ensure all the seperate files are included into the command_config.h
 * @version 0.1
 * @date 2023-06-17
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

// Standard imports
#include <stdint.h>

// Third-party imports
#include <librnp/rnp_packet.h>

// Internal imports
#include "config/forward_decl.h"

namespace Commands
{

    void FreeRamCommand(ForwardDecl_SystemClass &system, const RnpPacketSerialized &packet);

    /**
     * @brief Command for Blackbox telemetry
     *
     * Reponds to the source with a Blackbox Telemetry packet
     *
     * @author Max Hallgarten La Casta
     *
     * @param[in] system System
     * @param[in] packet Received packet
     */
    void BlackBoxTelemetryCommand(ForwardDecl_SystemClass &system, const RnpPacketSerialized &packet);

}