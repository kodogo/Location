#include "SegmentReverseGeocoder.h"
#include <limits>
#include <iostream>
#include <cmath>
#include "utils.h"

SegmentReverseGeocoder::SegmentReverseGeocoder() {
}

SegmentReverseGeocoder::SegmentReverseGeocoder(LocationData& ld)
{
    // Just copy all of the location data. Wasteful of space, but not a concern at this point.
	locationData = ld;
 }

HumanReadableLocation SegmentReverseGeocoder::ReverseGeocode(double latitude, double longitude) {
    Location l = NearestLocation(latitude, longitude);
    return HumanReadableLocation(l.fullName, l.suburb, l.speedLimit, l.valid);
}

HumanReadableLocation SegmentReverseGeocoder::ReverseGeocode(double latitude, double longitude, double headingDegrees) {
    Location l = NearestLocationWithBearing(latitude, longitude, headingDegrees);
    return HumanReadableLocation(l.fullName, l.suburb, l.speedLimit, l.valid);
}


Location SegmentReverseGeocoder::NearestLocation(double latitudeDegrees, double longitudeDegrees)
{
	Point target(0, latitudeDegrees, longitudeDegrees);
	long closest = -1;
    Location closestLocation{"","",0,false};
	double minDist = std::numeric_limits<double>::max();

	for(auto s:locationData.segments)
	{
		Point p1 = locationData.points[s.p1];
		Point p2 = locationData.points[s.p2];
		s.distance = PointToSegment(target.x, p1.x, p2.x);
		if(s.distance < minDist) {
			closest = s.locationNumber;
			minDist = s.distance;
		}
	}
    if(closest >= 0)
        closestLocation = locationData.locations[closest];
	return closestLocation;
}

Location SegmentReverseGeocoder::NearestLocationWithBearing(double latitudeDegrees, double longitudeDegrees, double bearing)
{
	std::vector<Segment> nearbySegments;
	Point target(0, latitudeDegrees, longitudeDegrees);
	long closest = -1;
    Location closestLocation{"","",0,false};

	//for(auto s:locationData.segments)
	for(int i = 0; i < locationData.segments.size(); i++)
	{
		auto s = locationData.segments[i];
		Point p1 = locationData.points[s.p1];
		Point p2 = locationData.points[s.p2];
		s.distance = PointToSegment(target.x, p1.x, p2.x) * EARTH_RADIUS;
        locationData.segments[i].distance = s.distance;
		// Fairly arbitrary choice of 50m
		if(s.distance < 50) {
			nearbySegments.push_back(s);
		}
	}
	
	// Now pick the segment whose bearing most closely matches the value we were given
    double diffMin = std::numeric_limits<double>::max();
    for(auto s:nearbySegments)
    {
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
