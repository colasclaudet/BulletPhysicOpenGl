#ifndef SPHEREBULLET_H
#define SPHEREBULLET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include "btBulletDynamicsCommon.h"

class SphereBullet
{
public:
    SphereBullet(QVector3D _position,float _size);
    void display(QOpenGLShaderProgram * buffer);
    void anime(float dt, QVector3D newpos);
private:
    QVector3D position;
    float size;
    QVector3D speed;
    float distance;
    QOpenGLBuffer vbo;
    QVector4D color = QVector4D(255,255,255,255);
    unsigned int nb_faces = 100;
    QMatrix4x4 matrix;
    QVector<GLfloat> vertData;


};

#endif // SPHEREBULLET_H
