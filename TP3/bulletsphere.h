#ifndef BULLETSPHERE_H
#define BULLETSPHERE_H

#include "btBulletDynamicsCommon.h"
#include <stdio.h>
#include <QKeyEvent>
#include <QTimer>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
class bulletsphere
{
public:
    bulletsphere();
    void init(GLuint vertexPositionID);
    void draw(QOpenGLShaderProgram * buffer);
private :
    bool isInited ;
    int m_vao;
    int m_vboVertex;
    int m_vboIndex;

    int lats;
    int longs;
    QOpenGLBuffer vbo_sphere;
};

#endif // BULLETSPHERE_H
