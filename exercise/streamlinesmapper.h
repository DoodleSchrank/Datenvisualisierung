#ifndef STREAMLINESMAPPER_H
#define STREAMLINESMAPPER_H

#include <QVector>
#include <QVector3D>
#include <vector>
#include <math.h>

/**
 * @brief The StreamLinesMapper class - traces seedpoints in a 3d vector grid
 */
class StreamLinesMapper {
public:
    /**
     * @brief StreamLinesMapper
     * @param data - 3d vector grid
     * @param dim - dimensionality of the grid
     * @param seedpoints - starting points of tracing
     * @param stepsize - stepsize required for EulerIntegration
     */
    StreamLinesMapper(float* data, int dim, QVector<QVector3D> seedpoints, float stepsize);

    /**
     * @brief ComputeStreamLines - Computes the streamlines with current data
     */
    void ComputeStreamLines();

    /**
     * @brief GetStreamLines - returns previously calculated streamlines
     * @return QVector<QVector<QVector3D>> - Vector of streamlines
     */
    QVector<QVector<QVector3D>> GetStreamLines();

private:
    QVector3D EulerIntegration(QVector3D vec, float stepsize);

    QVector3D RungeKuttaIntegration(QVector3D vec);

    QVector3D getVec(int x, int y, int z);

    bool isInCube(QVector3D point);

    QVector3D interpolateWindStream(QVector3D point);

    QVector<QVector<QVector3D>> streamlines_;
    QVector<QVector3D> seedpoints_;
    float* data_;
    int dim_;
    float stepsize_;
};

#endif // STREAMLINESMAPPER_H
