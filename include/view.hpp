#ifndef VIEW_HPP
#define VIEW_HPP

#include <vector>
#include <tuple>
#include <map>
#include <GL/glut.h>

#include "simulation.hpp"

class View
{
private:
    // PRIVATE ATTRIBUTES
    // Coordinates of the last mouse position
    int __m_last_mouse_x = 0, __m_last_mouse_y = 0;

    // Rotation angles
    float __m_rotate_x = 0, __m_rotate_y = 0;

    // The distance of the camera from the vesicle
    float __m_camera_distance = 100;

    // Map colors for each molecule type
    std::map<int, std::tuple<float, float, float>> m_colors;

    // PRIVATE METHODS
    /**
     * @brief Set the simulation object
     *
     * @param simulation
     */
    void __set_simulation(const Simulation &simulation);

    /**
     * @brief Generate a color for a molecule
     *
     * @param ident
     * @return std::tuple<float, float, float>
     */
    std::tuple<float, float, float> __generate_color(int ident);
    /**
     * @brief Map the colors of the molecules
     *
     */
    void __map_colors();

public:
    // ATTRIBUTES
    Simulation m_simulation;

    // Manage the window's width and height
    const int m_width = 800, m_height = 600;

    // Characteristics of the vesicle
    int m_detail_x = 10, m_detail_y = 10;
    int m_vesicle_radius = 620 / 2;

    // CONSTRUCTORS
    /**
     * @brief Construct a new View object
     *
     */
    View() = default;
    /**
     * @brief Construct a new View object
     *
     * @param simulation The simulation object
     */
    View(Simulation simulation);
    /**
     * @brief Construct a new View object
     *
     * @param simulation The simulation object
     * @param vesicle_radius The radius of the vesicle
     */
    View(Simulation simulation, int vesicle_radius);
    /**
     * @brief Construct a new View object
     *
     * @param simulation The simulation object
     * @param vesicle_radius The radius of the vesicle
     * @param detail_x The detail of the vesicle in the x direction
     * @param detail_y The detail of the vesicle in the y direction
     */
    View(Simulation simulation, int vesicle_radius, int detail_x, int detail_y);

    // METHODS
    void init_opengl(int argc, char **argv);

    /**
     * @brief Draw the vesicle
     *
     */
    void draw_vesicle();
    /**
     * @brief Draw the molecules
     *
     */
    void draw_molecules();
    /**
     * @brief Draw the scene
     *
     */
    void draw_scene();

    /**
     * @brief Handle the mouse move event
     *
     * @param x The mouse's x coordinate
     * @param y The mouse's y coordinate
     */
    void on_mouse_move(int x, int y);
    /**
     * @brief Handle the mouse click event
     *
     * @param button The button that was clicked
     * @param state The state of the button
     * @param x The mouse's x coordinate
     * @param y The mouse's y coordinate
     */
    void on_mouse_click(int button, int state, int x, int y);
    /**
     * @brief Handle the key pressed event
     *
     * @param key The key that was pressed
     * @param x The mouse's x coordinate
     * @param y The mouse's y coordinate
     */
    void on_key_pressed(int key, int x, int y);

    // STATIC METHODS
    /**
     * @brief Handle the mouse move event
     *
     * @param x The mouse's x coordinate
     * @param y The mouse's y coordinate
     */
    static void static_on_mouse_move(int x, int y);
    /**
     * @brief Handle the mouse click event
     *
     * @param button The button that was clicked
     * @param state The state of the button
     * @param x The mouse's x coordinate
     * @param y The mouse's y coordinate
     */
    static void static_on_mouse_click(int button, int state, int x, int y);
    /**
     * @brief Handle the key pressed event
     *
     * @param key The key that was pressed
     * @param x The mouse's x coordinate
     * @param y The mouse's y coordinate
     */
    static void static_on_key_pressed(int key, int x, int y);
    /**
     * @brief Draw the scene
     *
     */
    static void static_draw_scene();

    /**
     * @brief Get the instance object of the view used for GLUT callbacks
     *
     * @return View&
     */
    static View &instance();

    void display();
};

#endif // VIEW_HPP