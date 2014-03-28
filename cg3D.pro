TEMPLATE = app
TARGET   = cg3D
CONFIG  += qt opengl warn_on debug
QT *= xml opengl 

HEADERS  = $$system(find src -name '*.h')
SOURCES  = $$system(find src -name '*.cpp')

DESTDIR = bin
UI_DIR = .

OBJECTS_DIR = obj

macx:LIBS *= -framework Glut
linux-g++*:LIBS *= -lglut

exists( /usr/lib/i386-linux-gnu/libQGLViewer.so) {
    LIBS *= -lQGLViewer # Ubuntu 32
} else {
exists( /usr/lib/libqglviewer-qt4.so) |
exists( /usr/lib/x86_64-linux-gnu/libqglviewer-qt4.so ) {
  LIBS *= -lqglviewer-qt4  # Ubuntu
} else {
  exists( /usr/lib64/libQGLViewer.so ) {
    LIBS *= -lQGLViewer    # CentOS
  } else {
    LIBS *= "-framework qglviewer"   # MacOS
  }
}
}
