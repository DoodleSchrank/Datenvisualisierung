#ifndef HORIZONTALSLICETOCONTOURLINEMAPPER_H
#define HORIZONTALSLICETOCONTOURLINEMAPPER_H

#include <vector>
#include <QVector>
#include <QVector3D>

/**
 * @brief The HorizontalSliceToContourLineMapper class - calculates contourlines for different given isos on a 2d plane
 */
class HorizontalSliceToContourLineMapper {
public:
    /**
     * @brief HorizontalSliceToContourLineMapper
     * @param data - 2d slice
     * @param dim - dimensionality of data
     * @param c - vector of multiple iso values
     */
    HorizontalSliceToContourLineMapper(float *data, int dim, std::vector<float> c);

    /**
     * @brief mapSliceToContourLineSegments - maps 2d slice to contour lines
     * @return QVector<QVector3D> - pair-wise contour lines
     */
    QVector<QVector3D> mapSliceToContourLineSegments();

private:

    float* data_;
    int dim_;
    std::vector<float> c_;
};

#endif // HORIZONTALSLICETOCONTOURLINEMAPPER_H
