#include "../include/visualise.h"
#include "../include/cprintf.h"
#include "../include/data_t.h"
#include <stdio.h>
#include <stdlib.h>
#include <tps.h>

#define BUFFER_SIZE 1024

char _is_prim_active = 1;
char _is_delaunay_active = 0;

// texts
char _number_nodes[BUFFER_SIZE];
char _press_key[BUFFER_SIZE];
char _number_edges[BUFFER_SIZE];
char _sum_weight[BUFFER_SIZE] = " ";
char _binds[BUFFER_SIZE];

screen_t *_screen;

double normalize_to_screen(double coord, double coord_max, double coord_min)
{
    return (coord - coord_min) / (coord_max - coord_min);
}

void zoom_in()
{
    double coef = 0.1;
    double x_diff = _screen->x_max - _screen->x_min;
    double y_diff = _screen->y_max - _screen->y_min;
    _screen->x_max = _screen->x_max - x_diff * coef;
    _screen->x_min = _screen->x_min + x_diff * coef;
    _screen->y_max = _screen->y_max - y_diff * coef;
    _screen->y_min = _screen->y_min + y_diff * coef;
}

void zoom_out()
{
    double coef = 0.1;
    double x_diff = _screen->x_max - _screen->x_min;
    double y_diff = _screen->y_max - _screen->y_min;
    _screen->x_max = _screen->x_max + x_diff * coef;
    _screen->x_min = _screen->x_min - x_diff * coef;
    _screen->y_max = _screen->y_max + y_diff * coef;
    _screen->y_min = _screen->y_min - y_diff * coef;
}

void move_screen(int flag)
{
    double coef = 0.1;
    double x_diff = _screen->x_max - _screen->x_min;
    double y_diff = _screen->y_max - _screen->y_min;
    switch (flag)
    {
    case SDLK_RIGHT:
        _screen->x_max = _screen->x_max + x_diff * coef;
        _screen->x_min = _screen->x_min + x_diff * coef;
        break;
    case SDLK_LEFT:
        _screen->x_max = _screen->x_max - x_diff * coef;
        _screen->x_min = _screen->x_min - x_diff * coef;
        break;
    case SDLK_DOWN:
        _screen->y_max = _screen->y_max + y_diff * coef;
        _screen->y_min = _screen->y_min + y_diff * coef;
        break;
    case SDLK_UP:
        _screen->y_max = _screen->y_max - y_diff * coef;
        _screen->y_min = _screen->y_min - y_diff * coef;
        break;
    default:
        break;
    }
}

bool out_of_screen(double x, double y)
{
    double x_max = _screen->x_max;
    double x_min = _screen->x_min;
    double y_max = _screen->y_max;
    double y_min = _screen->y_min;
    if (x > x_max || x < x_min || y > y_max || y < y_min)
    {
        return true;
    }
    return false;
}

void draw_edge(double x1, double y1, double x2, double y2)
{
    if (out_of_screen(x1, y1) == false && out_of_screen(x2, y2) == false)
    {
        tps_drawLine(normalize_to_screen(x1, _screen->x_max, _screen->x_min) * _screen->width,
                     normalize_to_screen(y1, _screen->y_max, _screen->y_min) * _screen->height,
                     normalize_to_screen(x2, _screen->x_max, _screen->x_min) * _screen->width,
                     normalize_to_screen(y2, _screen->y_max, _screen->y_min) * _screen->height);
    }
}

void set_color(enum COLOR c)
{
    switch (c)
    {
    case COLOR_RED:
        tps_setColor(255, 0, 0);
        break;
    case COLOR_GREEN:
        tps_setColor(0, 255, 0);
        break;
    case COLOR_BLUE:
        tps_setColor(0, 0, 255);
        break;
    case COLOR_BLACK:
        tps_setColor(0, 0, 0);
        break;
    }
}

void draw_node(double x, double y, int rx, int ry, enum COLOR c)
{
    set_color(c);
    tps_drawEllipse(normalize_to_screen(x, _screen->x_max, _screen->x_min) * _screen->width,
                    normalize_to_screen(y, _screen->y_max, _screen->y_min) * _screen->height, rx, ry);
    set_color(COLOR_BLACK);
}

void get_xy_min_max(list_t *node_list, int size_vertices, double *x_max, double *x_min, double *y_max,
                    double *y_min)
{
    for (int i = 0; i < size_vertices; i++)
    {
        if ((*(data_t *)list_get(node_list, i)).latitude >= *x_max)
        {
            *x_max = (*(data_t *)list_get(node_list, i)).latitude;
        }
        if ((*(data_t *)list_get(node_list, i)).latitude < *x_min)
        {
            *x_min = (*(data_t *)list_get(node_list, i)).latitude;
        }
        if ((*(data_t *)list_get(node_list, i)).longitude >= *y_max)
        {
            *y_max = (*(data_t *)list_get(node_list, i)).longitude;
        }
        if ((*(data_t *)list_get(node_list, i)).longitude < *y_min)
        {
            *y_min = (*(data_t *)list_get(node_list, i)).longitude;
        }
    }
    deprintf("x_max: %f, x_min: %f, y_max: %f, y_min: %f\n", *x_max, *x_min, *y_max, *y_min);
}

void update_texts(list_t *node_list, delaunay_t *delaunay, graph_t *g, int *mst, enum TXT flag)
{
    switch (flag)
    {
    case TXT_DEFAULT:
        snprintf(_press_key, BUFFER_SIZE, "Show Prim: [O]");
        snprintf(_number_nodes, BUFFER_SIZE, "Number of nodes: %d", g->size_vertices);
        snprintf(_number_edges, BUFFER_SIZE, "Number of edges: %ld", (delaunay->size_triangles - 1) * 3);
        snprintf(_binds, BUFFER_SIZE,
                 "Quit: [ESC], Zoom In/Out: [Mouse wheel, A/Space], Move "
                 "camera: [Click and drag, Arrow U/D/L/R]");
        break;
    case TXT_DELAUNAY:
        snprintf(_press_key, BUFFER_SIZE, "Show Prim: [O]");
        snprintf(_number_edges, BUFFER_SIZE, "Number of edges: %d", g->size_edges);
        snprintf(_sum_weight, BUFFER_SIZE, " ");
        break;
    case TXT_PRIM:
        snprintf(_press_key, BUFFER_SIZE, "Show Delaunay: [O]");
        snprintf(_number_edges, BUFFER_SIZE, "Number of edges: %d", (g->size_vertices - 1));
        snprintf(_sum_weight, BUFFER_SIZE, "Sum of weights: %f",
                 sum_weight_graph(mst, node_list, g->size_vertices));
        break;
    default:
        break;
    }
}

void draw_texts()
{
    tps_drawText(20, 20, _press_key, 30);
    tps_drawText(20, 60, _binds, 15);
    tps_drawText(20, 80, _number_nodes, 15);
    tps_drawText(20, 100, _number_edges, 15);
    tps_drawText(20, 120, _sum_weight, 15);
}

void initialize_screen(int width, int height, list_t *node_list, int size_vertices)
{
    double x_max = 0;
    double x_min = DBL_MAX;
    double y_max = 0;
    double y_min = DBL_MAX;
    get_xy_min_max(node_list, size_vertices, &x_max, &x_min, &y_max, &y_min);
    CHK_ALLOC(_screen = malloc(sizeof(screen_t)), "malloc screen");
    _screen->width = width;
    _screen->height = height;
    _screen->x_max = x_max;
    _screen->x_min = x_min;
    _screen->y_max = y_max;
    _screen->y_min = y_min;
}

void visualize(int width, int height, list_t *node_list, int *mst, delaunay_t *delaunay, graph_t *g)
{
    tps_createWindow("Delaunay & Prim", width, height);
    initialize_screen(width, height, node_list, g->size_vertices);
    update_texts(node_list, delaunay, g, mst, TXT_DEFAULT);
    while (tps_isRunning())
    {
        tps_background(255, 255, 255);
        tps_setColor(0, 0, 0);
        draw_texts();
        if (_is_delaunay_active == 1)
        {
            update_texts(node_list, delaunay, g, mst, TXT_DELAUNAY);
            show_delaunay(delaunay);
        }
        else if (_is_prim_active == 1)
        {
            update_texts(node_list, delaunay, g, mst, TXT_PRIM);
            show_mst(node_list, mst, g->size_vertices);
        }
        // handle keyboard, mouse event
        handle_sdl_event();
        tps_render();
    }
    tps_closeWindow();
    free(_screen);
}

void show_mst(list_t *node_list, int *mst, int size_vertices)
{
    for (int i = 1; i < size_vertices; i++)
    {
        // No parent node or double node have the same coordinate to one has to
        // be -1
        if (mst[i] == -1)
        {
            data_t d1 = *(data_t *)list_get(node_list, i);
            // deprintf("Node %d (%f %f) has no parent node\n", i, d1.latitude,
            // d1.longitude);
            draw_node(d1.latitude, d1.longitude, 15, 15, COLOR_RED);
            continue;
        }
        data_t d1 = *(data_t *)list_get(node_list, i);
        data_t d2 = *(data_t *)list_get(node_list, mst[i]);
        // draw_node(d1.latitude, d1.longitude, 2, 2, COLOR_BLUE);
        draw_edge(d1.latitude, d1.longitude, d2.latitude, d2.longitude);
    }
}

void show_delaunay(delaunay_t *triangles)
{
    for (size_t i = 0; i < triangles->size_triangles; i++)
    {
        triangle_t *t = triangles->triangles[i];
        // draw edge between s1 and s2
        draw_edge(t->s1->latitude, t->s1->longitude, t->s2->latitude, t->s2->longitude);
        // draw edge between s2 and s3
        draw_edge(t->s2->latitude, t->s2->longitude, t->s3->latitude, t->s3->longitude);
        // draw edge between s3 and s1
        draw_edge(t->s3->latitude, t->s3->longitude, t->s1->latitude, t->s1->longitude);
    }
}

void handle_sdl_event(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);
    static int g_mouse_x;
    static int g_mouse_y;
    switch (event.type)
    {
    case SDL_MOUSEWHEEL:
        if (event.wheel.y > 0)
        {
            zoom_in();
        }
        else if (event.wheel.y < 0)
        {
            zoom_out();
        }
        break;
    case SDL_KEYDOWN:
        onKeyDown(event.key.keysym.sym);
        break;
    case SDL_MOUSEBUTTONDOWN:
        // Save the current mouse position when the button is pressed
        g_mouse_x = event.button.x;
        g_mouse_y = event.button.y;
        break;
    case SDL_MOUSEMOTION:
        if (event.motion.state & SDL_BUTTON_LMASK)
        {
            // Calculate the difference between the current mouse position and
            // the position when the button was first pressed
            int dx = event.motion.x - g_mouse_x;
            int dy = event.motion.y - g_mouse_y;

            // Update the camera position based on the difference
            camera_move(dx, dy);

            // Save the current mouse position for the next move event
            g_mouse_x = event.motion.x;
            g_mouse_y = event.motion.y;
        }
    default:
        break;
    }
}

void camera_move(int dx, int dy)
{
    // Calculate the ratio of the width and height to the x_max and y_max fields
    double x_ratio = (double)_screen->width / (_screen->x_max - _screen->x_min);
    double y_ratio = (double)_screen->height / (_screen->y_max - _screen->y_min);

    // Calculate the change in the x_min and y_min fields based on the mouse
    // movement
    double dx_screen = dx / x_ratio;
    double dy_screen = dy / y_ratio;

    // Update the x_min and y_min fields
    _screen->x_min -= dx_screen;
    _screen->y_min -= dy_screen;

    // Update the x_max and y_max fields based on the new x_min and y_min values
    // and the original ratio
    _screen->x_max = _screen->x_min + (double)_screen->width / x_ratio;
    _screen->y_max = _screen->y_min + (double)_screen->height / y_ratio;
}

void onKeyDown(int key)
{
    if (key == SDLK_o)
    {
        _is_delaunay_active = !_is_delaunay_active;
        _is_prim_active = !_is_prim_active;
    }
    if (key == SDLK_a)
    {
        zoom_in();
    }
    if (key == SDLK_SPACE)
    {
        zoom_out();
    }
    if (key == SDLK_DOWN || key == SDLK_UP || key == SDLK_RIGHT || key == SDLK_LEFT)
    {
        move_screen(key);
    }
    if (key == SDLK_ESCAPE)
    {
        tps_closeWindow();
    }
}