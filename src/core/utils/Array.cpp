//
// Created by mfranc on 17/09/15.
//

#include "Array.h"

template <class T> bool Array::inArray(T &needle, const vector<T> &haystack) {

    int size = haystack.size();
    if (0 == size) { // return early pattern
        return false;
    }

    bool isFound = false;
    for (int i = 0; i < size; i++) {
        if (needle == haystack[i]) {
            isFound = true;
            break;
        }
    }

    return isFound;
}
