//
// Created by dizhong on 08/07/2021.
//


#include "Utils/Line.h"
#include "Utils/Conic.h"
#include "Utils/Point.h"
#include <set>
#include <queue>
#include <vector>
#include "SweepLine.h"

#define MAX_X 10e5

enum pos_t {
    LEFT = 0,
    RIGHT,
};

struct event_t {
    CG::Geo_2D<double> *obj;
    CG::Point2<double> pt;
    pos_t pos;

    event_t() {}

    event_t(CG::Geo_2D<double> *_obj, CG::Point2<double> _pt, pos_t _pos) : obj(_obj), pt(_pt), pos(_pos) {}

    bool operator<(const event_t &_rhs) const {
        return this->pt.x > _rhs.pt.x;
    }
};

struct line_status_t {
    CG::Geo_2D<double> *obj;
    double y;

    line_status_t() {}

    line_status_t(CG::Geo_2D<double> *_obj, double _y) : obj(_obj), y(_y) {}

    bool operator<(const line_status_t &_rhs) const {
        return this->y > _rhs.y;
    }

    bool operator==(const line_status_t &_rhs) const {
        return this->obj == _rhs.obj;
    }
};

std::vector<CG::Point2<double>> intersection_sweep_line(const std::vector<CG::Geo_2D<double> *> &geo_objects) {
    /*
     * 1. Order the geometry objects by their x coordinates from left to right.
     *    We save them in a priority queue.
     */
    std::priority_queue<event_t> Q;
    for (auto obj: geo_objects) {
        event_t l, r;
        if (obj->type == CG::SEGMENT) {
            // Order by x coordinate value
            auto segment = dynamic_cast<CG::Segment2D<double> *>(obj);
            if (segment->q.x > segment->p.x) {
                l = event_t(segment, segment->p, LEFT);
                r = event_t(segment, segment->q, RIGHT);
            } else {
                r = event_t(segment, segment->p, RIGHT);
                l = event_t(segment, segment->q, LEFT);
            }
        } else if (obj->type == CG::RAY) {
            auto ray = dynamic_cast<CG::Ray2D<double> *>(obj);
            if (ray->q.x > ray->p.x) {
                l = event_t(ray, ray->p, LEFT);
                r = event_t(ray, CG::Point2<double>(MAX_X, ray->query_x(MAX_X)), RIGHT);
            } else {
                r = event_t(ray, ray->p, RIGHT);
                l = event_t(ray, CG::Point2<double>(-MAX_X, ray->query_x(-MAX_X)), LEFT);
            }
        } else if (obj->type == CG::LINE) {
            auto line = dynamic_cast<CG::Line2D<double> *>(obj);
            l = event_t(line, CG::Point2<double>(-MAX_X, line->query_x(-MAX_X)), LEFT);
            r = event_t(line, CG::Point2<double>(MAX_X, line->query_x(MAX_X)), RIGHT);
        } else if (obj->type == CG::CIRCLE) {
            auto circle = dynamic_cast<CG::Circle<double> *>(obj);
            l = event_t(circle, circle->left_most(), LEFT);
            r = event_t(circle, circle->right_most(), RIGHT);
        }

        Q.push(l);
        Q.push(r);
    }

    /*
     * 2. Use the sweep line sweep from left to right.
     */
    std::vector<CG::Point2<double>> intersections;
    std::multiset<line_status_t> active_objects; // The active objects which intersect with the sweep line

    while (!Q.empty()) {
        auto event = Q.top();
        if (event.pos == LEFT) {
            /*
             * a. if the point is the left of the object.
             *      we detect the intersection of the new object with all objects maintianed by the sweep line.
             *      We then push this object into currect active objects.
             */
            auto new_obj = line_status_t(event.obj, event.pt.y);
            for (auto item:active_objects) {
                auto inter_pts = item.obj->Intersect(*new_obj.obj);
                for (const auto &pt:inter_pts) {
                    intersections.push_back(pt);
                }

            }

            active_objects.insert(new_obj);
        } else if (event.pos == RIGHT) {
            //-------> b. if meet a right point, it means we left the region of this object, remove it from current active objects structure
            auto remove_obj = line_status_t(event.obj, event.pt.y);
            active_objects.erase(remove_obj);
        }

        Q.pop();
    }

    return intersections;
}