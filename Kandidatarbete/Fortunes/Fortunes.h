//#pragma once
//#include <vector>
//#include <queue>
//#include "SFML/Graphics.hpp"
//#include "Data Structures/BinTree.h"
//#include <random>
//#include "Data Structures/DCEL.h"
//
//#define CIRCLE_CENTER_EPSILON 1.0e-7
//
//class VPoint
//{
//
//public:
//	double x, y; 
//	VPoint(double x, double y); 
//	VPoint(); 
//	~VPoint(); 
//};
//
//class FreeNode
//{
//public:
//	FreeNode* nextFree; 
//};
//
//class FreeNodeArrayList
//{
//public: 
//	FreeNode* memory; 
//	FreeNodeArrayList* next; 
//};
//
//class FreeList
//{
//public:
//	FreeNode* head; 
//	int nodeSize; 
//};
//
//class Site
//{
//public:
//	VPoint coord; 
//	int siteNr; 
//	int refCount; 
//};
//
//class VoronoiEdge
//{
//	VPoint vertA; 
//	VPoint vertB; 
//	VPoint leftS; 
//	VPoint rightS; 
//};
//
////class Edge
////{
////public:
////	double a, b, c; 
////	Site* Vertices[2]; //point A, point B
////	Site* Sites[2]; // left site, right site
////	int edgeNr; 
////};
//
//class GraphEdge
//{
//public: 
//	double x1, y1, x2, y2; 
//	GraphEdge* next;  
//};
//
////HalfEdge is done 
//
//
//
//class Fortunes
//{
//public:
//	Fortunes(); 
//	~Fortunes(); 
//private: 
//	std::vector<VoronoiEdge> m_totalEdges;
//	void cleanUp();
//	void cleanUpEdges(); 
//	bool ELinitialize(); 
//	void geomInit();
//	HALF_EDGE::HE_Edge* m_HEcreate(Edge* e, int pm); 
//	void Elinsert(HALF_EDGE::HE_Edge* lb, HALF_EDGE::HE_Edge* newHe); 
//	HALF_EDGE::HE_Edge* ElGetHash(int b); 
//	HALF_EDGE::HE_Edge* ELleftbnd(VPoint* p); 
//	void ELdelete(HALF_EDGE::HE_Edge* he); 
//	HALF_EDGE::HE_Edge* ELright(HALF_EDGE::HE_Edge* he); 
//	HALF_EDGE::HE_Edge* ELleft(HALF_EDGE::HE_Edge* he);
//	Site* leftreg(HALF_EDGE::HE_Edge* he); 
//	Site* rightreg(HALF_EDGE::HE_Edge* he);
//	Edge* bisector(Site* s1, Site* s2); 
//	Site* intersect(HALF_EDGE::HE_Edge* el1, HALF_EDGE::HE_Edge* el2, VPoint* p = 0); 
//	int right_of(HALF_EDGE::HE_Edge* el, VPoint* p); 
//	void endPoint(Edge* e, int lr, Site* s); 
//	double dist(Site* s, Site* t); 
//	void makeVertex(Site* v); 
//	void deref(Site* v); 
//	void ref(Site* v); 
//	void PQInsert(HALF_EDGE::HE_Edge* he, Site* v, double offset); 
//	void PQdelete(HALF_EDGE::HE_Edge* he); 
//	void PQbucket(HALF_EDGE::HE_Edge* he); 
//	int PQEmpty(); 
//	VPoint PQ_min(); 
//	HALF_EDGE::HE_Edge* extractMin(); 
//	bool PQInit(); 
//	void freeInit(FreeList* fl, int size); 
//	char* getFree(FreeList* fl);
//	void makeFree(FreeNode* curr, FreeList* fl); 
//	void pushGraphEdge(double x1, double y1, double x2, double y2); 
//	void clip_line(Edge* e); 
//	bool voronoi(int triangulate); 
//	Site* nextone(); 
//	void clean(); 
//
//	FreeList hfl;
//	HALF_EDGE::HE_Edge* ELleftend, *ElrightEnd; 
//	double xmin, xmax, ymin, ymax, deltax, deltay; 
//	Site* sites; 
//	FreeList sfl; 
//	Site* bottomSite; 
//	FreeList ef1; 
//	HALF_EDGE::HE_Edge* PQHash; 
//	int ntry, totalSearch, total_alloc, PQmin, PQcount,
//		PQhashSize, nedges, nsites, siteIndex, sqrt_nsites,
//		nvertices, triangulate, sorted, plot, debug,
//		ELHashSize; 
//
//	double pxmin, pxmax, pymin, pymax, cradius; 
//	double borderMinX, borderMaxX, borderMinY, borderMaxY; 
//	FreeNodeArrayList* allMemoryList; 
//	FreeNodeArrayList* currentMemoryBlock; 
//	GraphEdge* allEdges; 
//	GraphEdge* iteratorEdges; 
//	HALF_EDGE::HE_Edge** ELhash; 
//	double minDistBetweenSites; 
//
//
//	struct Event
//	{
//		private:
//			sf::Vector2f m_position;
//		public:
//			Event(sf::Vector2f position)
//			{
//				m_position = position; 
//			}
//			virtual ~Event()
//			{
//
//			}
//
//			sf::Vector2f GetPosition()
//			{
//				return m_position; 
//			}
//
//			void SetPosition(sf::Vector2f position)
//			{
//				m_position = position; 
//			}
//
//			bool operator<(const Event& otherEvent)
//			{
//				bool isLess = false; 
//				if (this->m_position.y < otherEvent.m_position.y)
//				{
//					isLess = true; 
//				}
//				return isLess; 
//			}
//
//			virtual std::string GetType() = 0; 
//	};
//
//	struct BTNode
//	{
//	private:
//		Site* m_siteA = nullptr; 
//		Site* m_siteB = nullptr;  
//		//DCEL::Half_edge* m_halfEdge = nullptr; 
//	public:
//		BTNode()
//		{
//
//		}
//		virtual ~BTNode()
//		{
//
//		}
//
//		bool operator<(const BTNode& otherNode)
//		{
//		/*	bool isLess = false; 
//			if (otherNode.m_siteA->GetPosition().x < )
//			{
//
//			}*/
//		}
//	};
//
//	std::priority_queue<Event*> m_eventPriority;
//	std::vector<Event*> m_events;
//
//	BinTree<Event*> m_voronoiTree; 
//
//public:
//	Fortunes(sf::ConvexShape& shape, int nrOfSites); 
//	~Fortunes();
//
//	struct CircleSite : public Event
//	{
//	private: 
//	public:
//		CircleSite(sf::Vector2f position) : Event(position)
//		{
//
//		}
//		~CircleSite()
//		{
//
//		}
//
//		std::string GetType() override
//		{
//			return "CircleSite"; 
//		}
//	};
//
//	void GenerateVoronoiDiagram(std::vector<VPoint*> p, int minY, int maxY); 
//
//	void InitiateSites(sf::ConvexShape shape,int nrOfEvents); 
//	void HandleSiteEvent(Site* siteEvent); 
//	//void HandleCircleEvent(LeafNode* arc);
//	bool findCircleCenter(const sf::Vector2f &posA, const sf::Vector2f &posB, const sf::Vector2f &posC, sf::Vector2f& center);
//};

