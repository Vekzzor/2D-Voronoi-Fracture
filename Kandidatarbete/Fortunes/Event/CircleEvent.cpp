#include "CircleEvent.h"
#include "CircleEvent.h"

CircleEvent::CircleEvent(double x, double y) : Event(x, y)
{

}
CircleEvent::CircleEvent()
{

}

CircleEvent::~CircleEvent()
{
	
}

std::string CircleEvent::GetType()
{
	return "CIRCLE";
}
