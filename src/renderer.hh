#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Eigen/Dense>

#include <vector>

namespace renderer {

struct vertex {
    struct position { float x, y, z; } position;
    struct colour { float r, g, b; } colour;
};

struct mesh {
    GLenum mode = GL_TRIANGLE_FAN;
    std::vector<vertex> vertices;
    std::vector<GLint> counts;
};

struct renderer {
    renderer();
    void render(mesh& mesh, Eigen::Matrix4f mvp);

private:
    GLuint vao;
    GLuint buffer_vertices;
    GLuint pipeline;
    GLuint program_vertex;
};
}
