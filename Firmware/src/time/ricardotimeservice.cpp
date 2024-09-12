#include "ricardotimeservice.h"

// Standard imports
#include <sys/time.h>

// Third-party imports
#include <libriccore/platform/millis.h>

RicardoTimeService::RicardoTimeService(RnpNetworkManager &networkmanager, const uint8_t &service) : networkmanager(networkmanager)
{
    // Register service with network manager
    networkmanager.registerService(service, &RicardoTimeService::simpleTimeUpdate);
};

uint32_t RicardoTimeService::getMillis()
{
    // Return time from boot in milliseconds
    return millis();
}

int64_t RicardoTimeService::getEpochMillis()
{
    // Get time
    // NOTE: epoch times are susceptible to Y2K38 bug
    timeval time;
    gettimeofday(&time, nullptr);

    // Return time from epoch in milliseconds
    return timevalToMillis(time);
}

void RicardoTimeService::setEpochMillis(const int64_t &epochMillis)
{
    // Get current system time
    const int64_t oldEpochMillis = getEpochMillis();

    // Calculate difference between times
    const int64_t deltaMillis = epochMillis - oldEpochMillis;

    // Step the time if threshold exceeded (currently 10 minutes)
    if (abs(deltaMillis) > 600000LL)
    {
        // Create updated time structure
        const timeval epoch = millisToTimeval(epochMillis);

        // Set time
        settimeofday(&epoch, nullptr);

        // Return
        return;
    }

    // Generate delta structure
    // TODO: consider negative deltas
    timeval delta;
    delta.tv_sec = static_cast<time_t>(deltaMillis / 1000LL);
    delta.tv_usec = static_cast<suseconds_t>((deltaMillis * 1000LL) % 1000000LL);

    // Adjust time
    adjtime(&delta, nullptr);
}

int64_t RicardoTimeService::timevalToMillis(const timeval &time)
{
    // Return time in milliseconds
    return static_cast<int64_t>(time.tv_sec) * 1000LL + static_cast<int64_t>(time.tv_usec) / 1000LL;
}

timeval RicardoTimeService::millisToTimeval(const int64_t &millis)
{
    // Create time structure
    timeval time;
    time.tv_sec = static_cast<time_t>(millis / 1000LL);
    time.tv_usec = static_cast<suseconds_t>((millis * 1000LL) % 1000000LL);

    // Return time structure
    return time;
}

void RicardoTimeService::simpleTimeUpdate(packetptr_t packet)
{
    try
    {
        // Generate command packet
        SimpleCommandPacket command(*packet);

        // Get current time
        const int64_t oldEpochMillis = getEpochMillis();

        // Extract time
        const int64_t epochMillis = static_cast<int64_t>(command.arg) * 1000LL;

        // Set time
        setEpochMillis(epochMillis);

        // Calculate change in time
        const int64_t deltaMillis = epochMillis - oldEpochMillis;

        // Log update
        std::string message = "Previous time (s): " + std::to_string(oldEpochMillis / 1000LL) + " New time (s): " + std::to_string(epochMillis / 1000LL) + " Delta (s): " + std::to_string(deltaMillis / 1000LL);
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>(message);
    }
    catch (std::exception &e)
    {
        return;
    }
}