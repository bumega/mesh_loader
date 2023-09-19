
#ifndef M_LD_ELEMENTS_HPP
#define M_LD_ELEMENTS_HPP

#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <unordered_map>
#include <exception>
#include <array>

//сишный массив
///structures
struct Node {
    size_t ID;
    std::array<double, 3> coord;
    bool flag = false;
};

struct FiniteElement {
    size_t ID;
    size_t area_ID;
    std::vector<size_t> nodes_id;
};


///custom hasher
inline size_t func(double a1, double a2, double a3) {
    return std::hash<double>()(a1) + std::hash<double>()(a2) + std::hash<double>()(a3);
}

class tuple_hasher {
public:
    size_t operator()(const ::std::tuple<double, double, double> coord) const {
        return std::apply(func, coord);
    }
};


///custom comparators
inline auto cmprt = [](const Node& a, const Node& b) { return a.ID < b.ID; };
inline auto cmp_r = [](const Node& a,const Node& b) {
    return a.coord[0] < b.coord[0] or (a.coord[1] < b.coord[1] and a.coord[0] == b.coord[0]) or
           (a.coord[2] < b.coord[2] and a.coord[0] == b.coord[0] and a.coord[1] == b.coord[1]);
};


///exceptions
class except_ : public std::exception {

public:
    explicit except_(std::string &&st) : description(std::move(st)) {}

    [[nodiscard]] const char *what() const noexcept override {
        return description.c_str();
    }

private:
    std::string description;
};

#endif //M_LD_ELEMENTS_HPP
