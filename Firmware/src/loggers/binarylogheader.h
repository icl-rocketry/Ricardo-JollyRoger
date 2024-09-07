#pragma once

// Third-party imports
#include <librnp/rnp_serializer.h>
#include <librnp/rnp_packet.h>

class BinaryLogHeader
{
private:
    // Metadata
    const uint32_t index;
    const uint32_t millis;

    static constexpr auto getSerialiser()
    {
        // Return serialiser
        return RnpSerializer(
            &BinaryLogHeader::index,
            &BinaryLogHeader::millis);
    };

public:
    BinaryLogHeader(const uint32_t &index, const uint32_t &millis) : index(index), millis(millis) {}

    std::vector<uint8_t> serialise()
    {
        // Return serialised packet
        return getSerialiser().serialize(*this);
    }
};