
#ifndef MINCIRCLE_H
#define MINCIRCLE_H
#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;


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
#endif //MINCIRCLE_H
