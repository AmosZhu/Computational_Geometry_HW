//
// Created by amos on 5/26/19.
//

#ifndef COMPUTATIONALGEMOETRY_EDGE_H
#define COMPUTATIONALGEMOETRY_EDGE_H

#include "Point.h"
#include "Vec.h"
#include "type.h"
#include "geo_base.h"
#include "common.h"
#include "Conic.h"

namespace CG {

#define LINE_ERR(x) "Line error: " x

    template<typename T>
    class Segment2D;

    template<typename T>
    class Ray2D;

    /******************************************************************************************
     *
     *                                      Line 2D Implementation
     *
     *****************************************************************************************/
    template<typename T> // abstract type
    class Line2D : public Geo_2D<T> {
    public:
        Point2<T> p;
        Point2<T> q;

    public:
        Line2D() { this->type = CG::LINE; }

        Line2D(Point2<T> _p, Point2<T> _q)
                : p(_p), q(_q) { this->type = CG::LINE; }

        Line2D(const Line2D<T> &_rhs) {
            p = _rhs.p;
            q = _rhs.q;
            this->type = _rhs.type;
        }

        virtual ~Line2D() = default;

        // Return the slope of this segment
        // Return the radius, in case of q.x-p.x=0
        double slope() const {
            return atan2((double) (this->q.y - this->p.y), (double) (this->q.x - this->p.x));
        }

        //Compute the angle between another segment
        double angle(Line2D<T> &_l) const {
            double ang1 = this->Slope();
            double ang2 = _l.Slope();

            return ang1 - ang2;
        }

        // Convert it to homogeneous presentation
        Vec3<T> to_homo() const {
            Vec3<T> p1(this->p);
            Vec3<T> p2(this->q);

            return p1.cross(p2);
        }

        // Direction of the ray, present by a unit vector
        Vec2<T> dir() const {
            Vec2<T> d;
            Point2<T> dp = this->q - this->p;
            d.x = dp.x;
            d.y = dp.y;
            d.normalisation();

            return d;
        }

        // To left test on specified points
        LEFT_TEST_RES ToLeftTest(Point2<T> _s) const {
            T detval = p.x * q.y - p.y * q.x
                       + q.x * _s.y - q.y * _s.x
                       + _s.x * p.y - _s.y * p.x;

            if (detval > 0) {
                return LEFT;
            } else if (detval == 0) {
                if (this->onlineTest(_s))
                    return COLINEAR_ON;
                else
                    return COLINEAR_OUT;
            } else {
                return RIGHT;
            }
        }

        // Given x coordinate, get back the y value
        double query_x(double _x) const {
            Vec3<T> l_eq = this->to_homo();
            return (-l_eq.z - l_eq.x * _x) / l_eq.y;
        }

        // Give the y coordinate, get back the x value.
        double query_y(double _y) const {
            Vec3<T> l_eq = this->to_homo();
            return (-l_eq.z - l_eq.y * _y) / l_eq.x;
        }

        // operators
    public:
        Line2D<T> &operator=(const Line2D<T> &_rhs) {
            if (this == &_rhs)
                return *this;

            this->p = _rhs.p;
            this->q = _rhs.q;
            this->type = _rhs.type;

            return *this;
        }

        template<class T1>
        friend T1 operator+(T1 line, T _val) {
            if (&_val == nullptr)
                return line;

            auto res = line;
            res.p = line.p + _val;
            res.q = line.q + _val;
            res.type = line.type;
            return res;
        }

        template<class T1>
        friend T1 operator/(T1 line, T _val) {
            if (&_val == nullptr)
                return line;

            auto res = line;
            res.p = line.p / _val;
            res.q = line.q / _val;
            res.type = line.type;

            return res;
        }

        template<typename Tf>
        friend std::ostream &operator<<(std::ostream &_out, Line2D<Tf> &_obj) {
            _obj.print(_out);
            _out << "{" << _obj.p << ", " << _obj.q << "}";
            return _out;
        }

    public:
        // Do they intersect with each other?
        virtual bool is_intersect(const Geo_2D<T> &_l) const {
            if (_l.type == CG::LINE) {
                auto l = dynamic_cast<const Line2D<T> * >(&_l);
                // we use homogeneous coordinate to find the intersection of the line in 2D
                Vec3<T> l1 = this->to_homo();
                Vec3<T> l2 = l->to_homo();

                Vec3<T> int_pt = l1.cross(l2); // The intersection point
                if (int_pt.z == 0) // Point at infinity, no intersection! Parralle line
                    return false;
                else
                    return true;
            } else if (_l.type == CG::SEGMENT) {
                auto l = dynamic_cast<const Segment2D<T> * >(&_l);
                return l->is_intersect(*this);
            } else if (_l.type == CG::RAY) {
                auto l = dynamic_cast<const Ray2D<T> * >(&_l);
                return l->is_intersect(*this);
            } else if (_l.type == CG::CIRCLE) {
                auto circle = dynamic_cast<const CG::Circle<T> * >(&_l);
                return circle->is_intersect(*this);
            } else {
                return false;
            }
        }

        // Only call it after is_intersect return true
        std::vector<Point2<T>> potential_intersect_pt(const Geo_2D<T> &_l) const {
            // we use homogeneous coordinate to find the intersection of the line in 2D
            std::vector<Point2<T>> res;
            if (_l.type == CG::LINE || _l.type == CG::RAY || _l.type == CG::SEGMENT) {
                auto l = dynamic_cast<const Line2D<T> * >(&_l);
                Vec3<T> l1 = this->to_homo();
                Vec3<T> l2 = l->to_homo();

                Vec3<T> int_pt = l1.cross(l2); // The intersection point
                if (int_pt.z == 0)
                    return res;

                int_pt /= int_pt.z;

                res.push_back(Point2<T>(int_pt.x, int_pt.y));
            } else if (_l.type == CG::CIRCLE) {
                auto circle = dynamic_cast<const Circle<T> *>(&_l);
                res = circle->potential_intersect_pt(*this);
            }
            return res;
        }

        // Assume the point is colinear already, return true if _s on the line.
        virtual bool onlineTest(Point2<T> _s) const { return true; }

    protected:
        // Print the line information
        virtual void print(std::ostream &_out) { _out << "Line: "; }


    };

    /******************************************************************************************
    *
    *                                Segment 2D Implementation
    *
    *****************************************************************************************/
    template<typename T>
    class Segment2D : public Line2D<T> {
    public:
        Segment2D() : Line2D<T>() {
            this->type = CG::SEGMENT;
        }

        Segment2D(Point2<T> _p, Point2<T> _q) : Line2D<T>(_p, _q) {
            this->type = CG::SEGMENT;
        }

        Segment2D(const Segment2D<T> &_rhs) : Line2D<T>(_rhs) {}

        ~Segment2D() = default;

        // Return the length of this segment
        double length() {
            double dx = this->q.x - this->p.x;
            double dy = this->q.y - this->p.y;

            return sqrt(dx * dx + dy * dy);
        }

    public:
        // Do they intersect with each other?
        bool is_intersect(const Geo_2D<T> &_l) const {
            LEFT_TEST_RES r1, r2, r3, r4;

            // They might have intersection
            if (_l.type == CG::SEGMENT) {
                auto l = dynamic_cast<const Segment2D<T> *>(&_l);
                r1 = this->ToLeftTest(l->p);
                r2 = this->ToLeftTest(l->q);
                r3 = l->ToLeftTest(this->p);
                r4 = l->ToLeftTest(this->q);

                if (r1 == CG::COLINEAR_ON || r2 == CG::COLINEAR_ON || r3 == CG::COLINEAR_ON || r4 == CG::COLINEAR_ON)
                    return true;

                if (r1 == r2 || r3 == r4)
                    return false;
                else
                    return true;
            } else if (_l.type == CG::RAY) {
                auto l = dynamic_cast<const Ray2D<T> *>(&_l);
                r1 = this->ToLeftTest(l->p); // Test the anchor point of the ray.

                r3 = l->ToLeftTest(this->p);
                r4 = l->ToLeftTest(this->q);
                if (r1 == CG::COLINEAR_ON || r3 == CG::COLINEAR_ON || r4 == CG::COLINEAR_ON)
                    return true;

                if (r3 == r4 || r3 == CG::COLINEAR_OUT || r4 == CG::COLINEAR_OUT)
                    return false;

                // It might intersect, but we need to judge according to the anchor point and direction of the ray
                //  Check the possible intersection point, then test it on the ray or not.

                auto intersections = this->potential_intersect_pt(*l);
                if (intersections.empty()) return false;
                auto int_pt = intersections[0];

                return l->onlineTest(int_pt);
            } else if (_l.type == CG::LINE) {
                auto l = dynamic_cast<const Line2D<T> * >(&_l);

                r3 = l->ToLeftTest(this->p);
                r4 = l->ToLeftTest(this->q);

                if (r3 == CG::COLINEAR_ON || r4 == CG::COLINEAR_ON)
                    return true;

                if (r3 == r4)
                    return false;

                return true;
            } else if (_l.type == CG::CIRCLE) {
                auto circle = dynamic_cast<const CG::Circle<T> * >(&_l);
                return circle->is_intersect(*this);
            } else {
                return false;
            }
        }

        // Assume the point is colinear, return true if _s on the segment.
        bool onlineTest(Point2<T> _s) const {
            if (this->p.x != this->q.x) {
                return CG::between(this->p.x, this->q.x, _s.x);
            } else // If this is vertical segment
            {
                return CG::between(this->p.y, this->q.y, _s.y);
            }
        }

    protected:
        void print(std::ostream &_out) { _out << "Segment: "; }
    };

    /******************************************************************************************
     *
     *                                      Ray 2D Implementation
     *
     *****************************************************************************************/
    template<typename T>
    class Ray2D : public Line2D<T> {
    public:
        Ray2D() : Line2D<T>() {
            this->type = CG::RAY;
        }

        //Two point define a Ray, _p is the anchor, (_q-_p) define the direction
        Ray2D(Point2<T> _p, Point2<T> _q)
                : Line2D<T>(_p, _q) {
            this->type = CG::RAY;
        }

        Ray2D(const Ray2D<T> &_rhs) : Line2D<T>(_rhs) {}

        ~Ray2D() = default;

    public:
        // Do they intersect with each other?
        bool is_intersect(const Geo_2D<T> &_l) const {
            LEFT_TEST_RES r1, r2;
            if (_l.type == CG::SEGMENT) {
                auto l = dynamic_cast<const Segment2D<T> * >(&_l);
                return l->is_intersect(*this);
            } else if (_l.type == CG::RAY) {
                auto l = dynamic_cast<const Ray2D<T> * >(&_l);

                // Test whether the anchor point is
                r1 = this->ToLeftTest(l->p);
                r2 = l->ToLeftTest(this->p);

                if (r1 == CG::COLINEAR_ON || r2 == CG::COLINEAR_ON)
                    return true;

                auto intersections = this->potential_intersect_pt(*l);
                if (intersections.empty()) return false;
                auto int_pt = intersections[0];

                // if the intersection point is on both ray, then they surely have intersection
                return this->onlineTest(int_pt) & l->onlineTest(int_pt);
            } else if (_l.type == CG::LINE) {
                auto l = dynamic_cast<const Line2D<T> * >(&_l);

                r1 = l->ToLeftTest(this->p);
                if (r1 == CG::COLINEAR_ON) // if the anchor point of the ray lay on the line, then we done!
                    return true;

                auto intersections = this->potential_intersect_pt(*l);
                if (intersections.empty()) return false;
                auto int_pt = intersections[0];

                return this->onlineTest(int_pt);
            } else if (_l.type == CG::CIRCLE) {
                auto circle = dynamic_cast<const CG::Circle<T> * >(&_l);
                return circle->is_intersect(*this);
            } else {
                return false;
            }

        }

        // Assume the point is colinear already, return true if _s on the line.
        bool onlineTest(Point2<T> _s) const {
            if (this->p.x != this->q.x) {
                // Ray point to right
                if (this->q.x > this->p.x) {
                    return _s.x > this->p.x;
                } else {
                    return _s.x < this->p.x;
                }
            } else {
                // Ray point to up
                if (this->q.y > this->p.y) {
                    return _s.y > this->p.y;
                } else {
                    return _s.y < this->p.y;
                }
            }
        }

    protected:
        void print(std::ostream &_out) { _out << "Ray: "; }
    };


}

#endif //COMPUTATIONALGEMOETRY_EDGE_H
