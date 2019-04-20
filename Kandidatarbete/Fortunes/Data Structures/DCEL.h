#pragma once
#include <SFML/Graphics.hpp>
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