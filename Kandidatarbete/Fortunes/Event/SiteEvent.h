#pragma once
#include "Event.h"

class SiteEvent : public Event
{
private:

public:
	SiteEvent(double x, double y); 
	SiteEvent(); 
	~SiteEvent();

	std::string GetType() override;
};