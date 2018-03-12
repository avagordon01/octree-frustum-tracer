#include <iostream>

#include <Eigen/Dense>

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
struct Traits : public OpenMesh::DefaultTraits {
    typedef OpenMesh::Vec2f Point;
};
typedef OpenMesh::PolyMesh_ArrayKernelT<Traits>  Mesh;

typedef struct {
    Mesh::Point pos, dir;
} Line;

/*
TODO
start with a rectangle mesh at the screen coordinates
put an eye position at the users expected eye-distance from screen
put the object at coordinates in front of the screen

any time the frustum defined by the screen mesh and the eye coordinate is not contiguous, split along the discontinuity
so, for the first edge on the cube, split the rectangle by the edge projected onto the screen
*/

/*
TODO
optimisation, flag if frustum direction is only in a single octant, gets passed directly to children if true
optimisation, combine input faces on a node, and "flip" them into the output faces
optimisation, could have a uint32 mesh instead of floats...
optimisation, traverse breadth first, so we can join adjacent frustum faces
the perfect loop would be over the nodes with input faces, in a breadth first manner
probably the closest fast loop would be over the faces, checking the iteration number of the face (if it is an input or output of this iteration)
with this loop it wont be easy to pass the stack around between loop iterations, because its breadth first
*/
/*
find all the adjacent faces that are input faces on the same node, combine them
"backproject" the remaining corners of the node onto the combined face
maybe just delete the old ones and insert new ones?
split the combined face into the output face, move the vertices to their new positions
1-3 input faces, 0-6 output faces
*/
/*
it could be difficult to keep all of the faces in sync so they can be merged
maybe this is fixed with clever keeping track of the iteration count for each face
if iteration count is just the number of steps from their first face, then it is ok
also need to traverse from the corner outwards somehow? z order would work, so long as the corner is at the origin
or just loop over the neighbour faces outwards from the corner
could still pass the stack around by going neighbour to neighbour?
going in z order would be great for passing the stack around, perfect even, but we dont have a fast method for getting the mesh faces from a xyz position
with occlusion, its not perfect, you could have overlapping triangles but if they are rendered at the correct depth it doesnt matter
also it doesnt matter because rendering half a face or the whole face makes no difference to the processing
*/
/*
once a face has been coloured, remove it from the mesh, add it to the "final" mesh
split the frustum face into four if the octree node has children
if out_face is facing away from the frustum
project the frustum_face onto the out_face
clip the frustum_face with the out_face, discard the outside parts
if the resulting face is small
traverse the neighbour node, joined by the out_face
*/

OpenMesh::Vec2f line_intersect(OpenMesh::Vec2f a, OpenMesh::Vec2f b, OpenMesh::Vec2f c, OpenMesh::Vec2f d) {
    return OpenMesh::Vec2f {
        ((a[0] * b[1] - a[1] * b[0]) * (c[0] - d[0]) - (a[0] - b[0]) * (c[0] * d[1] - c[1] * d[0])) /
        ((a[0] - b[0]) * (c[1] - d[1]) - (a[1] - b[1]) * (c[0] - d[0])),
        ((a[0] * b[1] - a[1] * b[0]) * (c[1] - d[1]) - (a[1] - b[1]) * (c[0] * d[1] - c[1] * d[0])) /
        ((a[0] - b[0]) * (c[1] - d[1]) - (a[1] - b[1]) * (c[0] - d[0]))
    };
}

int float_sign(float x) {
    if (x > 0)
        return 1;
    else if (x < 0)
        return -1;
    else if (x == 0)
        return 0;
    else
        abort();
}

void split(Mesh mesh, Line line) {
    //TODO could instead traverse through by looping over the edges and swapping to the opposite face when an edge intersects
    std::vector<std::pair<Mesh::HalfedgeHandle, Mesh::HalfedgeHandle> > split_halfedges {};
    for (Mesh::FaceIter face = mesh.faces_begin(); face != mesh.faces_end(); ++face) {
        std::cout << "in face valence: " << mesh.valence(*face) << std::endl;
        Mesh::HalfedgeHandle first, second;
        bool intersect = false;
        for (Mesh::FaceHalfedgeIter halfedge = mesh.fh_iter(*face); halfedge.is_valid(); ++halfedge) {
            Mesh::Point from = mesh.point(mesh.from_vertex_handle(*halfedge));
            Mesh::Point to = mesh.point(mesh.to_vertex_handle(*halfedge));
            int from_side = float_sign((from[0] - line.pos[0]) * line.dir[1] - (from[1] - line.pos[1]) * line.dir[0]);
            int to_side = float_sign((to[0] - line.pos[0]) * line.dir[1] - (to[1] - line.pos[1]) * line.dir[0]);
            //check if the line crosses from one side to another
            //if they are both zero, they both lie on the line
            if (from_side == -to_side && from_side != 0) {
                if (!intersect) {
                    intersect = true;
                    Mesh::VertexHandle v_prev = mesh.from_vertex_handle(*halfedge);
                    Mesh::VertexHandle v_new = mesh.add_vertex((from + to) / 2);
                    mesh.split_edge(mesh.edge_handle(*halfedge), v_new);
                    first = mesh.find_halfedge(v_prev, v_new);
                    assert(mesh.face_handle(first) == *face);
                    assert(mesh.to_vertex_handle(first) == v_new);
                } else {
                    Mesh::VertexHandle v_next = mesh.to_vertex_handle(*halfedge);
                    Mesh::VertexHandle v_new = mesh.add_vertex((from + to) / 2);
                    mesh.split_edge(mesh.edge_handle(*halfedge), v_new);
                    second = mesh.find_halfedge(v_new, v_next);
                    assert(mesh.face_handle(second) == *face);
                    assert(mesh.from_vertex_handle(second) == v_new);
                    break;
                }
            }
        }
        if (intersect) {
            split_halfedges.push_back({first, second});
        }
        std::cout << "out face valence: " << mesh.valence(*face) << std::endl;
    }
    for (auto &[first, second] : split_halfedges) {
        mesh.insert_edge(first, second);
    }
}

void print_faces(Mesh mesh) {
    for (Mesh::FaceIter face = mesh.faces_begin(); face != mesh.faces_end(); ++face) {
        std::cout << *face << std::endl;
        for (Mesh::FaceVertexIter vertex = mesh.fv_iter(*face); vertex.is_valid(); ++vertex) {
            std::cout << "\t" << *vertex << std::endl;
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
        mesh.add_vertex({-1, -1}),
        mesh.add_vertex({-1, +1}),
        mesh.add_vertex({+1, +1}),
        mesh.add_vertex({+1, -1})
    });

    Line line {
        {0, 0},
        {0, 1},
    };
    print_faces(mesh);
    split(mesh, line);
    print_faces(mesh);
    return 0;
}
