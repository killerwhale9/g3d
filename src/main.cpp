
// QApplication est dans QtGui, même si on ajoute le dossier de QtGui avec -I
#include <QtGui/QApplication>
#include "viewer.hpp"
#include "torse.hpp"
#include "coral.hpp"
#include "lightsMaterials.hpp"
#include "dynamicSystem.hpp"
#include "NoiseTerrain.hpp"
#include "TextureManager.hpp"
#include "skybox.hpp"
#include "glm/gtx/noise.hpp"

int main(int argc, char** argv)
{
    // Read command lines arguments.
    QApplication application(argc,argv);
#ifndef __APPLE__ // sur mac cet appel est en trop
    glutInit(&argc, argv);
#endif

    // Instantiate the viewer.
    Viewer viewer;

    // build your scene here

    //viewer.addRenderable(new Torse());
    float coralOffsetX=0;
    float coralOffsetY=0;
    int i=0;
    for (i=0; i < 60; i++) {
        coralOffsetX = glm::simplex(glm::vec3(coralOffsetX*3, coralOffsetY ,1.0));
        coralOffsetY = glm::simplex(glm::vec3(coralOffsetX, coralOffsetY*10 ,1.0));
        viewer.addRenderable(new Coral(Coral::defaultDepth, coralOffsetX*10, coralOffsetY*10));
    } 

    viewer.addRenderable(new Skybox());

    viewer.addRenderable(new DynamicSystem());
    viewer.noise = new NoiseTerrain();
    viewer.noise_zoom = 4;
    viewer.noise_persistence = 0.3;
    viewer.noise_octaves = 2;

    viewer.noise->generateClouds(100, 100, viewer.noise_octaves, viewer.noise_persistence, viewer.noise_octaves);
    viewer.addRenderable(viewer.noise);

    viewer.setWindowTitle("viewer");
    // Make the viewer window visible on screen.
    viewer.show();

    // scene size and stuff
    qglviewer::Camera *cam = viewer.camera();
    cam->setSceneRadius(500.f);
    //viewer.setSceneRadius(500.0f);

    // Run main loop.
    return application.exec();
}
