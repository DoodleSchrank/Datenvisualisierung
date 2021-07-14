#ifndef HORIZONTALSLICETOIMAGEMAPPER_H
#define HORIZONTALSLICETOIMAGEMAPPER_H
#include <algorithm>
#include <QImage>

class HorizontalSliceToImageMapper
{
public:
    HorizontalSliceToImageMapper();
    void MapSliceToImage(float* data, QImage *out, int dim);
private:
    void normalize_data(float* data, int dim);
    void colorize_data(float* data, QImage *out, int dim);
};

#endif // HORIZONTALSLICETOIMAGEMAPPER_H
