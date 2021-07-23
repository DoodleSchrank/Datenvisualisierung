#include "streamlinesrenderer.h"

StreamLinesRenderer::StreamLinesRenderer(float* data, int dim, QVector<QVector3D> seedpoints, float stepsize)
    : vertexBuffer(QOpenGLBuffer::VertexBuffer), mapper_(data, dim, seedpoints, stepsize) {
    initOpenGLShaders();
    initStreamLineGeometry();
}


StreamLinesRenderer::~StreamLinesRenderer() {
    vertexBuffer.destroy();
}

void StreamLinesRenderer::drawStreamLines(QMatrix4x4 mvpMatrix) {
    shaderProgram.link();

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glLineWidth(2);

    // draw GL_LINE_STRIP for each streamline
    for (auto streamline : mapper_.GetStreamLines()) {
        shaderProgram.bind();
        shaderProgram.setUniformValue("mvpMatrix", mvpMatrix);
        vertexBuffer.bind();
        vertexBuffer.allocate(streamline.data(), streamline.length() * 3 * sizeof(float));
        vertexBuffer.release();

        vertexArrayObject.bind();

        f->glDrawArrays(GL_LINE_STRIP, 0, streamline.length());

        vertexArrayObject.release();
        shaderProgram.release();
    }
}


void StreamLinesRenderer::initOpenGLShaders() {
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                               "stream_vshader.glsl")) {
        std::cout << "Vertex shader error:\n"
                  << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }

    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                               "stream_fshader.glsl")) {
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

void StreamLinesRenderer::initStreamLineGeometry() {
    vertexBuffer.create();

    // let mapper compute the new streamlines so they're ready when drawing
    updateStreamLineGeometry();

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

void StreamLinesRenderer::updateStreamLineGeometry() {
    mapper_.ComputeStreamLines();
}


