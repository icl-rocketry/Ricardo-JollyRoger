#include "system.h"

#include <memory>

#include <libriccore/riccoresystem.h>

#include <HardwareSerial.h>

#include "Config/systemflags_config.h"
#include "Config/commands_config.h"
#include "Config/pinmap_config.h"
#include "Config/general_config.h"

#include "Commands/commands.h"

#include "States/idle.h"


#ifdef CONFIG_IDF_TARGET_ESP32S3
static constexpr int VSPI_BUS_NUM = 0;
static constexpr int HSPI_BUS_NUM = 1;
#else
static constexpr int VSPI_BUS_NUM = VSPI;
static constexpr int HSPI_BUS_NUM = HSPI;
#endif

System::System():
RicCoreSystem(Commands::command_map,Commands::defaultEnabledCommands,Serial),
                canlogger(systemstatus, PinMap::TxCan, PinMap::RxCan, 4),
                FLASHSPI(VSPI_BUS_NUM),
                flash(FLASHSPI, PinMap::CS, SD_SCK_MHZ(20),false,&systemstatus)
{};


void System::systemSetup(){
    
    Serial.setRxBufferSize(GeneralConfig::SerialRxSize);
    Serial.begin(GeneralConfig::SerialBaud);
   
    //initialize statemachine with idle state
    statemachine.initalize(std::make_unique<Idle>(systemstatus,commandhandler));

    //any other setup goes here
    canlogger.setup();
    networkmanager.setNodeType(NODETYPE::HUB);
    networkmanager.setNoRouteAction(NOROUTE_ACTION::BROADCAST, {1, 4});
    networkmanager.addInterface(&canlogger);

    pinMode(PinMap::CS, OUTPUT);
    digitalWrite(PinMap::CS, HIGH);

    FLASHSPI.begin(PinMap::SCLK,PinMap::MISO,PinMap::MOSI);
    FLASHSPI.setFrequency(5000000);
    FLASHSPI.setBitOrder(MSBFIRST);
    FLASHSPI.setDataMode(SPI_MODE0);

    flash.setup();
    initializeLogger();
};

long prevTime = 0;

std::vector<directory_element_t> dirstructure;

void System::systemUpdate(){

    if(millis()-prevTime > 1000){
    flash.ls(dirstructure);
    Serial.println(dirstructure.size());
    for (auto& elem : dirstructure)
    {
        std::string elemdata = elem.name + " Size: " + std::to_string(elem.size) + " Type: " + std::to_string((int)(elem.type));
        Serial.println(elemdata.c_str());
    }
    }
};

void System::initializeLogger()
{
    // check if sd card is mounted
    if (flash.getState() != StoreBase::STATE::NOMINAL)
    {
        loggerhandler.retrieve_logger<RicCoreLoggingConfig::LOGGERS::SYS>().initialize(nullptr, networkmanager);

        return;
    }

    // open log files
    // get unique directory for logs
    std::string log_directory_path = flash.generateUniquePath(log_path, "");
    // make new directory
    flash.mkdir(log_directory_path);

    std::unique_ptr<WrappedFile> syslogfile = flash.open(log_directory_path + "/syslog.txt", static_cast<FILE_MODE>(O_WRITE | O_CREAT | O_AT_END));
    std::unique_ptr<WrappedFile> telemetrylogfile = flash.open(log_directory_path + "/telemetrylog.txt", static_cast<FILE_MODE>(O_WRITE | O_CREAT | O_AT_END),50);

    // intialize sys logger
    loggerhandler.retrieve_logger<RicCoreLoggingConfig::LOGGERS::SYS>().initialize(std::move(syslogfile), networkmanager);

    // initialize telemetry logger
    loggerhandler.retrieve_logger<RicCoreLoggingConfig::LOGGERS::TELEMETRY>().initialize(std::move(telemetrylogfile),[](std::string_view msg){RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>(msg);});

}