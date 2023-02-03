#ifndef UTIL_H
#define UTIL_H

#include <memory>
#include <utility>

template<class T>
struct pointer_compare {
    bool operator() (const pair<uint32_t, weak_ptr<GameObject>> &leftPtr, const pair<uint32_t, weak_ptr<GameObject>> &rightPtr)const {
        if(leftPtr.first != rightPtr.first)
            return leftPtr.first < rightPtr.first;

        auto lptr = leftPtr.second.lock(), rptr = rightPtr.second.lock();
        if (!rptr) return false;
        if (!lptr) return true;
        return lptr.get() < rptr.get();
    }
};

#endif // UTIL_H
