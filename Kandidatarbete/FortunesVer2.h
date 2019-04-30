#pragma once
#include "Fortunes/Types/Point2D.h"
#include "Fortunes/Data Structures/BinTree.h"
#include "Fortunes/Data Structures/DCEL.h"

void build_voronoi(const std::vector<Point2D>& points,
	std::vector<HALF_EDGE::HE_Edge*> &halfEdges,
	std::vector<HALF_EDGE::HE_Edge*> &vertices,
	std::vector<HALF_EDGE::HE_Edge*> &faces); 



