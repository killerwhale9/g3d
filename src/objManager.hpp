#ifndef __OBJMANAGER_H__
#define __OBJMANAGER_H__
/*******************************************************************************
 *  objManager                                                                 *
 *  Fri May 02 CEST 2014                                                       *
 *  Copyright Eduardo San Martin Morote                                        *
 *  eduardo.san-martin-morote@ensimag.fr                                       *
 *  http://posva.net                                                           *
 ******************************************************************************/
#include "TextureManager.hpp"
#include <string>
#include <vector>
#include "objReader.hpp"

class objManager {
    static std::map<std::string, objReader*> models;

    //private
    objManager() {};

    public:
    inline static objReader& getObj(const std::string& key) {
        return *models[key];
    }

    static void loadObj(const std::string& file, const std::string& texture, const std::string &key);

    static void free();

};

#endif
