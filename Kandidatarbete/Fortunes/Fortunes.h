#pragma once
#include <vector>
#include <queue>
#include "Event/Event.h"
#include "Event/SiteEvent.h"
#include "Event/CircleEvent.h"
#include "SFML/Graphics.hpp"

class HalfEdge
{
	HalfEdge* leftEdge;
	HalfEdge* rightEdge;
	int ELrefCount; 
	char ELpm; 
	SiteEvent* Vertex; 
	double ystar; 
	HalfEdge* PQnext;
};

class Fortunes
{
private:
	//Priority Queue with all events. 
	std::priority_queue<Event*> m_events; 
	void addEvent(Event* eventToAdd);  

	int m_minY; 
	int m_maxY;

public:
	Fortunes(); 
	~Fortunes(); 

	void GenerateVoronoiDiagram(std::vector<Event>& seeds, int minY, int maxY); 

	void HandleSiteEvent(); 
	void HandleCircleEvent(); 
};

