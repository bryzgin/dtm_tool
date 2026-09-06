#include <gtest/gtest.h>

extern "C" {
    #include "core.h"
    #include "triangulation.h"
    #include "../src/triangulation.c"
}

TEST(DelaunayMathTest, ValidCircumcircle) {
    Point a = {0.0f, 0.0f, 0.0f};
    Point b = {4.0f, 0.0f, 0.0f};
    Point c = {4.0f, 4.0f, 0.0f};

    Circle circle = get_circumcircle(a, b, c);

    EXPECT_NEAR(circle.center_x, 2.0f, 0.00001f);
    EXPECT_NEAR(circle.center_y, 2.0f, 0.00001f);
    EXPECT_NEAR(circle.radius, 2.828427f, 0.00001f);
}

TEST(DelaunayMathTest, CollinearPoints) {
    Point a = {0.0f, 0.0f, 0.0f};
    Point b = {1.0f, 1.0f, 0.0f};
    Point c = {2.0f, 2.0f, 0.0f};

    Circle circle = get_circumcircle(a, b, c);

    EXPECT_FLOAT_EQ(circle.radius, -1.0f);
}

TEST(DelaunayMathTest, PointInCircumCircle) {
    Point a = {0.0f, 0.0f, 0.0f};
    Point b = {4.0f, 0.0f, 0.0f};
    Point c = {4.0f, 4.0f, 0.0f};

    Point inside_p = {2.0f, 2.0f, 0.0f};
    Point outside_p = {10.0f, 10.0f, 0.0f};


    int is_inside = is_point_in_circumcircle(inside_p, a, b, c);
    int is_outside = is_point_in_circumcircle(outside_p, a, b, c);

    EXPECT_EQ(is_inside, 1);
    EXPECT_EQ(is_outside, 0);
}
