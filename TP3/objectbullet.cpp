#include "objectbullet.h"

ObjectBullet::ObjectBullet(unsigned int nb_obj,unsigned int nb_sphere, unsigned int nb_bullet)
{

    collisionConfiguration = new btDefaultCollisionConfiguration();

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    dispatcher = new btCollisionDispatcher(collisionConfiguration);

    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    overlappingPairCache = new btDbvtBroadphase();

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    solver = new btSequentialImpulseConstraintSolver;

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0, -10, 0));


    create_ground();
    //FOR WALL
    for(int y = 0;y<this->dimMur_y;y++)
        for(int z = -(this->dimMur_z/2); z<(this->dimMur_z/2);z++)
            for(int x = -(this->dimMur_x)/2;x<(this->dimMur_x/2);x++)
            {
                this->l_mur.push_back(BoxBullet(QVector3D(x,y,z), 0.5,false));

                //btCollisionShape* colShape = new btSphereShape(btScalar(1.));
                btCollisionShape* colShape = new btBoxShape(btVector3(0.5,0.5,0.5));
                collisionShapes.push_back(colShape);

                /// Create Dynamic Objects
                btTransform startTransform;
                startTransform.setIdentity();

                btScalar mass(20.f); //3.f de base

                //rigidbody is dynamic if and only if mass is non zero, otherwise static
                bool isDynamic = (mass != 0.f);

                btVector3 localInertia(0, 0, 0);
                if (isDynamic)
                    colShape->calculateLocalInertia(mass, localInertia);

                startTransform.setOrigin(btVector3(x, y, z));

                //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
                myMotionState = new btDefaultMotionState(startTransform);
                btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
                btRigidBody* body = new btRigidBody(rbInfo);

                dynamicsWorld->addRigidBody(body);
                qDebug()<<"brique : "<<x+y+z;
            }
    //FOR BOX
    this->nbobj = nb_obj;
    for(unsigned int i = 0; i< nb_obj;i++)
    {
        //this->l_box.push_back(BoxBullet(QVector3D(rand()%10,(rand()%40) +10,rand()%10),(rand()%3 )+ 1));
        float x = rand()%20 - rand()%20;
        float y = (rand()%500) +50;
        float z = rand()%20 - rand()%20;
        this->l_box.push_back(BoxBullet(QVector3D(x,y,z), 1,true));

        //btCollisionShape* colShape = new btSphereShape(btScalar(1.));
        btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
        collisionShapes.push_back(colShape);

        /// Create Dynamic Objects
        btTransform startTransform;
        startTransform.setIdentity();

        btScalar mass(30.f);

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            colShape->calculateLocalInertia(mass, localInertia);

        startTransform.setOrigin(btVector3(x, y, z));

        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        myMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        dynamicsWorld->addRigidBody(body);

    }
    //FOR SPHERE
    this->nbsphere = nb_sphere;
    for(unsigned int i = 0; i< nb_sphere;i++)
    {
        //this->l_box.push_back(BoxBullet(QVector3D(rand()%10,(rand()%40) +10,rand()%10),(rand()%3 )+ 1));
        float x = rand()%20 - rand()%20;
        float y = (rand()%500) +50;
        float z = rand()%20 - rand()%20;
        this->l_sphere.push_back(SphereBullet(QVector3D(x,y,z), 1.0));

        btCollisionShape* colShape = new btSphereShape(btScalar(1.));
        //btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
        collisionShapes.push_back(colShape);

        /// Create Dynamic Objects
        btTransform startTransform;
        startTransform.setIdentity();

        btScalar mass(10.f);

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            colShape->calculateLocalInertia(mass, localInertia);

        startTransform.setOrigin(btVector3(x, y, z));

        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        myMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        dynamicsWorld->addRigidBody(body);

    }
    //FOR BULLET
    this->nbbullet = nb_bullet;
    for(unsigned int i = 0; i< nb_bullet;i++)
    {
        //this->l_box.push_back(BoxBullet(QVector3D(rand()%10,(rand()%40) +10,rand()%10),(rand()%3 )+ 1));
        float x = (- rand()%50)-20;
        float y = (rand()%10)+1;
        float z = rand()%15 - rand()%15;
        this->l_bullet.push_back(SphereBullet(QVector3D(x,y,z), 1.0));

        btCollisionShape* colShape = new btSphereShape(btScalar(1.));
        //btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
        collisionShapes.push_back(colShape);

        /// Create Dynamic Objects
        btTransform startTransform;
        startTransform.setIdentity();

        btScalar mass(10.f);

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            colShape->calculateLocalInertia(mass, localInertia);

        startTransform.setOrigin(btVector3(x, y, z));

        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        myMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        dynamicsWorld->addRigidBody(body);

    }

}

void ObjectBullet::create_ground()
{
    btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(20.), btScalar(50.), btScalar(20.)));

    collisionShapes.push_back(groundShape);

    //Création du sol
    btTransform groundTransform;
    groundTransform.setIdentity();
    //groundTransform.setOrigin(btVector3(0, -56, 0));
    groundTransform.setOrigin(btVector3(0, -50, 0));
    btScalar mass(0.);

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        groundShape->calculateLocalInertia(mass, localInertia);

    //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
    myMotionState = new btDefaultMotionState(groundTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    //add the body to the dynamics world
    dynamicsWorld->addRigidBody(body);
}
void ObjectBullet::anime(float dt)
{
    dynamicsWorld->stepSimulation(dt, 10);

    int j = dynamicsWorld->getNumCollisionObjects() - 1;
    //BULLET
    QList<SphereBullet>::iterator b;
    b = l_bullet.begin();

    //int l = dynamicsWorld->getNumCollisionObjects() - 1;
    while( b != l_bullet.end() && j>=0)
    {

        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
        btRigidBody* body = btRigidBody::upcast(obj);
        btTransform trans;
        if (body && body->getMotionState())
        {
            body->getMotionState()->getWorldTransform(trans);
            body->setLinearVelocity(btVector3(300,0,0));
            //body->getMotionState()->getWorldTransform(trans);
        }
        else
        {
            trans = obj->getWorldTransform();

        }

        if(j >= 1)
        {
            QVector3D position(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
            //qDebug()<<"world pos object %d = %f,%f,%f\n"<< j << float(trans.getOrigin().getX())<< float(trans.getOrigin().getY())<< float(trans.getOrigin().getZ());
            b->anime(dt,position);
        }

            b++;
            j--;
    }
    //SPHERE

    QList<SphereBullet>::iterator k;
    k = l_sphere.begin();

    //int l = dynamicsWorld->getNumCollisionObjects() - 1;
    while( k != l_sphere.end() && j>=0)
    {

        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
        btRigidBody* body = btRigidBody::upcast(obj);
        btTransform trans;
        if (body && body->getMotionState())
        {
            body->getMotionState()->getWorldTransform(trans);
            //body->setLinearVelocity(btVector3(0.0,-1.0,0.0));
        }
        else
        {
            trans = obj->getWorldTransform();

        }

        if(j >= 1)
        {
            QVector3D position(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
            //qDebug()<<"world pos object %d = %f,%f,%f\n"<< j << float(trans.getOrigin().getX())<< float(trans.getOrigin().getY())<< float(trans.getOrigin().getZ());
            k->anime(dt,position);
        }

            k++;
            j--;
    }

    //BOX
    QList<BoxBullet>::iterator i;
    i = l_box.begin();


    while( i != l_box.end() && j>=0)
    {

        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
        btRigidBody* body = btRigidBody::upcast(obj);
        btTransform trans;
        if (body && body->getMotionState())
        {
            body->getMotionState()->getWorldTransform(trans);
        }
        else
        {
            trans = obj->getWorldTransform();
        }

        if(j >= 1)
        {
            QVector3D position(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
            //qDebug()<<"world pos object %d = %f,%f,%f\n"<< j << float(trans.getOrigin().getX())<< float(trans.getOrigin().getY())<< float(trans.getOrigin().getZ());
            i->anime(dt,position);
        }

            i++;
            j--;
    }
    //MUR
    QList<BoxBullet>::iterator m;
    m = l_mur.begin();

    while( m != l_mur.end() && j>=0)
    {

        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
        btRigidBody* body = btRigidBody::upcast(obj);
        btTransform trans;
        if (body && body->getMotionState())
        {
            body->getMotionState()->getWorldTransform(trans);
        }
        else
        {
            trans = obj->getWorldTransform();
        }

        if(j >= 1)
        {
            QVector3D position(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
            //qDebug()<<"world pos object %d = %f,%f,%f\n"<< j << float(trans.getOrigin().getX())<< float(trans.getOrigin().getY())<< float(trans.getOrigin().getZ());
            m->anime(dt,position);
        }

            m++;
            j--;
    }

}

void ObjectBullet::display(QOpenGLShaderProgram *buffer)
{
    QList<BoxBullet>::iterator m;
    m = l_mur.begin();
    while( m != l_mur.end() )
    {
            m->display(buffer);
            m++;
    }
    QList<BoxBullet>::iterator i;
    i = l_box.begin();
    while( i != l_box.end() )
    {
            i->display(buffer);
            i++;
    }

    QList<SphereBullet>::iterator k;
    k = l_sphere.begin();
    while( k != l_sphere.end() )
    {
            k->display(buffer);
            k++;
    }

    QList<SphereBullet>::iterator b;
    b = l_bullet.begin();
    while( b != l_bullet.end() )
    {
            b->display(buffer);
            b++;
    }

}
