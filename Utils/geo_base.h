//
// Created by dizhong on 05/07/2021.
//

#ifndef COMPUTATIONALGEMOETRY_GEO_BASE_H
#define COMPUTATIONALGEMOETRY_GEO_BASE_H

#include "type.h"
#include "Point.h"
#include <vector>

namespace CG {

    template<typename T> // abstract type
    class Geo_2D {
    public:
        Geo_2D() = default; //Delete default constructor
        virtual ~Geo_2D() {}

        /*
         * Intersection funtions
         */
    public:
        //Only check whether intersect
//        bool Intersect(const Geo_2D<T> &_l) {
//            return this->is_intersect(_l);
//        }

        // Check if the two line intersect and return intersection point if possible
        std::vector<CG::Point2<T> > Intersect(const Geo_2D<T> &_l) {
            if (!this->is_intersect(_l))
                return std::vector<CG::Point2<T> >();

            return this->potential_intersect_pt(_l);
        }

    public:
        virtual bool is_intersect(const Geo_2D<T> &_l) const = 0;

        virtual std::vector<CG::Point2<T> > potential_intersect_pt(const Geo_2D<T> &_l) const = 0;

    public:
        GEO_TYPE type;
    };
}

#endif //COMPUTATIONALGEMOETRY_GEO_BASE_H
