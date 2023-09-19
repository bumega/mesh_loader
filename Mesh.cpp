#include "Mesh.hpp"

///mesh loader from file O(n)
void AneuMeshLoader::LoadMesh(std::ifstream &file_it) {
    int kol;
    int dim;
    file_it >> kol >> dim;
    Node tmp;
    double sch;
    for (size_t i = 0; i < kol; i++) {
        tmp.ID = i + 1;
        tmp.flag = false;
        //tmp.coord.clear();
        for (size_t j = 0; j < dim; j++) {
            file_it >> sch;
            //tmp.coord.push_back(sch);
            tmp.coord[j] = sch;
        }
        node_c.push_back(tmp);

        node_coord[std::make_tuple(tmp.coord[0], tmp.coord[1], tmp.coord[2])] = tmp.ID;
    }
    file_it >> kol >> dim;
    FiniteElement tm;
    for (size_t i = 0; i < kol; i++) {
        tm.ID = i + 1;
        file_it >> tm.area_ID;
        tm.nodes_id.clear();
        for (size_t j = 0; j < dim; j++) {
            file_it >> sch;
            node_c[sch - 1].flag = true;
            tm.nodes_id.push_back(sch);
        }
        std::sort(tm.nodes_id.begin(), tm.nodes_id.end());
        tetra_c.push_back(tm);
    }
    file_it >> kol >> dim;
    for (size_t i = 0; i < kol; i++) {
        tm.ID = i + 1;
        file_it >> tm.area_ID;
        tm.nodes_id.clear();
        for (size_t j = 0; j < dim; j++) {
            file_it >> sch;
            tm.nodes_id.push_back(sch);
        }
        std::sort(tm.nodes_id.begin(), tm.nodes_id.end());
        triang_c.push_back(tm);
    }
}

///getter of nodes O(1)
std::vector<Node> AneuMeshLoader::get_nodes() const {
    return node_c;
}

///getter of tetrahedral O(1)
std::vector<FiniteElement> AneuMeshLoader::get_tetra() const {
    return tetra_c;
}

///getter of triangles O(1)
std::vector<FiniteElement> AneuMeshLoader::get_triang() const {
    return triang_c;
}

///find finite elements by 3 nodes O(n)
std::vector<FiniteElement> AneuMeshLoader::find_el_by_nodes(std::vector<size_t> &&nodes) const {
    std::vector<FiniteElement> ans;
    auto it = std::find_if(tetra_c.begin(), tetra_c.end(), \
        [nodes](const FiniteElement &f_el) -> bool {
        return std::all_of(nodes.begin(), \
            nodes.end(), [f_el](size_t chsl) -> bool {
            return std::any_of(f_el.nodes_id.begin(), \
                f_el.nodes_id.end(), [chsl](size_t nd) -> bool { return nd == chsl; });
        });
    });
    while (it != tetra_c.end()) {
        ans.push_back(*it);
        it = std::find_if(it + 1, tetra_c.end(), [nodes](const FiniteElement &f_el) -> bool \
 {
            return std::all_of(nodes.begin(), nodes.end(), [f_el](size_t chsl) -> bool \
 {
                return std::any_of(f_el.nodes_id.begin(), f_el.nodes_id.end(), [chsl](size_t nd) -> bool \
 { return nd == chsl; });
            });
        });
    }
    return ans;
}

///find finite elements by edge(2 nodes) O(n)
std::vector<FiniteElement> AneuMeshLoader::find_el_by_edge(const std::pair<size_t, size_t> &id_s) const {
    std::vector<FiniteElement> ans;
    for (auto el: tetra_c) {
        if (std::find(el.nodes_id.begin(), el.nodes_id.end(), id_s.first) != el.nodes_id.end() and \
        std::find(el.nodes_id.begin(), el.nodes_id.end(), id_s.first) != el.nodes_id.end()) {
            ans.push_back(el);
        }
    }
    return ans;
}

///find finite elements by area O(n)
std::vector<FiniteElement> AneuMeshLoader::find_el_by_area(size_t ar_id) const {
    std::vector<FiniteElement> ans;
    for (const auto &el: tetra_c) {
        if (el.area_ID == ar_id) {
            ans.push_back(el);
        }
    }
    return ans;
}

///find finite elements by surface O(n)
std::vector<FiniteElement> AneuMeshLoader::find_el_by_surface(size_t id_c) const {
    std::vector<FiniteElement> ans;
    for (const auto &el: triang_c) {
        if (el.area_ID == id_c) {
            ans.push_back(el);
        }
    }
    return ans;
}

///all possible pairs of vector elements
std::vector<std::pair<size_t, size_t>> combinations(std::vector<size_t> nums) {
    std::vector<std::pair<size_t, size_t>> ans;
    for (auto f = nums.begin(); f != nums.end(); f++) {
        for (auto s = f + 1; s != nums.end(); s++) {
            ans.emplace_back(*f, *s);
        }
    }
    return ans;
}

///find nodes by surface O(n)
std::set<Node, decltype(cmprt)> AneuMeshLoader::find_node_by_surface(size_t id_c) const {
    std::set<Node, decltype(cmprt)> ans;
    for (const auto &el: triang_c) {
        if (el.area_ID == id_c) {
            for (auto i: el.nodes_id) {
                ans.insert(node_c[i - 1]);
            }
        }
    }
    return ans;
}

///insertion of nodes in middle of all edges O(n*m)
void AneuMeshLoader::insert_nodes() {
    for (auto &el: tetra_c) {
        auto comb = combinations(el.nodes_id);
        for (auto i: comb) {
            auto fn = node_coord.find(
                    std::make_tuple((node_c[i.first - 1].coord[0] + node_c[i.second - 1].coord[0]) / 2,
                                    (node_c[i.first - 1].coord[1] + node_c[i.second - 1].coord[1]) / 2,
                                    (node_c[i.first - 1].coord[2] + node_c[i.second - 1].coord[2]) / 2));
            if (fn == node_coord.end()) {
                Node n_nd;
                n_nd.ID = node_c.size() + 1;
                n_nd.flag = false;
                n_nd.coord = {(node_c[i.first - 1].coord[0] + node_c[i.second - 1].coord[0]) / 2,
                              (node_c[i.first - 1].coord[1] + node_c[i.second - 1].coord[1]) / 2,
                              (node_c[i.first - 1].coord[2] + node_c[i.second - 1].coord[2]) / 2};
                node_c.push_back(n_nd);
                node_coord[{(node_c[i.first - 1].coord[0] + node_c[i.second - 1].coord[0]) / 2,
                            (node_c[i.first - 1].coord[1] + node_c[i.second - 1].coord[1]) / 2,
                            (node_c[i.first - 1].coord[2] + node_c[i.second - 1].coord[2]) / 2}] = node_c.size();
                el.nodes_id.push_back(node_c.size());
            } else {
                el.nodes_id.push_back((fn->second));
            }
        }
    }
    for (auto &el: triang_c) {
        auto comb = combinations(el.nodes_id);
        for (auto i: comb) {
            auto fn = node_coord.find(
                    std::make_tuple((node_c[i.first - 1].coord[0] + node_c[i.second - 1].coord[0]) / 2,
                                    (node_c[i.first - 1].coord[1] + node_c[i.second - 1].coord[1]) / 2,
                                    (node_c[i.first - 1].coord[2] + node_c[i.second - 1].coord[2]) / 2));
            if (fn == node_coord.end()) {
                Node n_nd;
                n_nd.ID = node_c.size() + 1;
                n_nd.flag = false;
                n_nd.coord = {(node_c[i.first - 1].coord[0] + node_c[i.second - 1].coord[0]) / 2,
                              (node_c[i.first - 1].coord[1] + node_c[i.second - 1].coord[1]) / 2,
                              (node_c[i.first - 1].coord[2] + node_c[i.second - 1].coord[2]) / 2};
                node_c.push_back(n_nd);
                node_coord[{(node_c[i.first - 1].coord[0] + node_c[i.second - 1].coord[0]) / 2,
                            (node_c[i.first - 1].coord[1] + node_c[i.second - 1].coord[1]) / 2,
                            (node_c[i.first - 1].coord[2] + node_c[i.second - 1].coord[2]) / 2}] = node_c.size();
                el.nodes_id.push_back(node_c.size());
            } else {
                el.nodes_id.push_back((fn->second));
            }
        }
    }
}


///find neighbors for all finite elements O(n)
std::vector<std::set<Node, decltype(cmprt)>> AneuMeshLoader::find_neighbors() const {
    std::vector<std::set<Node, decltype(cmprt)>> ans(node_c.size());
    for (const auto &el: tetra_c) {
        for (auto i: el.nodes_id) {
            for (auto j: el.nodes_id) {
                if (i != j) {
                    ans[i - 1].insert(node_c[j - 1]);
                }
            }
        }
    }
    return ans;
}

