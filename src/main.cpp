
#include <qapplication.h>
#include "viewer.hpp"
#include "torse.hpp"
#include "coral.hpp"
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
    viewer.addRenderable(new Coral(3));
    viewer.addRenderable(new DynamicSystem());
    // viewer.addRenderable(new LightsMaterials());

    glPushMatrix();
	glTranslatef(4,0.2,0.2);

    viewer.setWindowTitle("viewer");
    // Make the viewer window visible on screen.
    viewer.show();

    // Run main loop.
    return application.exec();
}
