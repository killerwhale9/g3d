#ifndef __FLOCK_H__
#define __FLOCK_H__
/*******************************************************************************
 *  flock                                                                      *
 *  Thu May 01 CEST 2014                                                       *
 *  Copyright Eduardo San Martin Morote                                        *
 *  eduardo.san-martin-morote@ensimag.fr                                       *
 *  http://posva.net                                                           *
 ******************************************************************************/

#define NUM_FLOCKING_FISH 40
#define TIME_BETWEEN_UPDATES 3

#define kSpeed 0.003f
#include "renderable.hpp"
#include "environment.hpp"
#include "fish.hpp"
#include "glm/vec3.hpp"
#include <vector>

class Flock : public Renderable {
    /* Global Goal/Target */
    glm::vec3 goal;
    int step;

    /* Environment */
    Environment &env;
    std::vector<glm::vec3> envPos;
    std::vector<float> envRad;

    /* Flocking Fish */
    std::vector<Fish> school;

    int dx;
    int dy;

    public:
    virtual void draw(int pass);
    virtual void animate();
    virtual void init(Viewer& v);

    inline Fish* getLeader() { return &school[0]; }

    Flock(Environment &e);
    ~Flock();

};

#endif
