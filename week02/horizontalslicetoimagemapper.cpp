#include "horizontalslicetoimagemapper.h"


HorizontalSliceToImageMapper::HorizontalSliceToImageMapper()
{

}

void HorizontalSliceToImageMapper::MapSliceToImage(float* data, QImage *out, int dim) {
    float normalized_data[dim*dim];
    for(int i = 0; i < dim*dim; i++)
        normalized_data[i] = data[i];
    normalize_data(normalized_data, dim);
    colorize_data(normalized_data, out, dim);
}

void HorizontalSliceToImageMapper::normalize_data(float* data, int dim) {
    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();
    for(int i = 0; i < dim * dim * 3; i+=3) {
        if(data[i] < min)
            min = data[i];
        if(data[i] > max)
            max = data[i];
    }
    for(int i = 0; i < dim * dim; i++) {
        data[i] = (data[i] - min)/(max - min) * 2 - 1;
    }
}

void HorizontalSliceToImageMapper::colorize_data(float* data, QImage *out, int dim){
    QRgb color;
    for(int i = 0; i < dim * dim; i+=3) {
        color = qRgb(0, 0, 0);
        if(data[i] > 0)
            color = qRgb(data[i] * 255, 0, 0);
        if(data[i] < 0)
            color = qRgb(0, 0, data[i] * -255);
        out->setPixel(i / dim, i % dim, color);
    }
}
