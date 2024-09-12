#include "sdfat_store.h"

// Third-party imports
#include <SPI.h>
#include <SdFat.h>
#include <libriccore/storage/storebase.h>
#include <libriccore/riccoretypes.h>
#include <libriccore/riccorelogging.h>

// Internal imports
#include "sdfat_file.h"
#include "config/types.h"
#include "config/systemflags_config.h"

SdFat_Store::SdFat_Store(SPIClass &spi, const uint8_t cs, const uint32_t frequency, RicCoreThread::Lock_t &spiBusLock, bool dedicatedSPI, Types::CoreTypes::SystemStatus_t *systemstatus) : StoreBase(spiBusLock),
                                                                                                                                                                                            _spi(spi),
                                                                                                                                                                                            _config(cs, static_cast<uint8_t>(dedicatedSPI), frequency, &spi),
                                                                                                                                                                                            _systemstatus(systemstatus)
{
}

SdFat_Store::SdFat_Store(SPIClass &spi, const uint8_t cs, const uint32_t frequency, bool dedicatedSPI, Types::CoreTypes::SystemStatus_t *systemstatus) : SdFat_Store(spi, cs, frequency, dummyLock, dedicatedSPI, systemstatus)
{
}

void SdFat_Store::setup()
{
    if (!filesys.begin(_config))
    {
        // _systemstatus->newFlag(SYSTEM_FLAG::ERROR_)
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Error while begining filesys");
        _storeState = STATE::ERROR_SETUP;
    }
}

uint32_t SdFat_Store::getTotalSpace()
{
    // Return zero if the volume is not mounted
    if (getState() != STATE::NOMINAL)
    {
        return 0U;
    }

    // Get number of sectors (each sector being 512 bytes)
    const uint32_t totalSectorCount = filesys.vol()->clusterCount() * filesys.vol()->sectorsPerCluster();

    // Return total space (in mebibytes)
    return totalSectorCount / sectorsPerMiB;
}

uint32_t SdFat_Store::getFreeSpace()
{
    // Return zero if the volume is not mounted
    if (getState() != STATE::NOMINAL)
    {
        return 0U;
    }

    // Return zero
    return 0U;

    // // Get number of free clusters
    // const int32_t freeClusterCount = filesys.vol()->freeClusterCount();

    // // Return zero if error occurs
    // if (freeClusterCount < static_cast<int32_t>(0))
    // {
    //     return 0U;
    // }

    // // Get number of free sectors (each sector being 512 bytes)
    // const uint32_t freeSectorCount = static_cast<uint32_t>(freeClusterCount) * filesys.vol()->sectorsPerCluster();

    // // Return free space (in mebibytes)
    // return freeSectorCount / sectorsPerMiB;
}

uint32_t SdFat_Store::getUsedSpace()
{
    // Return used space (in mebibytes)
    return getTotalSpace() - getFreeSpace();
}

std::unique_ptr<WrappedFile> SdFat_Store::_open(std::string_view path, store_fd fileDesc, FILE_MODE mode, size_t maxQueueSize)
{
    return std::make_unique<SdFat_WrappedFile>(filesys.open(std::string(path).c_str(), static_cast<oflag_t>(mode)), fileDesc, *this, mode, maxQueueSize);
}

bool SdFat_Store::_ls(std::string_view path, std::vector<directory_element_t> &directory_structure)
{
    FsFile _file;
    _file = filesys.open(std::string(path).c_str()); // open supplied path

    if ((!_file) || (!_file.isDirectory()))
    {
        // path invalid so return

        return false;
    }

    FsFile child = _file.openNextFile(); // open next file in directory

    while (child)
    {                              // while child file is valid
        directory_element_t entry; // create new entry for directory list
        char filename[64];
        child.getName(filename, sizeof(filename)); // get file name
        entry.name = std::string{filename};        // convert char array to string and pass into entry structure
        // check if child is directory
        if (child.isDirectory())
        {
            entry.type = FILE_TYPE::DIRECTORY;
        }
        else
        {
            entry.type = FILE_TYPE::FILE;
        }
        entry.size = child.size(); // get file size

        directory_structure.push_back(entry); // add entry to vector
        child = _file.openNextFile();         // open next file
    }
    return true;
}

bool SdFat_Store::_mkdir(std::string_view path)
{
    return filesys.mkdir(std::string(path).c_str());
}

// Removes a file or an empty directory
bool SdFat_Store::_remove(std::string_view path)
{
    return filesys.remove(std::string(path).c_str());
}