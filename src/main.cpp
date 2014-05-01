
#include <QApplication>
#include "viewer.hpp"
#include "torse.hpp"
#include "coral.hpp"
#include "lightsMaterials.hpp"
#include "dynamicSystem.hpp"
#include "NoiseTerrain.hpp"
#include "TextureManager.hpp"
#include "skybox.hpp"

int main(int argc, char** argv)
{
    // Read command lines arguments.
    QApplication application(argc,argv);

    // Instantiate the viewer.
    Viewer viewer;

    // build your scene here

    //viewer.addRenderable(new Torse());
    viewer.addRenderable(new Coral(Coral::defaultDepth));

    //viewer.addRenderable(new Skybox());

    viewer.addRenderable(new DynamicSystem());
    viewer.noise = new NoiseTerrain();
    viewer.noise_zoom = 2;
    viewer.noise_persistence = 0.5;
    viewer.noise_octaves = 2;

    viewer.noise->generateClouds(25, 25, 2, 0.5, 2);
    viewer.addRenderable(viewer.noise);

    viewer.setWindowTitle("viewer");
    // Make the viewer window visible on screen.
    viewer.show();

    // scene size and stuff
    qglviewer::Camera *cam = viewer.camera();
    cam->setSceneRadius(500.f);
    viewer.setSceneRadius(500.0f);

    // Run main loop.
    return application.exec();
}
