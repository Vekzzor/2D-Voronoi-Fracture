
#include <SFML/Graphics.hpp>
#include "voronoi.h"
#include <iostream>
#include "Fortunes/Data Structures/BinTree.h"



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

	BinTree<int> bT;

	bT.AddNode(bT.GetRoot(), 50); 
	bT.AddNode(bT.GetRoot(), 20); 
	bT.AddNode(bT.GetRoot(), 70); 
	bT.AddNode(bT.GetRoot(), 80);
	bT.AddNode(bT.GetRoot(), 71);
	bT.AddNode(bT.GetRoot(), 81);
	bT.AddNode(bT.GetRoot(), 10); 
	bT.AddNode(bT.GetRoot(), 5);
	bT.AddNode(bT.GetRoot(), 22); 
	bT.AddNode(bT.GetRoot(), 49);

	int serachNum = 0;
	string stop = ""; 

	while (stop != "Y")
	{
		std::cout << "Please enter the number you want to search for: " << std::endl;
		std::cin >> serachNum;

		if (bT.Search(bT.GetRoot(), serachNum))
		{
			std::cout << "Number " << serachNum << " was found!" << std::endl;
		}
		else
		{
			std::cout << "Number " << serachNum << " was not found!" << std::endl;
		}

		int numDeleted = 0; 

		std::cout << "Enter a number to be deleted: "; 
		std::cin >> numDeleted; 
		bT.Delete(bT.GetRoot(), numDeleted); 

		std::cout << "Number " << numDeleted << " has been deleted! Let's search for it!" << std::endl; 
		if (bT.Search(bT.GetRoot(), numDeleted))
		{
			std::cout << numDeleted << " was still found, this should not happen!" << std::endl; 
		}
		else
		{
			std:cout << numDeleted << " was deleted successfully!" << std::endl; 
		}
		
		std::cout << "Do you want to stop the BinaryTree? (Y/N):"; 
		std::cin >> stop; 
	}

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
	}

	return 0; 
}