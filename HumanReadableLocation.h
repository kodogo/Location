#ifndef HUMAN_READABLE_LOCATION_H_
#define HUMAN_READABLE_LOCATION_H_

#include <string>

//-----------------------------------------------------------------------------
class HumanReadableLocation {
public:
    HumanReadableLocation(const std::string& Location, const std::string& Suburb, uint32_t Speed, bool valid);

    std::string FormattedLocation() const; //Fully formated location for text-to-speech application
    std::string Street() const;
    std::string Suburb() const;
    uint32_t SpeedKph() const;
    bool Valid() const;
private:
    std::string location;
    std::string suburb;
    uint32_t speed;
    bool valid;
};

#endif //HUMAN_READABLE_LOCATION_H_
