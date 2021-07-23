#ifndef HORIZONTALSLICETOIMAGEMAPPER_H
#define HORIZONTALSLICETOIMAGEMAPPER_H

#include <algorithm>
#include <QImage>

/**
 * @brief The HorizontalSliceToImageMapper class - maps given 2d slice to a QImage
 */
class HorizontalSliceToImageMapper {
public:
    /**
     * @brief HorizontalSliceToImageMapper
     * @param data - pointer to the 2d slice
     * @param dim - dimensionality of data
     */
    HorizontalSliceToImageMapper(float* data, int dim);

    /**
     * @brief MapSliceToImage - maps current data to an image
     */
    void MapSliceToImage();

    /**
     * @brief GetImage
     * @return QImage - returns the generated image
     */
    QImage GetImage();


private:
    void normalize_data(float* normalized_data);

    void colorize_data(float* normalized_data);

    float* data_;
    int dim_;
    QImage image_;
};

#endif // HORIZONTALSLICETOIMAGEMAPPER_H
