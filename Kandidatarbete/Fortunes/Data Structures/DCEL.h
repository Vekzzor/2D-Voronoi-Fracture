#pragma once
#include <SFML/Graphics.hpp>


#if 0
namespace DCEL
{
	struct Half_edge;
	struct Site;
	//Site
	struct Vertex {
		Half_edge *rep;  /* rep->tail == this */
		sf::Vector2f coords;
	};

	struct Face {
		Site* site;
		//outerComponent
		Half_edge *rep;  /* rep->left == this */
	};

	struct Site
	{
		std::size_t index;
		sf::Vector2f point;
		Face* face;
	};

	struct HE_Edge
	{
		Half_edge *rep;  /* rep->tail == this */
	};

	class Half_edge {
	public:
		Half_edge *next;  /* next->prev == this */
		Half_edge *prev;  /* prev->next == this */
		Half_edge *twin;  /* twin->twin == this */
		//tail / Origin
		Vertex *tail;     /* twin->next->tail == tail &&
							prev->twin->tail == tail */
		//incidentFace
		Face *left;       /* prev->left == left && next->left == left */

		void setTwin(Half_edge* newTwin) {
			this->twin = newTwin;
			newTwin->twin = this;
		};
		void setNext(Half_edge* newNext) {
			this->next = newNext;
			newNext->prev = this;
		};
		void setPrev(Half_edge* newPrev) {
			this->prev = newPrev;
			newPrev->next = this;
		};
	};

	//Traversing the edges around a face
	void getFaceEdges(Face* face)
	{
		Half_edge* start_Edge = face->rep;
		Half_edge* half_edge = start_Edge;
		do
		{
			//Collect edge;
			half_edge = half_edge->next;
		} while (half_edge != start_Edge);
	}

	//Traversing the edges around a vertex
	void getVertexEdges(Vertex* vertex)
	{
		Half_edge* start_Edge = vertex->rep;
		Half_edge* half_edge = start_Edge;
		do
		{
			//Collect edge
			half_edge = half_edge->twin->next;
		} while (half_edge != start_Edge);
	}

	Vertex* vertexNext(Half_edge* half_edge)
	{
		return half_edge->twin->next->tail;
	}
	Vertex* vertexPrev(Half_edge* half_edge)
	{
		return half_edge->prev->twin->tail;
	}
}
#endif

namespace HALF_EDGE
{
	struct HE_Edge;
	//struct Site;
	//Site
	struct HE_Vertex 
	{
		HE_Vertex(int x, int y, int index = -1)
		{
			point = new sf::Vector2f(x, y);
			arrayIndex = index;
		}
		HE_Vertex(sf::Vector2f* vector, int index = -1)
		{
			point = vector;
			arrayIndex = index;
		}
		HE_Vertex(sf::Vector2f vector, int index = -1)
		{
			point = new sf::Vector2f(vector);
			arrayIndex = index;
		}
		~HE_Vertex()
		{
			delete point;
		}
		sf::Vector2f getCoordinates()
		{
			return sf::Vector2f(*point);
		}
		
		HE_Edge *edge = nullptr;  /* rep->tail == this */
		sf::Vector2f* point = nullptr;
		int arrayIndex = -1;
	};

	struct HE_Face 
	{
		HE_Edge *edge = nullptr;  /* rep->left == this */
		sf::Vector2f circumCenter;
		float radius;
		HE_Face(){}
	};

	struct HE_Edge
	{
		HE_Edge *next = nullptr;  /* next->prev == this */
		HE_Edge *prev = nullptr;  /* prev->next == this */
		HE_Edge *twin = nullptr;  /* twin->twin == this */
		HE_Face *face = nullptr;  /* prev->left == left && next->left == left */
		HE_Vertex* vert = nullptr;
		HE_Edge(){}
		void setTwin(HE_Edge* newTwin) {
			this->twin = newTwin;
			newTwin->twin = this;
		};
		void setNext(HE_Edge* newNext) {
			this->next = newNext;
			newNext->prev = this;
		};
		void setPrev(HE_Edge* newPrev) {
			this->prev = newPrev;
			newPrev->next = this;
		};

	};

	//Traversing the edges around a face
	_inline void getFaceEdges(std::vector<HE_Edge*>& edgeList,HE_Face* face)
	{
		HE_Edge* start_Edge = face->edge;
		HE_Edge* half_edge = start_Edge;
		do
		{
			//Collect edge;
			edgeList.push_back(half_edge);
			half_edge = half_edge->next;
		} while (half_edge != start_Edge);

	}

	_inline void getFaceVertices(std::vector<HE_Vertex*>& vertexList, HE_Face* face)
	{
		HE_Edge* start_Edge = face->edge;
		HE_Edge* half_edge = start_Edge;
		do
		{
			//Collect vertex;
			vertexList.push_back(half_edge->vert);
			half_edge = half_edge->next;
		} while (half_edge != start_Edge);
	}

	_inline	bool getPolygon(std::vector<sf::Vector2f>& siteList, HE_Vertex* vertex)
	{
		bool hasNeighbor = true;
		HE_Edge* start_Edge = vertex->edge;
		HE_Edge* half_edge = start_Edge;
		do
		{
			if (half_edge->twin == nullptr)
			{
				hasNeighbor = false;
				break;
			}
			//Collect edge
			siteList.push_back(half_edge->face->circumCenter);
			half_edge = half_edge->twin->next;
		} while (half_edge != start_Edge);
		return hasNeighbor;
	}

	//Traversing the edges around a vertex
	_inline void getVertexEdges(HE_Vertex* vertex)
	{
		HE_Edge* start_Edge = vertex->edge;
		HE_Edge* half_edge = start_Edge;
		do
		{
			//Collect edge
			half_edge = half_edge->twin->next;
		} while (half_edge != start_Edge);
	}

	_inline void getVertexFaces(HE_Vertex* vertex)
	{
		HE_Edge* start_Edge = vertex->edge;
		HE_Edge* half_edge = start_Edge;
		do
		{
			//Collect edge
			half_edge = half_edge->twin->next;
		} while (half_edge != start_Edge);
	}

	_inline HE_Vertex* vertexNext(HE_Edge* half_edge)
	{
		return half_edge->twin->next->vert;
	}
	_inline HE_Vertex* vertexPrev(HE_Edge* half_edge)
	{
		return half_edge->prev->twin->vert;
	}
}