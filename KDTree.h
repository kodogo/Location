#ifndef KDTree_H
#define KDTree_H

#include "LocationData.h"

struct KDNode
{
	int depth;
	int axis;
	KDNode *left;
	KDNode *right;
	Point point;
};

class KDTree
{
public:
    KDTree() {};
    virtual ~KDTree();
	KDTree(LocationData& ld);
    void SetLocationData(LocationData& ld);
	Location NearestLocation(double latitudeDegrees, double longitudeDegrees);
private:
	KDNode *root;
	std::vector<Location> locations;
	KDNode *BuildTree(PointsIter pStart, PointsIter pEnd, int depth);
	double dsqmin;
	Point target;
	Point nearestPoint;
	void SearchTree(KDNode *node);
    void DeleteTree(KDNode *node);
};

#endif /* KDTree_H */
