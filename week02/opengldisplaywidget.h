#ifndef OPENGLDISPLAYWIDGET_H
#define OPENGLDISPLAYWIDGET_H

#include <QOpenGLWidget>
#include "datavolumeboundingboxrenderer.h"
#include "flowdatasource.h"
#include "horizontalslicetoimagemapper.h"
#include "horizontalslicerenderer.h"


class OpenGLDisplayWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit OpenGLDisplayWidget(QWidget *parent = nullptr);
    ~OpenGLDisplayWidget() override;

    QString openGLString();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;

    void keyPressEvent(QKeyEvent *e) override;

private:
    // VIEW PROJECTION:
    // ================

    // Matrices and related variables that control vertex transformation
    // from world space to OpenGL view space.
    QMatrix4x4 projectionMatrix;
    QMatrix4x4 mvpMatrix;
    QVector2D lastMousePosition;
    QVector2D rotationAngles;
    float distanceToCamera;

    // Recompute the mode-view-projection matrix from current rotation angles,
    // distance to camera, viewport geometry.
    void updateMVPMatrix();


    // VIISUALIZATION PIPELINE:
    // ========================

    DataVolumeBoundingBoxRenderer *bboxRenderer;
    // ....
    FlowDataSource *slice_;
    HorizontalSliceToImageMapper *slicemapper_;
    HorizontalSliceRenderer *slicerenderer_;

    // Initialize the pipeline (create instances of data source, mapping,
    // rendering etc. modules and connect them).
    void initVisualizationPipeline();
};

#endif // OPENGLDISPLAYWIDGET_H
