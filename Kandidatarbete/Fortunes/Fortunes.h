#pragma once
#include <vector>
#include <queue>
#include "SFML/Graphics.hpp"
#include "Data Structures/DCEL.h"
#include "Data Structures/BinTree.h"
#include <random>

class Fortunes
{
	//Tell the compiler that this struct exists so that it can be used everywhere
	struct Site;  
	struct CircleSite; 
	
private: 
	struct Event
	{
		private:
			sf::Vector2f m_position;
		public:
			Event(sf::Vector2f position)
			{
				m_position = position; 
			}
			virtual ~Event()
			{

			}

			sf::Vector2f GetPosition()
			{
				return m_position; 
			}

			void SetPosition(sf::Vector2f position)
			{
				m_position = position; 
			}

			bool operator<(const Event& otherEvent)
			{
				bool isLess = false; 
				if (this->m_position.y < otherEvent.m_position.y)
				{
					isLess = true; 
				}
				return isLess; 
			}

			virtual std::string GetType() = 0; 
	};

	struct BTNode
	{
	private:
		Site* m_siteA = nullptr; 
		Site* m_siteB = nullptr; 
		CircleSite* m_circleEvent = nullptr; 
		DCEL::Half_edge* m_halfEdge = nullptr; 
	public:
		BTNode()
		{

		}
		virtual ~BTNode()
		{

		}

		bool operator<(const BTNode& otherNode)
		{
		/*	bool isLess = false; 
			if (otherNode.m_siteA->GetPosition().x < )
			{

			}*/
		}
	};

	std::priority_queue<Event*> m_eventPriority;
	std::vector<Event*> m_events;

	BinTree<Event*> m_voronoiTree; 

public:
	Fortunes(sf::ConvexShape& shape, int nrOfSites); 
	~Fortunes();

	struct Site : public Event
	{
	private:
	public:
		Site(sf::Vector2f position) : Event(position)
		{

		}
		~Site()
		{

		}

		std::string GetType() override
		{
			return "Site"; 
		}
	};

	struct CircleSite : public Event
	{
	private: 
	public:
		CircleSite(sf::Vector2f position) : Event(position)
		{

		}
		~CircleSite()
		{

		}

		std::string GetType() override
		{
			return "CircleSite"; 
		}
	};

	void GenerateVoronoiDiagram(std::vector<Event>& seeds, int minY, int maxY); 

	void InitiateSites(sf::ConvexShape shape,int nrOfEvents); 
	void HandleSiteEvent(Site* siteEvent); 
	//void HandleCircleEvent(LeafNode* arc);
	bool findCircleCenter(const sf::Vector2f &posA, const sf::Vector2f &posB, const sf::Vector2f &posC, sf::Vector2f& center);
};

