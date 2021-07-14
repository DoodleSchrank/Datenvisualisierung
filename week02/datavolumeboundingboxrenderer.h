#ifndef DATAVOLUMEBOUNDINGBOXRENDERER_H
#define DATAVOLUMEBOUNDINGBOXRENDERER_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>


class DataVolumeBoundingBoxRenderer
{
public:
    DataVolumeBoundingBoxRenderer();
    virtual ~DataVolumeBoundingBoxRenderer();

    // Draw the bounding box to the current OpenGL viewport.
    void drawBoundingBox(QMatrix4x4 mvpMatrix);

private:
    void initOpenGLShaders();
    void initBoundingBoxGeometry();

    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;
};

#endif // DATAVOLUMEBOUNDINGBOXRENDERER_H
