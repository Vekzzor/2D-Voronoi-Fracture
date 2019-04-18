#include "Event.h"
#include "Event.h"
#include "Event.h"

Event::Event(double x, double y)
{
	m_x = x; 
	m_y = y; 
}
Event::Event()
{

}
Event::~Event()
{
}
bool Event::operator<(const Event& otherEvent)
{
	bool isLess = false;
	if (m_y < otherEvent.m_y)
		isLess = true;
	return isLess;
}

