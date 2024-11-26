#include "binarylogger.h"

// Third-party imports
#include <libriccore/networkinterfaces/serial/cobs.h>
#include <libriccore/platform/millis.h>

// Internal imports
#include <loggers/binarylogheader.h>
#include <time/ricardotimeservice.h>
#include <utilities/vector.h>

bool BinaryLogger::initialize(std::unique_ptr<WrappedFile> file)
{
    // Check that file pointer exists
    if (file == nullptr)
    {
        // Return failure
        return false;
    };

    // Store file pointer
    _file = std::move(file);
    initialized = true;

    // Return success
    return true;
}

void BinaryLogger::log(const std::vector<uint8_t> &payload)
{
    // Return if the logger is neither enabled nor initialised
    if (!enabled || !initialized)
    {
        return;
    }

    // Generate binary log packet
    BinaryLogHeader logHeader(log_index++, millis(), RicardoTimeService::getEpochMillis());

    // Generate packet
    std::vector<uint8_t> packet;
    append(packet, logHeader.serialise());
    append(packet, payload);

    // COBS encode the packet
    std::vector<uint8_t> packet_COBS;
    COBS::encode(packet, packet_COBS);

    // TODO: checksum? (e.g. CRC?)
    // TODO: COBS encode packet and checksum together

    // Construct final buffer
    std::vector<uint8_t> buffer;
    append(buffer, packet_COBS); // Payload
    append(buffer, END_BUFFER);  // End marker

    try
    {
        // Send buffer to file
        _file->append(buffer);
    }
    catch (std::exception &e)
    {
    }
}