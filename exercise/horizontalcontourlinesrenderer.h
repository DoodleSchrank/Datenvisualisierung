#ifndef HORIZONTALCONTOURLINESRENDERER_H
#define HORIZONTALCONTOURLINESRENDERER_H

#include "horizontalslicetocontourlinemapper.h"

#include <iostream>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

/**
 * @brief The HorizontalContourLinesRenderer class - remders contourlines of a 2d slice
 */
class HorizontalContourLinesRenderer {
public:
    /**
     * @brief HorizontalContourLinesRenderer
     * @param data - 2d slice
     * @param dim - dimensionality of data
     * @param c - vector of isovalues
     */
    HorizontalContourLinesRenderer(float *data, int dim, std::vector<float> c);

    virtual ~HorizontalContourLinesRenderer();

    /**
     * @brief drawContourLines - draws the contourlines using Qt
     * @param mvpMatrix
     */
    void drawContourLines(QMatrix4x4 mvpMatrix);

    /**
     * @brief updateContourGeometry - updates the z-axis of each contour line
     * @param slice - slice on the z-axis
     */
    void updateContourGeometry(int slice);

private:
    void initOpenGLShaders();

    void initHorizontalSliceGeometry();

    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;
    HorizontalSliceToContourLineMapper mapper_;

    int vectorlength_ = 0;
    float* data_;
    int dim_;
};

#endif // HORIZONTALCONTOURLINESRENDERER_H
