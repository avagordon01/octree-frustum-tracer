#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
typedef OpenMesh::PolyMesh_ArrayKernelT<>  Mesh;

typedef struct {
    OpenMesh::Vec3f x, d;
} Line;

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
TODO once a face has been coloured, remove it from the mesh, add it to the "final" mesh
split the frustum face into four if the octree node has children
if out_face is facing away from the frustum
project the frustum_face onto the out_face
clip the frustum_face with the out_face, discard the outside parts
if the resulting face is small
traverse the neighbour node, joined by the out_face
*/

int main() {
    Mesh mesh;

    //generate vertices
    Mesh::VertexHandle vh[] {
        mesh.add_vertex(Mesh::Point(-1, -1,  1)),
        mesh.add_vertex(Mesh::Point( 1, -1,  1)),
        mesh.add_vertex(Mesh::Point( 1,  1,  1)),
        mesh.add_vertex(Mesh::Point(-1,  1,  1)),
        mesh.add_vertex(Mesh::Point(-1, -1, -1)),
        mesh.add_vertex(Mesh::Point( 1, -1, -1)),
        mesh.add_vertex(Mesh::Point( 1,  1, -1)),
        mesh.add_vertex(Mesh::Point(-1,  1, -1))
    };

    //generate faces
    mesh.add_face(std::vector<Mesh::VertexHandle> {vh[0], vh[1], vh[2], vh[3]});
    mesh.add_face(std::vector<Mesh::VertexHandle> {vh[7], vh[6], vh[5], vh[4]});
    mesh.add_face(std::vector<Mesh::VertexHandle> {vh[1], vh[0], vh[4], vh[5]});
    mesh.add_face(std::vector<Mesh::VertexHandle> {vh[2], vh[1], vh[5], vh[6]});
    mesh.add_face(std::vector<Mesh::VertexHandle> {vh[3], vh[2], vh[6], vh[7]});
    mesh.add_face(std::vector<Mesh::VertexHandle> {vh[0], vh[3], vh[7], vh[4]});
    return 0;
}
