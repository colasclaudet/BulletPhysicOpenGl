// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#ifndef GLAREA_H
#define GLAREA_H

#include <QKeyEvent>
#include <QTimer>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <Qt3DRender/Qt3DRender>

#include "boxbullet.h"
#include "objectbullet.h"


class GLArea : public QOpenGLWidget,
               protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLArea(QWidget *parent = nullptr);
    ~GLArea() override;

    void myBox(float x, float y, float z);

    void setNb_box(unsigned int value);

    void setNb_sphere(unsigned int value);

    void setNb_bullet(unsigned int value);
    void update_bullet();

protected slots:
    void onTimeout();

protected:
    void initializeGL() override;
    void doProjection();
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *ev) override;
    void keyReleaseEvent(QKeyEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    float xRot=20.0f, yRot=0.0f, zRot=0.0f;
    float xPos=0.0f,  yPos=0.0f, zPos=-100.0f;
    QTimer *timer = nullptr;
    QElapsedTimer elapsedTimer;
    float dt = 0;
    float windowRatio = 1.0f;
    QPoint lastPos;

    QOpenGLShaderProgram *program_sol;
    QOpenGLShaderProgram *program_particule;
    QOpenGLShaderProgram *program_box;
    //QOpenGLShaderProgram *program_sphere;
    QOpenGLBuffer vbo_sol;
    QOpenGLBuffer vbo_particule;

    QOpenGLTexture *textures[2];

    void makeGLObjects();
    void tearGLObjects();

    BoxBullet * bl;
    ObjectBullet * obl;

    btDiscreteDynamicsWorld *myWorld;
    btBroadphaseInterface *myBroadphase;

    btCollisionDispatcher *myDispatcher;

    btDefaultCollisionConfiguration *myCollisionConfiguration;

    btSequentialImpulseConstraintSolver *mySequentialImpulseConstraintSolver;
    btTransform myTransform;

    btDefaultMotionState *myMotionState,
                         *myMotionState_Sol;

    btScalar matrix[16];

    btRigidBody *body,
                *body_sol;

    btCollisionShape* shape;
    btCollisionShape* shape_sol;

    unsigned int nb_box = 0;
    unsigned int nb_sphere = 0;
    unsigned int nb_bullet = 0;
    //Qt3DRender::QMesh * mesh;
};

#endif // GLAREA_H
