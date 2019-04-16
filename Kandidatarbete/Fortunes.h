#pragma once
#include <vector>
#include <queue>

//Base structure for events (seeds)
struct Event
{
	double x, y; 
	
	bool operator<(const Event& otherEvent)
	{
		bool isLess = false; 
		if (this->y < otherEvent.y)
			isLess = true;
		return isLess; 
	} 

	Event* a;
	Event* b; 
	Event* c; 

	Event* circleEvent; 
};

//A site event (seed)
struct SiteEvent : public Event
{
	//Arcs
	SiteEvent* a; 
	SiteEvent* b; 
	SiteEvent* c; 

	CircleEvent* cE; 
};

//A circle event. 
struct CircleEvent : public Event
{
};

//Status tree holding the events, showing their connections and arcs. 
struct StatusTree
{
	Event* root; 
};

struct HalfEdge
{
	//Doubly linked 
};

struct VoronoiData
{
	//Keep HalfEdges here
};

class Fortunes
{
private:
	//Vector keeping track of events.
	std::priority_queue<Event*> m_events; 
	void addEvent(Event* eventToAdd); 

public:
	Fortunes(); 
	~Fortunes(); 
};

