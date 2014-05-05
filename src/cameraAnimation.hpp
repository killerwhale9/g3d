#ifndef __CAMERAANIMATION_H__
#define __CAMERAANIMATION_H__
/*******************************************************************************
 *  cameraAnimation                                                            *
 *  Mon May 05 CEST 2014                                                       *
 *  Copyright Eduardo San Martin Morote                                        *
 *  eduardo.san-martin-morote@ensimag.fr                                       *
 *  http://posva.net                                                           *
 ******************************************************************************/

#include "renderable.hpp"
#include <QGLViewer/qglviewer.h>
#include <vector>
#include "glm/vec3.hpp"

class CameraFrame {
    public:
        glm::vec3 pos, look;
        bool interpolate;
        CameraFrame() : pos(), look(), interpolate(false) {}
        CameraFrame(const glm::vec3 &p, const glm::vec3 &l, bool in) : pos(p), look(l), interpolate(in) {}
        CameraFrame(const CameraFrame &a) : pos(a.pos), look(a.look), interpolate(a.interpolate) {}
        CameraFrame(const CameraFrame &a, const CameraFrame &b, int i, int n);
};

class CameraAnimation : public Renderable {
    protected:
        std::vector<CameraFrame*> m_original, m_frames;
        uint32_t m_ind;
        qglviewer::Camera *m_cam;
        bool *m_active;

    public:
        CameraAnimation(uint32_t nbFrames, qglviewer::Camera &cam, bool &active);
        ~CameraAnimation();
        inline void addFrame(int i, const CameraFrame &frame) { m_original[i] = (new CameraFrame(frame)); }
        inline void addFrame(int i, const glm::vec3 &pos, const glm::vec3 &look, bool interpolate) {
            m_original[i] = (new CameraFrame(pos, look, interpolate));
        }
        void interpolate();
        void draw(int pass) {}
        void animate();
};

#endif
