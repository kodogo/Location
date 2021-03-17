#ifndef IREVERSE_GEOCODER_H_
#define IREVERSE_GEOCODER_H_

#include "HumanReadableLocation.h"

//-----------------------------------------------------------------------------
class IReverseGeocoder {
public:
    virtual ~IReverseGeocoder() { };

    virtual HumanReadableLocation ReverseGeocode(double latitude, double longitude) = 0;
    virtual HumanReadableLocation ReverseGeocode(double latitude, double longitude, double heading) = 0;
};

#endif //IREVERSE_GEOCODER_H_
