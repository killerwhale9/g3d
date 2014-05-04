#ifndef __STONE_H__
#define __STONE_H__
/*******************************************************************************
 *  stone                                                                      *
 *  Fri May 02 CEST 2014                                                       *
 *  Copyright Eduardo San Martin Morote                                        *
 *  eduardo.san-martin-morote@ensimag.fr                                       *
 *  http://posva.net                                                           *
 ******************************************************************************/

#include "objReader.hpp"

class Stone : public Renderable {
    protected:
        objReader* m_model;
        uint8_t m_n;

    public:
        glm::vec3 m_pos;
        float m_size;
        Stone();
        void draw(int pass);
};

#endif
