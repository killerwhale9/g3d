#ifndef __GLOBALS_H__
#define __GLOBALS_H__
/*******************************************************************************
 *  globals                                                                    *
 *  Sun May 04 CEST 2014                                                       *
 *  Copyright Eduardo San Martin Morote                                        *
 *  eduardo.san-martin-morote@ensimag.fr                                       *
 *  http://posva.net                                                           *
 ******************************************************************************/

#define TERRAIN_WIDTH 800.f
#define TERRAIN_HEIGHT 800.f

enum frame_type {
    e_armUL,
    e_armLL,
    e_armUR,
    e_armLR,
    e_legUL,
    e_legLL,
    e_legUR,
    e_legLR,
    e_torse,
    e_head,
    e_frame_type_count
};


#endif
