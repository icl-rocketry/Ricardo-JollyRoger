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
    const uint32_t index;
    const uint32_t millis;
    const int64_t epochMillis;

    static constexpr auto getSerialiser()
    {
        // Return serialiser
        return RnpSerializer(
            &BinaryLogHeader::index,
            &BinaryLogHeader::millis,
            &BinaryLogHeader::epochMillis);
    };

public:
    BinaryLogHeader(const uint32_t &index, const uint32_t &millis, const int64_t &epochMillis) : index(index), millis(millis), epochMillis(epochMillis) {}

    std::vector<uint8_t> serialise()
    {
        // Return serialised packet
        return getSerialiser().serialize(*this);
    }
};