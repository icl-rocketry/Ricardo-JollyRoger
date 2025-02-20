#pragma once

// Standard imports
#include <cstdint>

// Third-party imports
#include <librnp/rnp_packet.h>
#include <librnp/rnp_networkmanager.h>

/**
 * @brief Probe task
 *
 * @author Max Hallgarten La Casta
 */
class ProbeTask
{

public:
    /**
     * @brief Construct a new probe task
     *
     * @author Max Hallgarten La Casta
     *
     * @param[in] source Source address
     * @param[in] destination Destination address
     * @param[in] destination_service Destination service
     * @param[in] command_id Command identifier
     * @param[in] command_arg Command argument
     * @param[in] poll_delta Polling interval [ms]
     */
    ProbeTask(const uint8_t &source,
              const uint8_t &destination,
              const uint8_t &destination_service,
              const uint8_t &command_id,
              const int32_t &command_arg,
              const uint32_t &poll_delta);

    /**
     * @brief Destroy the probe task
     *
     * @author Max Hallgarten La Casta
     */
    ~ProbeTask();

    /**
     * @brief Update the probe task
     *
     * @author Max Hallgarten La Casta
     *
     * @param[inout] network_manager Network manager
     */
    void update(RnpNetworkManager &network_manager);

    /**
     * @brief Send probe
     *
     * @author Max Hallgarten La Casta
     *
     * @param[inout] network_manager Network manager
     */
    void send(RnpNetworkManager &network_manager);

    /**
     * @brief Receive packet
     *
     * Updates probe time if the packet matches the probe task configuration
     *
     * @author Max Hallgarten La Casta
     *
     * @param[in] packet
     */
    void receive(RnpPacket &packet);

    /**
     * @brief Update probe time (by task's polling interval)
     *
     * @author Max Hallgarten La Casta
     */
    void updateProbeTime();

    /**
     * @brief Update probe time
     *
     * @author Max Hallgarten La Casta
     *
     * @param[in] delta Probe time update delta [ms]
     */
    void updateProbeTime(const uint32_t &delta);

private:
    /// @brief Source address
    const uint8_t source;

    /// @brief Destination address
    const uint8_t destination;

    /// @brief Destination service
    const uint8_t destination_service;

    /// @brief Command identifier
    const uint8_t command_id;

    /// @brief Command argument
    const int32_t command_arg;

    /// @brief Poll delta [ms]
    const uint32_t poll_delta;

    /// @brief Time (since boot) of the next boot [ms]
    uint32_t probeTime;
};