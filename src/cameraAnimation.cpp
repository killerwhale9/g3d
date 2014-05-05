#include "cameraAnimation.hpp"

void CameraAnimation::animate() {
    if (*m_active && m_ind < m_frames.size()) {
        if (m_frames[m_ind]) {
            m_cam->setPosition(qglviewer::Vec(m_frames[m_ind]->pos));
            m_cam->lookAt(qglviewer::Vec(m_frames[m_ind]->look));
        }
        m_ind++;
    }
}

CameraFrame::CameraFrame(const CameraFrame &a, const CameraFrame &b, int i, int n) :
    pos(a.pos[0] + (b.pos[0]-a.pos[0])*((double)i/n),
        a.pos[1] + (b.pos[1]-a.pos[1])*((double)i/n),
        a.pos[2] + (b.pos[2]-a.pos[2])*((double)i/n)
        ),
    look(a.look[0] + (b.look[0]-a.look[0])*((double)i/n),
        a.look[1] + (b.look[1]-a.look[1])*((double)i/n),
        a.look[2] + (b.look[2]-a.look[2])*((double)i/n)
        ),
    interpolate(false)
{}

 void CameraAnimation::interpolate()
{
    uint32_t k = m_original.size()+1;
    for(uint32_t i = 0; i < m_original.size(); ++i) {
        if (m_original[i]) {
            if (k > m_original.size()) {
                k = i;
                continue;
            } // else
            if (m_original[i]->interpolate) {
                for (uint32_t j = k+1; j < i; ++j) {
                    m_frames[j] = (new CameraFrame(*m_original[k], *m_original[i], j-k, i-k));
                }
            } else { // just add it
                m_frames[i] = new CameraFrame(*m_original[i]);
            }
            k = i;
        }
    }

}

CameraAnimation::CameraAnimation(uint32_t nbFrames, qglviewer::Camera &cam, bool &active) : m_original(nbFrames), m_frames(nbFrames),
    m_ind(0), m_cam(&cam), m_active(&active)
{
    for (uint32_t i = 0; i < nbFrames; ++i)
        m_original[i] = m_frames[i] = NULL;
}

CameraAnimation::~CameraAnimation()
{
    for (uint32_t i = 0; i < m_frames.size(); ++i)
        if (m_frames[i])
            delete m_frames[i];
    for (uint32_t i = 0; i < m_original.size(); ++i)
        if (m_frames[i])
            delete m_original[i];
    m_frames.clear();
    m_original.clear();
}
