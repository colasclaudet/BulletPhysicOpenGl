#ifndef OBJECTBULLET_H
#define OBJECTBULLET_H
#include "boxbullet.h"
#include "spherebullet.h"

class ObjectBullet
{
public:
    ObjectBullet(unsigned int nb_obj, unsigned int nb_sphere, unsigned int nb_bullet);
    void anime(float dt);
    void display(QOpenGLShaderProgram * buffer);
    void create_ground();
private:
    int dimMur_x = 2;
    int dimMur_y = 10;
    int dimMur_z = 30;
    QList<BoxBullet> l_mur;
    QList<BoxBullet> l_box;
    QList<SphereBullet> l_sphere;
    QList<SphereBullet> l_bullet;
    unsigned int nbbullet;
    unsigned int nbobj;
    unsigned int nbsphere;
    btDiscreteDynamicsWorld* dynamicsWorld;
    btScalar mat[16];
    btDefaultMotionState* myMotionState;

    btDefaultCollisionConfiguration* collisionConfiguration ;

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    btCollisionDispatcher* dispatcher ;

    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    btBroadphaseInterface* overlappingPairCache ;

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    btSequentialImpulseConstraintSolver* solver ;



    ///-----initialization_end-----

    //keep track of the shapes, we release memory at exit.
    //make sure to re-use collision shapes among rigid bodies whenever possible!
    btAlignedObjectArray<btCollisionShape*> collisionShapes;
};

#endif // OBJECTBULLET_H
