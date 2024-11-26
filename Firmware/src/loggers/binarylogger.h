#pragma once

// Standard imports
#include <memory>

// Third-party imports
#include <libriccore/logging/loggers/loggerbase.h>
#include <libriccore/storage/wrappedfile.h>

class BinaryLogger : public LoggerBase
{
public:
    /**
     * @brief Construct a new Binary Logger
     *
     * @author Max Hallgarten La Casta
     */
    BinaryLogger() {};

    /**
     * @brief Initialise the Binary Logger
     *
     * @author Max Hallgarten La Casta
     *
     * @param[in] file File pointer
     * @return true Successfully initialised
     * @return false Unsuccessfully initialised
     */
    bool initialize(std::unique_ptr<WrappedFile> file);

    /**
     * @brief Log a payload
     *
     * @author Max Hallgarten La Casta
     *
     * @param[in] payload_buffer Payload
     */
    void log(const std::vector<uint8_t> &payload_buffer);

private:
    /// @brief File pointer
    std::unique_ptr<WrappedFile> _file;

    /// @brief Log packet marker
    const std::vector<uint8_t> END_BUFFER = {0x0};

    /// @brief Index of payloads logged
    uint32_t log_index = 0;
};