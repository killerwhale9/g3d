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

class Chest : public Renderable {
    protected:
        objReader& m_model;

    public:
        Chest();
        void draw(int pass);

};

#endif
