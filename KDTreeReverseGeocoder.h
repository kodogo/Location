#ifndef KDREVERSE_GEOCODER_H_
#define KDREVERSE_GEOCODER_H_

#include "IReverseGeocoder.h"
#include "LocationData.h"
#include "KDTree.h"

//-----------------------------------------------------------------------------
class KDTreeReverseGeocoder : public IReverseGeocoder {
public:
    KDTreeReverseGeocoder();
	// Pass the location data by value - when the tree is constructed, the points array is repeatedly
	// sorted in order to find the median, which the calling routine may not want to affect the value it
	// passes in
    KDTreeReverseGeocoder(LocationData ld);

    HumanReadableLocation ReverseGeocode(double latitude, double longitude);
    HumanReadableLocation ReverseGeocode(double latitude, double longitude, double headingDegrees);
private:
    KDTree kdTree;
};

#endif //KDREVERSE_GEOCODER_H_
