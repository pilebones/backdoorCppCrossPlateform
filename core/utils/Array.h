//
// Created by mfranc on 17/09/15.
//

#ifndef CPPCROSSPLATEFORMSOCKETPROVIDER_ARRAY_H
#define CPPCROSSPLATEFORMSOCKETPROVIDER_ARRAY_H

#include <vector>

using namespace std;

class Array {
public:
    template <class T> static bool inArray(T &needle, const vector< T > &haystack);
};
#endif //CPPCROSSPLATEFORMSOCKETPROVIDER_ARRAY_H
