#ifndef BOUNDS_H
#define BOUNDS_H

struct Bounds {
    float left;
    float top;
    float right;
    float bottom;

    bool intercepts(const Bounds& other) const {
        if (right < other.left || left > other.right || bottom < other.top || top > other.bottom) {
            return false;
        }
        else {
            return true;
        }
    }
};

#endif 
