#pragma once
#include <string>
class Event
{
private:
	double m_x, m_y;
public:
	Event(double x, double y);
	Event();
	virtual ~Event();

	bool operator<(const Event& otherEvent); 

	std::string virtual GetType() = 0; 
};