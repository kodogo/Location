#include "LocationData.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include "Csv.h"
#include "json.h"
#include "utils.h"

using namespace std;
using json = nlohmann::json;

Point::Point(long ln, double latitudeDegrees, double longitudeDegrees)
{
    locationNumber = ln;
    this->latitudeDegrees = latitudeDegrees;
    this->longitudeDegrees = longitudeDegrees;

    latitude = latitudeDegrees * M_PI / 180.0;
    longitude = longitudeDegrees * M_PI / 180.0;
    x[0] = cos(longitude) * cos(latitude);
    x[1] = sin(longitude) * cos(latitude);
    x[2] = sin(latitude);
}

bool Location::operator==(const Location& l)
{
    return fullName == l.fullName &&
    suburb == l.suburb &&
    speedLimit==l.speedLimit;
};

long LocationData::locationNumber(Location& l)
{
	for(long i = 0; i < locations.size(); i++)
		if(l == locations[i])
			return i;
	locations.push_back(l);
	return locations.size()-1;
}

bool LocationData::loadFile(const std::string& fileName)
{
    // Reads the data from a file into the LocationData structures
    // Only minimal error checking is done because for the purposes of
    // this execise I am assuming that the data is in a CSV file with the
    // correct structure and that the data has been validated.
	ifstream roadData;
	string line;
	Csv csv;
	Location location;
	long ln;
	bool firstPoint;
	
	roadData.open(fileName);
    if(!roadData)
    {
        std::cout << "Could not open file " << fileName << "\n";
        return false;
    }
    // Read and discard the first line, assumed to contain field names
	getline(roadData, line);
    
    // Just wrap the whole input loop in a try catch block for now out
    // of pure laziness
    try {
        while (getline(roadData, line)) {
            csv.setline(line);
            if(csv.getfield(0) != "")
                location.fullName = csv.getfield(0);
                if(csv.getfield(1) != "")
                    location.suburb = csv.getfield(1);
            if(csv.getfield(2) != "")
                location.speedLimit = atol(csv.getfield(2).c_str());
            location.valid = true;
            ln = locationNumber(location);
            
            json j = json::parse(csv.getfield(3));
            auto coords = j["coordinates"].get<std::vector<std::vector<double>>>();
            firstPoint = true;
            for(auto& c : coords)
            {
                points.push_back(Point(ln, c[1], c[0]));
                // If this is not the first point for this line, add in a line segment from the previous point.
                // This assumes that the points come to us in order.
                if(!firstPoint)
                {
                    Segment s;
                    s.locationNumber = ln;
                    s.p2 = points.size() - 1;
                    s.p1 = s.p2 - 1;
                    s.bearing = Bearing(points[s.p1], points[s.p2]);
                    segments.push_back(s);
                }
                firstPoint = false;
            }
        }
        roadData.close();
        return true;
    } catch (exception& e) {
        std::cout << "An error occurred while processing the input file: " <<
        e.what() << "\n";
        return false;
    }
}
