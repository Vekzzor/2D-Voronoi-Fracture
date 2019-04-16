#include "Fortunes.h"

Fortunes::Fortunes()
{
	//Add all initial events (seeds and circles events)
	//addEvent(...); 
	//addEvent(...); 
	//addEvent(...); 
}

Fortunes::~Fortunes()
{

}

void Fortunes::addEvent(Event* eventToAdd)
{
	m_events.push(eventToAdd); 
}