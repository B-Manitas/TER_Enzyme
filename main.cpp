#include "include/simulation.hpp"
#include "include/view.hpp"

int main(int argc, char **argv)
{
    Simulation simulation = Simulation();
    simulation.init(argv[1]);

    View view = View(simulation);
    view.init_opengl(argc, argv);
    view.display();

    return 0;
}