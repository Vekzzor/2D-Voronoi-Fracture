#include "DCEL.h"
using namespace HALF_EDGE;

HE_Vertex::HE_Vertex(int x, int y, int index)
{
	point = new sf::Vector2f(x, y);
	arrayIndex = index;
}
HE_Vertex::HE_Vertex(sf::Vector2f* vector, int index)
{
	point = vector;
	arrayIndex = index;
}
HE_Vertex::HE_Vertex(sf::Vector2f vector, int index)
{
	point = new sf::Vector2f(vector);
	arrayIndex = index;
}
HE_Vertex::~HE_Vertex()
{
	delete point;
}
sf::Vector2f HE_Vertex::getCoordinates()
{
	return sf::Vector2f(*point);
}


HE_Face::HE_Face() {};

HE_Edge::HE_Edge() {}
void HE_Edge::setTwin(HE_Edge* newTwin) {
	this->twin = newTwin;
	newTwin->twin = this;
};
void HE_Edge::setNext(HE_Edge* newNext) {
	this->next = newNext;
	newNext->prev = this;
};
void HE_Edge::setPrev(HE_Edge* newPrev) {
	this->prev = newPrev;
	newPrev->next = this;
};