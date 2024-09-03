#include "binarylogger.h"

// Third-party imports
#include <libriccore/networkinterfaces/serial/cobs.h>

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
    initialised = true;

    // Return success
    return true;
}

void BinaryLogger::log(const std::vector<uint8_t> &payload_buffer)
{
    // Return if the logger is not enabled or initialised
    if (!enabled || !initialised)
    {
        return;
    }

    // COBS encode the payload
    std::vector<uint8_t> payload_buffer_cobs;
    COBS::encode(payload_buffer, payload_buffer_cobs);

    // TODO: checksum (e.g. CRC?)
    // TODO: COBS encode packet and checksum together

    // Construct final buffer
    std::vector<uint8_t> buffer;
    buffer.insert(buffer.end(), start_buffer.begin(), start_buffer.end());               // Start bytes
    buffer.insert(buffer.end(), payload_buffer_cobs.begin(), payload_buffer_cobs.end()); // Payload

    try
    {
        // Send buffer to file
        _file->append(buffer);
    }
    catch (std::exception &e)
    {
    }
}