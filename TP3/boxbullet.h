#ifndef BOXBULLET_H
#define BOXBULLET_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include "btBulletDynamicsCommon.h"
class BoxBullet
{
public:
    BoxBullet(QVector3D _position,float _size,bool multicolor);
    void display(QOpenGLShaderProgram * buffer);
    void anime(float dt, QVector3D newpos);
private:
    QVector3D position;
    float size;
    QVector3D speed;
    float distance;
    QOpenGLBuffer vbo;
    QVector4D color = QVector4D(255,255,255,255);

    QMatrix4x4 matrix;
    QVector<GLfloat> vertData;




};

#endif // BOXBULLET_H
