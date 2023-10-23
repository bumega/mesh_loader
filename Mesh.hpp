
#ifndef M_LD_MESH_HPP
#define M_LD_MESH_HPP

#include "Elements.hpp"


///abstract class
class MeshLoader {
public:
    virtual void LoadMesh(std::ifstream &file_it) = 0;
};


///heir class
class AneuMeshLoader : public MeshLoader {
    friend std::ostream &operator<<(std::ostream &stream, AneuMeshLoader &mesh);

public:
    AneuMeshLoader() = default;

    void LoadMesh(std::ifstream &file_it) override;

    std::vector<Node> get_nodes() const;

    std::vector<FiniteElement> get_tetra() const;

    std::vector<FiniteElement> get_triang() const;

    std::vector<FiniteElement> find_el_by_nodes(std::array<size_t, 3> &&els);

    std::vector<FiniteElement> find_el_by_edge(const std::pair<size_t, size_t> &id_s) const;

    std::set<Node, decltype(cmprt)> find_node_by_surface(size_t id_c) const;

    std::vector<FiniteElement> find_el_by_area(size_t ar_id) const;

    std::vector<FiniteElement> find_el_by_surface(size_t id_c) const;

    void insert_nodes();

    std::vector<std::set<Node, decltype(cmprt)>> find_neighbors() const;

    ~AneuMeshLoader() = default;

private:
    std::vector<Node> node_c;
    std::unordered_map<std::tuple<double, double, double>, size_t, tuple_hasher> node_coords;
    std::vector<FiniteElement> tetra_c;
    std::vector<FiniteElement> triang_c;
};

std::vector<std::pair<size_t, size_t>> combinations(std::vector<size_t>& nums);

///output of mesh
inline std::ostream &operator<<(std::ostream &stream, AneuMeshLoader &mesh) {
    std::vector<Node> node_c = mesh.get_nodes();
    std::vector<FiniteElement> tetra_c = mesh.get_tetra();
    std::vector<FiniteElement> triang_c = mesh.get_triang();
    std::cout << "nodes:\n";
    std::cout << "\n-------------\n";
    stream << node_c.size() << " " << node_c[0].coords.size() << "\n";
    std::ranges::for_each(node_c,
        [](const Node& el) -> void 
        {std::ranges::copy(
            el.coords, 
            std::ostream_iterator<int>(std::cout, " "));
        std::cout << "\n";
        }
    );
    std::cout << "\n";
    std::cout << "tetrahedral:\n";
    std::cout << "\n-------------\n";
    stream << tetra_c.size() << " " << tetra_c[0].nodes_id.size() << "\n";
    std::ranges::for_each(tetra_c,
        [](const FiniteElement& el) -> void {
        std::ranges::copy(
            el.nodes_id,
            std::ostream_iterator<int>(std::cout, " "));
        std::cout << "\n";
        }
    );
    std::cout << "triangles:\n";
    std::cout << "\n-------------\n";
    stream << triang_c.size() << " " << triang_c[0].nodes_id.size() << "\n";
    std::ranges::for_each(triang_c,
        [](const FiniteElement& el) -> void {
            std::ranges::copy(
                el.nodes_id,
                std::ostream_iterator<int>(std::cout, " ")
            );
            std::cout << "\n";
        }
    );
    return stream;
}

#endif //M_LD_MESH_HPP
