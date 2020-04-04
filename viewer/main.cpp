#include <chrono>
#include <iostream>
#include "application.h"
#include "snail/snail.h"

using namespace std::chrono;
using namespace snail;

int main(int, char*[])
{
    // Create the application
    snail::application application;

    // Get start time
    auto start = steady_clock::now();

    // Create meshes
    auto mesh_1 = generate::box(vector3<double>(0., 0., 0.), vector3<double>(1., 1., 1.));
    auto mesh_2 = generate::box(vector3<double>(0.25, 0.25, 0.25), vector3<double>(1., 1., 1.));
    auto mesh_3 = generate::sphere(vector3<double>(0.5, 0.5, 0.5), 0.6);

    auto mesh_v1 = std::make_unique<mesh<double>>(1);
    mesh_v1->addVertex(vector3<double>(-0.5,   0.5,   0.5));
    mesh_v1->addVertex(vector3<double>(-0.25,   0.25,    0.5));
    mesh_v1->addVertex(vector3<double>(0.5,  -0.25,    0.5));
    mesh_v1->addTriangle(0, 1, 2);

    auto mesh_v2 = std::make_unique<mesh<double>>(1);
    mesh_v2->addVertex(vector3<double>(-0.25,   0.25,    0.5));
    mesh_v2->addVertex(vector3<double>(-0.25,   0.75,   0.75));
    mesh_v2->addVertex(vector3<double>(-0.25,    0.5,    0.5));
    mesh_v2->addTriangle(0, 1, 2);

    log::info << "Pre compute - vertices: " << mesh_1->getVertexCount() << ", triangles: " << mesh_1->getTriangleCount();

    // Difference
    //mesh_v1->sub(mesh_v2->clone());
    //mesh_1->sub(mesh_2->clone());
    mesh_1->sub(mesh_3->clone());

    log::info << "Post compute - vertices: " << mesh_1->getVertexCount() << ", triangles: " << mesh_1->getTriangleCount();
    log::err.clear();

    // Log time
    auto total = duration_cast<milliseconds>(steady_clock::now() - start).count();
    log::info << "Total time: " << total << "ms" << std::endl;

    // Run the application
    application.add_mesh(*mesh_1, color(0.2f, 0.8f, 0.3f, 0.5f));
    //application.add_mesh(*mesh_v1, color(0.2f, 0.8f, 0.3f, 0.5f));
    //application.add_mesh(*mesh_v2, color(0.8f, 0.1f, 0.3f, 0.5f));
    return application.run();
}
