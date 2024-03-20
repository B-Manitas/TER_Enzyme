#ifndef VIEW_HPP
#define VIEW_HPP

#include <vector>
#include <tuple>
#include <map>
#include <GL/glut.h>
#include "simulation.hpp"

class View
{
public:
    // ATTRIBUTES
    Simulation m_simulation;

    // Manage the window's width and height
    const int m_width = 800;
    const int m_height = 600;

    // Coordinates of the last mouse position
    int m_last_mouse_x = 0, m_last_mouse_y = 0;
    float m_rotate_x = 0, m_rotate_y = 0;
    float m_camera_distance = 100;

    // Characteristics of the vesicle
    int m_vesicle_radius = 620 / 2;
    int m_detail_x = 10;
    int m_detail_y = 10;

    std::map<int, std::tuple<float, float, float>> m_colors;

    // CONSTRUCTORS
    View() = default;
    View(Simulation simulation);
    View(Simulation simulation, int vesicle_radius);
    View(Simulation simulation, int vesicle_radius, int detail_x, int detail_y);

    // METHODS
    void set_rotation(float x, float y);
    void set_last_mouse_position(int x, int y);
    void set_simulation(const Simulation &simulation);
    void set_map_colors(const std::map<int, std::tuple<float, float, float>> &colors);

    void init_opengl();
    void draw_vesicle();
    void draw_molecules();
    void draw_scene();

    std::tuple<float, float, float> generate_color(int ident);
    void map_colors();

    void on_mouse_move(int x, int y);
    void on_mouse_click(int button, int state, int x, int y);
    void on_key_pressed(int key, int x, int y);

    static void static_on_mouse_move(int x, int y);
    static void static_on_mouse_click(int button, int state, int x, int y);
    static void static_on_key_pressed(int key, int x, int y);
    static void static_draw_scene();

    static View &instance();

    void display();
};

#endif // VIEW_HPP