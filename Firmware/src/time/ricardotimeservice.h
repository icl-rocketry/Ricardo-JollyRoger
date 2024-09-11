#pragma once

// Standard imports
#include <cstdint>
#include <sys/time.h>

// Third-party imports
#include <librnp/rnp_networkmanager.h>
#include <librnp/default_packets/simplecommandpacket.h>
#include <libriccore/riccorelogging.h>

class RicardoTimeService
{
public:
    /**
     * @brief Construct a new Ricardo Time Service object
     *
     * @author Max Hallgarten La Casta
     *
     * @param[in] networkmanager Network manager
     * @param[in] service Registered port
     */
    RicardoTimeService(RnpNetworkManager &networkmanager, const uint8_t &service = 123);

    /**
     * @brief Get the current time
     *
     * @author Max Hallgarten La Casta
     *
     * @return int64_t Current time (milliseconds since epoch)
     */
    static int64_t getEpochMillis();

    /**
     * @brief Set the current time
     *
     * @author Max Hallgarten La Casta
     *
     * @param[in] epochMillis Current time (milliseconds since epoch)
     */
    static void setEpochMillis(const int64_t &epochMillis);

private:
    RnpNetworkManager &networkmanager;

    /**
     * @brief Convert a time structure to milliseconds
     *
     * @author Max Hallgarten La Casta
     *
     * @param[in] time Time
     * @return int64_t Time
     */
    static int64_t timevalToMillis(const timeval &time);

    /**
     * @brief Convert milliseconds to a time structure
     *
     * @author Max Hallgarten La Casta
     *
     * @param[in] millis Time
     * @return timeval Time
     */
    static timeval millisToTimeval(const int64_t &millis);

    /**
     * @brief Callback to update system time with a simple time packet
     *
     * @author Max Hallgarten La Casta
     *
     * @todo Develop more complete time synchronisation service
     *
     * @param[in] packet Command packet pointer
     */
    static void simpleTimeUpdate(packetptr_t packet);
};