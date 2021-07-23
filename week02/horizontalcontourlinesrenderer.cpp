#include "horizontalcontourlinesrenderer.h"

#include <iostream>
#include <QOpenGLFunctions>
#include <QOpenGLContext>


HorizontalContourLinesRenderer::HorizontalContourLinesRenderer(float *data, int dim, std::vector<float> c): vertexBuffer(QOpenGLBuffer::VertexBuffer), mapper_(data, dim, c), data_(data), dim_(dim) {
    initOpenGLShaders();
    initHorizontalSliceGeometry();
}


HorizontalContourLinesRenderer::~HorizontalContourLinesRenderer() {
    vertexBuffer.destroy();
}


void HorizontalContourLinesRenderer::drawContourLines(QMatrix4x4 mvpMatrix) {
    shaderProgram.link();
    shaderProgram.bind();
    vertexArrayObject.bind();
    shaderProgram.setUniformValue("mvpMatrix", mvpMatrix);

    // Draw GL_Lines (vector pairs)
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glLineWidth(2);
    f->glDrawArrays(GL_LINES, 0, vectorlength_);

    vertexArrayObject.release();
    shaderProgram.release();
}


void HorizontalContourLinesRenderer::initOpenGLShaders() {
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                               "contour_vshader.glsl")) {
        std::cout << "Vertex shader error:\n"
                  << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }

    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                               "contour_fshader.glsl")) {
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


void HorizontalContourLinesRenderer::initHorizontalSliceGeometry() {
    vertexBuffer.create();
    // get contour vectors and copy to buffer
    updateContourGeometry(0);

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

void HorizontalContourLinesRenderer::updateContourGeometry(int slice) {
    // get contour vectors
    QVector<QVector3D> vectors = mapper_.mapSliceToContourLineSegments();
    vectorlength_ = vectors.length();

    // set zaxis
    float zaxis = slice / (dim_ - 1.) + 0.001;
    for (int i = 0; i < vectorlength_; i++) {
        vectors[i].setZ(zaxis);
    }

    // copy vectors to buffer
    vertexBuffer.bind();
    vertexBuffer.allocate(vectors.data(), vectorlength_ * 3 * sizeof(float));
    vertexBuffer.release();
}

