#include "telemetrylogger.h"

#include <libriccore/logging/loggers/loggerbase.h>

#include <memory>
#include <string>


#include <libriccore/storage/wrappedfile.h>

#include <Arduino.h>


TelemetryLogger::TelemetryLogger():
_file(nullptr),
internalLogCB()
{};

bool TelemetryLogger::initialize(std::unique_ptr<WrappedFile> file,std::function<void(std::string_view message)> logcb)
{
    if (logcb)
    {
        internalLogCB = logcb;
    }

    if (file == nullptr){return false;};
    _file = std::move(file);
    initialized=true;
    return true;
}

void TelemetryLogger::log(std::vector<uint8_t> data)
{
    if (!initialized){return;};
    if (!enabled){return;};

    //if there is any exception we want to force the user to re-initialize the file, as a wrapped file will
    //automatically close itself when an exception is thrown
    try{
        _file->append(data);
    }
    catch(std::exception &e)
    {
        initialized=false;
        internalLogCB(e.what());
    }

}
