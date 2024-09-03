#include "loggerhandler_config.h"

// Third-party imports
#include <libriccore/logging/loggers/coutlogger.h>
#include <libriccore/logging/loggers/rnpmessagelogger.h>
#include <libriccore/logging/loggers/syslogger.h>

std::tuple<SysLogger, CoutLogger> RicCoreLoggingConfig::logger_list = {SysLogger(), CoutLogger("COUT_LOG")};
