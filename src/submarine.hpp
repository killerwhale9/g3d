#ifndef __SUBMARINE_H__
#define __SUBMARINE_H__
/*******************************************************************************
 *  submarine                                                                  *
 *  Mon May 05 CEST 2014                                                       *
 *  Copyright Eduardo San Martin Morote                                        *
 *  eduardo.san-martin-morote@ensimag.fr                                       *
 *  http://posva.net                                                           *
 ******************************************************************************/

#include "objReader.hpp"
#include "glm/vec3.hpp"

class Viewer;

class Submarine : public Renderable {
    protected:
        objReader& m_model;
        Viewer *m_viewer;
        glm::vec3 m_pos,
                  m_size;

    public:
        Submarine();
        void draw(int pass);
        void animate();
        inline virtual void init(Viewer& v) {m_viewer = &v;};

};

#endif
