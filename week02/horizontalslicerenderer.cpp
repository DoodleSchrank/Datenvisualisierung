#include "horizontalslicerenderer.h"

#include "horizontalslicerenderer.h"
#include <iostream>
#include <QOpenGLFunctions>
#include <QOpenGLContext>


HorizontalSliceRenderer::HorizontalSliceRenderer()
    : vertexBuffer(QOpenGLBuffer::VertexBuffer)
{
    initOpenGLShaders();
    initHorizontalSliceGeometry();
}


HorizontalSliceRenderer::~HorizontalSliceRenderer()
{
    vertexBuffer.destroy();
    texture_->destroy();
}


void HorizontalSliceRenderer::drawHorizontalSlice(QMatrix4x4 mvpMatrix,  QImage img)
{
    shaderProgram.link();
    // Tell OpenGL to use the shader program of this class.
    shaderProgram.bind();

    // Bind the vertex array object that links to the bounding box vertices.
    vertexArrayObject.bind();

    // Set the model-view-projection matrix as a uniform value.
    shaderProgram.setUniformValue("mvpMatrix", mvpMatrix);

    // Issue OpenGL draw commands.
    texture_->setData(img);
    const int textureUnit = 0; // select a texture unit
    texture_->bind(textureUnit);
    shaderProgram.setUniformValue("colorMappingTexture", textureUnit);


    // Release objects until next render cycle.
    vertexArrayObject.release();
    shaderProgram.release();
}


void HorizontalSliceRenderer::initOpenGLShaders()
{
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                               "slice_vshader.glsl")) {
        std::cout << "Vertex shader error:\n"
                  << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }

    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                               "slice_fshader.glsl")) {
        std::cout << "Fragment shader error:\n"
                  << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }

    if (!shaderProgram.link()) {
        std::cout << "Shader link error:\n"
                  << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }
}


void HorizontalSliceRenderer::initHorizontalSliceGeometry()
{
    // Vertices of a unit cube that represents the bounding box.
    const unsigned int numVertices = 6;
    float unitCubeVertices[numVertices][3] = {
        {0, 0, 0}, {1, 0, 0}, {1, 1, 0},
        {0, 0, 0}, {1, 1, 0}, {0, 1, 0}};

    // Create vertex buffer and upload vertex data to buffer.
    vertexBuffer.create(); // make sure to destroy in destructor!
    vertexBuffer.bind();
    vertexBuffer.allocate(unitCubeVertices, numVertices * 3 * sizeof(float));
    vertexBuffer.release();

    // Store the information OpenGL needs for rendering the vertex buffer
    // in a "vertex array object". This can easily be bound to the OpenGL
    // pipeline during rendering.
    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObject);
    if (vertexArrayObject.isCreated()) {
        vertexBuffer.bind();
        shaderProgram.setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3, 3*sizeof(float));
        shaderProgram.enableAttributeArray("vertexPosition");
        vertexBuffer.release();
    }

    texture_ = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture_->create();
    texture_->setWrapMode(QOpenGLTexture::ClampToEdge);
}
