//
// Created by yanir on 29/12/2020.
//

// 207631466	204752000

#include "minCircle.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;


// you may add helper functions here
float distance(Point &a, Point &b) {
    return sqrt(pow(a.x-b.x,2) + pow(a.y-b.y,2));
}


Circle circleFromTwoPoints(Point &a, Point &b) {
    Point center = Point((a.x + b.x) / 2, (a.y + b.y) / 2);
    float radius = distance(a, b) / 2;
    return {center, radius};
}

Point centerCircle(float bx, float by, float cx, float cy, Point a) {
    float powB = bx * bx + by * by;
    float powC = cx * cx + cy * cy;
    float bMinusC = bx * cy - by * cx;
    float centerX = ((cy * powB - by * powC) / (2 * bMinusC)) + a.x;
    float centerY = ((bx * powC - cx * powB) / (2 * bMinusC)) + a.y;
    return {centerX, centerY};
}

Circle circleFromThreePoints(Point &a, Point &b, Point &c) {
    Point center = centerCircle(b.x - a.x, b.y - a.y, c.x - a.x, c.y - a.y, a);
    return {center, distance(center, a)};
}

bool pointInsideCircle(Point &p, Circle &circle) {
    return distance(circle.center, p) <= circle.radius;
}

bool pointsInsideCircle(vector<Point> &points, Circle &circle) {
    int size = points.size();
    for (int i = 0; i < size; ++i) {
        if (!(pointInsideCircle(points[i], circle)))
            return false;
    }
    return true;
}


Circle trivialCircle(vector<Point> points) {
    int size = points.size();
    if (size == 0) {
        return {{0, 0}, 0};
    }
    if (size == 1) {
        return {points[0], 0};
    }
    if (size == 2) {
        return circleFromTwoPoints(points[0], points[1]);
    }
    if (size == 3) {
        //check if 2 points is enough to minimal circle.
        for (int i = 0; i < 3; i++) {
            for (int j = i + 1; j < 3; j++) {
                Circle circle = circleFromTwoPoints(points[i], points[j]);
                if (pointsInsideCircle(points, circle))
                    return circle;
            }
        }
        return circleFromThreePoints(points[0], points[1], points[2]);
    } else {
        //more then 3.
        exit(1);
    }
}

Circle welzlAlgorithm(vector<Point> &points, int n, vector<Point> R) {

    // Base case when all points processed or |R| = 3
    if (n == 0 || R.size() == 3) {
        return trivialCircle(R);
    }

    long int randomIndex = rand() % n;
    Point randomPoint = points[randomIndex];

    swap(points[randomIndex], points[n - 1]);

    Circle disc = welzlAlgorithm(points, n - 1, R);


    if (pointInsideCircle(randomPoint, disc)) {
        return disc;
    }

    // Otherwise, must be on the boundary of the MEC
    R.push_back(randomPoint);

    // Return the MEC for P - {p} and R U {p}
    return welzlAlgorithm(points, n - 1, R);
}

// implement
Circle findMinCircle(Point **points, size_t size) {

    vector<Point> pointsVector;
    for (int i = 0; i < size; ++i) {
        pointsVector.push_back(*points[i]);
    }

    return welzlAlgorithm(pointsVector, size, {});
}


