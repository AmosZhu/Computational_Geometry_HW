//
// Created by dizhong on 08/07/2021.
//

#include <cstdio>
#include <iostream>
#include "Utils/Line.h"
#include "Utils/Conic.h"
#include "SweepLine.h"
#include <vector>
#include <algorithm>

using namespace CG;

int main(int argc, const char *argv[]) {
    int noofSegments, noofRays, noofLines, noofCircles;
    std::cin >> noofSegments >> noofRays >> noofLines >> noofCircles;
    std::vector<Geo_2D<double> *> geo_objects;

    std::vector<double> coordinate_xy; // save all elements to this array for display sake, we need to scale to proper size

    for (int i = 0; i < noofSegments; i++) {
        auto segment = new Segment2D<double>();
        std::cin >> segment->p.x >> segment->p.y;
        std::cin >> segment->q.x >> segment->q.y;
        geo_objects.push_back(segment);
    }

    for (int i = 0; i < noofRays; i++) {
        auto ray = new Ray2D<double>();
        std::cin >> ray->p.x >> ray->p.y;
        std::cin >> ray->q.x >> ray->q.y;
        geo_objects.push_back(ray);

    }

    for (int i = 0; i < noofLines; i++) {
        auto line = new Line2D<double>();
        std::cin >> line->p.x >> line->p.y;
        std::cin >> line->q.x >> line->q.y;
        geo_objects.push_back(line);

    }

    for (int i = 0; i < noofCircles; i++) {
        auto circle = new Circle<double>();
        std::cin >> circle->center.x >> circle->center.y;
        std::cin >> circle->radius;
        geo_objects.push_back(circle);
    }

    auto intersections = intersection_sweep_line(geo_objects);

    std::cout << intersections.size() << std::endl;
    return 0;
}