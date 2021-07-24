#include "opengldisplaywidget.h"

OpenGLDisplayWidget::OpenGLDisplayWidget(QWidget *parent)
        : QOpenGLWidget(parent),
          distanceToCamera(-8.0) {
    setFocusPolicy(Qt::StrongFocus);

    // setup timer for animation
    timer_ = new QTimer(this);
    QObject::connect(timer_, SIGNAL(timeout()), this, SLOT(GenerateNextTimeStep()));
    timer_->start(500); // time in ms
}


OpenGLDisplayWidget::~OpenGLDisplayWidget() {
    // Clean up visualization pipeline.
    delete bboxRenderer;
    delete slicerenderer_;
    delete contourrenderer_;
    delete streamrenderer_;
    delete data_;
}


QString OpenGLDisplayWidget::openGLString() {
    QString profileStr;
    switch (format().profile()) {
        case QSurfaceFormat::NoProfile:
            profileStr = "no profile";
            break;
        case QSurfaceFormat::CompatibilityProfile:
            profileStr = "compatibility profile";
            break;
        case QSurfaceFormat::CoreProfile:
            profileStr = "core profile";
            break;
    }

    return QString("%1.%2 (%3)").arg(format().majorVersion())
            .arg(format().minorVersion()).arg(profileStr);
}


void OpenGLDisplayWidget::initializeGL() {
    // Query and display some information about the used OpenGL context.
    std::cout << "Initializing OpenGLDisplayWidget with OpenGL version "
              << openGLString().toStdString() << ".\n" << std::flush;

    // Set the backgound color of the OpenGL display enable the depth buffer.
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(0, 0, 0, 1);
    f->glEnable(GL_DEPTH_TEST);

    // Our own initialization of the visualization pipeline.
    initVisualizationPipeline();
}


void OpenGLDisplayWidget::resizeGL(int w, int h) {
    // Calculate aspect ratio of the current viewport.
    float aspectRatio = float(w) / std::max(1, h);

    // Reset projection and set new perspective projection.
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45.0, aspectRatio, 0.05, 25.0);
    //projectionMatrix.ortho(-5, 5, -5, 5, -5, 5000);

    // Update model-view-projection matrix with new projection.
    updateMVPMatrix();
}


void OpenGLDisplayWidget::paintGL() {
    // Clear color and depth buffer.
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Call renderer modules.
    bboxRenderer->drawBoundingBox(mvpMatrix);
    slicerenderer_->drawHorizontalSlice(mvpMatrix);
    contourrenderer_->drawContourLines(mvpMatrix);
    streamrenderer_->drawStreamLines(mvpMatrix);
}


void OpenGLDisplayWidget::mousePressEvent(QMouseEvent *e) {
    // Save the current position of the mouse pointer for subsequent use
    // in mouseMoveEvent().
    lastMousePosition = QVector2D(e->localPos());
}


void OpenGLDisplayWidget::mouseMoveEvent(QMouseEvent *e) {
    // If the user holds the left mouse button while moving the mouse, update
    // the rotation angles that specify from which side the grid visualization
    // is viewed.
    if (e->buttons() & Qt::LeftButton) {
        // Vector that points from the last stored position of the mouse
        // pointer to the current position.
        QVector2D mousePosDifference = QVector2D(e->localPos()) - lastMousePosition;

        // Update rotation angles in x and y direction. The factor "10" is an
        // arbitrary scaling constant that controls the sensitivity of the
        // mouse.
        rotationAngles.setX(
                fmod(rotationAngles.x() + mousePosDifference.x() / 10.,
                     360.));
        rotationAngles.setY(
                fmod(rotationAngles.y() + mousePosDifference.y() / 10.,
                     360.));

        // Store current position of mouse pointer for next call to this method.
        lastMousePosition = QVector2D(e->localPos());

        // Update model-view-projection matrix with new rotation angles.
        updateMVPMatrix();

        // Redraw OpenGL.
        update();
    }
}


void OpenGLDisplayWidget::wheelEvent(QWheelEvent *e) {
    // Update distance of the camera to the rendered visualization. The factor
    // "500" is arbitrary and controls that sensitivity of the mouse.
    distanceToCamera += e->delta() / 500.;

    // Update model-view-projection matrix with new distance to camera.
    updateMVPMatrix();

    // Redraw OpenGL.
    update();
}


void OpenGLDisplayWidget::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Up) {
        // increase slice and overwrite slice data with new slice
        slice_++;
        if (slice_ == dim_) slice_ = dim_ - 1;
        else {
            data_->getZSlice(slice_);
            slicerenderer_->updateHorizontalSliceGeometry(slice_);
            contourrenderer_->updateContourGeometry(slice_);
        }
    } else if (e->key() == Qt::Key_Down) {
        // decrease slice and overwrite slice data with new slice
        slice_--;
        if (slice_ == -1) slice_ = 0;
        else {
            data_->getZSlice(slice_);
            slicerenderer_->updateHorizontalSliceGeometry(slice_);
            contourrenderer_->updateContourGeometry(slice_);
        }
    } else {
        return;
    }

    // Redraw OpenGL.
    update();
}


void OpenGLDisplayWidget::updateMVPMatrix() {
    // Calculate a simple model view transformation from rotation angles
    // and distance to camera.
    // NOTE: Read from bottom to top.

    QMatrix4x4 mvMatrix;
    mvMatrix.translate(0.0, 0.0, distanceToCamera);
    mvMatrix.rotate(rotationAngles.y(), QVector3D(1.0, 0.0, 0.0));
    mvMatrix.rotate(rotationAngles.x(), QVector3D(0.0, 1.0, 0.0));
    mvMatrix.translate(-1.0, -1.0, -1.0);
    mvMatrix.scale(2.0);

    mvpMatrix = projectionMatrix * mvMatrix;
}


void OpenGLDisplayWidget::initVisualizationPipeline() {
    float seedpointStepsize = 0.01;

    // set seed points
    //for (float x = 0; x < 1; x += seedpointStepsize) {
    //for (float y = 0; y < 1; y += seedpointStepsize) {
    for (float z = 0; z < 1; z += seedpointStepsize) {
        QVector3D temp(0.5, 0.5, z);
        seedPoints_.append(temp);
        //}
    }
    //}

    // generate data
    data_ = new FlowDataSource(dim_);
    float *data = data_->generateTornadoAtTime(time_);
    float *slicedata = data_->getZSlice(0);

    // initialize render modules
    bboxRenderer = new DataVolumeBoundingBoxRenderer();
    slicerenderer_ = new HorizontalSliceRenderer(slicedata, dim_);

    // iso values for the marching squares algorithm to differentiate between
    std::vector<float> isovalues = {-0.1, 0, 0.1};
    contourrenderer_ = new HorizontalContourLinesRenderer(slicedata, dim_, isovalues);
    streamrenderer_ = new StreamLinesRenderer(data, dim_, seedPoints_, 0.2);
}

void OpenGLDisplayWidget::GenerateNextTimeStep() {
    // update data
    time_++;
    data_->generateTornadoAtTime(time_);
    data_->getZSlice(slice_);

    // update geometry
    slicerenderer_->updateHorizontalSliceGeometry(slice_);
    contourrenderer_->updateContourGeometry(slice_);
    streamrenderer_->updateStreamLineGeometry();
    update();
}
