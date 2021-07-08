//
// Created by dizhong on 08/07/2021.
//

#ifndef COMPUTATIONALGEMOETRY_CONIC_H
#define COMPUTATIONALGEMOETRY_CONIC_H

#include "Point.h"
#include "Vec.h"
#include "type.h"
#include "geo_base.h"
#include "common.h"

namespace CG {
    template<typename T>
    class Line2D;

    /******************************************************************************************
     *
     *                                      Circle 2D Implementation
     *
     *****************************************************************************************/
    template<typename T>
    class Circle : public Geo_2D<T> {
    public:
        Point2 <T> center; // center
        T radius; //radius

    public:
        Circle() { this->type = CG::CIRCLE; }

        Circle(Point2 <T> _center, double _radius)
                : center(_center), radius(_radius) { this->type = CG::CIRCLE; }

        Circle(const Circle<T> &_rhs) {
            center = _rhs.center;
            radius = _rhs.radius;
            this->type = _rhs.type;
        }

    public:
        // Return the most left point of the circle
        Point2 <T> left_most() const {
            return Point2<T>(center.x - radius, center.y);
        }

        // Return the most right point of the circle
        Point2 <T> right_most() const {
            return Point2<T>(center.x + radius, center.y);
        }

        // Give a point _p test whether inside the circle
        bool query_in_circle(Point2 <T> _p) const {

        };
        /*
         * operators
         */
    public:
        Circle<T> &operator=(const Circle<T> &_rhs) {
            if (this == &_rhs)
                return *this;

            this->center = _rhs.center;
            this->radius = _rhs.radius;
            this->type = _rhs.type;

            return *this;
        }


        Circle<T> operator+(T _val) {
            if (&_val == nullptr)
                return *this;

            auto res = *this;
            res.center = this->center + _val;
            return res;
        }

        Circle<T> operator/(T _val) {
            if (&_val == nullptr)
                return *this;

            auto res = *this;
            res.center = this->center / _val;
            res.radius = this->radius / _val;

            return res;
        }

        template<typename Tf>
        friend std::ostream &operator<<(std::ostream &_out, Circle<Tf> &_obj) {
            _out << "Circle: {center: " << _obj.center << ", radius: " << _obj.radius << "}";
            return _out;
        }

    public:
        // Do they intersect with each other?
        virtual bool is_intersect(const Geo_2D <T> &_obj) const {
            if (_obj.type == CG::LINE || _obj.type == CG::RAY || _obj.type == CG::SEGMENT) {
                auto l = dynamic_cast<const Line2D<T> * >(&_obj);
                /*
                 *  To compute the intersection between line and circle, we just substitute the line function in to
                 *  circle function. We use paramatric equation instead.
                 *  We transfer the line equation as p=E+t*D, where D is the line direction, and E is the start point of the ray
                 *  if the discriminant>=0, then we got intersection
                 */

                auto A = l->p - this->center;
                auto D = l->q - l->p; // direction of the line, seg or ray.

                double a = D.dot(D);
                double b = 2 * D.dot(A);
                double c = A.dot(A) - this->radius * this->radius;

                double discriminant = b * b - 4 * a * c;

                // when the line tangent with the circle
                if (discriminant >= 0)return true;
                else return false;

            } else if (_obj.type == CG::CIRCLE) {
                auto circle = dynamic_cast<const Circle<T> *>(&_obj);
                auto A = this->center - circle->center;
                double dist = sqrt(A.dot(A));

                if (dist == this->radius + circle->radius) {
                    return true;
                } else if (dist < this->radius + this->radius) {
                    return true;
                }
                return false;
            } else {
                return false;
            }
        }

        // Find potential intersection for all types of lines
        std::vector<CG::Point2<T>> potential_intersect_pt(const Geo_2D <T> &_obj) const {
            std::vector<CG::Point2<T>> res;
            if (_obj.type == CG::LINE || _obj.type == CG::RAY || _obj.type == CG::SEGMENT) {
                auto l = dynamic_cast<const Line2D<T> * >(&_obj);
                /*
                 *  To compute the intersection between line and circle, we just substitute the line function in to
                 *  circle function. We use paramatric equation instead.
                 *  We transfer the line equation as p=E+t*D, where D is the line direction, and E is the start point of the ray
                 *  We compute the value t here.
                 *  1. For ray we can only have intersection when t>0.
                 *  2. For line we can have both when t<0 or t>0
                 *  3. For segment we can only have intersection when 0<=t<=1;
                 */

                auto A = l->p - this->center;
                auto D = l->q - l->p; // direction of the line, seg or ray.

                double a = D.dot(D);
                double b = 2 * D.dot(A);
                double c = A.dot(A) - this->radius * this->radius;

                double discriminant = b * b - 4 * a * c;
                double denom = 2 * a;

                // when the line tangent with the circle
                if (discriminant >= 0) {
                    double t = (-b - sqrt(discriminant)) / denom;
                    auto int_pt = l->p + t * D;
                    if ((t >= 0 && t <= 1 && l->type == CG::SEGMENT) ||
                        l->type == CG::LINE ||
                        (t >= 0 && l->type == CG::RAY)) {
                        res.push_back(int_pt);
                    }
                    if (discriminant > 0) {
                        t = (-b + sqrt(discriminant)) / denom;
                        int_pt = l->p + t * D;
                        if ((t >= 0 && t <= 1 && l->type == CG::SEGMENT) ||
                            l->type == CG::LINE ||
                            (t >= 0 && l->type == CG::RAY)) {
                            res.push_back(int_pt);
                        }
                    }
                }
            } else if (_obj.type == CG::CIRCLE) {
                auto circle = dynamic_cast<const Circle<T> *>(&_obj);
                auto A = this->center - circle->center;
                double dist = sqrt(A.dot(A));

                if (dist == this->radius + circle->radius) {
                    res.push_back(Point2<T>());
                } else if (dist < this->radius + this->radius) {
                    res.push_back(Point2<T>());
                    res.push_back(Point2<T>());
                }
            }

            return res;
        }
    };
}

#endif //COMPUTATIONALGEMOETRY_CONIC_H
