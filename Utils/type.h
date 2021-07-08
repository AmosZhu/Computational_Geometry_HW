//
// Created by dizhong on 05/07/2021.
//

#ifndef COMPUTATIONALGEMOETRY_TYPE_H
#define COMPUTATIONALGEMOETRY_TYPE_H


namespace CG {
    enum GEO_TYPE {
        SEGMENT = 0,
        RAY,
        LINE,
        CIRCLE
    };

    enum LEFT_TEST_RES {
        LEFT,
        COLINEAR_ON, // The point on the edge
        COLINEAR_OUT, // The point colinear but not on the edge
        RIGHT
    };

    typedef long long int64;
}

#endif //COMPUTATIONALGEMOETRY_TYPE_H
