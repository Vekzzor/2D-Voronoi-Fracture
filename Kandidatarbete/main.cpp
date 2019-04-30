
#include <SFML/Graphics.hpp>
#include "voronoi.h"
#include <iostream>
#include "Fortunes/Data Structures/BinTree.h"
#include "Bowyer-Watson/Delunay.h"
#include "Bowyer-Watson/delaunay.hpp"
#include "Fortunes/Data Structures/DCEL.h"
#include <math.h> 
Voronoi* vdg;
vector<VoronoiPoint*> ver;
vector<VEdge> edges;
static const int MINSIZE = 150; //55
static const int MAXSIZE = 400; //390
static const int SEEDS = 100;

#define CUSTOM 1;

struct VoronoiEdge
{
	VoronoiEdge(sf::Vector2f _v1, sf::Vector2f _v2, sf::Vector2f _site) : v1{ _v1 }, v2{ _v2 }, site{ _site }{};
	sf::Vector2f v1;
	sf::Vector2f v2;
	sf::Vector2f site;
};

 sf::Vector2f CalculateCircleCenter(sf::Vector2f* p1, sf::Vector2f* p2, sf::Vector2f* p3)
{
	 sf::Vector2f center;

	 const float ax = p2->x - p1->x;
	 const float ay = p2->y - p1->y;
	 const float bx = p3->x - p1->x;
	 const float by = p3->y - p1->y;

	 const float m = p2->x * p2->x - p1->x * p1->x + p2->y * p2->y - p1->y * p1->y;
	 const float u = p3->x * p3->x - p1->x * p1->x + p3->y * p3->y - p1->y * p1->y;
	 const float s = 1. / (2. * (ax * by - ay * bx));

	center.x = ((p3->y - p1->y) * m + (p1->y - p2->y) * u) * s;
	center.y = ((p1->x - p3->x) * m + (p2->x - p1->x) * u) * s;

	 /*const float dx = v1->x - this->circle.x;
	 const float dy = v1->y - this->circle.y;
	 this->circle.radius = dx * dx + dy * dy;*/

	if (isinf(s))
	{
		int lul = 0;
	}

	/*float ma = (p2->y - p1->y) / (p2->x - p1->x);
	float mb = (p3->y - p2->y) / (p3->x - p2->x);

	center.x = (ma * mb * (p1->y - p3->y) + mb * (p1->x + p2->x) - ma * (p2->x + p3->x)) / (2 * (mb - ma));

	center.y = (-1 / ma) * (center.x - (p1->x + p2->x) / 2) + (p1->y + p2->y) / 2;
*/
	return center;
}

void addVoronoiEdge(HALF_EDGE::HE_Edge* e, sf::Vector2f center, std::vector<VoronoiEdge>& voronoiEdges)
{
	//Ignore if this edge has no neighboring triangle
	if (e->twin == nullptr)
	{
		return;
	}

	//Calculate the circumcenter of the neighbor
	HALF_EDGE::HE_Edge* eNeighbor = e->twin;

	sf::Vector2f* v1 = eNeighbor->vert->point;
	sf::Vector2f* v2 = eNeighbor->next->vert->point;
	sf::Vector2f* v3 = eNeighbor->next->next->vert->point;

	sf::Vector2f voronoiVertexNeighbor = eNeighbor->face->circumCenter;

	//Create a new voronoi edge between the voronoi vertices
	voronoiEdges.push_back(
		VoronoiEdge(center, voronoiVertexNeighbor, *e->vert->point));
}

int main()
{

	//FORTUNES ALGORITHM
	for (vector<VoronoiPoint*>::iterator i = ver.begin(); i != ver.end(); i++)
		delete((*i));
	ver.clear();
	edges.clear();
	for (int i = 0; i < SEEDS; i++)
	{
		ver.push_back(new VoronoiPoint(rand() % MAXSIZE +MINSIZE, rand() % MAXSIZE +MINSIZE));
	}

	vector<DVertex*> BWpoints;
	std::vector<delaunay::Point<float>> points;
	for (size_t i = 0; i < SEEDS; i++)
	{
		float x = float(rand() % MAXSIZE + MINSIZE);
		float y = float(rand() % MAXSIZE + MINSIZE);
		points.push_back(delaunay::Point<float>(x,y));
		BWpoints.push_back(new DVertex(x, y, i));
	}
	
	BWpoints.push_back(new DVertex(MINSIZE, MINSIZE, BWpoints.size()));
	BWpoints.push_back(new DVertex(MAXSIZE + MINSIZE, MINSIZE, BWpoints.size()));
	BWpoints.push_back(new DVertex(MAXSIZE + MINSIZE, MAXSIZE + MINSIZE, BWpoints.size()));
	BWpoints.push_back(new DVertex(MINSIZE, MAXSIZE + MINSIZE, BWpoints.size()));

#if CUSTOM

	Delunay triangulation;
	const std::vector<Triangle> triangles = triangulation.Triangulate(BWpoints);
	std::cout << triangles.size() << " triangles generated\n";

	for (auto& vert : triangulation.getSuperTriangle())
	{
		vert->arrayIndex = BWpoints.size();
		BWpoints.push_back(vert);
	}

	std::vector<pair<unsigned int, unsigned int>> EdgeIndex;
	
	for (const Triangle &triangle : triangles)
	{
		//for (int k = 0; k < 3; k++)
		//{
		//	int start = -1;
		//	int end = -1;
		//	/*for (int i = 0; i < BWpoints.size(); i++)
		//	{
		//		if (start == -1 && triangle.e[k].v1 == BWpoints[i])
		//		{
		//			start = i;
		//		}
		//		if (end == -1 && triangle.e[k].v2 == BWpoints[i])
		//		{
		//			end = i;
		//		}
		//	}*/
		//	EdgeIndex.push_back(make_pair(start, end));
		//}	

		EdgeIndex.push_back(make_pair(triangle.v1->arrayIndex, triangle.v2->arrayIndex));
		EdgeIndex.push_back(make_pair(triangle.v2->arrayIndex, triangle.v3->arrayIndex));
		EdgeIndex.push_back(make_pair(triangle.v3->arrayIndex, triangle.v1->arrayIndex));
	}

	map< pair<unsigned int, unsigned int>, HALF_EDGE::HE_Edge* > Edges;
	std::vector<HALF_EDGE::HE_Vertex*> vertexList;
	for (auto &point : BWpoints)
	{
		HALF_EDGE::HE_Vertex* vertex = new HALF_EDGE::HE_Vertex();
		vertex->point = point;
		vertexList.push_back(vertex);
	}
	std::vector<HALF_EDGE::HE_Face*> faceList;

	for (int i = 0; i < triangles.size(); i++)
	{
		const int offset = i * 3;
		HALF_EDGE::HE_Face* tempF = new HALF_EDGE::HE_Face();
		tempF->circumCenter = sf::Vector2f(triangles[i].circle.x, triangles[i].circle.y);
		tempF->radius = triangles[i].circle.radius;
		for (size_t k = 0; k < 3; k++)
		{
			const int location = offset + k;

			pair<unsigned int, unsigned int> memEdges(EdgeIndex[location]);
			Edges[memEdges] = new HALF_EDGE::HE_Edge();
			Edges[memEdges]->face = tempF;

			Edges[memEdges]->vert = vertexList[memEdges.first];
			Edges[memEdges]->vert->edge = Edges[EdgeIndex[offset]];
		}

		tempF->edge = Edges[EdgeIndex[offset]];

		for (size_t k = 0; k < 3; k++)
		{
			const int location = offset + k;
			Edges[EdgeIndex[location]]->setNext(
				Edges[EdgeIndex[offset + (k + 1) % 3]]);
			
			pair<unsigned int, unsigned int> otherPair = make_pair(EdgeIndex[location].second, EdgeIndex[location].first);
			if (Edges.find(otherPair) != Edges.end())
			{
				Edges[EdgeIndex[location]]->setTwin(Edges[otherPair]);
			}

		}
		faceList.push_back(tempF);
	}
	
	std::vector <HALF_EDGE::HE_Vertex*> faceEdgePoints;
	for (auto &face : faceList)
	{
		HALF_EDGE::getFaceVertices(faceEdgePoints, face);
	}

	std::vector <HALF_EDGE::HE_Edge*> faceEdges;
	for (auto &face : faceList)
	{
		HALF_EDGE::getFaceEdges(faceEdges, face);
	}

	
	
	{
		//int Vmin = 54;
		//int Vmax = 445;
		//std::vector< pair<sf::Vector2f, sf::Vector2f>> VorEdges;

		//for (auto &halfedge : Edges)
		//{
		//	if (halfedge.second->twin != nullptr)
		//	{

		//		sf::Vector2f cc1 = halfedge.second->face->circumCenter;
		//		sf::Vector2f cc2 = halfedge.second->twin->face->circumCenter;
		//		bool cc1Inside = (cc1.x < Vmax && cc1.y < Vmax && cc1.x > Vmin && cc1.y > Vmin);
		//		bool cc2Inside = (cc2.x < Vmax && cc2.y < Vmax && cc2.x > Vmin && cc2.y > Vmin);
		//	/*	if (!cc1Inside && !cc2Inside)
		//			continue;

		//		if (!cc1Inside)
		//		{
		//			if (cc1.x < Vmin)
		//				cc1.x = Vmin;
		//			if (cc1.y < Vmin)
		//				cc1.y = Vmin;

		//			if (cc1.x > Vmax)
		//				cc1.x = Vmax;
		//			if (cc1.y > Vmax)
		//				cc1.y = Vmax;
		//		}

		//		if (!cc2Inside)
		//		{
		//			if (cc2.x < Vmin)
		//				cc2.x = Vmin;
		//			if (cc2.y < Vmin)
		//				cc2.y = Vmin;

		//			if (cc2.x > Vmax)
		//				cc2.x = Vmax;
		//			if (cc2.y > Vmax)
		//				cc2.y = Vmax;
		//		}
		//		if(cc1Inside && cc2Inside)*/
		//			VorEdges.push_back(make_pair(cc1, cc2));
		//	}
		//}
		/*std::vector<bool> remove(edges.size(), false);

		for (auto it1 = VorEdges.begin(); it1 != VorEdges.end(); ++it1) {
			for (auto it2 = VorEdges.begin(); it2 != VorEdges.end(); ++it2) {
				if (it1 == it2) {
					continue;
				}
				if (*it1 == *it2) {
					remove[std::distance(VorEdges.begin(), it1)] = true;
					remove[std::distance(VorEdges.begin(), it2)] = true;
				}
			}
		}*/
		//auto is_duplicate = [&](auto const& e) { return remove[&e - &VorEdges[0]]; };
		//erase_where(VorEdges, is_duplicate);
	}

#else
	const auto triangulation = delaunay::triangulate(points);
	std::cout << triangulation.triangles.size() << " triangles generated\n";
#endif

	
	vdg = new Voronoi();
	double minY = MINSIZE;
	double maxY = MAXSIZE;
	edges = vdg->ComputeVoronoiGraph(ver, minY, maxY);
	delete vdg;
	std::vector<sf::Vertex*> lines;
	std::vector<sf::Vertex*> polygons;
	std::vector<int> shared;

#if CUSTOM
	{

	//for (auto& tri : triangles)
	//{
	//	int val = (tri.v2->y - tri.v1->y) * (tri.v3->x - tri.v2->x) -
	//		(tri.v2->x - tri.v1->x) * (tri.v3->y - tri.v2->y);

	//	if (val == 0) return 0;  // colinear 

	//	val = (val > 0) ? 1 : 2; // clock or counterclock wise 
	//	if (val == 1)
	//	{
	//		auto lol = 0;
	//	}

	//	val = 0;
	//	if (tri.e1.v2 == tri.e2.v1 && tri.e2.v2 == tri.e3.v1 && tri.e3.v2 == tri.e1.v1)
	//	{
	//		val = 1;
	//	}
	//	else if (tri.e1.v2 == tri.e3.v1 && tri.e3.v2 == tri.e2.v1 && tri.e2.v2 == tri.e1.v1)
	//	{
	//		val = 2;
	//	}

	//}
	}
	

	for (size_t i = 0; i < EdgeIndex.size(); i++)
	{
		sf::Vector2f* start = BWpoints[EdgeIndex[i].first];
		sf::Vector2f* end = BWpoints[EdgeIndex[i].second];

		sf::Vertex* test = new sf::Vertex[2];
		test[0] = sf::Vector2f(start->x, start->y);
		test[1] = sf::Vector2f(end->x, end->y);
		lines.push_back(test);
		lines.back()[0].color = sf::Color(0, 0, 255);
		lines.back()[1].color = sf::Color(0, 0, 255);
	}
	
	vector<sf::CircleShape> circumPoints;
	int stride = 3;
	bool once = true;
	for (size_t i = 0; i < triangles.size(); i++)
	{
		int offset = i * stride;
		//2 11 26 are incorrect edges 
		//(caused during the creation of the data structure)
		//Try flipping with the next edge?
		for (size_t k = 0; k < 3; k++)
		{
			sf::Vector2f* start = faceEdgePoints[offset+k]->point;
			sf::Vector2f* end = faceEdgePoints[offset + (k + 1) % 3]->point;

			sf::Vertex* test = new sf::Vertex[2];
			test[0] = sf::Vector2f(start->x, start->y);
			test[1] = sf::Vector2f(end->x, end->y);
			lines.push_back(test);
			lines.back()[0].color = sf::Color(0, 255, 0);
			lines.back()[1].color = sf::Color(0, 255, 0);
		}
		if (once)
		{
			

			//once = false;
			sf::Vector2f* v1 = faceEdgePoints[offset]->point;
			sf::Vector2f* v2 = faceEdgePoints[offset + 1]->point;
			sf::Vector2f* v3 = faceEdgePoints[offset + 2]->point;
			const float ax = v2->x - v1->x;
			const float ay = v2->y - v1->y;
			const float bx = v3->x - v1->x;
			const float by = v3->y - v1->y;

			const float m = v2->x * v2->x - v1->x * v1->x + v2->y * v2->y - v1->y * v1->y;
			const float u = v3->x * v3->x - v1->x * v1->x + v3->y * v3->y - v1->y * v1->y;
			const float s = 1. / (2. * (ax * by - ay * bx));


			sf::Vector2f circle;
			circle.x = ((v3->y - v1->y) * m + (v1->y - v2->y) * u) * s;
			circle.y = ((v1->x - v3->x) * m + (v2->x - v1->x) * u) * s;

			const float dx = v1->x - circle.x;
			const float dy = v1->y - circle.y;
			float radius = sqrt(dx * dx + dy * dy);

			sf::CircleShape point;
			point.setRadius(2);
			point.setPosition(*v1 - sf::Vector2f(2, 2));
			point.setFillColor({ 255,0,0 });
			circumPoints.push_back(point);

			point = sf::CircleShape();
			point.setRadius(2);
			point.setPosition(*v2 - sf::Vector2f(2, 2));
			point.setFillColor({ 255,0,0 });
			circumPoints.push_back(point);

			point = sf::CircleShape();
			point.setRadius(2);
			point.setPosition(*v3 - sf::Vector2f(2, 2));
			point.setFillColor({ 255,0,0 });
			circumPoints.push_back(point);


			
			/*sf::Vector2f newPos;
			newPos.x = std::max(float(MINSIZE), std::min(circle.x, float(MAXSIZE+MINSIZE)));
			newPos.y = std::max(float(MINSIZE), std::min(circle.y, float(MAXSIZE+MINSIZE)));*/

			point = sf::CircleShape();
			point.setRadius(2);
			point.setPosition(circle - sf::Vector2f(2, 2));
			point.setFillColor({ 0,0,255 });
			circumPoints.push_back(point);

			point = sf::CircleShape();
			point.setOutlineThickness(1.0f);
			point.setOutlineColor({ 255,0,255 });
			point.setRadius(radius);
			point.setPosition(circle - sf::Vector2f(point.getRadius(), point.getRadius()));
			point.setFillColor(sf::Color::Transparent);
			circumPoints.push_back(point);
		}
	}
	std::vector<VoronoiEdge> allVoronoiEdges;
	for (size_t i = 0; i < faceList.size(); i++)
	{
		HALF_EDGE::HE_Edge* e1 = faceList[i]->edge;
		HALF_EDGE::HE_Edge* e2 = e1->next;
		HALF_EDGE::HE_Edge* e3 = e2->next;
 
		sf::Vector2f* v1 = e1->vert->point;
		sf::Vector2f* v2 = e2->vert->point;
		sf::Vector2f* v3 = e3->vert->point;

		sf::Vector2f voronoiVertex = faceList[i]->circumCenter;


		addVoronoiEdge(e1, voronoiVertex, allVoronoiEdges);
		addVoronoiEdge(e2, voronoiVertex, allVoronoiEdges);
		addVoronoiEdge(e3, voronoiVertex, allVoronoiEdges);
	}	
	struct VoronoiCell
	{
		sf::Vector2f site;
		std::vector<VoronoiEdge> edges;

		VoronoiCell(sf::Vector2f sitePos) : site{ sitePos } {};
	};


	std::vector<VoronoiCell> voronoiCells;
	for (int i = 0; i < allVoronoiEdges.size(); i++)
	{
		VoronoiEdge e = allVoronoiEdges[i];

		int index = -1;
		//Find the position in the list of all cells that includes this site
		for (int k = 0; k < voronoiCells.size(); k++)
		{
			if (e.site == voronoiCells[k].site)
			{
				index = k;
				break;
			}
		}


		int cellPos = index;
		
		//No cell was found so we need to create a new cell
		if (cellPos == -1)
		{
			VoronoiCell newCell(e.site);

			voronoiCells.push_back(newCell);

			voronoiCells.back().edges.push_back(e);
		}
		else
		{
			voronoiCells[cellPos].edges.push_back(e);
		}
	}


#if 0	
	for (int i = 0; i < triangles.Count; i++)
	{
		Triangle t = triangles[i];

		//Each triangle consists of these edges
		HalfEdge e1 = t.halfEdge;
		HalfEdge e2 = e1.nextEdge;
		HalfEdge e3 = e2.nextEdge;

		//Calculate the circumcenter for this triangle
		Vector3 v1 = e1.v.position;
		Vector3 v2 = e2.v.position;
		Vector3 v3 = e3.v.position;

		//The circumcenter is the center of a circle where the triangles corners is on the circumference of that circle
		//The .XZ() is an extension method that removes the y value of a vector3 so it becomes a vector2
		Vector2 center2D = Geometry.CalculateCircleCenter(v1.XZ(), v2.XZ(), v3.XZ());

		//The circumcenter is also known as a voronoi vertex, which is a position in the diagram where we are equally
		//close to the surrounding sites
		Vector3 voronoiVertex = new Vector3(center2D.x, 0f, center2D.y);

		TryAddVoronoiEdgeFromTriangleEdge(e1, voronoiVertex, voronoiEdges);
		TryAddVoronoiEdgeFromTriangleEdge(e2, voronoiVertex, voronoiEdges);
		TryAddVoronoiEdgeFromTriangleEdge(e3, voronoiVertex, voronoiEdges);
	}
#endif
	//std::vector<int> polygonVertices;
	int c = 0;
	bool pST = false;
	for (auto &cell : voronoiCells)
	{
		pST = false;
		sf::Color color(rand() % 255, rand() % 255, rand() % 255);
		
		for (int i = 0; i < cell.edges.size(); i++)
		{
			if (!(cell.site.x < MAXSIZE+MINSIZE && cell.site.x > MINSIZE) &&
				!(cell.site.y < MAXSIZE + MINSIZE && cell.site.y > MINSIZE))
			{
				pST = true;
				break;
			}
			if (!(cell.edges[i].site.x < MAXSIZE + MINSIZE && cell.edges[i].site.x > MINSIZE) &&
				!(cell.edges[i].site.y < MAXSIZE + MINSIZE && cell.edges[i].site.y > MINSIZE))
			{
				pST = true;
				break;
			}

			if (!(cell.edges[i].v1.x < MAXSIZE + MINSIZE && cell.edges[i].v1.x > MINSIZE) &&
				!(cell.edges[i].v1.y < MAXSIZE + MINSIZE && cell.edges[i].v1.y > MINSIZE))
			{
				pST = true;
				break;
			}

			if (!(cell.edges[i].v2.x < MAXSIZE + MINSIZE && cell.edges[i].v2.x > MINSIZE) &&
				!(cell.edges[i].v2.y < MAXSIZE + MINSIZE && cell.edges[i].v2.y > MINSIZE))
			{
				pST = true;
				break;
			}
		}
		if (pST)
			continue;
		for (int i = 0; i < cell.edges.size(); i++)
		{
			sf::Vector2f p3 = cell.edges[i].v1;
			sf::Vector2f p2 = cell.edges[i].v2;
			sf::Vertex* test = new sf::Vertex[3];
			test[0] = cell.site;
			test[0].color = color;
			test[1] = p2;
			test[1].color = color;
			test[2] = p3;
			test[2].color = color;
			polygons.push_back(test);

			/*test = new sf::Vertex[2];
			test[0] = cell.site;
			test[0].color = color;
			test[0] = p3;
			test[0].color = { 255,0,0 };
			test[1] = cell.site;  
			test[1].color = { 255,0,0 };
			polygons.push_back(test);*/
		}
		//if(c == 0)
		//break;

		c++;
		//polygonVertices.push_back(cell.edges.size() * 2 + 1);
	}
	cout << polygons.size() << endl;
	//for (auto &edge : allVoronoiEdges)
	//{

	//	sf::Color color(rand() % 255, rand() % 255, rand() % 255);
	//	sf::Vector2f p3 = edge.v1;
	//	sf::Vector2f p2 = edge.v2;
	//	sf::Vertex* test = new sf::Vertex[2];
	//	/*test[0] = cell.site;
	//	test[0].color = color;*/
	//	test[0] = p2;
	//	test[0].color = { 0,0,255 };
	//	test[1] = p3;
	//	test[1].color = { 0,0,255 };
	//	polygons.push_back(test);

	//	//test = new sf::Vertex[2];
	//	///*test[0] = cell.site;
	//	//test[0].color = color;*/
	//	//test[0] = p3;
	//	//test[0].color = { 255,0,0 };
	//	//test[1] = cell.site;
	//	//test[1].color = { 255,0,0 };
	//	//polygons.push_back(test);
	//	/*if (c == 0)
	//		break;*/

	//	c++;
	//	//polygonVertices.push_back(cell.edges.size() * 2 + 1);
	//}

	/*std::vector <sf::Vector2f> polygon;
	HALF_EDGE::getPolygon(polygon, vertexList[0]);
	sf::Vertex* poly = new sf::Vertex[polygon.size()];
	for (int i = 0; i < polygon.size(); i++)
	{
		sf::Vector2f point = polygon[i];

		poly[i] = sf::Vector2f(point.x, point.y);
		poly[i].color = sf::Color(2MINSIZE, 0, 0);
	}*/
	/*for (size_t i = 0; i < VorEdges.size(); i++)
	{
		sf::Vertex* test = new sf::Vertex[2];
		test[0] = sf::Vector2f((VorEdges[i].first.x), (VorEdges[i].first.y));
		test[1] = sf::Vector2f((VorEdges[i].second.x), (VorEdges[i].second.y));
		lines.push_back(test);
		lines.back()[0].color = sf::Color(2MINSIZE, 0, 0);
		lines.back()[1].color = sf::Color(2MINSIZE, 0, 0);
	}*/
#else
	for (auto const& e : triangulation.edges)
	{
		sf::Vertex* test = new sf::Vertex[2];
		test[0] = sf::Vector2f((e.p0.x), (e.p0.y));
		test[1] = sf::Vector2f((e.p1.x), (e.p1.y));
		lines.push_back(test);
		lines.back()[0].color = sf::Color(0, 0, 2MINSIZE);
		lines.back()[1].color = sf::Color(0, 0, 2MINSIZE);
	}

#endif
	
	//for (int i = 0; i < edges.size(); i++)
	//{
	//	sf::Vertex* test = new sf::Vertex[2];
	//	test[0] = sf::Vector2f(edges[i].VertexA.x, edges[i].VertexA.y);
	//	test[1] = sf::Vector2f(edges[i].VertexB.x, edges[i].VertexB.y);
	//	lines.push_back(test);
	//}


	sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");

	//BinTree<int> bT;

	//bT.AddNode(bT.GetRoot(), 50); 
	//bT.AddNode(bT.GetRoot(), 20); 
	//bT.AddNode(bT.GetRoot(), 70); 

	//int serachNum = 0;

	//std::cout << "Please enter the number you want to search for: " << std::endl; 
	//std::cin >> serachNum; 

	//if (bT.Search(bT.GetRoot(), serachNum))
	//{
	//	std::cout << "Number " << serachNum << " was found!" << std::endl; 
	//}
	//else
	//{
	//	std::cout << "Number " << serachNum << " was not found!" << std::endl; 
	//}

	
	//for (size_t i = 0; i < faceList.size(); i++)
	//{
	//	//if (faceList[i]->radius < MAXSIZE)
	//	{
	//		/*sf::CircleShape point;
	//		point.setRadius(faceList[i]->radius);
	//		point.setPosition(faceList[i]->circumCenter - sf::Vector2f(faceList[i]->radius, faceList[i]->radius));
	//		point.setOutlineThickness(1.0f);
	//		point.setOutlineColor({ 255,0,255 });
	//		point.setFillColor(sf::Color::Transparent);
	//		circumPoints.push_back(point);*/

	//		sf::CircleShape point;
	//		point.setRadius(2);
	//		point.setPosition(faceList[i]->circumCenter - sf::Vector2f(2, 2));
	//		point.setFillColor({ 0,0,255 });
	//		circumPoints.push_back(point);
	//	}
	//}

	vector<DVertex*> sTri = triangulation.getSuperTriangle();
	for (int i = 0; i < 3; i++)
	{
		sf::Vector2f* start = sTri[i];
		sf::Vector2f* end = sTri[(i+1)%3];

		sf::Vertex* test = new sf::Vertex[2];
		test[0] = sf::Vector2f(start->x, start->y);
		test[1] = sf::Vector2f(end->x, end->y);
		lines.push_back(test);
		lines.back()[0].color = sf::Color(255, 0, 0);
		lines.back()[1].color = sf::Color(255, 0, 0);
	}
	sf::RectangleShape rect;
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineThickness(2);
	rect.setOutlineColor({ 255,255,255 });
	rect.setSize({ float(MAXSIZE),float(MAXSIZE) });
	rect.setPosition({ float(MINSIZE), float(MINSIZE)  });
	sf::CircleShape point;
	int position = 3;//8 1

	point.setPosition(sf::Vector2f(BWpoints[position]->x , BWpoints[position]->y));
	point.setRadius(5);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Escape)
						window.close();
					break;
			}
		}

		window.clear();
		
		//window.draw(poly, polygon.size(), sf::LineStrip);
		//window.draw(point);

		for(auto line : lines)
			window.draw(line, 2, sf::Lines);
		
		for (int i = 0; i < polygons.size(); i++)
		{
			window.draw(polygons[i], 3, sf::Triangles);
		}
		/*for (auto point : circumPoints)
		{
			window.draw(point);
		}*/
		window.draw(rect);
		window.display();
	}

	return 0; 
}