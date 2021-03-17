#ifndef BFREVERSE_GEOCODER_H_
#define BFREVERSE_GEOCODER_H_

#include "IReverseGeocoder.h"
#include "LocationData.h"

//-----------------------------------------------------------------------------
class BruteForceReverseGeocoder : public IReverseGeocoder {
public:
    BruteForceReverseGeocoder();
    BruteForceReverseGeocoder(LocationData& ld);

    HumanReadableLocation ReverseGeocode(double latitude, double longitude);
    HumanReadableLocation ReverseGeocode(double latitude, double longitude, double headingDegrees);
private:
	LocationData locationData;
	std::vector<Point> points;
	Location NearestLocation(double latitudeDegrees, double longitudeDegrees);
};

#endif //BFREVERSE_GEOCODER_H_
