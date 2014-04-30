#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__
/*******************************************************************************
 *  TextureManager                                                             *
 *  Fri Mar 28 CET 2014                                                        *
 *  Copyright Eduardo San Martin Morote                                        *
 *  eduardo.san-martin-morote@ensimag.fr                                       *
 *  http://posva.net                                                           *
 ******************************************************************************/

#include <map>
#include <QGLViewer/qglviewer.h>
#include <QImage>

class TextureManager {
    struct GLImg {
        GLuint id;
        QImage img;
        GLImg() {}
    };

    static std::map<std::string, GLImg> m_images;

    TextureManager();
    ~TextureManager();

    public:
    static GLuint loadTexture(const QString &file, const std::string &key, bool smooth = true);

    // On vérifie pas si la clé existe! C'est fait exprès
    inline static GLuint getTexture(const std::string &key) {
        return m_images[key].id;
    }

    inline static void bindTexture(const std::string &key) {
        glBindTexture(GL_TEXTURE_2D, m_images[key].id);
    }

    static GLuint loadTextureMipmaps(const QString &file, const std::string &key);
};

#endif
