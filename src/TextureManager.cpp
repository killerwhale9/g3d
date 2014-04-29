#include "TextureManager.hpp"

std::map<std::string, TextureManager::GLImg> TextureManager::m_images;

TextureManager::TextureManager()
{}

TextureManager::~TextureManager()
{
    for (std::map<std::string, GLImg>::iterator it(m_images.begin()); it != m_images.end(); ++it) {
        glDeleteTextures(1, &it->second.id);
    }
    m_images.clear();
}


GLuint TextureManager::loadTexture(const QString &file, const std::string &key, bool smooth)
{
    QImage i;
    if (m_images.find(key) != m_images.end()) {
        std::cerr<<"Duplicate key '"<<key<<"' when loading "<<file.toStdString()<<"\n";
        return m_images[key].id;
    }
    if (i.load(file)) {
        m_images[key].img = QGLWidget::convertToGLFormat(i);

        //construire les textures openGL
        glGenTextures( 1, &m_images[key].id );
        std::cerr<<"Generated texture with id "<<m_images[key].id<<" from "<<file.toStdString()<<"\n";
        glBindTexture( GL_TEXTURE_2D, m_images[key].id );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, smooth?GL_LINEAR:GL_NEAREST );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, smooth?GL_LINEAR:GL_NEAREST );

        //les deux commandes suivantes servent a elargir la texture pq. la surface soit remplie;
        //.. clamp_to_edge = repeter la derniere ligne de la texture (marche bien ici)
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_images[key].img.width(), m_images[key].img.height(), 0,
                GL_RGBA, GL_UNSIGNED_BYTE, m_images[key].img.bits() );
    } else {
        std::cerr<<"Error loading file "<<file.toStdString()<<"\n";
        return 0;
    }

    return m_images[key].id;
}
