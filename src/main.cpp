
#include <QApplication>
#include "viewer.hpp"
#include "torse.hpp"
#include "coral.hpp"
#include "lightsMaterials.hpp"
#include "dynamicSystem.hpp"
#include "NoiseTerrain.hpp"

int main(int argc, char** argv)
{
    // Read command lines arguments.
    QApplication application(argc,argv);

    // Instantiate the viewer.
    Viewer viewer;

    // build your scene here

    //viewer.addRenderable(new Torse());
    viewer.addRenderable(new Coral(Coral::defaultDepth));
    viewer.addRenderable(new DynamicSystem());
    viewer.noise = new NoiseTerrain();
    viewer.noise_zoom = 4;
    viewer.noise_persistence = 0.5;
    viewer.noise_octaves = 2;

    viewer.noise->generateClouds(100, 100, 40, 0.5, 2);
    viewer.addRenderable(viewer.noise);
    // viewer.addRenderable(new LightsMaterials());

    glPushMatrix();
	glTranslatef(4,0.2,0.2);

    viewer.setWindowTitle("viewer");
    // Make the viewer window visible on screen.
    viewer.show();

    // Run main loop.
    return application.exec();
}
