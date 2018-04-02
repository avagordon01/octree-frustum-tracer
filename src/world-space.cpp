#include <iostream>

#include <Eigen/Dense>

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
struct Traits : public OpenMesh::DefaultTraits {
    typedef OpenMesh::Vec3i Point;
};
typedef OpenMesh::PolyMesh_ArrayKernelT<Traits>  Mesh;

/*
TODO for the world-space voxel frustum tracing...
have an Vec3i mesh, with vertices on the points of the voxel grid
start with the mesh at the top level, on the one/two/three sides facing the eye coordinate
if it is a leaf node -> "push" the mesh through the voxel
if it isnt a leaf -> split the faces of the mesh

transform the eye coordinate into model coordinates
*/

/*
void split_face(const Mesh &mesh, Mesh::FaceHandle face) {
}
*/

void print_faces(const Mesh &mesh) {
    for (Mesh::FaceIter face = mesh.faces_begin(); face != mesh.faces_end(); ++face) {
        std::cout << *face << std::endl;
        for (Mesh::ConstFaceVertexIter vertex = mesh.cfv_iter(*face); vertex.is_valid(); ++vertex) {
            std::cout << "\t" << mesh.point(*vertex) << std::endl;
        }
    }
}

int main() {
    float focal_length = 0.2;
    Eigen::Vector3f eye_pos, eye_dir, screen_pos, screen_dir, object_pos;
    Eigen::Matrix3f object_orientation;
    eye_pos = {0, 0, 0};
    eye_dir = {1, 0, 0};
    screen_pos = eye_pos + (eye_dir.normalized() * focal_length);
    screen_dir = eye_dir;
    object_pos = {10, 0, 0};
    object_orientation = Eigen::Matrix3f::Identity();

    Mesh mesh;
    mesh.add_face({
        mesh.add_vertex({-10, -10, 0}),
        mesh.add_vertex({-10, +10, 0}),
        mesh.add_vertex({+10, +10, 0}),
        mesh.add_vertex({+10, -10, 0})
    });

    print_faces(mesh);
    return 0;
}
