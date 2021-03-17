#include "KDTreeReverseGeocoder.h"
#include <limits>
#include <iostream>

KDTreeReverseGeocoder::KDTreeReverseGeocoder()
{
}

KDTreeReverseGeocoder::KDTreeReverseGeocoder(LocationData ld)
{
    kdTree.SetLocationData(ld);
}

HumanReadableLocation KDTreeReverseGeocoder::ReverseGeocode(double latitude, double longitude) {
    Location l = kdTree.NearestLocation(latitude, longitude);
    return HumanReadableLocation(l.fullName, l.suburb, l.speedLimit, l.valid);
}

HumanReadableLocation KDTreeReverseGeocoder::ReverseGeocode(double latitude, double longitude, double headingDegrees) {
    Location l = kdTree.NearestLocation(latitude, longitude);
    return HumanReadableLocation(l.fullName, l.suburb, l.speedLimit, l.valid);
}
