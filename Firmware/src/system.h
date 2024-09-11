#pragma once

// Third-party imports
#include <libriccore/riccoresystem.h>
#include <libriccore/networkinterfaces/can/canbus.h>

// Internal imports
#include "config/systemflags_config.h"
#include "config/commands_config.h"
#include "config/pinmap_config.h"
#include "commands/commands.h"
#include "storage/sdfat_store.h"
#include "time/ricardotimeservice.h"

/* System class: a class which encapsulates all the classes used to describe components on
    the board, creating the board system. Interacting with the board involves interacting
    the system */
class System : public RicCoreSystem<System, SYSTEM_FLAG, Commands::ID>
{
public:
    System();

    void systemSetup();

    void systemUpdate();

    CanBus<SYSTEM_FLAG> canbus;

    SPIClass SDSPI;

    SdFat_Store primarysd;

    RicardoTimeService timeService;

private:
    void setupSD();
    void initializeLoggers();

    const std::string log_path = "/Logs";
    const std::string config_path = "/Config";
};