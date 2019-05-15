#include "Delunay.h"
#include <chrono>
#include <iostream>

Delunay::Delunay()
{
}


Delunay::~Delunay()
{

}

std::vector<Triangle>& Delunay::Triangulate(std::vector<HALF_EDGE::HE_Vertex*>& points)
{
	//sf::RenderWindow window(sf::VideoMode(1000, 1000), "Paused: 0");
	bool pause = false;
	double timer = 0;
		_vertices = points;
		// Determinate the super triangle
		float minX = points[0]->point->x;
		float minY = points[0]->point->y;
		float maxX = minX;
		float maxY = minY;

		for (HALF_EDGE::HE_Vertex* pt : points)
		{
			minX = std::min(minX, pt->point->x);
			maxX = std::max(maxX, pt->point->x);
			minY = std::min(minY, pt->point->y);
			maxY = std::max(maxY, pt->point->y);
		}

		const float dx = maxX - minX;
		const float dy = maxY - minY;
		const float deltaMax = std::max(dx, dy);
		const float midx = (minX + maxX)*0.5f;
		const float midy = (minY + maxY)*0.5f;

		_triangles.clear();
		_edges.clear();

		HALF_EDGE::HE_Vertex* p1 = new  HALF_EDGE::HE_Vertex(midx - 20 * deltaMax, midy - deltaMax);
		HALF_EDGE::HE_Vertex* p2 = new  HALF_EDGE::HE_Vertex(midx, midy + 20 * deltaMax);
		HALF_EDGE::HE_Vertex* p3 = new  HALF_EDGE::HE_Vertex(midx + 20 * deltaMax, midy - deltaMax);

		_triangles.push_back({ p1, p2, p3 });

		int pointIndex = 0;
		double circumCheckTime = 0;
		double dublicateEdgesTime = 0;
		double triangleCreationTime = 0;
		double pushBackTime = 0;
		for (HALF_EDGE::HE_Vertex* const pt : points)
		{
			std::vector<DEdge> edges;
			
			for (Triangle& t : _triangles)
			{
				const auto dist = (t.circle.x - pt->point->x) * (t.circle.x - pt->point->x) +
					(t.circle.y - pt->point->y) * (t.circle.y - pt->point->y);
				if ((dist - t.circle.radius) <= eps)
				{
					//Pushing bad triangle
					t.isBad = true;
					for (int i = 0; i < 3; i++)
						edges.push_back(t.e[i]);		
				}
				else
				{
					t.isBad = false;
					//does not contain in his circum center
				}
			}
			auto is_bad = [&](auto const& tri) { return tri.isBad; };
			erase_where(_triangles, is_bad);

			/* Delete duplicate edges. */
			std::vector<bool> remove(edges.size(), false);
			//for (int i = 0; i < edges.size(); ++i)
			//{
			//	for (int k = 0; k < edges.size(); ++k)
			//	{
			//		if (i == k) {
			//			continue;
			//		}
			//		if (edges[i] == edges[k])
			//		{
			//			remove[i] = true;
			//			remove[k] = true;
			//		}
			//	}
			//}
			if (pointIndex == 29000)
			{
				//_triangles[0].lul = pointIndex;
			}

			for (auto it1 = edges.begin(); it1 != edges.end(); ++it1) {
				for (auto it2 = edges.begin(); it2 != edges.end(); ++it2) {
					if (it1 == it2) {
						continue;
					}
					if (*it1 == *it2) {
						remove[std::distance(edges.begin(), it1)] = true;
						remove[std::distance(edges.begin(), it2)] = true;
					}
				}
			}
			auto is_duplicate = [&](auto const& e) { return remove[&e - &edges[0]]; };
			erase_where(edges, is_duplicate);

			/* Update triangulation. */
			for (const DEdge &e : edges)
			{
				_triangles.push_back(Triangle(e.v1, e.v2, pt));
			}
			pointIndex++;
#if 0
			sf::Event event;
			while (timer < 3)
			{
				auto start = std::chrono::system_clock::now();
				window.clear();
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
						else if (event.key.code == sf::Keyboard::Space)
						{
							pause = 1;
							window.setTitle("Paused: " + std::to_string(pause));
						}
						else if (event.key.code == sf::Keyboard::Enter)
						{
							pause = 0;
							window.setTitle("Paused: " + std::to_string(pause));
							start = std::chrono::system_clock::now();
						}
						break;
					}
				}
				if (!pause)
				{
					for (auto &triangle : _triangles)
					{
						sf::CircleShape point;
						point.setRadius(triangle.circle.radius);
						point.setPosition(sf::Vector2f(triangle.circle.x, triangle.circle.y) - sf::Vector2f(triangle.circle.radius, triangle.circle.radius));
						point.setOutlineThickness(1.0f);
						point.setOutlineColor({ 255,0,255 });
						point.setFillColor(sf::Color::Transparent);
						window.draw(point);

						point = sf::CircleShape();
						point.setRadius(2);
						point.setPosition(sf::Vector2f(triangle.circle.x, triangle.circle.y) - sf::Vector2f(2, 2));
						point.setFillColor({ 0,0,255 });
						window.draw(point);
						sf::Vertex test[3];
						test[0].position = *triangle.v1->point;
						test[1].position = *triangle.v2->point;
						test[2].position = *triangle.v3->point;
						window.draw(test, 3, sf::LineStrip);
						point = sf::CircleShape();
						point.setRadius(2);
						point.setPosition(*triangle.v1->point- sf::Vector2f(2, 2));
						point.setFillColor({ 255,0,0 });
						window.draw(point);
						point = sf::CircleShape();
						point.setRadius(2);
						point.setPosition(*triangle.v2->point - sf::Vector2f(2, 2));
						point.setFillColor({ 255,0,0 });
						window.draw(point);
						point = sf::CircleShape();
						point.setRadius(2);
						point.setPosition(*triangle.v3->point - sf::Vector2f(2, 2));
						point.setFillColor({ 255,0,0 });
						window.draw(point);
					}
					window.display();
					std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - start;
					timer += elapsed_seconds.count();
				}
			
			}
			timer = 0;
#endif
		}

	/*	std::cout << circumCheckTime << std::endl;
		std::cout << dublicateEdgesTime << std::endl;
		std::cout << triangleCreationTime << std::endl;
		system("pause");*/
		auto is_part_STriangle = [&](Triangle const& tri) {
			return (tri.v1->arrayIndex == -1 || tri.v2->arrayIndex == -1 || tri.v3->arrayIndex == -1); };

		//return ((tri.v1 == p1 || tri.v2 == p1 || tri.v3 == p1) ||
		//	(tri.v1 == p2 || tri.v2 == p2 || tri.v3 == p2) ||
		//	(tri.v1 == p3 || tri.v2 == p3 || tri.v3 == p3)); };

		erase_where(_triangles, is_part_STriangle);

		delete p1;
		delete p2;
		delete p3;

		for (auto & t : _triangles)
		{
			if (t.orientation() == 2)
				t.flipOrientation();
		}
#if 0
		sf::Event event;
		while (timer < 10)
		{
			auto start = std::chrono::system_clock::now();
			window.clear();
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
					else if (event.key.code == sf::Keyboard::Space)
					{
						pause = !pause;
						window.setTitle("Paused: " + pause);
					}
					break;
				}
			}
			for (auto &triangle : _triangles)
			{
				sf::CircleShape point;
				point.setRadius(triangle.circle.radius);
				point.setPosition(sf::Vector2f(triangle.circle.x, triangle.circle.y) - sf::Vector2f(triangle.circle.radius, triangle.circle.radius));
				point.setOutlineThickness(1.0f);
				point.setOutlineColor({ 255,0,255 });
				point.setFillColor(sf::Color::Transparent);
				window.draw(point);

				point = sf::CircleShape();
				point.setRadius(2);
				point.setPosition(sf::Vector2f(triangle.circle.x, triangle.circle.y) - sf::Vector2f(2, 2));
				point.setFillColor({ 0,0,255 });
				window.draw(point);
				sf::Vertex test[3];
				test[0].position = *triangle.v1->point;
				test[1].position = *triangle.v2->point;
				test[2].position = *triangle.v3->point;
				window.draw(test, 3, sf::LineStrip);
			}
			window.display();
			std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - start;
			timer += elapsed_seconds.count();
		}
		window.close();
#endif
	return _triangles;
}
