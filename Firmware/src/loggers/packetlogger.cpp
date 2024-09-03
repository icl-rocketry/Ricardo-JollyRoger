#include "packetlogger.h"

// Third-party imports
#include <libriccore/networkinterfaces/serial/cobs.h>

bool PacketLogger::initialize(std::unique_ptr<WrappedFile> file, RnpNetworkManager &networkmanager)
{
    // Initialise underlying binary logger
    bool success = BinaryLogger::initialize(std::move(file));

    // Return if the underlying binary logger fails to initialise
    if (!success)
    {
        return false;
    }

    // Register packet capture callback
    networkmanager.registerPacketCapture(getCaptureCallback());

    // Return false
    return true;
}

void PacketLogger::log(RnpPacket &packet)
{
    // Serialise the packet
    std::vector<uint8_t> packet_buffer;
    packet.serialize(packet_buffer);

    // Send the serialised packet to the underlying binary logger
    BinaryLogger::log(packet_buffer);
}

PacketCaptureHandlerCb PacketLogger::getCaptureCallback()
{
    // Return the packet capture callback
    return [this](RnpPacket& packet)
    {
        // Send packet to logger
        log(packet);
    };
}