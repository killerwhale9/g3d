
// QApplication est dans QtGui, même si on ajoute le dossier de QtGui avec -I
#include <QtGui/QApplication>
#include "viewer.hpp"
#include "torse.hpp"
#include "NoiseTerrain.hpp"
#include "TextureManager.hpp"
#include "glm/gtx/noise.hpp"
#include "particlesystem.hpp"

int main(int argc, char** argv)
{
    // Read command lines arguments.
    QApplication application(argc,argv);
#ifndef __APPLE__ // sur mac cet appel est en trop
    glutInit(&argc, argv);
#endif

    // Instantiate the viewer.
    Viewer viewer;

    viewer.addRenderable(new ParticleSystem(glm::vec3()));

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
