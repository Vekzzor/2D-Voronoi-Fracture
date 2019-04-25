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
		HE_Edge *edge = nullptr;  /* rep->tail == this */
		sf::Vector2f coords;
	};

	struct HE_Face 
	{
		HE_Edge *edge = nullptr;  /* rep->left == this */
	};

	//struct Site : HE_Face
	//{
	//	std::size_t index;
	//	sf::Vector2f point; //CircumCenter
	//	HE_Face* face = nullptr;
	//};

	struct HE_Edge
	{
		HE_Edge *next = nullptr;  /* next->prev == this */
		HE_Edge *prev = nullptr;  /* prev->next == this */
		HE_Edge *twin = nullptr;  /* twin->twin == this */
		HE_Face *face = nullptr;       /* prev->left == left && next->left == left */
		HE_Vertex* vert = nullptr;

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
	void getFaceEdges(HE_Face* face)
	{
		HE_Edge* start_Edge = face->edge;
		HE_Edge* half_edge = start_Edge;
		do
		{
			//Collect edge;
			half_edge = half_edge->next;
		} while (half_edge != start_Edge);
	}

	//Traversing the edges around a vertex
	void getVertexEdges(HE_Vertex* vertex)
	{
		HE_Edge* start_Edge = vertex->edge;
		HE_Edge* half_edge = start_Edge;
		do
		{
			//Collect edge
			half_edge = half_edge->twin->next;
		} while (half_edge != start_Edge);
	}

	HE_Vertex* vertexNext(HE_Edge* half_edge)
	{
		return half_edge->twin->next->vert;
	}
	HE_Vertex* vertexPrev(HE_Edge* half_edge)
	{
		return half_edge->prev->twin->vert;
	}
}