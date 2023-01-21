#pragma once
#include "array_list.h"
#include "data_t.h"
#include "delaunay.h"
#include "graph.h"

/**
 * @defgroup VisualiseModule
 * @{
 * @brief Visualise Delaunay and Prim result using the tps library and the SDL2
 * library.
 */

/**
 * Hold screen informations.
 */
typedef struct screen_t
{
    int width;
    int height;
    double x_max;
    double x_min;
    double y_max;
    double y_min;
    double x_mouse;
    double y_mouse;
} screen_t;

/**
 * Enum for text to display.
 */
enum TXT
{
    TXT_DEFAULT = 0,
    TXT_DELAUNAY = 1,
    TXT_PRIM = 2
};

/**
 * Enum for color.
 */
enum COLOR
{
    COLOR_BLACK = 0,
    COLOR_RED = 1,
    COLOR_GREEN = 2,
    COLOR_BLUE = 3,
};

/**
 * Return a normalized value based on
 * a coordinate and his max and min value.
 * @param coord coordinate to normalize
 * @param coord_max max value of the coordinate
 * @param coord_min min value of the coordinate
 * @return normalized value
 */
double normalize_to_screen(double coord, double coord_max, double coord_min);

/**
 * Check if a point is out of the screen.
 * @param x x coordinate of the point
 * @param y y coordinate of the point
 * @return true if the point is out of the screen, false otherwise
 */
bool out_of_screen(double x, double y);

/**
 * Draw an edge.
 * @param x1 x coordinate of the first point
 * @param y1 y coordinate of the first point
 * @param x2 x coordinate of the second point
 * @param y2 y coordinate of the second point
 */
void draw_edge(double x1, double y1, double x2, double y2);

/**
 * Set color.
 * @param c color to set (COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_BLACK)
 */
void set_color(enum COLOR c);

/**
 * Draw a node.
 * @param x x coordinate of the node
 * @param y y coordinate of the node
 * @param rx radius of the node in x
 * @param ry radius of the node in y
 * @param c color of the node
 */
void draw_node(double x, double y, int rx, int ry, enum COLOR c);

/**
 * Based on a node_list, get x=lattitude y=longitude min max value.
 * @param node_list list of nodes
 * @param size_vertices number of nodes
 * @param x_max max value of x
 * @param x_min min value of x
 * @param y_max max value of y
 * @param y_min min value of y
 * @return 0 if no problems, otherwise 1.
 */
void get_xy_min_max(list_t *node_list, int size_vertices, double *x_max, double *x_min, double *y_max,
                    double *y_min);

/**
 * Update texts informations.
 * @param node_list list of nodes
 * @param delaunay delaunay triangles
 * @param g graph of delaunay triangles
 * @param mst prim mst
 * @param flag flag to know which text to update (TXT_DELAUNAY, TXT_PRIM,
 * TXT_DEFAULT)
 */
void update_texts(list_t *node_list, delaunay_t *delaunay, graph_t *g, int *mst, enum TXT flag);

/**
 * Draw inputs bindings, informations about the visualization (Numbers of nodes,
 * edges, weight of the mst)
 */
void draw_texts();

/**
 * Initialize screen_t global variable.
 * @param width width of the window
 * @param height height of the window
 * @param node_list list of nodes
 * @param size_vertices number of nodes
 */
void initialize_screen(int width, int height, list_t *node_list, int size_vertices);

/**
 * Visualize delaunay and prim result in a SDL window.
 * @param width width of the window
 * @param height height of the window
 * @param node_list list of nodes
 * @param mst prim mst
 * @param delaunay delaunay triangles
 * @param g graph of delaunay triangles
 */
void visualize(int width, int height, list_t *node_list, int *mst, delaunay_t *delaunay, graph_t *g);

/**
 * Draw mst in a SDL window.
 * @param node_list list of nodes
 * @param mst prim mst
 * @param size nubmer of nodes
 */
void show_mst(list_t *node_list, int *mst, int size_vertices);

/**
 * Draw delaunay triangulation in a SDL window.
 * @param delaunay delaunay triangles
 */
void show_delaunay(delaunay_t *triangles);

/**
 * According to the event (mouse,keyboard ..), do an action.
 */
void handle_sdl_event(void);

/**
 * Update the camera position.
 * @param x x coordinate of the mouse
 * @param y y coordinate of the mouse
 * @note there is not a really camera, it just move the screen.
 */
void camera_move(int x, int y);

/**
 * According to the key pressed, do an action.
 * @param keyPressed key pressed
 */
void onKeyDown(int keyPressed);
/** @} */