#pragma once

// Standard imports
#include <memory>

// Third-party imports
#include <libriccore/logging/loggers/loggerbase.h>
#include <libriccore/storage/wrappedfile.h>

class BinaryLogger : public LoggerBase
{
public:
    BinaryLogger() {};

    bool initialize(std::unique_ptr<WrappedFile> file);

    void log(const std::vector<uint8_t> &payload_buffer);

private:
    std::unique_ptr<WrappedFile> _file;

    bool initialised = false;

    const std::vector<uint8_t> start_buffer = {0x0, 0x0};

    uint32_t log_index = 0;
};