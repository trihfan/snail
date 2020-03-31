#include "gtest/gtest.h"
#include "snail/snail.h"

using namespace snail;

/*TEST(triangleRayIntersection, side)
{
    ray<double> ray(vector3<double>(0., 0.2, 1.), vector3<double>(0., 0.2, -1.));
    vector3<double> a(0., 0., 0.), b(1., 0., 0.), c(0., 1., 0.);
    float t, u, v;

    ASSERT_EQ(triangleRayIntersection<double>::intersects(ray, a, b, c, t, u, v), triangleRayIntersection<double>::inside);
    ASSERT_EQ(t, 1.f);
    ASSERT_EQ(u, 0.f);
    ASSERT_EQ(v, 0.2f);
}

TEST(triangleRayIntersection, inside)
{
    ray<double> ray(vector3<double>(0.2, 0.2, 1.), vector3<double>(0.2, 0.2, -1.));
    vector3<double> a(0., 0., 0.), b(1., 0., 0.), c(0., 1., 0.);
    float t, u, v;

    ASSERT_EQ(triangleRayIntersection<double>::intersects(ray, a, b, c, t, u, v), triangleRayIntersection<double>::inside);
    ASSERT_EQ(t, 1.f);
    ASSERT_EQ(u, 0.2f);
    ASSERT_EQ(v, 0.2f);
}

TEST(triangleRayIntersection, extremity)
{
    ray<double> ray(vector3<double>(0., 0., 1.), vector3<double>(0., 0., -1.));
    vector3<double> a(0., 0., 0.), b(1., 0., 0.), c(0., 1., 0.);
    float t, u, v;

    ASSERT_EQ(triangleRayIntersection<double>::intersects(ray, a, b, c, t, u, v), triangleRayIntersection<double>::inside);
    ASSERT_EQ(t, 1.f);
    ASSERT_EQ(u, 0.f);
    ASSERT_EQ(v, 0.f);
}

TEST(triangleRayIntersection, outside)
{
    ray<double> ray(vector3<double>(1., 1., 1.), vector3<double>(1., 1., -1.));
    vector3<double> a(0., 0., 0.), b(1., 0., 0.), c(0., 1., 0.);
    float t, u, v;

    ASSERT_EQ(triangleRayIntersection<double>::intersects(ray, a, b, c, t, u, v), triangleRayIntersection<double>::outside);
}

TEST(triangleRayIntersection, parallel)
{
    ray<double> ray(vector3<double>(0., 0., 0.), vector3<double>(1., 1., 0.));
    vector3<double> a(0., 0., 0.), b(1., 0., 0.), c(0., 1., 0.);
    float t, u, v;

    ASSERT_EQ(triangleRayIntersection<double>::intersects(ray, a, b, c, t, u, v), triangleRayIntersection<double>::parallels);
}*/

