#include "controller.hh"
#include "renderer.hh"

#include <random>

int main() {
    renderer::renderer renderer{};
    controller::controller controller{};

    std::vector<renderer::vertex> vertices {
        {{0, 0, 0}, {1, 0, 0}},
        {{0, 10, 0}, {0, 1, 0}},
        {{10, 0, 0}, {0, 0, 1}},
    };
    std::mt19937 engine(0xfeed);
    std::uniform_real_distribution<float> pd(0, 10);
    for (size_t i = 0; i < 1000; i++) {
        vertices.push_back({{pd(engine), pd(engine), pd(engine)}, {1, 0, 0}});
        vertices.push_back({{pd(engine), pd(engine), pd(engine)}, {0, 1, 0}});
        vertices.push_back({{pd(engine), pd(engine), pd(engine)}, {0, 0, 1}});
    }

    renderer::mesh mesh {};
    mesh.vertices = vertices;
    mesh.counts = {3};
    while (true) {
        renderer.render(mesh, controller.tick());
    }

    return 0;
}
