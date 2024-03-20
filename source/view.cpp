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

// ============================
// GENERAL FUNCTIONS
void View::init_opengl()
{
    View::instance().set_simulation(m_simulation);
    glutInitWindowSize(m_width, m_height);
    glutCreateWindow("Diabetic Simulation");

    glutDisplayFunc(View::static_draw_scene);
    glutMouseFunc(View::static_on_mouse_click);
    glutSpecialFunc(View::static_on_key_pressed);

    // Set the clear color to black
    glClearColor(0.0, 0.0, 0.0, 1.0);

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

void View::set_simulation(const Simulation &simulation)
{
    m_simulation = simulation;
    map_colors();
}

void View::set_map_colors(const std::map<int, std::tuple<float, float, float>> &colors)
{
    m_colors = colors;
}

// ============================
// DRAWING FUNCTIONS
void View::draw_vesicle()
{
    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(1.0, 1.0, 1.0);

    // Draw the vesicle sphere
    glutWireSphere(m_vesicle_radius, m_detail_x, m_detail_y);
}

void View::draw_molecules()
{
    for (Molecule m : m_simulation.m_molecules)
    {
        glPushMatrix();

        glTranslatef(m.x, m.y, m.z);
        glColor3f(std::get<0>(m_colors[m.ident]), std::get<1>(m_colors[m.ident]), std::get<2>(m_colors[m.ident]));

        glutSolidSphere(m.diameter / 2, 5, 5);
        glPopMatrix();
    }
}

void View::draw_scene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0, 0.0, 900.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);

    glTranslatef(0.0, 0.0, -m_camera_distance);
    glRotatef(m_rotate_y, 0.0, 1.0, 0.0);
    glRotatef(m_rotate_x, 1.0, 0.0, 0.0);

    draw_vesicle();
    draw_molecules();

    glutSwapBuffers();
}

std::tuple<float, float, float> View::generate_color(int ident)
{
    // Convertir le nombre en une valeur d'intensité entre 0 et 255
    int intensity = static_cast<int>(fmod(ident * 255.0, 256.0));

    // Utiliser l'intensité pour déterminer les composantes R, G et B de la couleur
    std::tuple<float, float, float> color;
    std::get<0>(color) = (intensity * 3 + rand() % 50) % 256;
    std::get<1>(color) = (intensity * 5 + rand() % 100) % 256;
    std::get<2>(color) = (intensity * 7 + rand() % 150) % 256;

    std::get<0>(color) /= 255;
    std::get<1>(color) /= 255;
    std::get<2>(color) /= 255;

    return color;
}

void View::map_colors()
{
    for (auto &&ident : m_simulation.m_ident_molecules)
        m_colors[ident] = generate_color(ident);
}

// ============================
// MOUSE EVENT HANDLERS
void View::on_mouse_move(int x, int y)
{
    int deltaX = x - m_last_mouse_x;
    int deltaY = y - m_last_mouse_y;

    m_rotate_x += deltaY * 0.1;
    m_rotate_y += deltaX * 0.1;

    m_last_mouse_x = x;
    m_last_mouse_y = y;

    glutPostRedisplay();
}

void View::on_mouse_click(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        m_last_mouse_x = x;
        m_last_mouse_y = y;

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
        m_camera_distance = std::max(-1000.0f, m_camera_distance - 10);
        break;

    case GLUT_KEY_DOWN:
        m_camera_distance = std::min(100.0f, m_camera_distance + 10);
        break;

    default:
        break;
    }

    glutPostRedisplay();
}
