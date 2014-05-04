#ifndef __ANIMATION_H__
#define __ANIMATION_H__
/*******************************************************************************
 *  animation                                                                  *
 *  Sun May 04 CEST 2014                                                       *
 *  Copyright Eduardo San Martin Morote                                        *
 *  eduardo.san-martin-morote@ensimag.fr                                       *
 *  http://posva.net                                                           *
 ******************************************************************************/

#include <vector>
#include <list>
#include <stdint.h>
#include "glm/vec3.hpp"
#include "globals.hpp"

class Torse;

// rotations dans une frame donnée
class Frame {
    public:
    glm::vec3 rot; // une valeur négative = prends la valeur courante
    frame_type type;
    Frame(const glm::vec3 v, frame_type t) : rot(v), type(t) {}
    Frame(frame_type t) : rot(-360, -360, -360), type(t) {} // prends les valeurs antérieures
    Frame(const Frame &o) : rot(o.rot), type(o.type) {}
    Frame(const Frame &a, const Frame &b, uint32_t i, uint32_t n); // interpolates
    inline bool operator==(const Frame& b) { return type == b.type; }
    inline bool operator!=(const Frame& b) { return type != b.type; }
    private:
    void correct(); // pas d'angles négatifs
};

// on ajoute des frames qui indiquent le changement des rotations d'une des
// parties du coprs(torse). Si on met une valeur de -360 ou plus petit, alors ça sera la
// valeur actuelle lors de l'asssignation de l'animation des rotations du torse
// qui sont utilisées. Il faut obligatoirement avoir un élément dans la frame 0
// On peut utiliser plusieurs animations si certaines actions ont une durée
// différente
class Animation {
    std::vector<std::list<Frame> > m_frames, m_original;

    public:
    Animation(int nbFrames) : m_frames(), m_original(nbFrames) {}
    ~Animation() {m_frames.clear(); m_original.clear();}

    void generateInterpolation(Torse& me);
    void update(Torse &me, uint32_t frame);
    inline uint32_t getSize() const { return m_frames.size(); }
    inline void clear() { m_original.clear(); }
    inline void addFrame(int index, Frame& f) { m_original[index].push_back(f); }
    inline void addFrame(int index, frame_type type, const glm::vec3& angles) { m_original[index].push_back(Frame(angles, type));}
    inline void addFrame(int index, frame_type type) { m_original[index].push_back(Frame(type));}
};

#endif
