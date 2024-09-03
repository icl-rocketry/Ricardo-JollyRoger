#pragma once

// Standard imports
#include <memory>

// Third-party imports
#include <libriccore/logging/loggers/loggerbase.h>
#include <libriccore/storage/wrappedfile.h>
#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>

// Internal imports
#include <loggers/binarylogger.h>

class PacketLogger : public BinaryLogger
{
public:
    PacketLogger() {};

    bool initialize(std::unique_ptr<WrappedFile> file, RnpNetworkManager &networkmanager);

private:
    void log(RnpPacket &packet);

    PacketCaptureHandlerCb getCaptureCallback();
};