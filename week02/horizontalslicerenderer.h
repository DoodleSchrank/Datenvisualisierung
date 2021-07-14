#ifndef HORIZONTALSLICERENDERER_H
#define HORIZONTALSLICERENDERER_H
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QImage>
#include <QOpenGLTexture>


class HorizontalSliceRenderer
{
public:
    HorizontalSliceRenderer();
    virtual ~HorizontalSliceRenderer();

    // Draw the bounding box to the current OpenGL viewport.
    void drawHorizontalSlice(QMatrix4x4 mvpMatrix, QImage img);

private:
    void initOpenGLShaders();
    void initHorizontalSliceGeometry();

    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;
    QOpenGLTexture *texture_;
};

#endif // HORIZONTALSLICERENDERER_H
