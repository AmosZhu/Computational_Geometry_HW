//
// Created by dizhong on 05/07/2021.
//

#ifndef COMPUTATIONALGEMOETRY_COMMON_H
#define COMPUTATIONALGEMOETRY_COMMON_H

namespace CG {
    template<typename T>
    bool al_compare_less(T _a, T _b) {
        return _a < _b;
    }

/*******************************************************************************************
*                                   Utility functions
*******************************************************************************************/
    template<typename T, typename cmp_f= bool (*)(T, T)>
    T max(T _a, T _b, cmp_f cmp = al_compare_less) {
        if (cmp(_a, _b))
            return _b;
        else
            return _a;
    }

    template<typename T>
    void swap(T &_a, T &_b) {
        T tmp;
        tmp = _a;
        _a = _b;
        _b = tmp;
    }

    template<typename T, typename cmp_f= bool (*)(T, T)>
    bool between(T _a, T _b, T _c, cmp_f cmp = al_compare_less) // Check wheterh _c is between [_a,_b]
    {
        if (_a > _b) {
            swap(_a, _b);
        }

        return _a <= _c && _c <= _b;
    }
}

#endif //COMPUTATIONALGEMOETRY_COMMON_H
