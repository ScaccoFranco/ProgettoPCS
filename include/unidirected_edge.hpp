#pragma once

#include <ostream>

template<typename T>
class unidirected_edge 
{

private:
    T from_;
    T to_;


public:
    unidirected_edge(T t1, T t2) {
        if (t1 < t2) {
            from_ = t1;
            to_ = t2;
        } else {
            from_ = t2;
            to_ = t1;
        }
    }

    // OUTPUT delle varibaili private
    T from() const {
        return from_;
    }

    T to() const {
        return to_;
    }

    bool operator<(const unidirected_edge& other) const {
        if (from_ != other.from_) return from_ < other.from_;
        return to_ < other.to_;
    }

    bool operator==(const unidirected_edge& other) const {
        if (from_ == other.from() && to_ == other.to()) {
            return true;
        } else {
            return false;
        }
    }
};



template<typename T>
std::ostream& operator<<(std::ostream& os, const unidirected_edge<T>& other) 
{
    os << '(' << other.from() << ',' << other.to() << ')';

    return os;
}
