#include "SiteEvent.h"
#include "SiteEvent.h"

SiteEvent::SiteEvent(double x, double y) : Event(x, y)
{

}
SiteEvent::SiteEvent()
{

}

SiteEvent::~SiteEvent()
{

}

std::string SiteEvent::GetType()
{
	return "SITE";
}
