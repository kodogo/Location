# Location
Experimental code to find street name given a location and bearing. Done as part of an application for a job that I failed to get, so may well be rubbish, but some of the algorithms are interesting, especially the PMR wuadtree.

The program reads street data using a hard-wired file name, RoadData.csv. The file is assumed to contain 4 commaseparated
fields per line, with the first line containing the field names. In the remaining lines, the
first three fields contain street name, suburb, and speed limit in km/h. The 4th field contains a list
of co-ordinate pairs for the street in JSON format.

In parsing the CSV file, I have lifted code from Kernighan and Pike’s book “The Practice of
Programming”. For the JSON, I used Niels Lohmann’s JSON for Modern C++:
https://github.com/nlohmann/json
This is implemented in a single header file, json.hpp, which I have re-named json.h for
consistency with the other header files.

I have implemented four variants of reverse decoding. The first one just does a linear scan of all
the points in the co-ordinate list and returns the nearest one to the search point. This fails the
tests for invalid data and empty suburb when passed (0, 0). This is to be expected, since (0, 0) is a
perfectly valid search location, in the absence of a specified criterion for validity - possibly a
maximum distance value? The final test for Oteha Valley Road also fails, because the search point
is closer to a point on the Northern Motorway, and the heading information is not used.

The second method also searches for the nearest point. However, it does this by first constructing
a KD-Tree search tree, which should allow log(N) search time at the cost of up-front processing.
The implementation is not complete, although it passes the same test as the brute force search,
but it suffers from the same problem with the Oteha Valley Road point.

The third method uses the distances from the point to each line segment. If no bearing is
provided, it just picks the nearest line segment. If a bearing is provided, it makes a list of line
segments within a given distance (currently 50m) and then picks the one whose bearing is closest
to the specified heading. This works reasonably well, but the selection by distance first and then
bearing needs some work - as it stands a segment with a very close distance will lose out to one
with a substantially larger distance but a very slightly closer bearing.

The third method is currently a linear search over all line segments, which is potentially too slow
for a large number of segments. 

The fourth method is the most interesting - it's an experiment with the use of a PMR
quadtree structure to speed up the search, as described here:

• Erik Hoel and Hanan Samet, "Efficient Processing of Spatial Queries in Line Segment
Databases", Advances in Spatial Databases - 2nd Symp., SSD '91, (O. Gunther and H. J. Schek,
eds.), Lecture Notes in Computer Science 525, Springer-Verlag, Berlin, 1991, 237-256.

• G. R. Hjaltason and H. Samet, "Ranking in spatial databases", in Advances in Spatial Databases -
4th Symposium, SSD'95, M. J. Egenhofer and J. R. Herring, Eds., Lecture Notes in Computer
Science 951, Springer-Verlag, Berlin, 1995, 83-95.

• Gisli Hjaltason and Hanan Samet, "Speeding Up Construction of PMR Quadtree-Based Spatial
Indexes", The VLDB Journal, 11 (2002) 2, pp. 109-137. Springer-Verlag.

I haven’t had time to do exhaustive testing, but the approach seems to work well and looks
promising.
