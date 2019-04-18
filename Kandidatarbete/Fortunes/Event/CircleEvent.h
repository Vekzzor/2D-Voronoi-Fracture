#pragma once
#include "Event.h"

class CircleEvent : public Event
{
private:

public:
	CircleEvent(double x, double y); 
	CircleEvent(); 
	~CircleEvent(); 

	std::string GetType() override; 
};

