#ifndef __CHEST_H__
#define __CHEST_H__
/*******************************************************************************
 *  chest                                                                      *
 *  Thu May 01 CEST 2014                                                       *
 *  Copyright Eduardo San Martin Morote                                        *
 *  eduardo.san-martin-morote@ensimag.fr                                       *
 *  http://posva.net                                                           *
 ******************************************************************************/

#include "objReader.hpp"
#include "glm/vec3.hpp"

class Viewer;

class Chest : public Renderable {
    protected:
        objReader& m_model;
        Viewer *m_viewer;
        uint32_t m_timer;
        glm::vec3 m_pos,
                  m_size;

    public:
        Chest();
        void draw(int pass);
        void animate();
        inline virtual void init(Viewer& v) {m_viewer = &v;};

};

#endif
