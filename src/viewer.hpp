/*
 * Main GL viewer class, based on the QGLViewer library.
 * 
 * QGLViewer is a Qt-based viewer, which provides an OpenGL rendering
 * context and GUI event management.
 * The documentation of the QGLViewer library can be found at the following 
 * url: http://www.libqglviewer.com
 */

#ifndef _VIEWER_
#define _VIEWER_

#include <QGLViewer/qglviewer.h>
#include <list>
#include "NoiseTerrain.hpp"
#define NUM_PATTERNS 32
using namespace std;

class Renderable;


class Viewer : public QGLViewer
{
    public :

        Viewer();
        virtual ~Viewer();
        void addRenderable(Renderable *r);
        double noise_zoom, noise_persistence;
        int noise_octaves;
        NoiseTerrain *noise;

        /* Scene methods */
    protected :
        /// List of the scene objects, to render, animate, ...
        list<Renderable *> renderableList;

        /// Create the scene and initializes rendering parameters
        virtual void init();

        /// Draw every objects of the scene
        virtual void draw();

        /// Animate every objects of the scene
        virtual void animate();

        // load all the textures
        void loadTextures();


        /* Viewing parameters */
    protected :
        bool toogleWireframe;
        bool toogleLight;
        int currentCaustic;
        GLuint causticsTex[NUM_PATTERNS];

        /// Handle keyboard events specifically
        virtual void keyPressEvent(QKeyEvent *e);

        /// Handle keyboard events specifically
        virtual void mouseMoveEvent(QMouseEvent *e);

        /// Draw every objects of the scene
        virtual QString helpString() const;
};

#endif
