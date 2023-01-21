#include "../include/triangle.h"

triangle_t create_triangle(data_t *a, data_t *b, data_t *c)
{
    triangle_t t;
    t.s1 = a;
    t.s2 = b;
    t.s3 = c;
    return t;
}

int edge_shared(edge_t bad_t_a, edge_t bad_t_b, edge_t bad_t_c, edge_t current, double EPSILON)
{
    return (compare_data_t(bad_t_a.org, current.org, EPSILON) &&
            compare_data_t(bad_t_a.dest, current.dest, EPSILON)) ||
           (compare_data_t(bad_t_a.org, current.dest, EPSILON) &&
            compare_data_t(bad_t_a.dest, current.org, EPSILON)) ||
           (compare_data_t(bad_t_b.org, current.org, EPSILON) &&
            compare_data_t(bad_t_b.dest, current.dest, EPSILON)) ||
           (compare_data_t(bad_t_b.org, current.dest, EPSILON) &&
            compare_data_t(bad_t_b.dest, current.org, EPSILON)) ||
           (compare_data_t(bad_t_c.org, current.org, EPSILON) &&
            compare_data_t(bad_t_c.dest, current.dest, EPSILON)) ||
           (compare_data_t(bad_t_c.org, current.dest, EPSILON) &&
            compare_data_t(bad_t_c.dest, current.org, EPSILON));
}

int compare_triangle_point(triangle_t a, triangle_t b, double EPSILON)
{
    return (compare_data_t(a.s1, b.s1, EPSILON) || compare_data_t(a.s1, b.s2, EPSILON) ||
            compare_data_t(a.s1, b.s3, EPSILON) || compare_data_t(a.s2, b.s1, EPSILON) ||
            compare_data_t(a.s2, b.s2, EPSILON) || compare_data_t(a.s2, b.s3, EPSILON) ||
            compare_data_t(a.s3, b.s1, EPSILON) || compare_data_t(a.s3, b.s2, EPSILON) ||
            compare_data_t(a.s3, b.s3, EPSILON));
}

int compare_triangle(triangle_t a, triangle_t b, double EPSILON)
{
    return (compare_data_t(a.s1, b.s1, EPSILON) || compare_data_t(a.s1, b.s2, EPSILON) ||
            compare_data_t(a.s1, b.s3, EPSILON)) &&
           (compare_data_t(a.s2, b.s1, EPSILON) || compare_data_t(a.s2, b.s2, EPSILON) ||
            compare_data_t(a.s2, b.s3, EPSILON)) &&
           (compare_data_t(a.s3, b.s1, EPSILON) || compare_data_t(a.s3, b.s2, EPSILON) ||
            compare_data_t(a.s3, b.s3, EPSILON));
}