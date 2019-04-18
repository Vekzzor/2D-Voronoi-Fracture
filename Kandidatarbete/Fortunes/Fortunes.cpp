#include "Fortunes.h"

Fortunes::Fortunes()
{ 
	//Add all initial site events
	//addEvent(event1); 
	//addEvent(...); 
	//addEvent(...); 
}

Fortunes::~Fortunes()
{

}

void Fortunes::GenerateVoronoiDiagram(std::vector<Event>& seeds, int minY, int maxY)
{
	m_minY = minY; 
	m_maxY = maxY; 

	Event* currEvent = m_events.top(); 

	//Handle site event
	if (currEvent->GetType() == "SITE")
	{

	}
	//Handle circle event
	else
	{

	}
	
}

void Fortunes::HandleSiteEvent()
{

}

void Fortunes::HandleCircleEvent()
{

}

void Fortunes::addEvent(Event* eventToAdd)
{
	m_events.push(eventToAdd); 
}