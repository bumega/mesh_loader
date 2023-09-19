#include "Mesh.hpp"

int main(int argc, char **argv) {

    AneuMeshLoader mesh = AneuMeshLoader();
    ///console start
    if (argc == 2) {
        std::ifstream inf(argv[1], std::ios::binary | std::ios::in);
        try {
            if (!inf.is_open()) {
                throw except_("file not found");
            } else {
                mesh.LoadMesh(inf);
            }
        } catch (const except_ &exception) {
            std::cout << "an exception was called \nits description:\n" << exception.what() << "\n";
        }
    } else {
        std::ifstream inf("C:/Users/Nikit/source/repos/mesh_loader/mesh_loader/Ishape3d.aneu",
                          std::ios::binary | std::ios::in);
        mesh.LoadMesh(inf);
    }
    ///method of find element by nodes
    auto els = mesh.find_el_by_nodes({9, 14, 6});
    std::cout << "id of correct elements\n";
    for (const auto& i: els) {
        std::cout << i.ID << " ";
    }
    std::cout << "\n";

    ///method of find element by edge
    els = mesh.find_el_by_edge({9, 14});
    std::cout << "\n-------------\n";
    std::cout << "id of correct elements\n";
    for (const auto& i: els) {
        std::cout << i.ID << " ";
    }
    std::cout << "\n";

    ///method of find node by surface
    auto nodes = mesh.find_node_by_surface(1);
    std::cout << "\n-------------\n";
    std::cout << "id of correct nodes\n";
    for (const auto& i: nodes) {
        std::cout << i.ID << " ";
    }
    std::cout << "\n";


    ///method of find elements by area
    els = mesh.find_el_by_area(1);
    std::cout << "\n-------------\n";
    std::cout << "id of correct elements\n";
    for (const auto& i: els) {
        std::cout << i.ID << " ";
    }
    std::cout << "\n";

    ///method of find elements by surface
    els = mesh.find_el_by_surface(1);
    std::cout << "\n-------------\n";
    std::cout << "id of correct elements\n";
    for (const auto& i: els) {
        std::cout << i.ID << " ";
    }
    std::cout << "\n";

    ///method of output mesh
    std::cout << "\n-------------\n";
    std::cout << "\nmesh:\n";
    std::cout << mesh;

    ///method of insertion new nodes
    std::cout << "\n-------------\n";
    std::cout << "\nmesh after insertion:\n";
    mesh.insert_nodes();
    std::cout << mesh;

    ///method of creating neighbors container
    std::cout << "\n-------------\n";
    std::cout << "\nneighbors:\n";
    auto neighbors = mesh.find_neighbors();
    int sch = 1;
    for (const auto &elements: neighbors) {
        std::cout << sch << ": ";
        for (const auto &node: elements) {
            std::cout << node.ID << " ";
        }
        sch++;
        std::cout << "\n";
    }

    return 0;
}