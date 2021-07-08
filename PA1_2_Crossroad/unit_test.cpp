//
// Created by dizhong on 05/07/2021.
//

#include <cstdio>
#include <iostream>
#include "Utils/Line.h"
#include "Utils/Conic.h"
#include "SweepLine.h"
#include <vector>
#include <algorithm>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

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

        coordinate_xy.push_back(segment->p.x);
        coordinate_xy.push_back(segment->p.y);
        coordinate_xy.push_back(segment->q.x);
        coordinate_xy.push_back(segment->q.y);
    }

    for (int i = 0; i < noofRays; i++) {
        auto ray = new Ray2D<double>();
        std::cin >> ray->p.x >> ray->p.y;
        std::cin >> ray->q.x >> ray->q.y;
        geo_objects.push_back(ray);

        coordinate_xy.push_back(ray->p.x);
        coordinate_xy.push_back(ray->p.y);
        coordinate_xy.push_back(ray->q.x);
        coordinate_xy.push_back(ray->q.y);
    }

    for (int i = 0; i < noofLines; i++) {
        auto line = new Line2D<double>();
        std::cin >> line->p.x >> line->p.y;
        std::cin >> line->q.x >> line->q.y;
        geo_objects.push_back(line);

        coordinate_xy.push_back(line->p.x);
        coordinate_xy.push_back(line->p.y);
        coordinate_xy.push_back(line->q.x);
        coordinate_xy.push_back(line->q.y);
    }

    for (int i = 0; i < noofCircles; i++) {
        auto circle = new Circle<double>();
        std::cin >> circle->center.x >> circle->center.y;
        std::cin >> circle->radius;
        geo_objects.push_back(circle);
    }

    auto intersections = intersection_sweep_line(geo_objects);
    std::cout << "There are " << intersections.size() << " intersect points in all" << std::endl;

    // Display the geometries
    std::sort(coordinate_xy.begin(), coordinate_xy.end());
    auto l_min = coordinate_xy[0];
    auto l_max = coordinate_xy[coordinate_xy.size() - 1];

    if (abs(l_min) > l_max)
        l_max = abs(l_min);

    // Normalise the coordinate to (0,1) for opencv display
    auto l_size = l_max;

    double w = 1080;
    double h = 1080;

    cv::Mat img(w, h, CV_8UC3);
    img.setTo(cv::Scalar::all(255));

    for (auto v:geo_objects) {
        if (v->type == CG::LINE) {
            auto line = dynamic_cast<CG::Line2D<double> *>(v);
            auto line1 = (*line / l_size + 1) / 2; // Normalise the line

            auto p1 = cv::Point(0, line1.query_x(0) * h);
            auto p2 = cv::Point(1 * w, line1.query_x(1) * h);
            cv::line(img, p1, p2, cv::Scalar(0, 255, 0), 2);

        } else if (v->type == CG::RAY) {
            auto ray = dynamic_cast<CG::Ray2D<double> *>(v);
            auto ray1 = (*ray / l_size + 1) / 2;

            std::cout << ray1 << std::endl;
            cv::Point p1, p2;
            if (ray1.p.x > ray1.q.x) {
                p1 = cv::Point(ray1.p.x * w, ray1.p.y * h);
                p2 = cv::Point(0, ray1.query_x(0) * h);
            } else {
                p1 = cv::Point(ray1.p.x * w, ray1.p.y * h);
                p2 = cv::Point(1 * w, ray1.query_x(1) * h);
            }

            cv::line(img, p1, p2, cv::Scalar(0, 0, 255), 2);
        }
        if (v->type == CG::SEGMENT) {
            auto segment = dynamic_cast<CG::Segment2D<double> *>(v);
            auto segment1 = (*segment / l_size + 1) / 2;

            auto p1 = cv::Point(segment1.p.x * w, segment1.p.y * h);
            auto p2 = cv::Point(segment1.q.x * w, segment1.q.y * h);
            cv::line(img, p1, p2, cv::Scalar(0, 0, 0), 2);
        }
        if (v->type == CG::CIRCLE) {
            auto circle = dynamic_cast<CG::Circle<double> *>(v);
            auto circle1 = (*circle / l_size + 1) / 2;

            auto center = cv::Point(circle1.center.x * w, circle1.center.y * h);
            auto radius = circle1.radius * w;
            cv::circle(img, center, radius, cv::Scalar(255, 0, 0), 2);
        }
    }

    // draw intersections
    for (auto item:intersections) {
        auto p_norm = (item / l_size + 1) / 2;
        auto center = cv::Point(p_norm.x * w, p_norm.y * h);
        cv::circle(img, center, 10, cv::Scalar(0, 128, 128), 1);
    }

    cv::imshow("display", img);
    cv::waitKey();
    cv::destroyAllWindows();

}