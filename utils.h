// This file contains a not entirely random collection of utility functions
// to do with 3D geometry. This could be tidied up and put in a class at
// some point

#ifndef utils_h
#define utils_h

#include "LocationData.h"

// Point2D defined here so that I can just lift the line segment intersection code
// from "Mastering Algorithms with C" by Kyle Loudon
struct Point2D
{
    double x;
    double y;
};

bool LinesIntersect(Point2D p1, Point2D p2, Point2D p3, Point2D p4);

double SquaredDistance(Point& p1, Point& p2);
double PointToSegment(double *x0, double *x1, double *x2);
double Bearing(Point& p1, Point& p2);
double dotProduct(double *a, double *b);
void crossProduct(double *a, double *b, double *c);
void vecdiff(double *a, double *b, double *c);
double NormalisedAngle(double angle);

#define M_PI 3.14159265358979323846264338327950288

#endif /* utils_h */
