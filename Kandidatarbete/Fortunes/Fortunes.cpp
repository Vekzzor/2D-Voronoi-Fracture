//#include "Fortunes.h"
//#include <cmath>
//#include <iostream>
//
//Fortunes::Fortunes(sf::ConvexShape& shape, int nrOfSites)
//{ 
//	//Initiate the sites
//	InitiateSites(shape, nrOfSites); 
//	//Create an empty binary tree storing Node structs
//}
//
//Fortunes::Fortunes()
//{
//	siteIndex = 0; 
//	sites = 0; 
//	allMemoryList = new FreeNodeArrayList; 
//	allMemoryList->memory = 0; 
//	allMemoryList->next = 0; 
//	currentMemoryBlock = allMemoryList; 
//	allEdges = 0; 
//	iteratorEdges = 0; 
//	minDistBetweenSites = 0; 
//}
//Fortunes::~Fortunes()
//{
//
//}
//
//void Fortunes::cleanUp()
//{
//}
//
//void Fortunes::cleanUpEdges()
//{
//}
//
//bool Fortunes::ELinitialize()
//{
//	return false;
//}
//
//void Fortunes::geomInit()
//{
//	double sn; 
//
//	freeInit(&ef1, sizeof(Edge)); 
//	nvertices = 0;
//	nedges = 0;
//	sn = (double)nsites + 4; 
//	sqrt_nsites = (int)sqrt(sn); 
//	deltay = ymax - ymin; 
//	deltax = xmax - xmin; 
//
//}
//
//HALF_EDGE::HE_Edge * Fortunes::m_HEcreate(Edge * e, int pm)
//{
//	return nullptr;
//}
//
//void Fortunes::Elinsert(HALF_EDGE::HE_Edge * lb, HALF_EDGE::HE_Edge * newHe)
//{
//}
//
//HALF_EDGE::HE_Edge * Fortunes::ElGetHash(int b)
//{
//	return nullptr;
//}
//
//HALF_EDGE::HE_Edge * Fortunes::ELleftbnd(VPoint * p)
//{
//	return nullptr;
//}
//
//void Fortunes::ELdelete(HALF_EDGE::HE_Edge * he)
//{
//}
//
//HALF_EDGE::HE_Edge * Fortunes::ELright(HALF_EDGE::HE_Edge * he)
//{
//	return nullptr;
//}
//
//HALF_EDGE::HE_Edge * Fortunes::ELleft(HALF_EDGE::HE_Edge * he)
//{
//	return nullptr;
//}
//
//Site * Fortunes::leftreg(HALF_EDGE::HE_Edge * he)
//{
//	return nullptr;
//}
//
//Site * Fortunes::rightreg(HALF_EDGE::HE_Edge * he)
//{
//	return nullptr;
//}
//
//Edge * Fortunes::bisector(Site * s1, Site * s2)
//{
//	return nullptr;
//}
//
//Site * Fortunes::intersect(HALF_EDGE::HE_Edge * el1, HALF_EDGE::HE_Edge * el2, VPoint * p)
//{
//	return nullptr;
//}
//
//int Fortunes::right_of(HALF_EDGE::HE_Edge * el, VPoint * p)
//{
//	return 0;
//}
//
//void Fortunes::endPoint(Edge * e, int lr, Site * s)
//{
//}
//
//double Fortunes::dist(Site * s, Site * t)
//{
//	return 0.0;
//}
//
//void Fortunes::makeVertex(Site * v)
//{
//}
//
//void Fortunes::deref(Site * v)
//{
//}
//
//void Fortunes::ref(Site * v)
//{
//}
//
//void Fortunes::PQInsert(HALF_EDGE::HE_Edge * he, Site * v, double offset)
//{
//}
//
//void Fortunes::PQdelete(HALF_EDGE::HE_Edge * he)
//{
//}
//
//void Fortunes::PQbucket(HALF_EDGE::HE_Edge * he)
//{
//}
//
//int Fortunes::PQEmpty()
//{
//	return 0;
//}
//
//VPoint Fortunes::PQ_min()
//{
//	return VPoint();
//}
//
//HALF_EDGE::HE_Edge * Fortunes::extractMin()
//{
//	return nullptr;
//}
//
//bool Fortunes::PQInit()
//{
//	return false;
//}
//
//void Fortunes::freeInit(FreeList * fl, int size)
//{
//	fl->head = nullptr; 
//	fl->nodeSize = size; 
//}
//
//char * Fortunes::getFree(FreeList * fl)
//{
//	return nullptr;
//}
//
//void Fortunes::makeFree(FreeNode * curr, FreeList * fl)
//{
//}
//
//void Fortunes::pushGraphEdge(double x1, double y1, double x2, double y2)
//{
//}
//
//void Fortunes::clip_line(Edge * e)
//{
//}
//
//bool Fortunes::voronoi(int triangulate)
//{
//	return false;
//}
//
//Site * Fortunes::nextone()
//{
//	return nullptr;
//}
//
//void Fortunes::clean()
//{
//}
//
//int VPointCompare(const void *p1, const void *p2)
//{
//	VPoint *s1 = (VPoint*)p1, *s2 = (VPoint*)p2;
//	if (s1->y < s2->y) return -1;
//	if (s1->y > s2->y) return 1;
//	if (s1->x < s2->x) return -1;
//	if (s1->x > s2->x) return 1;
//	return 0;
//}
//
//void Fortunes::GenerateVoronoiDiagram(std::vector<VPoint*> p, int minY, int maxY)
//{
//	iteratorEdges = allEdges; 
//	cleanUp(); 
//	cleanUpEdges(); 
//	int i; 
//
//	minDistBetweenSites = 0; 
//
//	nsites = p.size(); 
//	plot = 0; 
//	triangulate = 0; 
//	debug = 1; 
//	sorted = 0; 
//	freeInit(&sfl, sizeof(Site)); 
//
//	//Create sites
//	sites = new Site[nsites]; 
//
//	//set min and max
//	xmin = p[0]->x; 
//	ymin = p[0]->y; 
//	xmax = p[0]->x; 
//	ymax = p[0]->y; 
//
//	for (int i = 0; i < nsites; i++)
//	{
//		sites[i].coord.x = p[i]->x; 
//		sites[i].coord.y = p[i]->y;
//		sites[i].siteNr = i; 
//		sites[i].refCount = 0; 
//
//		//Store the max and min value in x and y
//		if (p[i]->x < xmin)
//		{
//			xmin = p[i]->x; 
//		}
//		else if (p[i]->x > xmax)
//		{
//			xmax = p[i]->x; 
//		}
//
//		if (p[i]->y < ymin)
//		{
//			ymin = p[i]->y;
//		}
//		else if (p[i]->y > ymax)
//		{
//			ymax = p[i]->y; 
//		}
//	}
//
//	//Sort the sites accordingly to be used in 
//	//correct order by the algorithm
//	qsort(sites, nsites, sizeof(*sites), VPointCompare); 
//
//	siteIndex = 0; 
//
//	//Init the geomerty based on collected data
//	//so far. 
//	geomInit(); 
//
//	double temp = 0; 
//	
//	if (minY > maxY)
//	{
//		temp = minY;
//		minY = maxY;
//		maxY = temp;
//	}
//	if (minY > maxY)
//	{
//		temp = minY;
//		minY = maxY;
//		maxY = temp;
//	}
//	borderMinX = minY;
//	borderMinY = minY;
//	borderMaxX = maxY;
//	borderMaxY = maxY;
//
//
//	while (!m_eventPriority.empty())
//	{
//		Event* currEvent = m_eventPriority.top(); 
//	
//		//Check if Site event
//		if (currEvent->GetType() == "Site")
//		{
//			HandleSiteEvent(dynamic_cast<Site*>(currEvent)); 
//		}
//		//Check if Circle event
//		if (currEvent->GetType() == "CircleSite")
//		{
//			//HandleCircleEvent(dynamic_cast<CircleSite*>(currEvent)->GetLeaf()); 
//		}
//	}
//}
//
////Do this only once. 
//void Fortunes::InitiateSites(sf::ConvexShape shape, int nrOfEvents)
//{
//	//Add initial sites inside the geometry. 
//	srand(time(NULL)); 
//	for (int i = 0; i < nrOfEvents; i++)
//	{
//		m_eventPriority.push(new Site(sf::Vector2f(shape.getOrigin().x + (rand() % shape.getTextureRect().width),
//			shape.getOrigin().y + (rand() % shape.getTextureRect().height)))); 
//	}
//}
//
//void Fortunes::HandleSiteEvent(Site* siteEvent)
//{
//	//Check if tree is empty
//	if (m_voronoiTree.GetRoot() == nullptr)
//	{
//		//Insert site into tree (new parabola)
//		m_voronoiTree.AddNode(m_voronoiTree.GetRoot(), siteEvent); 
//	}
//}
//
////void Fortunes::HandleCircleEvent(LeafNode* arc)
////{
////
////}
//
//VPoint::VPoint(double x, double y)
//{
//	this->x = x; 
//	this->y = y; 
//}
//
//VPoint::VPoint()
//{
//	this->x = 0; 
//	this->y = 0; 
//}
//
//VPoint::~VPoint()
//{
//}
//

