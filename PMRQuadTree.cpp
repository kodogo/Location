#include "PMRQuadTree.h"
#include <queue>
#include <iostream>
#include <cmath>

PMRNode::PMRNode(double _x, double _y, double _width, double _height, PMRNode *_parent)
: x(_x), y(_y), width(_width), height(_height), parent(_parent)
{
    for(int i = 0; i < 4; i++)
        child[i] = nullptr;
}

PMRQuadTree::PMRQuadTree(LocationData& ld)  : splittingThreshold(8)
{
    SetLocationData(ld);
}

PMRQuadTree::~PMRQuadTree()
{
    DeleteTree(root);
}

void PMRQuadTree::SetLocationData(LocationData& ld)
{
    locationData = ld;
    root = new PMRNode(-180.0, -180.0, 360.0, 360.0, nullptr);
    for(long i = 0; i < locationData.segments.size(); i++)
        InsertSegment(root, i);
}

bool PMRQuadTree::SegmentIntersects(PMRNode* b, Segment& s)
{
    // Returns true if the segment intersects any of the borders of the node,
    // or if the segment is fully contained within the node
    
    Point2D sp1 = {locationData.points[s.p1].longitudeDegrees,
        locationData.points[s.p1].latitudeDegrees};
    Point2D sp2 = {locationData.points[s.p2].longitudeDegrees,
        locationData.points[s.p2].latitudeDegrees};

    // Test for containment first
    if ((std::max(sp1.x, sp2.x) >= b->x) &&
        (b->x + b->width >= std::min(sp1.x, sp2.x)) &&
        (std::max(sp1.y, sp2.y) >= b->y) &&
        (b->y + b->height) >= std::min(sp1.y, sp2.y))
        return true;;

    Point2D bp1 = {b->x, b->y};
    Point2D bp2 = {b->x, b->y + b->height};
    

    if(LinesIntersect(bp1, bp2, sp1, sp2))
       return true;
    
    bp1 = {b->x + b->width, b->y + b->height};
    if(LinesIntersect(bp1, bp2, sp1, sp2))
        return true;
    
    bp2 = {b->x + b->width, b->y};
    if(LinesIntersect(bp1, bp2, sp1, sp2))
        return true;

    bp1 = {b->x, b->y};
    if(LinesIntersect(bp1, bp2, sp1, sp2))
        return true;

    return false;

}

void PMRQuadTree::InsertSegment(PMRNode *node, long segmentNumber)
{
    if(node->child[0] == nullptr)
    {
        // leaf node
        node->segmentNumbers.push_back(segmentNumber);
        if(node->segmentNumbers.size() > splittingThreshold)
            Split(node);
    }
    else
    {
        Segment s = locationData.segments[segmentNumber];
        for(int i = 0; i < 4; i++)
            if(SegmentIntersects(node->child[i], s))
                InsertSegment(node->child[i], segmentNumber);
    }
}

void PMRQuadTree::Split(PMRNode *node)
{
    // Split the node into 4 sub-blocks. Assign a line segment to the
    // sub-block or blocks it intersects (or is wholly contained in)
    double newWidth = node->width / 2.0;
    double newHeight = node->height / 2.0;
    node->child[0] = new PMRNode(node->x, node->y, newWidth, newHeight, node);
    node->child[1] = new PMRNode(node->x + newWidth, node->y, newWidth, newHeight, node);
    node->child[2] = new PMRNode(node->x, node->y + newHeight, newWidth, newHeight, node);
    node->child[3] = new PMRNode(node->x + newWidth, node->y + newHeight, newWidth, newHeight, node);
    for(long segmentNumber:node->segmentNumbers)
    {
        Segment s = locationData.segments[segmentNumber];
        for(int i = 0; i < 4; i++)
            if(SegmentIntersects(node->child[i], s))
                node->child[i]->segmentNumbers.push_back(segmentNumber);
    }
    // Remove all of the segments from the parent node. They are now
    // added to one or more of the child nodes
    node->segmentNumbers.clear();
}

bool PMRQuadTree::PointInBlock(PMRNode *node, Point& p)
{
    double x = p.longitudeDegrees;
    double y = p.latitudeDegrees;
    return (node->x <= x) &&
        (x < node->x + node->width) &&
        (node->y <= y) &&
    (y < node->y + node->height);
}

double PMRQuadTree::DistanceToBlock(PMRNode *node, Point& p)
{
    // Finds the shortest distance from the point to the block.
	// If the point is inside the block return 0
	
	// if block contains point, return 0
	if (node->x <= p.longitudeDegrees &&
		p.longitudeDegrees < node->x + node->width &&
		node->y <= p.latitudeDegrees &&
		p.latitudeDegrees < node->y + node->width)
		return 0.0;

    // Otherwise find the shortest distance to a corner
    // We are using x for longitude & y for latitude, so be careful
    // to get the order of the Point constructor parameters correct
    Point corners[] =
        {
            Point(-1, node->y, node->x),
            Point(-1, node->y, node->x + node->width),
            Point(-1, node->y + node->height, node->x + node->width),
            Point(-1, node->y + node->height, node->x)
        };
    
    // Find the smallest distance to an edge
    double dsqmin = std::numeric_limits<double>::max();
    for(int i = 0; i < 4; i++)
    {
        double dsq = SquaredDistance(p, corners[i]);
        if(dsq < dsqmin)
            dsqmin = dsq;
    }
    return sqrt(dsqmin);
}

std::vector<long> PMRQuadTree::NearbySegments(Point& p, double radius)
{
	// Finds all line segments within the given radius of the input point.
	// The algorithm used is that specified in the paper "Ranking in spatial databases"
	// referenced in Quadtree.h
	std::vector<long> segments;
	std::priority_queue<PQElement> pq;
	
	pq.push(PQElement(false, 0.0, root, 0));
	while(!pq.empty())
	{
		PQElement e = pq.top();
		pq.pop();
		// Segments will be retrieved from the priority queue in increasing order of distance
		// so we can stop when we reach the specified radius
		if(e.isSegment)
		{
            while(pq.top().segmentNumber == e.segmentNumber)
               pq.pop();
 			if(e.dist <= radius)
				segments.push_back(e.segmentNumber);
            else
                // Exit the loop when we reach the target radius
                break;
		}
		else if(e.node->child[0] == nullptr)
		{
			// This is a leaf node. Add all of the elements to the list
			for(auto segmentNumber:e.node->segmentNumbers)
			{
				Segment s = locationData.segments[segmentNumber];
				Point p1 = locationData.points[s.p1];
				Point p2 = locationData.points[s.p2];
				double dist = PointToSegment(p.x, p1.x, p2.x) * EARTH_RADIUS;
                double distToBlock = DistanceToBlock(e.node, p) * EARTH_RADIUS;
                if(dist >= distToBlock)
                {
                    pq.push(PQElement(true, dist, nullptr, segmentNumber));
                }
 			}
		}
		else
		{
			// Non-leaf container block
			for(int i = 0; i < 4; i++)
			{
				double dist = DistanceToBlock(e.node->child[i], p) * EARTH_RADIUS;
                pq.push(PQElement(false, dist, e.node->child[i], 0));
			}
		}		
	}
	return segments;
}

void PMRQuadTree::DeleteTree(PMRNode *node)
{
    if(node == nullptr)
        return;
    for(PMRNode * c:node->child)
        DeleteTree(c);
    delete node;
}
