#pragma once

// Standard imports
#include <tuple>

// Third-party imports
#include <libriccore/logging/loggers/coutlogger.h>
#include <libriccore/logging/loggers/rnpmessagelogger.h>
#include <libriccore/logging/loggers/syslogger.h>

namespace RicCoreLoggingConfig
{
    enum class LOGGERS
    {
        SYS, // default system logging
        COUT // cout logging
    };

    extern std::tuple<SysLogger, CoutLogger> logger_list;
};
