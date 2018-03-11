#include <iostream>

#include <Eigen/Dense>

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
struct Traits : public OpenMesh::DefaultTraits {
    typedef OpenMesh::Vec2f Point;
};
typedef OpenMesh::PolyMesh_ArrayKernelT<Traits>  Mesh;

typedef struct {
    OpenMesh::Vec3f pos, dir;
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

void split(Mesh mesh, Line line) {
    for (Mesh::FaceIter face = mesh.faces_begin(); face != mesh.faces_end(); ++face) {
        for(Mesh::FaceHalfedgeIter halfedge = mesh.fh_iter(*face); halfedge.is_valid(); ++halfedge) {
            Mesh::Point from = mesh.point(mesh.from_vertex_handle(*halfedge));
            Mesh::Point to = mesh.point(mesh.to_vertex_handle(*halfedge));
            bool from_side = (from[0] - line.pos[0]) * line.dir[1] - (from[1] - line.pos[1]) * line.dir[0] > 0;
            bool to_side = (to[0] - line.pos[0]) * line.dir[1] - (to[1] - line.pos[1]) * line.dir[0] > 0;
            if (from_side != to_side) {
                //TODO
                mesh.insert_edge(..., ...);
            }
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
    mesh.add_face(std::vector<Mesh::VertexHandle> {
        mesh.add_vertex(Mesh::Point{-1, -1}),
        mesh.add_vertex(Mesh::Point{-1, +1}),
        mesh.add_vertex(Mesh::Point{+1, +1}),
        mesh.add_vertex(Mesh::Point{+1, -1})
    });
    return 0;
}
