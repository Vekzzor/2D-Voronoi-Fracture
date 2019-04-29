
#include <SFML/Graphics.hpp>
#include "voronoi.h"
#include <iostream>
#include "Fortunes/Data Structures/BinTree.h"
#include "vmath.h"
#include <vector>

Voronoi* vdg;
vector<VoronoiPoint*> ver;
vector<VEdge> edges;

int main()
{
	//FORTUNES ALGORITHM
	for (vector<VoronoiPoint*>::iterator i = ver.begin(); i != ver.end(); i++)
		delete((*i));
	ver.clear();
	edges.clear();
	for (int i = 0; i < 100; i++)
	{
		ver.push_back(new VoronoiPoint(rand() % 500, rand() % 500));
	}

	vdg = new Voronoi();
	double minY = 0;
	double maxY = 500;
	edges = vdg->ComputeVoronoiGraph(ver, minY, maxY);
	delete vdg;
	std::vector<sf::Vertex*> lines;

	for (int i = 0; i < edges.size(); i++)
	{
		sf::Vertex* test = new sf::Vertex[2];
		test[0] = sf::Vector2f((edges[i].VertexA.x), (edges[i].VertexA.y));
		test[1] = sf::Vector2f((edges[i].VertexB.x), (edges[i].VertexB.y));
		lines.push_back(test);
	}

	sf::RenderWindow window(sf::VideoMode(500, 500), "SFML works!");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Escape)
						window.close();
					break;
			}

		}

		window.clear();
		for(auto line : lines)
			window.draw(line, 2, sf::Lines);
		window.display();


		float arrA[] = { 3,-5,4 };
		float  arrB[] = { 2,6,5 };
		float crossArr[] = { 0,0,0 }; 

		std::cout << "Current vectors: " << std::endl; 
		for (int i = 0; i < 3; i++)
		{
			std::cout << "arrA[" << i << "] : " << arrA[i] << std::endl; 
			std::cout << "arrB[" << i << "] : " << arrB[i] << std::endl;
		}
		system("PAUSE"); 

		sf::Vector3f* crossDone; 
		crossDone = cross(arrA, arrB, crossArr);
		
		std::cout << "Result from the cross:" << std::endl; 
		for (int i = 0; i < 3; i++)
		{
			std::cout << "X: " << crossDone->x << std::endl; 
			std::cout << "Y: " << crossDone->y << std::endl;
			std::cout << "Z: " << crossDone->z << std::endl;
		}
		delete crossDone; 
	}
	
	return 0; 
}