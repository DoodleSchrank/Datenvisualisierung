#include "streamlinesmapper.h"

StreamLinesMapper::StreamLinesMapper(float* data, int dim, QVector<QVector3D> seedpoints, float stepsize): seedpoints_(seedpoints), data_(data), dim_(dim), stepsize_(stepsize) {}

#define Runge

void StreamLinesMapper::ComputeStreamLines() {
    streamlines_.clear();
    QVector<QVector3D> temp;
    QVector3D point;
    int iter;

    /*
     * trail all seed points until they leave the cube
     * trails will be saved into a vector to be drawn out later
     */
#pragma omp parallel for private(iter, temp, point) schedule(dynamic)
    for (int i = 0; i < seedpoints_.length(); i++) {
        iter = 0;
        point = seedpoints_.at(i);
        while (isInCube(point) && iter < 100000) {
            temp.append(point);
#ifdef Euler
            point += EulerIntegration(point, stepsize_);
#else
            point += RungeKuttaIntegration(point);
#endif
            iter++;
        }
#pragma omp critical
            streamlines_.append(temp);
        temp.clear();
    }
}

QVector3D StreamLinesMapper::EulerIntegration(QVector3D vec, float stepsize) {
    return stepsize * interpolateWindStream(vec);
}

QVector3D StreamLinesMapper::RungeKuttaIntegration(QVector3D vec) {
    float h = 1.;
    // order 4 and stepsize 1 was assumed
    QVector3D k1 = h * EulerIntegration(vec, stepsize_);
    QVector3D k2 = h * EulerIntegration(vec + k1/2., stepsize_ + h / 2.);
    //while (isnan(k2.x()))
    //   k2 = h * EulerIntegration(vec + k1 / 2., stepsize_ + h / 2.);
    QVector3D k3 = h * EulerIntegration(vec + k2/2., stepsize_ + h / 2.);
    QVector3D k4 = h * EulerIntegration(vec + k3, stepsize_ + h);

    vec = (k1 + 2. * k2 + 2. * k3 + k4) / 6.;
    return vec;
}



QVector3D StreamLinesMapper::getVec(int x, int y, int z) {
    return QVector3D(data_[3 * (x + dim_ * (y + dim_ * z)) + 0],
                     data_[3 * (x + dim_ * (y + dim_ * z)) + 1],
                     data_[3 * (x + dim_ * (y + dim_ * z)) + 2]);
}

bool StreamLinesMapper::isInCube(QVector3D point) {
    return (point.x() > 0) && (point.x() < 1.0) && (point.y() > 0) && (point.y() < 1.0) && (point.z() > 0) && (point.z() < 1.0);
}

QVector3D StreamLinesMapper::interpolateWindStream(QVector3D point) {
    // de-normalize to [0;15]
    point *= (dim_ - 1);
    // get upper and lower coordinates (xl < point.x() < xu)
    int xl = static_cast<int>(point.x());
    int yl = static_cast<int>(point.y());
    int zl = static_cast<int>(point.z());
    int xu = static_cast<int>(ceil(point.x()));
    int yu = static_cast<int>(ceil(point.y()));
    int zu = static_cast<int>(ceil(point.z()));

    // retreive surrounding vectors
    QVector3D v0 = getVec(xl, yl, zl);
    QVector3D v01 = getVec(xl, yl, zu);
    QVector3D v010 = getVec(xl, yu, zl);
    QVector3D v011 = getVec(xl, yu, zu);
    QVector3D v10 = getVec(xu, yl, zl);
    QVector3D v101 = getVec(xu, yl, zu);
    QVector3D v110 = getVec(xu, yu, zl);
    QVector3D v1 = getVec(xu, yu, zu);

    // calculate distances per dimension
    float xd = (point.x() - xl);
    float yd = (point.y() - yl);
    float zd = (point.z() - zl);

    // interpolate x-axis
    v0 = v0 * (1 - xd) + v10 * xd;
    v01 = v01 * (1 - xd) + v101 * xd;
    v10 = v010 * (1 - xd) + v110 * xd;
    v1 = v011 * (1 - xd) + v1 * xd;

    // interpolate y-axis
    v0 = v0 * (1 - yd) + v10 * yd;
    v1 = v01 * (1 - yd) + v1 * yd;

    // interpolate z-axis
    v0 = v0 * (1 - zd) + v1 * zd;

    // normalize back to [0;1] and return
    return v0 / (dim_ - 1);
}

QVector<QVector<QVector3D>> StreamLinesMapper::GetStreamLines() {
    return streamlines_;
}
