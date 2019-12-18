#include "boxbullet.h"

BoxBullet::BoxBullet(QVector3D _position,float _size,bool multicolor)
{
    this->position = _position;
    this->size = _size;

    GLfloat vertices[] = {
           -this->size,-this->size,-this->size,
           +this->size,-this->size,-this->size,
           -this->size,-this->size,+this->size,
           +this->size,-this->size,+this->size,
           -this->size,+this->size,-this->size,
           +this->size,+this->size,-this->size,
           -this->size,+this->size,+this->size,
           +this->size,+this->size,+this->size,

           -this->size,-this->size,-this->size,
           -this->size,-this->size,+this->size,
           -this->size,+this->size,-this->size,
           -this->size,+this->size,+this->size,
           +this->size,-this->size,-this->size,
           +this->size,-this->size,+this->size,
           +this->size,+this->size,-this->size,
           +this->size,+this->size,+this->size,

           -this->size,-this->size,-this->size,
           -this->size,+this->size,-this->size,
           +this->size,-this->size,-this->size,
           +this->size,+this->size,-this->size,
           -this->size,-this->size,+this->size,
           -this->size,+this->size,+this->size,
           +this->size,-this->size,+this->size,
           +this->size,+this->size,+this->size
        };
    GLfloat texCoords[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f
        };


    for (int i = 0; i < 24; ++i) {
        // coordonnées sommets
        for (int j = 0; j < 3; j++)
            vertData.append(vertices[i*3+j]);
        // coordonnées texture
        for (int j = 0; j < 2; j++)
            vertData.append(texCoords[i*2+j]);
    }
    if(multicolor)
    {
        float r = (rand()%255)/255.0;
        float g = (rand()%255)/255.0;
        float b = (rand()%255)/255.0;
        float a = 0.7;
        this->color = QVector4D(r,g,b,a);
    }
    else
    {
        float r = 0.0;
        float g = 0.0;
        float b = 0.0;
        float a = 1.0;
        this->color = QVector4D(r,g,b,a);
    }



}

void BoxBullet::display(QOpenGLShaderProgram *buffer)
{
    vbo.create();
    this->vbo.bind();
    this->vbo.allocate(vertData.constData(), vertData.count() * int(sizeof(GLfloat)));

    matrix.setToIdentity();
    matrix.translate(this->position);
    //--------Code de Lucas Moragues pour la rotation des poissons


    buffer->setUniformValue("modelMatrix", matrix);
    buffer->setUniformValue("particleSize", this->size);
    //QColor c(255,0,0,255);
    //GLfloat color[] = {255.0f, 0.0f, 0.0f, 200.0f};
    //QVector4D color(1.0f/this->position[1],1.0f/this->position[1],1.0f/this->position[1],1.0f);



    //glBindVertexArray(color);
    //buffer->setUniformValue("uv",color);
    buffer->setUniformValue("color",this->color);

    //buffer->setUniformValue("uv",color);
    buffer->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    buffer->setAttributeBuffer("in_uv", GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
    buffer->enableAttributeArray("in_position");
    buffer->enableAttributeArray("in_uv");


    //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_BLEND);
    glDrawArrays(GL_LINES, 0, 24);
    //glDisable(GL_BLEND);
}

void BoxBullet::anime(float dt, QVector3D newpos)
{
    //dynamicsWorld->stepSimulation(dt, 10);
    this->position = newpos;
}
