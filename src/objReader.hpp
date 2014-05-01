#ifndef __OBJREADER_H__
#define __OBJREADER_H__

#include "renderable.hpp"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include <string>
#include <vector>

class objReader : public Renderable {

    struct vec3 {
        GLfloat v[3];
        vec3(GLfloat x, GLfloat y, GLfloat z) {
            v[0] = x;
            v[1] = y;
            v[2] = z;
        }
        vec3(const vec3& c) {
            v[0] = c.v[0];
            v[1] = c.v[1];
            v[2] = c.v[2];
        }
        inline GLfloat* vec() { return v; }
    };

    struct vec2 {
        GLfloat v[2];
        vec2(GLfloat x, GLfloat y) {
            v[0] = x;
            v[1] = y;
        }

        vec2(const vec2& c){
            v[0] = c.v[0];
            v[1] = c.v[1];
        }
        inline GLfloat* vec() { return v; }
    };

    class face {
        public:
            // index positif, si < 0 pas d'index
            std::vector<int> vx;
            std::vector<int> nr;
            std::vector<int> tx;
            face() :vx(), nr(), tx() {
            }

            face(const face& f) {
                for (unsigned int i = 0; i < f.vx.size(); i++)
                    vx.push_back(f.vx[i]);
                for (unsigned int i = 0; i < f.nr.size(); i++)
                    nr.push_back(f.nr[i]);
                for (unsigned int i = 0; i < f.tx.size(); i++)
                    tx.push_back(f.tx[i]);
            }
    };

    std::vector<vec3> m_vertices,
        m_normals;
    std::vector<vec2> m_texCoord;
    std::vector<face> m_faces;
    std::vector<GLuint> m_textures;
    //GLuint m_vertN, m_normN, m_texN;

    void computeNormals();

    // change les indices relatifs (negatif) en absolues
    // 0 vertices
    // 1 texCoord
    // 2 normales
    void changeIndex(int &x, int vector)
    {
        switch (vector) {
            case 0:
                x = x<0?(m_vertices.size() + x):(x-1);
                break;
            case 1:
                x = x<0?(m_texCoord.size() + x):(x-1);
                break;
            default :
                x = x<0?(m_normals.size() + x):(x-1);
                break;
        }
    }

    void loadObj(const std::string &file);

    public:
    objReader(const std::string &file, const char* texture);
    objReader(const std::string &file, GLuint texture);
    ~objReader();

    virtual void draw(int pass);
};

#endif
