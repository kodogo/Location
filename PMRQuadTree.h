
/*
 The PMRQuadTree class is an implementation of the PMR QuadTree structure described in
 the hollowing references:

Erik Hoel and Hanan Samet, "Efficient Processing of Spatial
Queries in Line Segment Databases", Advances in Spatial
Databases - 2nd Symp., SSD '91, (O. Gunther and H. J. Schek,
eds.), Lecture Notes in Computer Science 525, Springer-Verlag,
Berlin, 1991, 237-256.
 
G. R. Hjaltason and H. Samet, "Ranking in spatial databases", in
Advances in Spatial Databases - 4th Symposium, SSD'95, M. J.
Egenhofer and J. R. Herring, Eds., Lecture Notes in Computer
Science 951, Springer-Verlag, Berlin, 1995, 83-95.

Gisli Hjaltason and Hanan Samet, "Speeding Up Construction of PMR
Quadtree-Based Spatial Indexes," The VLDB Journal, 11 (2002) 2,
pp. 109-137. Springer-Verlag.

 The line segments in the input data are stored in boxes in the tree.
 When adding a new segment to a box would result in more segments than
 some limit - apparently 8 is a good number - the box is split into 4 subboxes.
 The boxes are currently defined on a pseudo rectangulat co-ordinate space
 using lonitude for x, latitude for y. This does mean that boxes nearer the pole are smaller
 than those near the equator. When calculating actual distances between points,
 the real 3D Euclidean straight line distance is used. For small distances
 this is close to the great circle distance, and is quicker to calculate. It also
 has exactly the same ordering as great circle distance.
 */

#ifndef PMRQuadTree_hpp
#define PMRQuadTree_hpp

#include <vector>
#include "LocationData.h"
#include "utils.h"

class PMRQuadtree;

struct PMRNode
{
    PMRNode(){};
    PMRNode(double _x, double _y, double _width, double _height, PMRNode *_parent);
 	PMRNode* parent;
	PMRNode *child[4];
	
	// Vector of segments added to the block
	std::vector<long> segmentNumbers;
	
	// Potentially confusingly, the x and y co-ordinates here are actuall longitude and latitude in degrees.
	// We are just using this to partition the surface of the earth treated as a sphere - when we need to
	// calculate actual lengths we will do it properly
	double x;
	double y;
	double width;
	double height;
};

struct PQElement
{
	// Data structure which either contains a line segment or a pointer to a block.
	// This is used as the base type of the priority queue used in the search
	// algorithm
	PQElement(bool _isSegment, double _dist, PMRNode *_node, long _segmentNumber) : isSegment(_isSegment), dist(_dist), node(_node), segmentNumber(_segmentNumber) {};
	bool isSegment;
	double dist; // Distance from the reference point
	PMRNode *node;
	long segmentNumber;
	
	// Definition  of ordering operator for the priority queue in which
	// smaller distances correspond to a higher priority
	friend bool operator<(const PQElement& x, const PQElement& y)
	{
		return (x.dist > y.dist);
	}
};

class PMRQuadTree
{
public:
    PMRQuadTree() : splittingThreshold(8){};
	virtual ~PMRQuadTree();
	PMRQuadTree(LocationData& ld);
	void SetLocationData(LocationData& ld);
	std::vector<long> NearbySegments(Point& p, double radius);
	
private:
	int splittingThreshold;
    LocationData locationData;
	PMRNode* root;
    void InsertSegment(PMRNode *node, long segmentNumber);
    bool SegmentIntersects(PMRNode* b, Segment& s);
    void Split(PMRNode* node);
    bool PointInBlock(PMRNode *node, Point& p);
    double DistanceToBlock(PMRNode *node, Point& p);
    void DeleteTree(PMRNode *node);
};

#endif /* PMRQuadTree_hpp */
