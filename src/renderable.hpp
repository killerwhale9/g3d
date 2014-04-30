#ifndef _RENDERABLE_
#define _RENDERABLE_
#include <QKeyEvent>

class Viewer;
#define PASS_NORMAL 0
#define PASS_CAUSTIC 1

/**
 * General interface of renderable objetcs, that can be displayed
 * in the Viewer class.
 * It just defines three methods to override in child classes to
 * initialize, draw and animate a renderable objetc.
 */
class Renderable
{
    public:
        /// Virtual destructor (mandatory!)
        virtual ~Renderable() {};

        /** 
         * Initializes a Renderable objet before it is draw.
         * Default behavior: nothing is done.
         */
        virtual void init(Viewer&) {};

        /** 
         * Draw a Renderable object.
         * This pure virtual method must be overriden if child classes.
         * pass is used for the caustics. Only when pass is PASS_NORMAL there should be bind texture
         */
        virtual void draw(int pass) = 0;

        /** 
         * Animate an object. This method is invoked before each call of draw().
         * Default behavior: nothing is done.
         */
        virtual void animate() {};

        /** 
         * Objects can respond to key events.
         * Default behavior: nothing is done.
         */
        virtual void keyPressEvent(QKeyEvent*, Viewer&) {};

        /** 
         * Objects can respond to mouse events.
         * Default behavior: nothing is done.
         */
        virtual void mouseMoveEvent(QMouseEvent*, Viewer&) {};

};

#endif

