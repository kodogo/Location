#ifndef SEGMENTREVERSE_GEOCODER_H_
#define SEGMENTREVERSE_GEOCODER_H_

#include "IReverseGeocoder.h"
#include "LocationData.h"

//-----------------------------------------------------------------------------
class SegmentReverseGeocoder : public IReverseGeocoder {
public:
    SegmentReverseGeocoder();
    SegmentReverseGeocoder(LocationData& ld);

    HumanReadableLocation ReverseGeocode(double latitude, double longitude);
    HumanReadableLocation ReverseGeocode(double latitude, double longitude, double headingDegrees);
private:
	LocationData locationData;
	Location NearestLocation(double latitudeDegrees, double longitudeDegrees);
	Location NearestLocationWithBearing(double latitudeDegrees, double longitudeDegrees, double bearing);
};

#endif //SEGMENTREVERSE_GEOCODER_H_
