#include "horizontalslicerenderer.h"

HorizontalSliceRenderer::HorizontalSliceRenderer(float* data, int dim)
    : vertexBuffer(QOpenGLBuffer::VertexBuffer), mapper_(data, dim), data_(data), dim_(dim) {
    initOpenGLShaders();
    initHorizontalSliceGeometry();
}


HorizontalSliceRenderer::~HorizontalSliceRenderer() {
    vertexBuffer.destroy();
    texture_->destroy();
}


void HorizontalSliceRenderer::drawHorizontalSlice(QMatrix4x4 mvpMatrix) {
    shaderProgram.link();
    shaderProgram.bind();

    vertexArrayObject.bind();
    shaderProgram.setUniformValue("mvpMatrix", mvpMatrix);

    // load image and bind to texture unit
    texture_->setData(mapper_.GetImage());
    const int textureUnit = 0;
    texture_->bind(textureUnit);

    // setup shader and draw 2d plane
    shaderProgram.setUniformValue("colorMappingTexture", textureUnit);
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

    vertexArrayObject.release();
    shaderProgram.release();
}


void HorizontalSliceRenderer::initOpenGLShaders() {
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


void HorizontalSliceRenderer::initHorizontalSliceGeometry() {
    // vertices
    const unsigned int numVertices = 6;
    float unitCubeVertices[numVertices][3] = {
        {0, 0, 0},
        {1, 0, 0},
        {1, 1, 0},
        {0, 0, 0},
        {1, 1, 0},
        {0, 1, 0}};

    vertexBuffer.create();

    // load vertices into buffer
    vertexBuffer.bind();
    vertexBuffer.allocate(unitCubeVertices, numVertices * 3 * sizeof(float));
    vertexBuffer.release();

    // Store the information OpenGL needs for rendering the vertex buffer
    // in a "vertex array object". This can easily be bound to the OpenGL
    // pipeline during rendering.
    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObject);
    if (vertexArrayObject.isCreated()) {
        vertexBuffer.bind();
        shaderProgram.setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3, 3 * sizeof(float));
        shaderProgram.enableAttributeArray("vertexPosition");
        vertexBuffer.release();
    }

    // texture settings
    texture_ = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture_->create();
    texture_->setWrapMode(QOpenGLTexture::ClampToEdge);
}

void HorizontalSliceRenderer::updateHorizontalSliceGeometry(int slice) {
    // generate new image
    mapper_.MapSliceToImage();

    // vertices
    const unsigned int numVertices = 6;
    // calculate zaxis position
    float zaxis = static_cast<float>(slice) / (dim_ - 1);
    float unitCubeVertices[numVertices][3] = {
            {0, 0, zaxis},
            {1, 0, zaxis},
            {1, 1, zaxis},
            {0, 0, zaxis},
            {1, 1, zaxis},
            {0, 1, zaxis}};

    // load vertices into buffer
    vertexBuffer.bind();
    vertexBuffer.allocate(unitCubeVertices, numVertices * 3 * sizeof(float));
    vertexBuffer.release();

    // reset texture
    texture_->destroy();
    texture_ = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture_->create();
    texture_->setWrapMode(QOpenGLTexture::ClampToEdge);
}
