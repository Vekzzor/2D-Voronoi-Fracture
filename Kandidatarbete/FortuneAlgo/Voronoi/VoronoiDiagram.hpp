//
//  VoronoiDiagram.hpp
//  FortuneAlgo
//
//  Created by Dmytro Kotsur on 06/05/2018.
//  Copyright © 2018 Dmytro Kotsur. All rights reserved.
//

#ifndef VoronoiDiagram_hpp
#define VoronoiDiagram_hpp


#include "../Types/Point2D.h"
#include "../Datastruct/Beachline.hpp"

typedef Event* EventPtr;

namespace bl = beachline;

struct Event {

	enum { SITE = 0, CIRCLE = 1, SKIP = 2, };


	int type;
	Point2D point;

	/*
	 Site event attributes:
	 */
	int index;

	/*
	 Circle event attributes:
	 */
	Point2D center;
	bl::BLNodePtr arc;


	Event(int _index = -1, int _type = Event::SKIP, const Point2D &_point = Point2D(0.0, 0.0)) :
		index(_index), type(_type), point(_point), arc(nullptr) {}
};

void build_voronoi(const std::vector<Point2D> &points,
                   std::vector<bl::HalfEdgePtr> &halfedges,
                   std::vector<bl::VertexPtr> &vertices,
                   std::vector<bl::HalfEdgePtr> &faces,
				   bl::BLNodePtr &root, std::vector<EventPtr> &events);

//std::vector<bl::HalfEdgePtr> init
//

#endif /* VoronoiDiagram_hpp */
