//
//  LocationData.h
//  Location
//
//  Created by ajs on 5/08/19.
//  Copyright © 2019 ajs. All rights reserved.
//

#ifndef LocationData_H
#define LocationData_H

#include <string>
#include <vector>

#define EARTH_RADIUS 6.3781e6

struct Location
{
    std::string fullName;
    std::string suburb;
    uint32_t speedLimit;
    bool valid;
    bool operator==(const Location& l);
};

struct Point
{
    Point() : locationNumber(-1),latitudeDegrees(0), longitudeDegrees(0),
        latitude(0), longitude(0), x{1.0, 0, 0} {};
    Point(long ln, double latitudeDegrees, double longitudeDegrees);
	
	// Store latitude and longitude in both degrees and radians to assist in debugging, at the
	// cost of extra storage. The Cartesian co-ordinates (x,y,z) are also stored for use in the
	// minimum distance calculation - the ordering of straight line distances calculated using
	// these is the same as the ordering of great circle distances using latitude and longitude,
	// and means we can avoid using the haversine formula.
	long locationNumber;
    double latitudeDegrees;
    double longitudeDegrees;
    double latitude;
    double longitude;
    double x[3];
};

struct Segment
{
	// p1 and p2 are indexes into a points array for start and end points of a line segment
	// Store the location number here to avoid looking up by point
	long locationNumber;
	long p1;
	long p2;
	double bearing;
	double distance; // Distance from search point
};

typedef std::vector<Point>::iterator PointsIter;

class LocationData
{
public:
	std::vector<Location> locations;
	std::vector<Point> points;
	std::vector<Segment> segments;
	long locationNumber(Location& l);
	bool loadFile(const std::string& fileName);
};

#endif /* LocationData_H */
