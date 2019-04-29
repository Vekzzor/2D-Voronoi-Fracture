#include "Fortunes.h"
#include <cmath>


Fortunes::Fortunes(sf::ConvexShape& shape, int nrOfSites)
{ 
	//Initiate the sites
	InitiateSites(shape, nrOfSites); 
	//Create an empty binary tree storing Node structs
}

Fortunes::~Fortunes()
{

}

void Fortunes::GenerateVoronoiDiagram(std::vector<Event>& seeds, int minY, int maxY)
{
	while (!m_eventPriority.empty())
	{
		Event* currEvent = m_eventPriority.top(); 
	
		//Check if Site event
		if (currEvent->GetType() == "Site")
		{
			HandleSiteEvent(dynamic_cast<Site*>(currEvent)); 
		}
		//Check if Circle event
		if (currEvent->GetType() == "CircleSite")
		{
			//HandleCircleEvent(dynamic_cast<CircleSite*>(currEvent)->GetLeaf()); 
		}
	}
}

//Do this only once. 
void Fortunes::InitiateSites(sf::ConvexShape shape, int nrOfEvents)
{
	//Add initial sites inside the geometry. 
	srand(time(NULL)); 
	for (int i = 0; i < nrOfEvents; i++)
	{
		m_eventPriority.push(new Site(sf::Vector2f(shape.getOrigin().x + (rand() % shape.getTextureRect().width),
			shape.getOrigin().y + (rand() % shape.getTextureRect().height)))); 
	}
}

void Fortunes::HandleSiteEvent(Site* siteEvent)
{
	//Check if tree is empty
	if (m_voronoiTree.GetRoot() == nullptr)
	{
		//Insert site into tree (new parabola)
		m_voronoiTree.AddNode(m_voronoiTree.GetRoot(), siteEvent); 
	}
}

bool Fortunes::findCircleCenter(const sf::Vector2f & posA, const sf::Vector2f & posB, const sf::Vector2f & posC, sf::Vector2f & center)
{
	//Get the normalized vectors
	sf::Vector2f AB = (posB - posA); 
	sf::Vector2f BC = (posC - posB); 
	sf::Vector2f normAB = AB / sqrt(pow(AB.x, 2) + pow(AB.y, 2)); 
	sf::Vector2f normBC = BC / sqrt(pow(BC.x, 2) + pow(BC.y, 2)); 

	//double cross = 
	
	return true;
}

//void Fortunes::HandleCircleEvent(LeafNode* arc)
//{
//
//}

