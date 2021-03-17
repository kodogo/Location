#include <limits>
#include <iostream>
#include "LocationData.h"
#include "BruteForceReverseGeocoder.h"
#include "KDTreeReverseGeocoder.h"
#include "PMRTreeReverseGeocoder.h"
#include "SegmentReverseGeocoder.h"
#include "PMRQuadTree.h"

using namespace std;

void PrintTestResult( const char *test, bool result ) {
	printf( "%s: %s\n", test, (result) ? "PASS" : "FAIL");
}

/*Unittests*/
void CorbanAve_ReverseGeocode_LocationFormattedCorrectly( IReverseGeocoder &reverseGeocoder ) {
	bool result = reverseGeocoder.ReverseGeocode( -36.722666, 174.707354 ).FormattedLocation() == "Corban Ave, Albany";
	PrintTestResult( __func__, result );
}

void CorbanAve_ReverseGeocode_SuburbIsAlbany( IReverseGeocoder &reverseGeocoder ) {
	bool result = reverseGeocoder.ReverseGeocode( -36.722666, 174.707354 ).Suburb() == "Albany";
	PrintTestResult( __func__, result );
}

void DaviesDr_ReverseGeocodeWithHeading_LocationFormattedCorrectly( IReverseGeocoder &reverseGeocoder ) {
	bool result = reverseGeocoder.ReverseGeocode( -36.723606, 174.707167, 240.0 ).FormattedLocation() == "Davies Dr, Albany";
	PrintTestResult( __func__, result );
}

void CorbanAve_ReverseGeocode_SpeedIs50Kph( IReverseGeocoder &reverseGeocoder ) {
	bool result = reverseGeocoder.ReverseGeocode( -36.722666, 174.707354 ).SpeedKph() == 50;
	PrintTestResult( __func__, result );
}

void NullIsland_ReverseGeocode_SuburbIsEmpty(IReverseGeocoder &reverseGeocoder)
{
	bool result = reverseGeocoder.ReverseGeocode(0, 0).Suburb() == "";
	PrintTestResult(__func__, result);
	if ( !result ) {
		printf("Failed: \"%s\"!=\"%s\"\n", reverseGeocoder.ReverseGeocode(0, 0).Suburb().c_str(), "");
	}
}

void NullIsland_ReverseGeocode_NotValid(IReverseGeocoder &reverseGeocoder)
{
	bool result = reverseGeocoder.ReverseGeocode(0, 0).Valid() == false;
	PrintTestResult(__func__, result);
}

void UnderNorthernMotorwayHeadingOnOteha_ReverseGeocode_OnOtehaValley(IReverseGeocoder &reverseGeocoder)
{
	bool result = reverseGeocoder.ReverseGeocode(-36.718318, 174.712423, 56.15)
	.FormattedLocation() == "Oteha Valley Rd, Albany";
	PrintTestResult(__func__, result);
	if ( !result ) {
		printf("Failed: \"%s\"!=\"%s\"\n",
			   reverseGeocoder.ReverseGeocode(-36.718318, 174.712423, 56.15)
			   .FormattedLocation().c_str(), "Oteha Valley Rd, Albany");
	}
}

int main(void)
{
    LocationData ld;
    if(!ld.loadFile("RoadData.csv"))
        return 0;
	
    if(ld.points.size()==0)
    {
        std::cout << "No points found in file" << "\n";
        return 0;
    }
    
    BruteForceReverseGeocoder bfReverseGeocoder(ld);
	SegmentReverseGeocoder segmentReverseGeocoder(ld);
   	KDTreeReverseGeocoder kdReverseGeocoder(ld);
    PMRTreeReverseGeocoder pmrReverseGeocoder(ld);

	// Tests for brute force search for the closest point to the target
	CorbanAve_ReverseGeocode_LocationFormattedCorrectly(bfReverseGeocoder);
	CorbanAve_ReverseGeocode_SuburbIsAlbany(bfReverseGeocoder);
	DaviesDr_ReverseGeocodeWithHeading_LocationFormattedCorrectly(bfReverseGeocoder);
	CorbanAve_ReverseGeocode_SpeedIs50Kph(bfReverseGeocoder);
	NullIsland_ReverseGeocode_SuburbIsEmpty(bfReverseGeocoder);
	NullIsland_ReverseGeocode_NotValid(bfReverseGeocoder);
	UnderNorthernMotorwayHeadingOnOteha_ReverseGeocode_OnOtehaValley(bfReverseGeocoder);

	// Tests for search for the closest point to the target using a KD-Tree
    printf("\n\n************************ KD Tree\n");
    //Basic tests implemented without a framework
    CorbanAve_ReverseGeocode_LocationFormattedCorrectly(kdReverseGeocoder);
    CorbanAve_ReverseGeocode_SuburbIsAlbany(kdReverseGeocoder);
    DaviesDr_ReverseGeocodeWithHeading_LocationFormattedCorrectly(kdReverseGeocoder);
    CorbanAve_ReverseGeocode_SpeedIs50Kph(kdReverseGeocoder);
    NullIsland_ReverseGeocode_SuburbIsEmpty(kdReverseGeocoder);
    NullIsland_ReverseGeocode_NotValid(kdReverseGeocoder);
    UnderNorthernMotorwayHeadingOnOteha_ReverseGeocode_OnOtehaValley(kdReverseGeocoder);

	// Tests for search for the closest line segment to the target, taking into account the
	// heading, if provided
	printf("\n\n************************ Segments\n");
	CorbanAve_ReverseGeocode_LocationFormattedCorrectly(segmentReverseGeocoder);
	CorbanAve_ReverseGeocode_SuburbIsAlbany(segmentReverseGeocoder);
	DaviesDr_ReverseGeocodeWithHeading_LocationFormattedCorrectly(segmentReverseGeocoder);
	CorbanAve_ReverseGeocode_SpeedIs50Kph(segmentReverseGeocoder);
	NullIsland_ReverseGeocode_SuburbIsEmpty(segmentReverseGeocoder);
	NullIsland_ReverseGeocode_NotValid(segmentReverseGeocoder);
	UnderNorthernMotorwayHeadingOnOteha_ReverseGeocode_OnOtehaValley(segmentReverseGeocoder);
	
    // Tests for search for the closest line segment to the target, taking into account the
    // heading, if provided
    printf("\n\n************************ PMR quadtree\n");
    CorbanAve_ReverseGeocode_LocationFormattedCorrectly(pmrReverseGeocoder);
    CorbanAve_ReverseGeocode_SuburbIsAlbany(pmrReverseGeocoder);
    DaviesDr_ReverseGeocodeWithHeading_LocationFormattedCorrectly(pmrReverseGeocoder);
    CorbanAve_ReverseGeocode_SpeedIs50Kph(pmrReverseGeocoder);
    NullIsland_ReverseGeocode_SuburbIsEmpty(pmrReverseGeocoder);
    NullIsland_ReverseGeocode_NotValid(pmrReverseGeocoder);
    UnderNorthernMotorwayHeadingOnOteha_ReverseGeocode_OnOtehaValley(pmrReverseGeocoder);


	return 0;
}
