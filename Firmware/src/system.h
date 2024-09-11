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
    /**
     * @brief Construct a new System object
     *
     * @author Max Hallgarten La Casta
     */
    System();

    /**
     * @brief Set up system
     *
     * @author Max Hallgarten La Casta
     */
    void systemSetup();

    /**
     * @brief Update system
     *
     * @author Max Hallgarten La Casta
     */
    void systemUpdate();

    /// @brief Canbus
    CanBus<SYSTEM_FLAG> canbus;

    /// @brief SPI interface for SD card
    SPIClass SDSPI;

    /// @brief SD card file system
    SdFat_Store primarysd;

    /// @brief System time service
    RicardoTimeService timeService;

private:
    /**
     * @brief Set up SD card
     *
     * @author Max Hallgarten La Casta
     */
    void setupSD();

    /**
     * @brief Initialise loggers
     *
     * @author Max Hallgarten La Casta
     */
    void initializeLoggers();

    /// @brief Log file directory
    const std::string log_path = "/Logs";

    /// @brief Configuration directory
    const std::string config_path = "/Config";
};