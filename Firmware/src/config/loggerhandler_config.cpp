#include "loggerhandler_config.h"

// Third-party imports
#include <libriccore/logging/loggers/coutlogger.h>
#include <libriccore/logging/loggers/rnpmessagelogger.h>
#include <libriccore/logging/loggers/syslogger.h>

// Internal imports
#include <loggers/packetlogger.h>

std::tuple<SysLogger, CoutLogger, PacketLogger> RicCoreLoggingConfig::logger_list = {SysLogger(), CoutLogger("COUT_LOG"), PacketLogger()};
