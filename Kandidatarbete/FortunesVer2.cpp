#include "FortunesVer2.h"
#include "Fortunes/Math/Circle.h"
#include "Fortunes/Math/Parabola.h"
#include <queue>

#define BREAKPOINTS_EPSILON 1.0e-5; 
#define _DEBUG_

struct Event
{
	enum {SITE = 0, CIRCLE = 1, SKIP = 2,};

	int type; 
	Point2D point; 

	//Site event attributes
	int index; 
	//Circle event attributes
	Point2D center; 

};

void build_voronoi(const std::vector<Point2D>& points, std::vector<HALF_EDGE::HE_Edge*>& halfEdges, std::vector<HALF_EDGE::HE_Edge*>& vertices, std::vector<HALF_EDGE::HE_Edge*>& faces)
{

}
