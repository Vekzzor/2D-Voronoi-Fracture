#pragma once

inline sf::Vector3f* cross(float * arrA, float * arrB,float* crossArr)
{
	crossArr[0] = arrA[1] * arrB[2] - arrA[2] * arrB[1];
	crossArr[1] = arrA[0] * arrB[2] - arrA[2] * arrB[0];
	crossArr[2] = arrA[0] * arrB[1] - arrA[1] * arrB[0];

	sf::Vector3f* crossDone = new sf::Vector3f{ crossArr[0], crossArr[1], crossArr[2] }; 
	return crossDone; 
}
