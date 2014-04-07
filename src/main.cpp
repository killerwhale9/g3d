
#include <qapplication.h>
#include "viewer.hpp"
#include "torse.hpp"
#include "lightsMaterials.hpp"
#include "dynamicSystem.hpp"

int main(int argc, char** argv)
{
    // Read command lines arguments.
    QApplication application(argc,argv);

    // Instantiate the viewer.
    Viewer viewer;

    // build your scene here
    viewer.addRenderable(new Torse());
    viewer.addRenderable(new DynamicSystem());
    // viewer.addRenderable(new LightsMaterials());

    viewer.setWindowTitle("viewer");
    // Make the viewer window visible on screen.
    viewer.show();

    // Run main loop.
    return application.exec();
}
