#pragma once
#include "../Types/Point2D.h"

//Calclate the number of intersection points there are
//between two parabolas with foci f1 and f2 and with 
//given directrix

int numOfIntersectionPts(const Point2D &f1, const Point2D &f2, double directrix); 

//Find intersectui points
std::vector<Point2D> findIntersectionPoints(const Point2D &f1, const Point2D &f2, double directrix); 