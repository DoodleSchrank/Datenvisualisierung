#include "datavolumeboundingboxrenderer.h"

DataVolumeBoundingBoxRenderer::DataVolumeBoundingBoxRenderer()
        : vertexBuffer(QOpenGLBuffer::VertexBuffer) {
    initOpenGLShaders();
    initBoundingBoxGeometry();
}


DataVolumeBoundingBoxRenderer::~DataVolumeBoundingBoxRenderer() {
    vertexBuffer.destroy();
}


void DataVolumeBoundingBoxRenderer::drawBoundingBox(QMatrix4x4 mvpMatrix) {
    shaderProgram.link();
    shaderProgram.bind();
    vertexArrayObject.bind();
    shaderProgram.setUniformValue("mvpMatrix", mvpMatrix);

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glLineWidth(2);
    f->glDrawArrays(GL_LINE_STRIP, 0, 16);

    vertexArrayObject.release();
    shaderProgram.release();
}


void DataVolumeBoundingBoxRenderer::initOpenGLShaders() {
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                               "lines_vshader.glsl")) {
        std::cout << "Vertex shader error:\n"
                  << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }

    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                               "lines_fshader.glsl")) {
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


void DataVolumeBoundingBoxRenderer::initBoundingBoxGeometry() {
    // Vertices of a unit cube that represents the bounding box.
    const unsigned int numVertices = 16;
    float unitCubeVertices[numVertices][3] = {
            {0, 0, 0},
            {1, 0, 0},
            {1, 1, 0},
            {0, 1, 0},
            {0, 0, 0},
            {0, 0, 1},
            {1, 0, 1},
            {1, 0, 0},
            {1, 0, 1},
            {1, 1, 1},
            {1, 1, 0},
            {1, 1, 1},
            {0, 1, 1},
            {0, 1, 0},
            {0, 1, 1},
            {0, 0, 1}};

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
        shaderProgram.setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3, 3 * sizeof(float));
        shaderProgram.enableAttributeArray("vertexPosition");
        vertexBuffer.release();
    }
}
