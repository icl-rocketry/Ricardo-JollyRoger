#pragma once

// Standard imports
#include <vector>

// Third-party imports
#include <librnp/rnp_packet.h>
#include <librnp/rnp_networkmanager.h>

// Internal imports
#include "config/services_config.h"
#include "probetask.h"

/**
 * @brief Prober
 *
 * @author Max Hallgarten La Casta
 */
class Prober
{

public:
    /**
     * @brief Construct a new prober
     *
     * @author Max Hallgarten La Casta
     *
     * @param[in] network_manager Network manager
     * @param[in] service_id Service
     */
    Prober(RnpNetworkManager &network_manager, const Services::ID &service_id);

    /**
     * @brief Construct a new prober
     *
     * @author Max Hallgarten La Casta
     *
     * @param[in] network_manager Network manager
     * @param[in] service_id Service
     * @param[in] tasks Probe tasks
     */
    Prober(RnpNetworkManager &network_manager, const Services::ID &service_id, std::vector<ProbeTask> tasks);

    /**
     * @brief Destroy the prober
     *
     * @author Max Hallgarten La Casta
     */
    ~Prober();

    /**
     * @brief Add task to prober
     *
     * @author Max Hallgarten La Casta
     *
     * @param[in] task Probe task
     */
    void addTask(ProbeTask task);

    /**
     * @brief Add task to prober
     *
     * @author Max Hallgarten La Casta
     *
     * @param[in] task Probe tasks
     */
    void addTask(std::vector<ProbeTask> task);

    /**
     * @brief Update prober
     *
     * @author Max Hallgarten La Casta
     */
    void update();

private:
    /**
     * @brief Process packet received via service
     *
     * @author Max Hallgarten La Casta
     *
     * @param[in] packet Packet
     */
    void service(RnpPacket &packet);

    /**
     * @brief Process packet received via packet capture
     *
     * @author Max Hallgarten La Casta
     *
     * @param[in] packet Packet
     */
    void receive(RnpPacket &packet);

    /**
     * @brief Update probe times
     *
     * @author Max Hallgarten La Casta
     *
     * @param[in] delta Probe time update delta [ms]
     */
    void updateProbeTime(const uint32_t &delta);

    /**
     * @brief Get the service callback
     *
     * @author Max Hallgarten La Casta
     *
     * @return PacketHandlerCb Service callback
     */
    PacketHandlerCb getServiceCallback();

    /**
     * @brief Get the packet capture callback
     *
     * @author Max Hallgarten La Casta
     *
     * @return PacketCaptureHandlerCb Packet capture callback
     */
    PacketCaptureHandlerCb getCaptureCallback();

private:
    /// @brief Network manager
    RnpNetworkManager &network_manager;

    /// @brief Probe tasks
    std::vector<ProbeTask> tasks;
};