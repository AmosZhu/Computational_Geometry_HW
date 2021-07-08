//
// Created by dizhong on 08/07/2021.
// The BO alogorithmn is too complicated to handle all cases,
// So I use a simple version of sweep line algorithm.
//
// 1. When encounter a left point, we detect the new edge with all edges in current sweep line active edges.
// 2. Pop out the edge when we meet a right point.
// 3. This might not meet O(nlog(n)), but at least we ignore the case when the edges was sperate by x coordinate.
// 4. The worst case is when the elements are all lines, the time will be O(n^2).
//

#ifndef COMPUTATIONALGEMOETRY_SWEEPLINE_H
#define COMPUTATIONALGEMOETRY_SWEEPLINE_H

#include <vector>

std::vector<CG::Point2<double>> intersection_sweep_line(const std::vector<CG::Geo_2D<double> *> &geo_objects);


#endif //COMPUTATIONALGEMOETRY_SWEEPLINE_H
