#include "../include/view.hpp"

// ============================
// CONSTRUCTORS
View::View(Simulation simulation) : m_simulation(simulation) {}

View::View(Simulation simulation, int vesicle_radius) : m_simulation(simulation), m_vesicle_radius(vesicle_radius) {}

View::View(Simulation simulation, int vesicle_radius, int detail_x, int detail_y) : m_simulation(simulation),
                                                                                    m_vesicle_radius(vesicle_radius),
                                                                                    m_detail_x(detail_x),
                                                                                    m_detail_y(detail_y) {}

// ============================
// GENERAL FUNCTIONS
void View::init_opengl(int argc, char **argv)
{
    View::instance().__set_simulation(m_simulation);

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(m_width, m_height);

    // Set the window's title
    glutCreateWindow("Diabetic Simulation");

    // Set the display function
    glutDisplayFunc(View::static_draw_scene);
    glutMouseFunc(View::static_on_mouse_click);
    glutSpecialFunc(View::static_on_key_pressed);

    // Set the clear color
    glClearColor(0.137, 0.137, 0.137, 1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the perspective of the camera to 45 degrees,
    // the aspect ratio to the window's width and height,
    // and the near and far planes to 1 and 2000
    gluPerspective(45.0, (float)m_width / (float)m_height, 1.0, 2000.0);

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

void View::display()
{
    glutMainLoop();
}

void View::__set_simulation(const Simulation &simulation)
{
    m_simulation = simulation;
    __map_colors();
}

// ============================
// DRAWING FUNCTIONS
void View::draw_vesicle()
{
    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0, 1.0, 1.0, 0.1);

    // Draw the vesicle sphere
    glutWireSphere(m_vesicle_radius, m_detail_x, m_detail_y);
}

void View::draw_molecules()
{
    for (Molecule m : m_simulation.m_molecules)
    {
        glPushMatrix();

        glTranslatef(m.position.x, m.position.y, m.position.z);
        glColor3f(std::get<0>(m_colors[m.ident]), std::get<1>(m_colors[m.ident]), std::get<2>(m_colors[m.ident]));

        glutSolidSphere(m.diameter / 2, 5, 5);
        glPopMatrix();
    }
}

void View::draw_scene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0, 0.0, 900.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glTranslatef(0.0, 0.0, -__m_camera_distance);
    glRotatef(__m_rotate_y, 0.0, 1.0, 0.0);
    glRotatef(__m_rotate_x, 1.0, 0.0, 0.0);

    draw_vesicle();
    draw_molecules();

    glutSwapBuffers();
}

void View::__map_colors()
{
    const int &n_colors = m_simulation.m_ident_molecules.size();

    // Compute the number of colors per direction in the RGB space
    const int &num_per_direction = ceil(pow(n_colors, 1.0 / 3.0));

    // Compute the spacing between each color
    const int &spacing = ceil(255.0 / (num_per_direction - 1));

    // Generate the colors for each molecule type
    for (int r = 0; r < num_per_direction; ++r)
        for (int g = 0; g < num_per_direction; ++g)
            for (int b = 0; b < num_per_direction; ++b)
            {
                const int &id_color = r * num_per_direction * num_per_direction + g * num_per_direction + b;
                const int &ident = m_simulation.m_ident_molecules[id_color];

                m_colors[ident] = {r * spacing, g * spacing, b * spacing};
            }
}

// ============================
// MOUSE EVENT HANDLERS
void View::on_mouse_move(int x, int y)
{
    __m_rotate_x += (y - __m_last_mouse_y) * 0.5;
    __m_rotate_y += (x - __m_last_mouse_x) * 0.5;

    __m_last_mouse_x = x;
    __m_last_mouse_y = y;

    glutPostRedisplay();
}

void View::on_mouse_click(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        __m_last_mouse_x = x;
        __m_last_mouse_y = y;

        glutMotionFunc(static_on_mouse_move);
    }

    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        glutMotionFunc(nullptr);
}

void View::on_key_pressed(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        __m_camera_distance = std::max(-1000.0f, __m_camera_distance - 10);
        break;

    case GLUT_KEY_DOWN:
        __m_camera_distance = std::min(100.0f, __m_camera_distance + 10);
        break;

    case GLUT_KEY_RIGHT:
        m_simulation.move_all_molecules();
        break;

    default:
        break;
    }

    glutPostRedisplay();
}

// ============================
// STATIC FUNCTIONS

void View::static_draw_scene()
{
    View::instance().draw_scene();
}

void View::static_on_mouse_move(int x, int y)
{
    View::instance().on_mouse_move(x, y);
}

void View::static_on_mouse_click(int button, int state, int x, int y)
{
    View::instance().on_mouse_click(button, state, x, y);
}

void View::static_on_key_pressed(int key, int x, int y)
{
    View::instance().on_key_pressed(key, x, y);
}

View &View::instance()
{
    static View view = View();
    return view;
}
