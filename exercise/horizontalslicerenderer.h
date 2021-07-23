#ifndef HORIZONTALSLICERENDERER_H
#define HORIZONTALSLICERENDERER_H

#include "flowdatasource.h"
#include "horizontalslicetoimagemapper.h"

#include <iostream>
#include <QImage>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>

/**
 * @brief The HorizontalSliceRenderer class -  renders the image of a 2d slice
 */
class HorizontalSliceRenderer {
public:
    /**
     * @brief HorizontalSliceRenderer
     * @param data - 2d slice
     * @param dim - dimensionality of data
     */
    HorizontalSliceRenderer(float* data, int dim);

    virtual ~HorizontalSliceRenderer();

    /**
     * @brief drawHorizontalSlice - draws the image using Qt
     * @param mvpMatrix
     */
    void drawHorizontalSlice(QMatrix4x4 mvpMatrix);

    /**
     * @brief updateHorizontalSliceGeometry - updates the z-axis of the geometric plane
     * @param slice - slice to be updated to
     */
    void updateHorizontalSliceGeometry(int slice);

private:
    void initOpenGLShaders();

    void initHorizontalSliceGeometry();

    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;
    QOpenGLTexture *texture_;
    HorizontalSliceToImageMapper mapper_;
    float* data_;
    int dim_;
};

#endif // HORIZONTALSLICERENDERER_H
