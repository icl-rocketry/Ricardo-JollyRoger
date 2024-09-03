#include "system.h"

// Standard imports
#include <cstdlib>
#include <memory>

// Third-party imports
#include <libriccore/riccoresystem.h>
#include <HardwareSerial.h>

// Internal imports
#include "config/systemflags_config.h"
#include "config/commands_config.h"
#include "config/pinmap_config.h"
#include "config/general_config.h"
#include "commands/commands.h"
#include "states/idle.h"

#ifdef CONFIG_IDF_TARGET_ESP32S3
static constexpr int VSPI_BUS_NUM = 0;
static constexpr int HSPI_BUS_NUM = 1;
#else
static constexpr int VSPI_BUS_NUM = VSPI;
static constexpr int HSPI_BUS_NUM = HSPI;
#endif

System::System() : RicCoreSystem(Commands::command_map, Commands::defaultEnabledCommands, Serial),
                   canbus(systemstatus, PinMap::CAN_TX, PinMap::CAN_RX, 3), SDSPI(VSPI_BUS_NUM), primarysd(SDSPI, PinMap::SD_CS, SD_SCK_MHZ(20), true, &systemstatus) {}

void System::systemSetup()
{
    // Configure serial interface
    Serial.setRxBufferSize(GeneralConfig::SerialRxSize);
    Serial.begin(GeneralConfig::SerialBaud);

    // Initialise state machine (idle)
    statemachine.initalize(std::make_unique<Idle>(systemstatus, commandhandler));

    // Configure CAN bus interface
    canbus.setup();

    // Configure network manager
    networkmanager.setNodeType(NODETYPE::HUB);
    networkmanager.setNoRouteAction(NOROUTE_ACTION::BROADCAST, {1, 3});
    networkmanager.addInterface(&canbus);

    // Initialise SD
    setupSD();

    // Initialise loggers
    initializeLoggers();
}

void System::systemUpdate()
{
}

void System::setupSD()
{
    // Enable CS for SD
    pinMode(PinMap::SD_CS, OUTPUT);
    digitalWrite(PinMap::SD_CS, HIGH);

    // Initialise SPI for SD
    SDSPI.begin(PinMap::SD_SCLK, PinMap::SD_MISO, PinMap::SD_MOSI);
    SDSPI.setFrequency(SD_SCK_MHZ(20));
    SDSPI.setBitOrder(MSBFIRST);
    SDSPI.setDataMode(SPI_MODE0);

    // Initialise flash storage
    primarysd.setup();
}

void System::initializeLoggers()
{
    // Check if SD card is mounted
    if (primarysd.getState() != StoreBase::STATE::NOMINAL)
    {
        // Initialise system logger without file pointer
        loggerhandler.retrieve_logger<RicCoreLoggingConfig::LOGGERS::SYS>().initialize(nullptr, networkmanager);

        // Return
        return;
    }

    // Generate unique path for log files
    std::string log_directory_path = primarysd.generateUniquePath(log_path, "");

    // Create new log directory
    primarysd.mkdir(log_directory_path);

    // Open system log file
    std::unique_ptr<WrappedFile> syslogfile = primarysd.open(log_directory_path + "/syslog.txt", static_cast<FILE_MODE>(O_WRITE | O_CREAT | O_AT_END));

    // Initialise system log file
    loggerhandler.retrieve_logger<RicCoreLoggingConfig::LOGGERS::SYS>().initialize(std::move(syslogfile), networkmanager);
}
