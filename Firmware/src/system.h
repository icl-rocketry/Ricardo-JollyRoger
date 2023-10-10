#pragma once

#include <libriccore/riccoresystem.h>

#include "Config/systemflags_config.h"
#include "Config/commands_config.h"
#include "Config/pinmap_config.h"

#include "Commands/commands.h"

#include "Storage/sdfat_store.h"
#include <libriccore/networkinterfaces/canlogger/canlogger.h>


class System : public RicCoreSystem<System,SYSTEM_FLAG,Commands::ID>
{
    public:

        System();
        
        void systemSetup();

        void systemUpdate();

        CanLogger<SYSTEM_FLAG> canlogger;
        
        SPIClass FLASHSPI;
        SdFat_Store flash;

    private:

        void initializeLogger();
        void logData();
        const std::string log_path = "/Logs";


};