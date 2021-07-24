#include "horizontalslicetoimagemapper.h"

HorizontalSliceToImageMapper::HorizontalSliceToImageMapper(float* data, int dim) : data_(data), dim_(dim), image_(dim, dim, QImage::Format::Format_RGB16) {
    MapSliceToImage();
}

void HorizontalSliceToImageMapper::MapSliceToImage() {
    QRgb color;

    // set pixels according to slice value
#pragma omp parallel for private(color)
    for (int i = 0; i < dim_ * dim_; i++) {
        color = qRgb(0, 0, 0);
        if (data_[i] > 0.)
            color = qRgb(data_[i] * 3 * 255, 0, 0);
        if (data_[i] < 0.)
            color = qRgb(0, 0, data_[i] * 3 * -255);
        image_.setPixel(i % dim_, i / dim_, color);
    }
    image_.save("image.png");
}

QImage HorizontalSliceToImageMapper::GetImage() {
    return image_;
}
