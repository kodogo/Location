#include "HumanReadableLocation.h"

HumanReadableLocation::HumanReadableLocation(const std::string& Location, const std::string& Suburb, uint32_t Speed, bool Valid) : location(Location),
suburb(Suburb), speed(Speed), valid(Valid)
{
}

std::string HumanReadableLocation::FormattedLocation() const {
    return location + ", " + suburb;
}

std::string HumanReadableLocation::Street() const {
    return location;
}

std::string HumanReadableLocation::Suburb() const {
    return suburb;
}

uint32_t HumanReadableLocation::SpeedKph() const {
    return speed;
}

bool HumanReadableLocation::Valid() const {
    return valid;
}
