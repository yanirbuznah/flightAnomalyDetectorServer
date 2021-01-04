//
// Created by yanir on 29/12/2020.
//

#ifndef EX4_MINCIRCLE_H
#define EX4_MINCIRCLE_H
#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;


// ------------ DO NOT CHANGE -----------
//class Point{
//public:
//	float x,y;
//	Point(float x,float y):x(x),y(y){}
//};
class Circle{
public:
    Point center;
    float radius;
    Circle(Point c,float r):center(c),radius(r){}
    Circle():center(0,0), radius(0){}
};
// --------------------------------------



bool pointInsideCircle(Point &p, Circle &circle) ;
Circle welzlAlgorithm(vector<Point> &points, int n, vector<Point> R) ;
Circle findMinCircle(Point** points,size_t size);
#endif //EX4_MINCIRCLE_H
