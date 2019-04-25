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
	struct LeafNode; 
	
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

	public:
		BTNode()
		{

		}
		virtual ~BTNode()
		{

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
		LeafNode* m_leaf; 
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

		LeafNode* GetLeaf()
		{
			return m_leaf; 
		}
	};

	////////////////////////////////////////////////////////////////////////BST STRUCTURES/////////////////////////////////////////////////////////////////////////////////

	//These keep track of the active arcs. 
	struct LeafNode : public BTNode
	{
	private:
		Site* m_activeParabola;
		CircleSite* m_circleSite; 
	public:
		LeafNode(Site* activeParabola)
		{
			m_activeParabola = activeParabola;
			m_circleSite = nullptr; 
		}
	};

	//These keep track of the Voronoi diagram breakpoints (The edges of the Voronoi diagram)
	struct BreakPoint : public BTNode
	{
	private:
		Site* m_breakpoint[2];
		//This is the new bisector created from the brekpoint
		DCEL::Half_edge* m_halfEdge; 
	public:
		BreakPoint(Site* siteA, Site* siteB)
		{
			if (siteA->GetPosition().x < siteB->GetPosition().x)
			{
				m_breakpoint[0] = siteA; 
				m_breakpoint[1] = siteB; 
			}
			else
			{
				m_breakpoint[0] = siteB;
				m_breakpoint[1] = siteA; 
			}
			//TODO : MAKE SURE TO USE THIS CORRECTLY!
			m_halfEdge = new DCEL::Half_edge();
		}
	};
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void GenerateVoronoiDiagram(std::vector<Event>& seeds, int minY, int maxY); 

	void InitiateSites(sf::ConvexShape shape,int nrOfEvents); 
	void HandleSiteEvent(Site* siteEvent); 
	void HandleCircleEvent(LeafNode* arc);
};

