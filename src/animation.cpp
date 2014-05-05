#include "animation.hpp"
#include "torse.hpp"
#include <algorithm>
#include <iostream>

void Animation::generateInterpolation(Torse &me)
{
    m_frames.clear();
    m_frames.resize(m_original.size());
    for(uint32_t i = 0; i < m_original.size(); ++i) {
        for (std::list<Frame>::iterator it(m_original[i].begin()); it != m_original[i].end(); ++it) {
            std::cout<<"[1]List at index "<<i<<" is not empty\n";
            bool found = false;
            for (uint32_t k = i+1; k < m_original.size(); ++k) {
                std::list<Frame>::iterator next(find(m_original[k].begin(), m_original[k].end(), *it));
                if (next != m_original[k].end()) { // on peut interpoler
                    uint32_t n = k-i;
                    Frame f1(*it),
                          f2(*next);
                    // if any value is neg copy current value from me
                    for (int j = 0; j < 3; j++) {
                        if (f1.rot[j] < -360.f)
                            f1.rot[j] = me.getCurrentRotation(f1.type)[j];
                        // avoir -1 dans les deux c'est stupide...
                        if (f2.rot[j] < -360.f)
                            f2.rot[j] = me.getCurrentRotation(f2.type)[j];
                    }
                    // add the interpolations
                    std::cout<<"("<<f1.rot[0]<<","<<f1.rot[1]<<","<<f1.rot[2]<<")\n";
                    std::cout<<"[2]Interpolated a frame of type "<<f1.type<<" for frame "<<i<<"\n";
                    if (f1.rot != f2.rot) { // makes no sense to interpolate
                        for (uint32_t j = i+1; j < k; j++) {
                            m_frames[j].push_back(Frame(f1, f2, j-i, n));
                            std::cout<<"[2]Interpolated a frame of type "<<f1.type<<" for frame "<<j<<"\n";
                        }
                        std::cout<<"("<<f2.rot[0]<<","<<f2.rot[1]<<","<<f2.rot[2]<<")\n";
                        std::cout<<"[2]Interpolated a frame of type "<<f1.type<<" for frame "<<k<<"\n";
                        // we add first and last
                        m_frames[i].push_back(f1);
                        m_frames[k].push_back(f2);
                        found = true;
                        break; // exit this for
                    }
                }
            }
            if (!found) {// we still can add the first one 
                for (int j = 0; j < 3; j++) {
                    if (it->rot[j] < -360.f)
                        it->rot[j] = me.getCurrentRotation(it->type)[j];
                }
                m_frames[i].push_back(*it);
            }
        }
    }
}

Frame::Frame(const Frame &a, const Frame &b, uint32_t i, uint32_t n) :
    rot(a.rot[0] +(b.rot[0]-a.rot[0])*((double)i/n),
        a.rot[1] +(b.rot[1]-a.rot[1])*((double)i/n),
        a.rot[2] +(b.rot[2]-a.rot[2])*((double)i/n)
       ),
    type(a.type)
{std::cout<<"("<<rot[0]<<","<<rot[1]<<","<<rot[2]<<")\n";}

void Frame::correct()
{
    for (uint32_t i = 0; i < 3; i++) {
        if(rot[i] > -360.f && rot[i] < 0.f)
            rot[i] += 360.f;
    }
}

void Animation::update(Torse &me, uint32_t frame)
{
    assert(frame < m_frames.size());
    for (std::list<Frame>::iterator it(m_frames[frame].begin()); it != m_frames[frame].end(); ++it) {
        switch (it->type) {
            case e_armUL:
                me.m_angULArm = it->rot;
                break;
            case e_armLL:
                me.m_angLLArm = it->rot;
                break;
            case e_armUR:
                me.m_angURArm = it->rot;
                break;
            case e_armLR:
                me.m_angLRArm = it->rot;
                break;
            case e_legUL:
                me.m_angULLeg = it->rot;
                break;
            case e_legLL:
                me.m_angLLLeg = it->rot;
                break;
            case e_legUR:
                me.m_angURLeg = it->rot;
                break;
            case e_legLR:
                me.m_angLRLeg = it->rot;
                break;
            case e_head:
                me.m_angHead = it->rot;
                break;
            case e_torse:
                me.m_angTorse = it->rot;
                break;
            default:
                break;
        }
    }
}
