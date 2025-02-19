#pragma once

// Standard imports
#include <cstdint>

// Third-party imports
#include <librnp/rnp_serializer.h>
#include <librnp/rnp_packet.h>

class BinaryLogHeader
{
private:
    // Metadata
    const uint16_t bootCount;
    const uint32_t index;
    const uint32_t millis;

    /**
     * @brief Get the header serialiser
     *
     * @author Max Hallgarten La Casta
     *
     * @return constexpr auto Header serialiser
     */
    static constexpr auto getSerialiser()
    {
        // Return serialiser
        return RnpSerializer(
            &BinaryLogHeader::bootCount,
            &BinaryLogHeader::index,
            &BinaryLogHeader::millis);
    };

public:
    /**
     * @brief Construct a new Binary Log Header object
     *
     * @author Max Hallgarten La Casta
     *
     * @param[in] bootCount Boot count
     * @param[in] index Packet index
     * @param[in] millis System time since boot
     */
    BinaryLogHeader(const uint16_t &bootCount,
                    const uint32_t &index,
                    const uint32_t &millis)
        : bootCount(bootCount),
          index(index),
          millis(millis) {}

    /**
     * @brief Return the serialised header
     *
     * @author Max Hallgarten La Casta
     *
     * @return std::vector<uint8_t> Serialised header
     */
    std::vector<uint8_t> serialise()
    {
        // Return serialised packet
        return getSerialiser().serialize(*this);
    }
};