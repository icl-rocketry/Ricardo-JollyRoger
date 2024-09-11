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
    /**
     * @brief Construct a new Packet Logger object
     *
     * @author Max Hallgarten La Casta
     */
    PacketLogger() {};

    /**
     * @brief Initialise the Packet Logger
     *
     * @author Max Hallgarten La Casta
     *
     * @param[in] file File pointer
     * @param[in] networkmanager Network manager
     * @return true Successfully initialised
     * @return false Unsuccessfully initialised
     */
    bool initialize(std::unique_ptr<WrappedFile> file, RnpNetworkManager &networkmanager);

private:
    /**
     * @brief Log a packet
     *
     * @author Max Hallgarten La Casta
     *
     * @param[in] packet Packet
     */
    void log(RnpPacket &packet);

    /**
     * @brief Get the Packet Capture Callback
     *
     * @author Max Hallgarten La Casta
     *
     * @return PacketCaptureHandlerCb Packet capture callback
     */
    PacketCaptureHandlerCb getCaptureCallback();
};