#include "objReader.hpp"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <algorithm>
#include <sstream>
#include <cmath>
#include "TextureManager.hpp"
#include <QGLViewer/qglviewer.h>

objReader::objReader(const std::string &file, const char* texture)
: m_vertices(), m_normals(), m_texCoord(), m_faces(), m_textures()
{
    loadObj(file);

    //computeNormals();

    if (texture) {
        m_textures.push_back(TextureManager::loadTexture(texture, texture));
    }

    std::cout<<"Read "<<m_vertices.size()<<" vertices, "<<m_texCoord.size()
        <<" texcoord, "<<m_normals.size()<<" normals, "<<m_faces.size()<<" faces.\n"
        <<m_textures.size()<<" textures files were loaded\n";
}

objReader::objReader(const std::string &file, GLuint texture)
{
    loadObj(file);

    m_textures.push_back(texture);

}

void objReader::loadObj(const std::string &file)
{
    std::string r;
    std::ifstream f;
    f.open(file.c_str());
    float x, y, z;
    unsigned int line = 0;
    while (getline(f, r)) {
        if (r[0] != '#' && r[0] != '\0' && r[0] != '\n') {
            //else std::cout<<r<<"\n";

            if (r[0] == 'v') {
                switch (r[1]) {
                    case ' ': // vertex
                        sscanf(r.c_str(), "v %f %f %f", &x, &y, &z);
                        m_vertices.push_back(vec3(x,y,z));
                        //std::cerr<<"Read vx("<<x<<","<<y<<","<<z<<")\n";
                        break;
                    case 't': // text coord
                        sscanf(r.c_str(), "vt %f %f", &x, &y);
                        m_texCoord.push_back(vec2(x,y));
                        break;
                    case 'n': // normal
                        sscanf(r.c_str(), "vn %f %f %f", &x, &y, &z);
                        m_normals.push_back(vec3(x,y,z));
                        break;
                    default:
                        std::cout<<"Error reading line '"<<r<<"' in file "<<file<<"\n";
                        break;
                }
            } else if (r[0] == 'f') {
                std::istringstream is(r);
                std::string ff("");
                is>>ff; // read f
                m_faces.push_back(face());
                ff = "";
                is>>ff;
                while(ff.compare("") != 0) {
                    //std::cerr<<"read: "<<ff<<"\n";
                    size_t count = std::count(ff.begin(), ff.end(), '/');
                    int v, n, t;
                    switch (count) {
                        case 0: // que des vertex
                            sscanf(ff.c_str(),"%d", &v);
                            changeIndex(v, 0);
                            //std::cerr<<"pushed vx with index "<<v<<"\n";
                            m_faces.back().vx.push_back(v);
                            break;
                        case 1: // vertex et text coor
                            break;
                        case 2: // vertex, texcoor et normales OU vertex et normals
                            sscanf(ff.c_str(),"%d/%d/%d", &v, &t, &n);
                            changeIndex(v, 0);
                            changeIndex(t, 1);
                            changeIndex(n, 2);
                            m_faces.back().vx.push_back(v);
                            m_faces.back().nr.push_back(n);
                            m_faces.back().tx.push_back(t);
                            break;
                        default:
                            break;
                    }
                    ff = "";
                    is>>ff;
                }
            } else  {
                //std::cout<<"Error in line "<<line<<"'"<<r<<"' for file "<<file<<"...\n";
            }
        }
        line++;
    }
    f.close();
}

objReader::~objReader()
{
    // eventually destroy textures from texturemanager
}

void objReader::computeNormals()
{
    // not really needed
}

void objReader::draw(int pass)
{
    glPushMatrix();

    //std::cout<<"DRAW\n";
    //glScalef(4.f, 4.f, 4.f);
    if (m_textures.size() > 0 && pass == PASS_NORMAL)
        glBindTexture(GL_TEXTURE_2D, m_textures[0]);
    for (std::vector<face>::iterator it(m_faces.begin()); it != m_faces.end(); ++it) {
        glBegin(GL_POLYGON);
        std::vector<int>::iterator nr(it->nr.begin());
        std::vector<int>::iterator tx(it->tx.begin());
        for (std::vector<int>::iterator vx(it->vx.begin()); vx != it->vx.end(); ++vx) {
            if (nr != it->nr.end()) {
                glNormal3fv(m_normals[*nr].vec());
                ++nr;
            }
            if (tx != it->tx.end()) {
                //glTexCoord2fv(m_texCoord[*tx].vec());
                glTexCoord2f(m_texCoord[*tx].v[0],
                             m_texCoord[*tx].v[1]);
                //std::cerr<<"Tex["<<*tx<<"]: "<<m_texCoord[*tx].v[0]<<","<<m_texCoord[*tx].v[1]<<"\n";
                ++tx;
            }
                glVertex3fv(m_vertices[*vx].vec());
                //std::cout<<"v["<<*vx<<"] = ("<<
                    //m_vertices[*vx].vec()[0]<<","<<
                    //m_vertices[*vx].vec()[1]<<","<<
                    //m_vertices[*vx].vec()[2]<<
                    //")\n";
        }
        glEnd();
    }
    glPopMatrix();
}
