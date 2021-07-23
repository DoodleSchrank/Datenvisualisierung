#include "horizontalslicetoimagemapper.h"

HorizontalSliceToImageMapper::HorizontalSliceToImageMapper(float* data, int dim) : data_(data), dim_(dim), image_(dim, dim, QImage::Format::Format_RGB16) {
    MapSliceToImage();
}

void HorizontalSliceToImageMapper::MapSliceToImage() {
    float normalized_data[dim_ * dim_];

    // copy slice so that normalization can happen without changing the original slice
#pragma omp parallel for
    for (int i = 0; i < dim_ * dim_; i++)
        normalized_data[i] = data_[i];

    normalize_data(normalized_data);
    colorize_data(normalized_data);
}

QImage HorizontalSliceToImageMapper::GetImage() {
    return image_;
}

void HorizontalSliceToImageMapper::normalize_data(float* normalized_data) {
    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();

    // find min/max
#pragma omp parallel for reduction(min: min) reduction(max: max)
    for (int i = 0; i < dim_ * dim_; i++) {
        if (normalized_data[i] < min)
            min = normalized_data[i];
        if (normalized_data[i] > max)
            max = normalized_data[i];
    }

    // min-max normalization
#pragma omp parallel for
    for (int i = 0; i < dim_ * dim_; i++) {
        normalized_data[i] = 2 * (normalized_data[i] - min) / (max - min) - 1;
    }


}

void HorizontalSliceToImageMapper::colorize_data(float* normalized_data) {
    QRgb color;

    // set pixels according to slice value
#pragma omp parallel for private(color)
    for (int i = 0; i < dim_ * dim_; i++) {
        color = qRgb(0, 0, 0);
        if (normalized_data[i] > 0.)
            color = qRgb(normalized_data[i] * 255, 0, 0);
        if (normalized_data[i] < 0.)
            color = qRgb(0, 0, normalized_data[i] * -255);
        image_.setPixel(i % dim_, i / dim_, color);
    }
}
