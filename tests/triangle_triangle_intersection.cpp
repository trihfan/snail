#include "gtest/gtest.h"
#include "snail/snail.h"

using namespace snail;

/*#define ASSERT_EQ_V3(a, b) ASSERT_TRUE(equals(a.x, b.x)); ASSERT_TRUE(equals(a.y, b.y)); ASSERT_TRUE(equals(a.z, b.z))

std::vector<triangleTriangleIntersection<double>::intersection> compute(const vector3<double>& a2, const vector3<double>& b2, const vector3<double>& c2)
{
    vector3<double> a1(0., 0., 0.), b1(1., 0., 0.), c1(0., 1., 0.);
    triangle<double> triangle_a(0, 0, 0, ray(a1, b1), ray(b1, c1), ray(c1, a1));
    triangle<double> triangle_b(0, 0, 0, ray(a2, b2), ray(b2, c2), ray(c2, a2));
    //return triangleTriangleIntersection<double>::intersects(triangle_a, triangle_b);
    return std::vector<triangleTriangleIntersection<double>::intersection>();
}

TEST(triangleTriangleIntersection, case_0)
{
    ASSERT_EQ(compute(vector3<double>(0., 0., -1.), vector3<double>(0., 0., 1.), vector3<double>(-1., -1., 1.)).size(), 0);
    ASSERT_EQ(compute(vector3<double>(1., 0., -1.), vector3<double>(1., 0., 1.), vector3<double>(-1., -1., 1.)).size(), 0);
    ASSERT_EQ(compute(vector3<double>(0., 1., -1.), vector3<double>(0., 1., 1.), vector3<double>(-1., -1., 1.)).size(), 0);
}

TEST(triangleTriangleIntersection, case_1_ab)
{
    auto intersections = compute(vector3<double>(0.2, 0., 0.), vector3<double>(0.2, 0., 1.), vector3<double>(0., 0., 1.));
    ASSERT_EQ(intersections.size(), 1);
    ASSERT_EQ(intersections[0].hint, triangleTriangleIntersection<double>::ab);
    ASSERT_EQ_V3(intersections[0].position, vector3<double>(0.2, 0., 0.));
}

TEST(triangleTriangleIntersection, case_1_bc)
{
    auto intersections = compute(vector3<double>(0.5, 0.5, 0.), vector3<double>(0.5, 0.5, 1.), vector3<double>(0., 0., 1.));
    ASSERT_EQ(intersections.size(), 1);
    ASSERT_EQ(intersections[0].hint, triangleTriangleIntersection<double>::bc);
    ASSERT_EQ_V3(intersections[0].position, vector3<double>(0.5, 0.5, 0.));
}

TEST(triangleTriangleIntersection, case_1_ac)
{
    auto intersections = compute(vector3<double>(0., 0.2, 0.), vector3<double>(0., 0.2, 1.), vector3<double>(0., 0., 1.));
    ASSERT_EQ(intersections.size(), 1);
    ASSERT_EQ(intersections[0].hint, triangleTriangleIntersection<double>::ac);
    ASSERT_EQ_V3(intersections[0].position, vector3<double>(0., 0.2, 0.));
}

TEST(triangleTriangleIntersection, case_1_inside)
{
    auto intersections = compute(vector3<double>(0.2, 0.2, 0.), vector3<double>(0.2, 0.2, 1.), vector3<double>(0., 0., 1.));
    ASSERT_EQ(intersections.size(), 1);
    ASSERT_EQ(intersections[0].hint, triangleTriangleIntersection<double>::inside);
    ASSERT_EQ_V3(intersections[0].position, vector3<double>(0.2, 0.2, 0.));
}

TEST(triangleTriangleIntersection, case_2_one_side)
{
    auto intersections = compute(vector3<double>(0.2, 0., 0.), vector3<double>(0.4, 0., 0.), vector3<double>(0., 0., 1.));
    ASSERT_EQ(intersections.size(), 2);
    ASSERT_EQ(intersections[0].hint, triangleTriangleIntersection<double>::ab);
    ASSERT_EQ_V3(intersections[0].position, vector3<double>(0.4, 0., 0.));
    ASSERT_EQ(intersections[1].hint, triangleTriangleIntersection<double>::ab);
    ASSERT_EQ_V3(intersections[1].position, vector3<double>(0.2, 0., 0.));
}

TEST(triangleTriangleIntersection, case_2_side_inside)
{
    auto intersections = compute(vector3<double>(0.3, 0., 0.), vector3<double>(0.2, 0.2, 0.), vector3<double>(0., 0., 1.));
    ASSERT_EQ(intersections.size(), 2);
    ASSERT_EQ(intersections[0].hint, triangleTriangleIntersection<double>::ab);
    ASSERT_EQ_V3(intersections[0].position, vector3<double>(0.3, 0., 0.));
    ASSERT_EQ(intersections[1].hint, triangleTriangleIntersection<double>::inside);
    ASSERT_EQ_V3(intersections[1].position, vector3<double>(0.2, 0.2, 0.));
}

TEST(triangleTriangleIntersection, case_2_two_inside)
{
    auto intersections = compute(vector3<double>(0.2, 0.2, 0.), vector3<double>(0.3, 0.4, 0.), vector3<double>(0., 0., 1.));
    ASSERT_EQ(intersections.size(), 2);
    ASSERT_EQ(intersections[0].hint, triangleTriangleIntersection<double>::inside);
    ASSERT_EQ_V3(intersections[0].position, vector3<double>(0.3, 0.4, 0.));
    ASSERT_EQ(intersections[1].hint, triangleTriangleIntersection<double>::inside);
    ASSERT_EQ_V3(intersections[1].position, vector3<double>(0.2, 0.2, 0.));
}*/

