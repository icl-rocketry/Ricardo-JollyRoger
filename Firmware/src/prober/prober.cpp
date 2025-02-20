#include "prober.h"

// Third-party imports
#include <librnp/default_packets/simplecommandpacket.h>

Prober::Prober(RnpNetworkManager &network_manager,
               const Services::ID &service_id)
    : Prober(network_manager, service_id, {}) {};

Prober::Prober(RnpNetworkManager &network_manager,
               const Services::ID &service_id,
               std::vector<ProbeTask> tasks)
    : network_manager(network_manager),
      tasks(tasks)
{
    // Register service callback
    network_manager.registerService(static_cast<uint8_t>(service_id), getServiceCallback());

    // Register packet capture callback
    network_manager.registerPacketCapture(getCaptureCallback());
};

Prober::~Prober() {};

void Prober::addTask(ProbeTask task)
{
    // Add task to tasks vector
    tasks.push_back(task);
}

void Prober::addTask(std::vector<ProbeTask> task)
{
    // Iterate through tasks
    for (ProbeTask &itask : task)
    {
        // Probe
        addTask(itask);
    }
}

void Prober::update()
{
    // Iterate through tasks
    for (ProbeTask &task : tasks)
    {
        // Probe
        task.update(network_manager);
    }
}

void Prober::service(RnpPacket &packet)
{
    // Check that the packet is a command packet
    if (packet.header.type != 0)
    {
        return;
    }

    // Cast to command packet
    SimpleCommandPacket &command = static_cast<SimpleCommandPacket &>(packet);

    // Check for correct command
    /// @todo replace with more complete implementation
    if (command.command != 1)
    {
        return;
    }

    // Extract probe delta
    const int32_t delta = command.arg;

    // Ignore negative deltas
    if (delta < 0)
    {
        return;
    }

    // Update all probe times
    updateProbeTime(static_cast<uint32_t>(delta));
}

void Prober::receive(RnpPacket &packet)
{
    // Iterate through tasks
    for (ProbeTask &task : tasks)
    {
        // Receive packet
        task.receive(packet);
    }
}

void Prober::updateProbeTime(const uint32_t &delta)
{
    // Iterate through tasks
    for (ProbeTask &task : tasks)
    {
        // Update probe time
        task.updateProbeTime(delta);
    }
}

PacketHandlerCb Prober::getServiceCallback()
{
    // Return the service callback
    return [this](packetptr_t packet)
    {
        // Send packet to service
        service(*packet);
    };
}

PacketCaptureHandlerCb Prober::getCaptureCallback()
{
    // Return the packet capture callback
    return [this](RnpPacket &packet)
    {
        // Send packet to probe tasks
        receive(packet);
    };
}