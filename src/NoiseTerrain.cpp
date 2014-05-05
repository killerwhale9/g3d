#include "NoiseTerrain.hpp"
#include <cassert>
#include "glm/geometric.hpp"
#include "globals.hpp"

double NoiseTerrain::noise(double x,double y)
{
    double floorx=(double)((int)x);//This is kinda a cheap way to floor a double integer.
    double floory=(double)((int)y);
    double s,t,u,v;//Integer declaration
    s=findnoise2(floorx,floory); 
    t=findnoise2(floorx+1,floory);
    u=findnoise2(floorx,floory+1);//Get the surrounding pixels to calculate the transition.
    v=findnoise2(floorx+1,floory+1);
    double int1=interpolate(s,t,x-floorx);//Interpolate between the values.
    double int2=interpolate(u,v,x-floorx);//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
    return interpolate(int1,int2,y-floory);//Here we use y-floory, to get the 2nd dimension.
}

NoiseTerrain::NoiseTerrain() : m_hmap(NULL), m_w(0), m_h(0), m_normals(NULL), m_triangles(NULL)
{}

NoiseTerrain::~NoiseTerrain()
{
    if (m_hmap)
        delete[] m_hmap,
        delete[] m_normals;
}

void NoiseTerrain::generateClouds(uint32_t w, uint32_t h, double zoom, double persistence, int octaves)
{
    // allocate heighmap
    m_w = w;
    m_h = h;
    m_hmap = new double[w*h];
    assert(m_hmap);

    // generate

    for (uint32_t y = 0; y < m_h; y++) {
        for (uint32_t x = 0; x < m_w; x++) {
            double getNoise = 0;

            for (int a = 0; a < octaves-1; a++) {
                double freq = pow(2, a); // incr freq every loop
                double ampl = pow(persistence, a); // decrease ampl every loop

                getNoise += noise(static_cast<double>(x)*freq/zoom, (static_cast<double>(y)/zoom*freq)*ampl);
            }

            m_hmap[x+y*m_w] = 0.5*getNoise+0.5;
        }
    }

    computeNormals();
}

void NoiseTerrain::drawHMap()
{
    glPushMatrix();

    glPopMatrix();
}

float NoiseTerrain::getZ(float x, float y)
{
    x/=TERRAIN_WIDTH;
    x+=0.5;
    x*=m_w;
    y/=TERRAIN_HEIGHT;
    y+=0.5;
    y*=m_h;
    float px = x-floor(x);
    float py = y-floor(y);
    float p = (px+py)/2.f;

    return m_hmap[(int)x+((int)y)*m_w]*p+
           m_hmap[(int)x+1+((int)y+1)*m_w]*(1-p);
}


void NoiseTerrain::triangle_t::computeNormal(uint32_t w, uint32_t h) {
    for (uint32_t i = 0; i < 3; i++)
        vx[i].x /= (double)w,
        vx[i].y /= (double)h;
    normal = glm::normalize(glm::cross(vx[1]-vx[2], vx[0]-vx[2]));
}

void NoiseTerrain::computeNormals()
{
    // clé indice dans m_hmap;
    // pour chaque point liste des triangles qui le contient
    if (m_triangles) {
        for (std::list<triangle_t*>::iterator it(m_trianglePool.begin()); it != m_trianglePool.end(); ++it)
            delete *it;
        m_trianglePool.clear();
        delete[] m_triangles;
    }
    m_triangles = new std::list<triangle_t*>[m_w*m_h];

    // on calcule tout les triangles et les normales de ceux-ci
    for (uint32_t y = 0; y < m_h-1; ++y) {
        for (uint32_t x = 0; x < m_w; ++x) {
            triangle_t* t = NULL;
            if (x < m_w-1) {
                t = new triangle_t;
                assert(t);
                t->vx[0] = glm::vec3((x-0.5)*TERRAIN_WIDTH, (y-0.5)*TERRAIN_HEIGHT, m_hmap[x+y*m_w]);
                t->vx[1] = glm::vec3((x-0.5)*TERRAIN_WIDTH, (y+1-0.5)*TERRAIN_HEIGHT, m_hmap[x+(1+y)*m_w]);
                t->vx[2] = glm::vec3((x+1-0.5)*TERRAIN_WIDTH, (y-0.5)*TERRAIN_HEIGHT, m_hmap[x+1+y*m_w]);

                m_triangles[x+y*m_w].push_back(t);
                m_triangles[x+1+y*m_w].push_back(t);
                m_triangles[x+(y+1)*m_w].push_back(t);
                m_trianglePool.push_back(t);
                
                t->computeNormal(m_w, m_h);
            }
            if (x > 0) {
                t = NULL;
                t = new triangle_t;
                assert(t);
                t->vx[2] = glm::vec3((x-0.5)*TERRAIN_WIDTH, (y-0.5)*TERRAIN_HEIGHT, m_hmap[x+y*m_w]);
                t->vx[1] = glm::vec3((x-0.5)*TERRAIN_WIDTH, (y+1-0.5)*TERRAIN_HEIGHT, m_hmap[x+(1+y)*m_w]);
                t->vx[0] = glm::vec3((x-1-0.5)*TERRAIN_WIDTH, (y-0.5)*TERRAIN_HEIGHT, m_hmap[x-1+y*m_w]);

                m_triangles[x+y*m_w].push_back(t);
                m_triangles[x-1+y*m_w].push_back(t);
                m_triangles[x+(y+1)*m_w].push_back(t);
                m_trianglePool.push_back(t);

                t->computeNormal(m_w, m_h);
            }
        }
    }

    // on interpole les normale des triangles contenant chaque point
    if (m_normals)
        delete[] m_normals;
    m_normals = new glm::vec3[m_w*m_h];
    for (uint32_t y = 0; y < m_h; ++y) {
        for (uint32_t x = 0; x < m_w; ++x) {
            glm::vec3 sum;
            std::list<triangle_t*> *l = &m_triangles[x+y*m_w];
            for (std::list<triangle_t*>::iterator it(l->begin()); it != l->end(); ++it) {
                sum += (*it)->normal;
            }
            m_normals[x+y*m_w] = glm::normalize(sum);
        }
    }
}

double* NoiseTerrain::getHMap() {
    return m_hmap;
}
/*
void NoiseTerrain::computeNormals()
{
    if (m_normals)
        delete[] m_normals;

    m_normals = new glm::vec3[m_w*m_h];
    for (uint32_t y = 0; y < m_h; y++) {
        for (uint32_t x = 0; x < m_w; x++) {
            glm::vec3 a, b, c, sum;

            a.x = x;
            a.y = y;
            a.z = m_hmap[x+y*m_w];

            b.y = y;
            if (x < m_w-1) 
                b.x = x+1;
            else
                b.x = x-1;
            b.z = m_hmap[(int)b.x+y*m_w];

            c.x = x;
            if (y < m_h-1) 
                c.y = y+1;
            else
                c.y = y-1;
            c.z = m_hmap[x+(int)c.y*m_w];


            sum += glm::cross(c - a, b - a);

            m_normals[x+y*m_w] = glm::normalize(sum);

            if (a.x < m_w-1 && a.y < m_h-1)
                m_normals[x+y*m_w].z *= -1.0;
        }
    }
}
*/
