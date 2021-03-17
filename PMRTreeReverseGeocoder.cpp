#include "PMRTreeReverseGeocoder.h"
#include <limits>
#include <iostream>
#include <cmath>
#include "utils.h"

PMRTreeReverseGeocoder::PMRTreeReverseGeocoder() {
}

PMRTreeReverseGeocoder::PMRTreeReverseGeocoder(LocationData& ld)
{
    locationData = ld;
    pmrTree.SetLocationData(locationData);
}

HumanReadableLocation PMRTreeReverseGeocoder::ReverseGeocode(double latitude, double longitude) {
    Location l = NearestLocation(latitude, longitude);
    return HumanReadableLocation(l.fullName, l.suburb, l.speedLimit, l.valid);
}

HumanReadableLocation PMRTreeReverseGeocoder::ReverseGeocode(double latitude, double longitude, double headingDegrees) {
    Location l = NearestLocationWithBearing(latitude, longitude, headingDegrees);
    return HumanReadableLocation(l.fullName, l.suburb, l.speedLimit, l.valid);
}

Location PMRTreeReverseGeocoder::NearestLocation(double latitudeDegrees, double longitudeDegrees)
{
	Point target(0, latitudeDegrees, longitudeDegrees);
    std::vector<long> nearbySegments = pmrTree.NearbySegments(target, 50.0);
    Location closestLocation{"","",0,false};
    if(nearbySegments.size() > 0)
    {
        Segment s = locationData.segments[nearbySegments[0]];
        closestLocation = locationData.locations[s.locationNumber];
    }
	return closestLocation;
}

Location PMRTreeReverseGeocoder::NearestLocationWithBearing(double latitudeDegrees, double longitudeDegrees, double bearing)
{
    Point target(0, latitudeDegrees, longitudeDegrees);
    std::vector<long> nearbySegments = pmrTree.NearbySegments(target, 50.0);

    long closest = -1;
    Location closestLocation{"","",0,false};

	// Now pick the segment whose bearing most closely matches the value we were given
    double diffMin = std::numeric_limits<double>::max();
    for(auto segmentNumber:nearbySegments)
    {
        Segment s = locationData.segments[segmentNumber];
        double diff1 = fabs(bearing - NormalisedAngle(s.bearing));
        double diff2 = fabs(bearing - NormalisedAngle(s.bearing + 180.0));
        double diff = std::min(diff1, diff2);
        if(diff < diffMin)
        {
            diffMin = diff;
            closest = s.locationNumber;
        }

    }
    if(closest >= 0)
        closestLocation = locationData.locations[closest];

	return closestLocation;
}
