#ifndef STREAMLINESRENDERER_H
#define STREAMLINESRENDERER_H

#include "streamlinesmapper.h"

#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <iostream>

/**
 * @brief The StreamLinesRenderer class - creates mapper that generates streamlines based on current data then visualizes them using qt
 */
class StreamLinesRenderer {
public:
    /**
     * @brief StreamLinesRenderer - initializes streamline geometry and streamlinemapper
     * @param data - input data
     * @param dim - dimensionality of data
     * @param seedpoints - seedpoints from which the streamlines begin
     * @param stepsize - stepsize, needed for the mapper
     */
    StreamLinesRenderer(float* data, int dim, QVector<QVector3D> seedpoints, float stepsize = 0.5);

    virtual ~StreamLinesRenderer();

    /**
     * @brief drawStreamLines - draws streamlines one by one
     * @param mvpMatrix
     */
    void drawStreamLines(QMatrix4x4 mvpMatrix);

    /**
     * @brief updateStreamLineGeometry - makes the mapper generate new streamlines
     */
    void updateStreamLineGeometry();

private:
    void initOpenGLShaders();

    void initStreamLineGeometry();

    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;
    StreamLinesMapper mapper_;
};

#endif // STREAMLINESRENDERER_H
