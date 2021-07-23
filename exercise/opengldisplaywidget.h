#ifndef OPENGLDISPLAYWIDGET_H
#define OPENGLDISPLAYWIDGET_H

#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <iostream>
#include <QOpenGLWidget>
#include <QTimer>
#include "flowdatasource.h"
#include "datavolumeboundingboxrenderer.h"
#include "horizontalslicerenderer.h"
#include "horizontalcontourlinesrenderer.h"
#include "streamlinesrenderer.h"
#include "math.h"


class OpenGLDisplayWidget : public QOpenGLWidget {
    Q_OBJECT

public:
    explicit OpenGLDisplayWidget(QWidget *parent = nullptr);

    ~OpenGLDisplayWidget() override;

    QString openGLString();

public slots:
    void GenerateNextTimeStep();
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

    // Initialize the pipeline (create instances of data source, mapping,
    // rendering etc. modules and connect them).
    void initVisualizationPipeline();

    FlowDataSource *data_;
    DataVolumeBoundingBoxRenderer *bboxRenderer;
    HorizontalSliceRenderer *slicerenderer_;
    HorizontalContourLinesRenderer *contourrenderer_;
    StreamLinesRenderer *streamrenderer_;

    QVector<QVector3D> seedPoints_;
    QVector<QVector<QVector3D>> streamlines_;
    int dim_ = 16;
    int slice_ = 0;
    int time_ = 0;
    QTimer *timer_;


};

#endif // OPENGLDISPLAYWIDGET_H
