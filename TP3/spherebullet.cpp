#include "spherebullet.h"

SphereBullet::SphereBullet(QVector3D _position,float _size)
{
    this->position = _position;
    this->size = _size;

    //sphere
    // Création des spheres
    //https://stackoverflow.com/questions/7687148/drawing-sphere-in-opengl-without-using-glusphere
    QVector<GLfloat> vertices_sphere;
    QVector<GLfloat> vertices_sphere_color;
    float rayon = this->size;
    float rouge;
    float vert;
    float bleue;
    float pi = static_cast<float>(M_PI);
    for(int i = 0; i <= 40; i++)
    {
        float lat0 = pi * (-0.5 + static_cast<double>(i - 1) / 40.0f);
        float z0  = sin(lat0);
        float zr0 = cos(lat0);

        float lat1 = pi * (-0.5 + i / 40.0f);
        float z1 = sin(lat1);
        float zr1 = cos(lat1);

        for(int j = 0; j <= 40; j++)
        {
            float lng = static_cast<double>(2) * pi * static_cast<double>(j - 1) / 40.0f;
            float x = cos(lng);
            float y = sin(lng);

            vertices_sphere.push_back(x * zr0 * rayon); //X
            vertices_sphere.push_back(y * zr0 * rayon); //Y
            vertices_sphere.push_back(z0 * rayon);      //Z
            vertices_sphere_color.push_back(rouge);
            vertices_sphere_color.push_back(vert);
            vertices_sphere_color.push_back(bleue);

            vertices_sphere.push_back(x * zr1 * rayon); //X
            vertices_sphere.push_back(y * zr1 * rayon); //Y
            vertices_sphere.push_back(z1 * rayon);      //Z
            vertices_sphere_color.push_back(rouge);
            vertices_sphere_color.push_back(vert);
            vertices_sphere_color.push_back(bleue);
        }
    }
    GLfloat texCoords[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f
        };

    for (int i = 0; i < 41*41*2; ++i) {
        // coordonnées sommets
        for (int j = 0; j < 3; j++)
            vertData.append(vertices_sphere.at(i*3+j));
        // coordonnées texture
        for (int j = 0; j < 2; j++)
        {
            vertData.append(1);
        }

            //vertData.append(texCoords[i*2+j]);
    }

    float r = (rand()%255)/255.0;
    float g = (rand()%255)/255.0;
    float b = (rand()%255)/255.0;

    float a = 0.7;
    this->color = QVector4D(r,g,b,a);
}

void SphereBullet::display(QOpenGLShaderProgram *buffer)
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

    //QVector4D color(r,g,b,a);
    //qDebug()<<"COULEUR ROUGE "<<r;

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
    glDrawArrays(GL_QUAD_STRIP, 0, 41*41*2);
    //glDisable(GL_BLEND);
}

void SphereBullet::anime(float dt, QVector3D newpos)
{
    this->position = newpos;
}
