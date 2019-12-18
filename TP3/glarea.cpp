// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "glarea.h"
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>




GLArea::GLArea(QWidget *parent) :
    QOpenGLWidget(parent)
{
    QSurfaceFormat sf;
    sf.setDepthBufferSize(24);
    sf.setSamples(16);
    setFormat(sf);

    setEnabled(true);                   // événements clavier et souris
    setFocusPolicy(Qt::StrongFocus);    // accepte focus
    setFocus();                         // donne le focus

    timer = new QTimer(this);
    timer->setInterval(50);           // msec
    connect (timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer->start();
    elapsedTimer.start();
}


GLArea::~GLArea()
{
    delete timer;

    // Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
    // dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
    makeCurrent();
    tearGLObjects();
    doneCurrent();
}


void GLArea::initializeGL()
{
    bl = new BoxBullet(QVector3D(0,20,0),1,true);
    obl = new ObjectBullet(this->nb_box,this->nb_sphere,this->nb_bullet);
    initializeOpenGLFunctions();
    glClearColor(0.5f,0.5f,1.0f,1.0f);
    glEnable(GL_DEPTH_TEST);

    makeGLObjects();
    //load3DMod("name");

    // shader du sol
    program_sol = new QOpenGLShaderProgram(this);
    program_sol->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vsh");
    program_sol->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.fsh");
    if (! program_sol->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_sol->log();
    }
    program_sol->setUniformValue("texture", 0);

    // shader de billboard
    program_particule = new QOpenGLShaderProgram(this);
    program_particule->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/billboard.vsh");
    program_particule->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/billboard.fsh");
    if (! program_particule->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_particule->log();
    }
    program_particule->setUniformValue("texture", 0);

    program_box = new QOpenGLShaderProgram(this);
    program_box->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/billboard.vsh");
    program_box->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/billboard.fsh");
    if (! program_box->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_box->log();
    }
    program_box->setUniformValue("texture", 0);


}

void GLArea::myBox(float x, float y, float z)
{
    glPushMatrix();
    glScalef(x,y,z);
    glBegin (GL_QUADS);
    //face 1
    glNormal3i(-1, 1,-1);
    glVertex3i(-1, 1,-1); glVertex3i( 1, 1,-1);
    glVertex3i( 1,-1,-1); glVertex3i(-1,-1,-1);
    //face 2
    glNormal3i(-1,-1,-1);
    glVertex3i(-1,-1,-1); glVertex3i( 1,-1,-1);
    glVertex3i( 1,-1, 1); glVertex3i(-1,-1, 1);
    // face 3
    glNormal3i( 1,-1, 1);
    glVertex3i( 1,-1, 1); glVertex3i( 1,-1,-1);
    glVertex3i( 1, 1,-1); glVertex3i( 1, 1, 1);
    //face 4
    glNormal3i( 1, 1,-1);
    glVertex3i( 1, 1,-1); glVertex3i(-1, 1,-1);
    glVertex3i(-1, 1, 1); glVertex3i( 1, 1, 1);
    //face 5
    glNormal3i(-1, 1, 1);
    glVertex3i(-1, 1, 1); glVertex3i(-1, 1,-1);
    glVertex3i(-1,-1,-1); glVertex3i(-1,-1, 1);
    //face 6
    glNormal3i( 1,-1, 1);
    glVertex3i( 1,-1, 1); glVertex3i( 1, 1, 1);
    glVertex3i(-1, 1, 1); glVertex3i(-1,-1, 1);
    glEnd();
    glPopMatrix();
}

void GLArea::makeGLObjects()
{
    // Création du sol

    float tailleSol = 20.0f;

    GLfloat vertices_sol[] = {
       -tailleSol, 0.0f,-tailleSol,
       -tailleSol, 0.0f, tailleSol,
        tailleSol, 0.0f, tailleSol,
        tailleSol, 0.0f, tailleSol,
        tailleSol, 0.0f,-tailleSol,
       -tailleSol, 0.0f,-tailleSol
    };

    GLfloat texCoords_sol[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f
        };

    QVector<GLfloat> vertData_sol;
    for (int i = 0; i < 6; ++i) {
        // coordonnées sommets
        for (int j = 0; j < 3; j++)
            vertData_sol.append(vertices_sol[i*3+j]);
        // coordonnées texture
        for (int j = 0; j < 2; j++)
            vertData_sol.append(texCoords_sol[i*2+j]);
    }

    vbo_sol.create();
    vbo_sol.bind();
    vbo_sol.allocate(vertData_sol.constData(), vertData_sol.count() * int(sizeof(GLfloat)));


    // Création d'une particule de fumée
    GLfloat vertices_particule[] = {
       -0.5f, 0.5f, 0.0f,
       -0.5f,-0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
       -0.5f,-0.5f, 0.0f,
        0.5f,-0.5f, 0.0f,
        0.5f, 0.5f, 0.0f
    };

    GLfloat texCoords_particule[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
        };

    QVector<GLfloat> vertData_particule;
    for (int i = 0; i < 6; ++i) {
        // coordonnées sommets
        for (int j = 0; j < 3; j++)
            vertData_particule.append(vertices_particule[i*3+j]);
        // coordonnées texture
        for (int j = 0; j < 2; j++)
            vertData_particule.append(texCoords_particule[i*2+j]);
    }

    vbo_particule.create();
    vbo_particule.bind();
    vbo_particule.allocate(vertData_particule.constData(), vertData_particule.count() * int(sizeof(GLfloat)));

    /*myCollisionConfiguration = new btDefaultCollisionConfiguration();
    myDispatcher = new btCollisionDispatcher(myCollisionConfiguration);
    myBroadphase = new btDbvtBroadphase();
    mySequentialImpulseConstraintSolver = new btSequentialImpulseConstraintSolver;

    myWorld = new btDiscreteDynamicsWorld(myDispatcher,myBroadphase,mySequentialImpulseConstraintSolver,myCollisionConfiguration);
    myWorld->setGravity(btVector3(0,-9.4f,0));

    //maybe in draw
    shape = new btBoxShape(btVector3(1,1,1));
    myTransform.setIdentity();
    myTransform.setOrigin(btVector3(0,10,0));

    btVector3 localInertia(0,0,0);
    btScalar mass = 0.5f;

    //if ( mass )
    shape->calculateLocalInertia( mass, localInertia );

    myMotionState = new btDefaultMotionState(myTransform);

    btRigidBody::btRigidBodyConstructionInfo myBoxRigidBodyConstructionInfo( mass, myMotionState, shape, localInertia );

    body = new btRigidBody(myBoxRigidBodyConstructionInfo);

    myWorld->addRigidBody(body);

    //pour le sol
    // Forme en tant que boite
    shape_sol = new btBoxShape(btVector3(10,1,10));

    myTransform.setIdentity();

    // Position du sol
    myTransform.setOrigin( btVector3(0,0,0) );
    btVector3 localInertiaSol(0,0,0);

    mass = 0; // Le fait que le poids de cet objet soit zéro le rends statique

    myMotionState_Sol = new btDefaultMotionState(myTransform);

    btRigidBody::btRigidBodyConstructionInfo sol_info( mass, myMotionState_Sol, shape_sol, localInertiaSol );

    body_sol = new btRigidBody(sol_info);

    // On ajoute le sol dans le monde Bullet
    myWorld->addRigidBody(body_sol);*/

    // Création de textures
    QImage image_sol(":/textures/solplanche.jpg");
    if (image_sol.isNull())
        qDebug() << "load image ground.jpg failed";
    textures[0] = new QOpenGLTexture(image_sol);

    QImage image_particule(":/textures/ball-6x6.png");
    if (image_particule.isNull())
        qDebug() << "load image puff.png failed";
    textures[1] = new QOpenGLTexture(image_particule);
}


void GLArea::tearGLObjects()
{
    vbo_sol.destroy();
    vbo_particule.destroy();
    for (int i = 0; i < 2; i++)
        delete textures[i];
}

void GLArea::setNb_bullet(unsigned int value)
{
    nb_bullet = value;
}

void GLArea::update_bullet()
{
    obl = new ObjectBullet(this->nb_box,this->nb_sphere,this->nb_bullet);
}

void GLArea::setNb_sphere(unsigned int value)
{
    nb_sphere = value;
}

void GLArea::setNb_box(unsigned int value)
{
    nb_box = value;
}


void GLArea::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    windowRatio = float(w) / h;
}



void GLArea::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*if (myWorld)
    {
        myWorld->stepSimulation(static_cast<float>(0.001));
    }

    myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(matrix);
    //myMotionState->m_graphicsWorldTrans.setFromOpenGLMatrix(matrix);
    glPushMatrix();
    glMultMatrixf( matrix );
    glPopMatrix();*/
    // grâce à glPushMatrix (ouverture) et glPopMatrix (fermeture) on retiens les transformations seulement appliquées à cette boite
    /*glPushMatrix();
    // On applique les transformations à la boite
    glMultMatrixf(matrix);
    // Ensuite, on affiche la boite

    myBox(1,1,1);
    glPopMatrix();
    myBox(10,1,10);*/
    //qDebug()<<myWorld->getNumCollisionObjects();


    // Matrice de projection
    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(45.0f, windowRatio, 1.0f, 1000.0f);

    // Matrice de vue (caméra)
    QMatrix4x4 viewMatrix;
    viewMatrix.translate(xPos, yPos, zPos);
    viewMatrix.rotate(xRot, 1, 0, 0);
    viewMatrix.rotate(yRot, 0, 1, 0);
    viewMatrix.rotate(zRot, 0, 0, 1);

    // Affichage du sol
    vbo_sol.bind();
    program_sol->bind(); // active le shader program du sol

    QMatrix4x4 modelMatrixSol;
    modelMatrixSol.translate(0.0f, 0.0f, 0.0f);
    program_sol->setUniformValue("projectionMatrix", projectionMatrix);
    program_sol->setUniformValue("viewMatrix", viewMatrix);
    program_sol->setUniformValue("modelMatrix", modelMatrixSol);

    program_sol->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    program_sol->setAttributeBuffer("in_uv", GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
    program_sol->enableAttributeArray("in_position");
    program_sol->enableAttributeArray("in_uv");

    textures[0]->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    textures[0]->release();

    program_sol->disableAttributeArray("in_position");
    program_sol->disableAttributeArray("in_uv");
    program_sol->release();


    // Affichage d'une particule
    vbo_particule.bind();
        program_particule->bind(); // active le shader program des particules

        QMatrix4x4 modelMatrixParticule(matrix[0],matrix[1],matrix[2],matrix[3],
                matrix[4],matrix[5],matrix[6],matrix[7],
                matrix[8],matrix[9],matrix[10],matrix[11],
                matrix[12],matrix[13],matrix[14],matrix[15]);
        //modelMatrixParticule.translate(0,0.3,0);
        //modelMatrixParticule.translate(0.0f, 10.0f, 0.0f);

        /*int k =0;
        for (int i =0;i<4;i++)
        {
            modelMatrixParticule.row(i).setX(matrix[k+i]);
            k++;
            modelMatrixParticule.row(i).setY(matrix[k+i]);
            k++;
            modelMatrixParticule.row(i).setZ(matrix[k+i]);
            k++;
            modelMatrixParticule.row(i).setW(matrix[k+i]);

        }*/
        glDepthMask(GL_FALSE);
        program_particule->setUniformValue("projectionMatrix", projectionMatrix);
        program_particule->setUniformValue("viewMatrix", viewMatrix);
        program_particule->setUniformValue("modelMatrix", modelMatrixParticule);
        program_particule->setUniformValue("particleSize", 1.0f);

        program_particule->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
        program_particule->setAttributeBuffer("in_uv", GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
        program_particule->enableAttributeArray("in_position");
        program_particule->enableAttributeArray("in_uv");

        textures[1]->bind();
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        //glEnable(GL_BLEND);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //glDisable(GL_BLEND);
        textures[1]->release();

        program_particule->disableAttributeArray("in_position");
        program_particule->disableAttributeArray("in_uv");
        program_particule->release();

        //box fall
        program_box->bind();
        program_box->setUniformValue("projectionMatrix", projectionMatrix);
        program_box->setUniformValue("viewMatrix", viewMatrix);
        //textures[2]->bind();

        //bl->display(program_box);
        obl->display(program_box);
        //textures[2]->release();
        program_box->disableAttributeArray("in_position");
        program_box->disableAttributeArray("in_uv");
        program_box->release();
        glDepthMask(GL_TRUE);
}


void GLArea::keyPressEvent(QKeyEvent *ev)
{
    float da = 1.0f;

    switch(ev->key()) {
        case Qt::Key_A :
            xRot -= da;
            update();
            break;

        case Qt::Key_Q :
            xRot += da;
            update();
            break;

        case Qt::Key_Z :
            yRot -= da;
            update();
            break;

        case Qt::Key_S :
            yRot += da;
            update();
            break;

        case Qt::Key_E :
            zRot -= da;
            update();
            break;

        case Qt::Key_D :
            zRot += da;
            update();
            break;
        case Qt::Key_T :
            initializeGL();
            update();
            break;
        case Qt::Key_Up:
            zPos +=0.5;
            update();
            break;
        case Qt::Key_Down:
            zPos -=0.5;
            update();
            break;
    }
}


void GLArea::keyReleaseEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();
}


void GLArea::mousePressEvent(QMouseEvent *ev)
{
    lastPos = ev->pos();
}


void GLArea::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}


void GLArea::mouseMoveEvent(QMouseEvent *ev)
{
    int dx = ev->x() - lastPos.x();
    int dy = ev->y() - lastPos.y();

    if (ev->buttons() & Qt::LeftButton) {
        xRot += dy;
        yRot += dx;
        update();
    } else if (ev->buttons() & Qt::RightButton) {
        xPos += dx/10.0f;
        yPos -= dy/10.0f;
        update();
    } else if (ev->buttons() & Qt::MidButton) {
        xPos += dx/10.0f;
        zPos += dy;
        update();
    }

    lastPos = ev->pos();
}

void GLArea::wheelEvent(QWheelEvent *event)
{
    if(event->delta()>0)
    {
        zPos+=1.5;
    }
    else
    {
        zPos-=1.5;
    }


    event->accept();
}


void GLArea::onTimeout()
{
    static qint64 old_chrono = elapsedTimer.elapsed(); // static : initialisation la première fois et conserve la dernière valeur
    qint64 chrono = elapsedTimer.elapsed();
    dt = (chrono - old_chrono) / 1000.0f;
    old_chrono = chrono;
    obl->anime(dt);


    update();
}
