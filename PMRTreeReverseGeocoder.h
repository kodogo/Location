#ifndef PMRTREEREVERSE_GEOCODER_H_
#define PMRTREEREVERSE_GEOCODER_H_

#include "IReverseGeocoder.h"
#include "PMRQuadTree.h"

//-----------------------------------------------------------------------------
class PMRTreeReverseGeocoder : public IReverseGeocoder {
public:
    PMRTreeReverseGeocoder();
    PMRTreeReverseGeocoder(LocationData& ld);

    HumanReadableLocation ReverseGeocode(double latitude, double longitude);
    HumanReadableLocation ReverseGeocode(double latitude, double longitude, double headingDegrees);
private:
    PMRQuadTree pmrTree;
    LocationData locationData;
    Location NearestLocation(double latitudeDegrees, double longitudeDegrees);
    Location NearestLocationWithBearing(double latitudeDegrees, double longitudeDegrees, double bearing);

};

#endif //PMRTREEREVERSE_GEOCODER_H_
