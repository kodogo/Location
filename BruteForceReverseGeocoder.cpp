#include "BruteForceReverseGeocoder.h"
#include <limits>
#include <iostream>
#include "utils.h"

BruteForceReverseGeocoder::BruteForceReverseGeocoder() {
}

BruteForceReverseGeocoder::BruteForceReverseGeocoder(LocationData& ld)
{
    // Just copy all of the location data. Wasteful of space, but not a concern at this point.
	locationData = ld;
    //locationData.locations = ld.locations;
	//locationData.points = ld.points;
}

HumanReadableLocation BruteForceReverseGeocoder::ReverseGeocode(double latitude, double longitude) {
    Location l = NearestLocation(latitude, longitude);
    return HumanReadableLocation(l.fullName, l.suburb, l.speedLimit, l.valid);
}

HumanReadableLocation BruteForceReverseGeocoder::ReverseGeocode(double latitude, double longitude, double headingDegrees) {
    Location l = NearestLocation(latitude, longitude);
    return HumanReadableLocation(l.fullName, l.suburb, l.speedLimit, l.valid);
}

Location BruteForceReverseGeocoder::NearestLocation(double latitudeDegrees, double longitudeDegrees)
{
    Point target(0, latitudeDegrees, longitudeDegrees);
    Point closest;
    double dsqmin = std::numeric_limits<double>::max();
    double dsq;
    for(auto p:locationData.points)
    {
        dsq = SquaredDistance(p, target);
        if(dsq < dsqmin) {
            closest = p;
            dsqmin = dsq;
        }
    }
    Location l = locationData.locations[closest.locationNumber];
    return l;
}

