#include "utils.h"
#include <cmath>

double SquaredDistance(Point& p1, Point& p2)
{
    // Returns the square of the Euclidean distance between two points.
    // "Euclidean" to distinguish it from the geodesic distance on
    // the surface of the sphere
    double dsq = 0;
    for(int i = 0; i < 3; i++)
    {
        double d = p1.x[i] - p2.x[i];
        dsq += (d * d);
    }
    return dsq;
}

double Distance(double *x1, double *x2)
{
	double dsq = 0;
	for(int i = 0; i < 3; i++)
	{
		double d = x1[i] - x2[i];
		dsq += (d * d);
	}
	return sqrt(dsq);

}
double PointToSegment(double *x0, double *x1, double *x2)
{
    // Calculate the straight line perpendicular distance from a point to a line
    // Algorithm and notation from Wolfram
    // http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
    
    double xdiff1[3];
    double xdiff2[3];
    double xcross[3];
	double d = 0.0;
	
    vecdiff(x1, x0, xdiff1);
    vecdiff(x2, x1, xdiff2);
    
    double t = -(dotProduct(xdiff1, xdiff2)/dotProduct(xdiff2, xdiff2));
    if(t < 0)
	{
		// Calculate distance to x1
		d = Distance(x0, x1);
	}
	else if(t > 1.0)
	{
		// Calculate distance to x2
		d = Distance(x0, x2);
	}
	else
	{
    	crossProduct(xdiff2, xdiff1, xcross);
		d = sqrt(dotProduct(xcross, xcross))/sqrt(dotProduct(xdiff2, xdiff2));
	}
	return d;
}

double dotProduct(double *a, double *b)
{
    double dprod = 0;
    for(int i = 0; i < 3; i++)
        dprod += (a[i] * b[i]);
    return dprod;
}

void crossProduct(double *a, double *b, double *c)
{
    c[0] = (a[1]*b[2]) - (a[2]*b[1]);
    c[1] = (a[2]*b[0]) - (a[0]*b[2]);
    c[2] = (a[0]*b[1]) - (a[1]*b[0]);
    
}

void vecdiff(double *a, double *b, double *c)
{
    for(int i = 0; i < 3; i++)
        c[i] = a[i] - b[i];
}

double NormalisedAngle(double angle)
{
    // Normalises an angle to fall in the range 0 - 360
    double na = angle;
    while(na < 0)
        na += 360;
    while(na >=360)
        na -= 360;
    return na;
}

double Bearing(Point& p1, Point& p2)
{
    // Calculate bearing from point 1 to point 2 in degrees, normalised to
    // the range 0 - 360
    double dlong = p2.longitude - p1.longitude;
    double x = cos(p2.latitude) * sin(dlong);
    double y = cos(p1.latitude)*sin(p2.latitude) - sin(p1.latitude)*cos(p2.latitude)*cos(dlong);
    double b = atan2(x, y);
    b *= (180.0 / M_PI);
    return NormalisedAngle(b);
}

bool LinesIntersect(Point2D p1, Point2D p2, Point2D p3, Point2D p4)
{
    // Code taken pretty much directly from "Mastering Algorithms with C" by Kyle Loudon
   double z1, z2, z3, z4;
    int s1, s2, s3, s4;

    // Perform the quick rejection test.
    if (!(std::max(p1.x, p2.x) >= std::min(p3.x, p4.x) && std::max(p3.x, p4.x)
          >= std::min(p1.x, p2.x) && std::max(p1.y, p2.y) >= std::min(p3.y, p4.y)
          && std::max(p3.y, p4.y) >= std::min(p1.y, p2.y)))
    {
        return false;
    }
        
    // Determine whether the line segments straddle each other.
    if ((z1 = ((p3.x - p1.x)*(p2.y - p1.y)) - ((p3.y - p1.y)*(p2.x - p1.x))) < 0)
        s1 = -1;
    else if (z1 > 0)
        s1 = 1;
    else
        s1 = 0;
    
    if ((z2 = ((p4.x - p1.x)*(p2.y - p1.y)) - ((p4.y - p1.y)*(p2.x - p1.x))) < 0)
        s2 = -1;
    else if (z2 > 0)
        s2 = 1;
    else
        s2 = 0;
    
    if ((z3 = ((p1.x - p3.x)*(p4.y - p3.y)) - ((p1.y - p3.y)*(p4.x - p3.x))) < 0)
        s3 = -1;
    else if (z3 > 0)
        s3 = 1;
    else
        s3 = 0;
    
    if ((z4 = ((p2.x - p3.x)*(p4.y - p3.y)) - ((p2.y - p3.y)*(p4.x - p3.x))) < 0)
        s4 = -1;
    else if (z4 > 0)
        s4 = 1;
    else
        s4 = 0;
    
    if ((s1 * s2 <= 0) && (s3 * s4 <= 0))
        return true;
    
    return false;
}
