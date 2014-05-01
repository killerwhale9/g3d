#ifndef __ENVIRONEMENT_H__
#define __ENVIRONEMENT_H__
/*******************************************************************************
 *  environement                                                               *
 *  Thu May 01 CEST 2014                                                       *
 *  Copyright Eduardo San Martin Morote                                        *
 *  eduardo.san-martin-morote@ensimag.fr                                       *
 *  http://posva.net                                                           *
 ******************************************************************************/

#include "glm/vec3.hpp"
#define SCENE_WIDTH 100
#define SCENE_HEIGHT 50
#define SCENE_DEPTH 100
#include <vector>

class Environment {
    public:
        // test si les poissons collisiones avec l'environement
        // retourne vecteur de pour se rediriger
        glm::vec3 steer(glm::vec3 pos, glm::vec3 vel);
        void setObjs(std::vector<glm::vec3> sPos, std::vector<float> sRad);

    private:
        std::vector<glm::vec3> spherePos;
        std::vector<float> sphereRad;
        float intexp( float base, int exp );
        float rad2deg( float rad );
        float deg2rad( float deg );


};
#endif
