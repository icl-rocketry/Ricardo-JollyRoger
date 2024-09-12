#pragma once

// Standard imports
#include <cstdint>
#include <vector>

// Third-party imports
#include <librnp/rnp_packet.h>
#include <librnp/rnp_serializer.h>

class BlackBoxTelemetryPacket : public RnpPacket
{
private:
    /**
     * @brief Get the Serializer object
     *
     * @author Max Hallgarten La Casta
     *
     * @return constexpr auto Packet payload serialiser
     */
    static constexpr auto getSerializer()
    {
        return RnpSerializer(
            &BlackBoxTelemetryPacket::millis,
            &BlackBoxTelemetryPacket::epochMillis,
            &BlackBoxTelemetryPacket::state,
            &BlackBoxTelemetryPacket::totalSpace,
            &BlackBoxTelemetryPacket::usedSpace);
    }

public:
    /**
     * @brief Construct a new Blackbox Telemetry Packet
     *
     * @author Max Hallgarten La Casta
     *
     * @param[in] millis Time since boot
     * @param[in] epochMillis Time since epoch
     * @param[in] state SD card state
     * @param[in] totalSpace SD card total space (in mebibytes)
     * @param[in] usedSpace SD card used space (in mebibytes)
     */
    BlackBoxTelemetryPacket(const uint64_t &millis, const int64_t &epochMillis, const uint8_t &state, const uint32_t &totalSpace, const uint32_t &usedSpace);

    /**
     * @brief Destroy the Blackbox Telemetry Packet
     *
     * @author Max Hallgarten La Casta
     */
    ~BlackBoxTelemetryPacket();

    /**
     * @brief Serialise the packet
     *
     * @author Max Hallgarten La Casta
     *
     * @param[out] buf Packet buffer
     */
    void serialize(std::vector<uint8_t> &buf) override;

    /// @brief Packet type
    static constexpr uint8_t packetType = 200U;

    /**
     * @brief Size of payload when serialised
     *
     * @author Max Hallgarten La Casta
     *
     * @return constexpr size_t Payload size
     */
    static constexpr size_t size()
    {
        return getSerializer().member_size();
    }

    /// @brief Time since boot
    const uint64_t millis;

    /// @brief Time since epoch
    const int64_t epochMillis;

    /// @brief SD card state
    const uint8_t state;

    /// @brief SD card total space (in mebibytes)
    const uint32_t totalSpace;

    /// @brief SD card used space (in mebibytes)
    const uint32_t usedSpace;
};