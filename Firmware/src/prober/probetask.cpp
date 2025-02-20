#include "probetask.h"

// Third-party imports
#include <librnp/default_packets/simplecommandpacket.h>
#include <libriccore/platform/millis.h>

#include <libriccore/riccorelogging.h>

ProbeTask::ProbeTask(const uint8_t &source,
                     const uint8_t &destination,
                     const uint8_t &destination_service,
                     const uint8_t &command_id,
                     const int32_t &command_arg,
                     const uint32_t &poll_delta)
    : source(source),
      destination(destination),
      destination_service(destination_service),
      command_id(command_id),
      command_arg(command_arg), poll_delta(poll_delta)
{
    // Update next probe time
    updateProbeTime();
}

ProbeTask::~ProbeTask() {};

void ProbeTask::update(RnpNetworkManager &network_manager)
{
    // Check if it is time to probe
    if (millis() < probeTime)
    {
        return;
    }

    // Send probe
    send(network_manager);

    // Update next probe time
    // NOTE: the probe time should be updated again when the probe loops
    //       back via the network manager, however forcing the update
    //       here prevents spam if the probe method is called again before
    //       the probe packet is processed by the network manager
    updateProbeTime();
}

void ProbeTask::send(RnpNetworkManager &network_manager)
{
    // Declare command packet
    SimpleCommandPacket probePacket(command_id, command_arg);

    // Set command packet header
    /// @todo change source service and uid values
    probePacket.header.source = source;
    probePacket.header.source_service = 0;
    probePacket.header.destination = destination;
    probePacket.header.destination_service = destination_service;
    probePacket.header.uid = 0;

    // Send command packet
    network_manager.sendPacket(probePacket);
}

void ProbeTask::receive(RnpPacket &packet)
{
    // Check that the addresses match
    if ((packet.header.source != source) || (packet.header.destination != destination))
    {
        return;
    }

    // Check that the services match
    if (packet.header.destination_service != destination_service)
    {
        return;
    }

    // Check that the packet is a command packet
    if (packet.header.type != 0)
    {
        return;
    }

    // Cast to command packet
    SimpleCommandPacket &command = static_cast<SimpleCommandPacket &>(packet);

    // Check that the command identifier and argument match
    if ((command.command != command_id) || (command.arg != command_arg))
    {
        return;
    }

    // Update next probe time
    updateProbeTime();
}

void ProbeTask::updateProbeTime()
{
    // Update probe time with polling delta
    updateProbeTime(poll_delta);
}

void ProbeTask::updateProbeTime(const uint32_t &delta)
{
    /// @todo clamp delta to prevent excessive deltas?

    // Get current time
    const uint32_t currentTime = millis();

    // Calculate jitter
    /// @todo determine jitter bounds
    const uint32_t jitter = random(0, 50);

    // Update probe time (w/ jitter to reduce collisions between multiple probers)
    probeTime = currentTime + delta + jitter;
}