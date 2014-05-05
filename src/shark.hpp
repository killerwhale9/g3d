#ifndef __SHARK_H__
#define __SHARK_H__
/*******************************************************************************
 *  shark                                                                      *
 *  Mon May 05 CEST 2014                                                       *
 *  Copyright Eduardo San Martin Morote                                        *
 *  eduardo.san-martin-morote@ensimag.fr                                       *
 *  http://posva.net                                                           *
 ******************************************************************************/

#include "objReader.hpp"

class Shark : public Renderable {
    objReader &m_body, &m_teeth, &m_eyes;
    Viewer *m_viewer;
    uint32_t m_timer;
    glm::vec3 m_pos;

    public:
    Shark();
    void draw(int pass);
    void animate();
    inline virtual void init(Viewer& v) {m_viewer = &v;};
    private:
    float m_rot;
};

#endif
