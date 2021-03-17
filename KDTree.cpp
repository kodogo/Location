/* Implements a version of Jon Bentley's KD-Tree data structure
 Bentley, J. L. (1975). "Multidimensional binary search trees used for associative searching". Communications of the ACM. 18 (9): 509–517.
 Freidman, J. H.; Bentley, J. L.; Finkel, R. A. (1977). "An Algorithm for Finding Best Matches in Logarithmic Expected Time". ACM Transactions on Mathematical Software. 3 (3): 209.

 The search for the closest point does not implement the full algorithm described in the
 second reference because it became clear that searching for the nearest point was not
 an adequate approach in the case of intersecting roads with different directions.
*/

#include "KDTree.h"
#include <limits>
#include <iostream>
#include <algorithm>
#include "utils.h"

using namespace std;

auto sortByLongitude = [] (Point const& p1, Point const& p2) -> bool
{
	return p1.longitude < p2.longitude;
};

auto sortByLatitude = [] (Point const& p1, Point const& p2) -> bool
{
	return p1.latitude < p2.latitude;
};

KDTree::KDTree(LocationData &ld)
{
    SetLocationData(ld);
}

KDTree::~KDTree()
{
    DeleteTree(root);
}

void KDTree::SetLocationData(LocationData& ld)
{
    // Copy the list of locations into a private variable.
    // This is convenient for debugging, but wasteful of space
    this->locations = ld.locations;
    
    // Build the tree
    this->root = BuildTree(ld.points.begin(), ld.points.end(), 0);
}

KDNode *KDTree::BuildTree(PointsIter pStart, PointsIter pEnd, int depth)
{
	long nPoints = pEnd - pStart;
	if(nPoints == 0)
		return nullptr;
	
	// Alternate splitting direction with depth, rather than using the axis
	// with the biggest spread. The median value of the current axis is chosen to split on - currently
	// this is done by sorting the sub-list each time, but as this only needs to be done once when setting up
	// the tree the inefficiency is not too distrubing.
	int axis = depth % 2;
	if(axis == 0)
		std::sort(pStart, pEnd, sortByLatitude);
	else
		std::sort(pStart, pEnd, sortByLongitude);
	
	long median = nPoints / 2;
	KDNode *node = new KDNode();
	node->depth = depth;
	node->axis = axis;
	node->point = *(pStart + median);
	node->left = BuildTree(pStart, pStart + median, depth + 1);
	node->right = BuildTree(pStart + median + 1, pEnd, depth + 1);
	return node;
}

Location KDTree::NearestLocation(double latitudeDegrees, double longitudeDegrees)
{
	// Searches the tree recursively and returns the closest point found in the search path.
	// As currently implemented this is only an approximation to the globally nearest point
	// because other branches of the tree may need to be inspected. However, just finding
	// the nearest point is not an adequate solution to the given problem, so it's
	// not worth pursuing at this stage
	target = Point(-1, latitudeDegrees, longitudeDegrees);
	dsqmin = std::numeric_limits<double>::max();
	SearchTree(root);
	// Assumes that the search always completes with a valid location number in nearestPoint
	return locations[nearestPoint.locationNumber];
}

void KDTree::SearchTree(KDNode *node)
{
	KDNode *next;
	if(node == nullptr)
		return;
	if(((node->axis == 0) && (target.latitude < node->point.latitude)) ||
	   ((node->axis == 1) && (target.longitude < node->point.longitude)))
		next = node->left;
	else
		next = node->right;
	
	double dsq = SquaredDistance(node->point, target);
	if(dsq < dsqmin)
	{
		nearestPoint = node->point;
		dsqmin = dsq;
	}
	SearchTree(next);
}

void KDTree::DeleteTree(KDNode *node)
{
    if(node == nullptr)
        return;
    DeleteTree(node->left);
    DeleteTree(node->right);
    delete node;
}
