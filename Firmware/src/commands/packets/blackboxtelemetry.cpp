#include "blackboxtelemetry.h"

// Internal imports
#include "utilities/vector.h"

BlackBoxTelemetryPacket::BlackBoxTelemetryPacket(const uint64_t &millis, const int64_t &epochMillis, const uint8_t &state, const uint32_t &totalSpace, const uint32_t &usedSpace) : RnpPacket(0, packetType, size()), millis(millis), epochMillis(epochMillis), state(state), totalSpace(totalSpace), usedSpace(usedSpace) {}

BlackBoxTelemetryPacket::~BlackBoxTelemetryPacket() {}

void BlackBoxTelemetryPacket::serialize(std::vector<uint8_t> &buffer)
{
    // Serialise packet header onto buffer
    RnpPacket::serialize(buffer);

    // Serialise packet payload onto buffer
    append(buffer, getSerializer().serialize(*this));
}