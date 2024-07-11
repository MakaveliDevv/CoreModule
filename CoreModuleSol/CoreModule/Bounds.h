#ifndef BOUNDS_H
#define BOUNDS_H

struct Bounds {
    float left;
    float top;
    float right;
    float bottom;

    bool intersects(const Bounds& other) const {
        return !(right < other.left || left > other.right || bottom < other.top || top > other.bottom);
    }
};

#endif // BOUNDS_H
