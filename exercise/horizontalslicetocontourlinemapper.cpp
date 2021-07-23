#include "horizontalslicetocontourlinemapper.h"

QVector <QVector3D> marching_squares(float *data, int dim, float c) {
    QVector <QVector3D> vectors;
    int mark[dim][dim];
    int v0, v1, v2, v3;
    float x1, x2, y1, y2;

    // mark cells based on relation to given iso value "c"
#pragma omp parallel for collapse(2)
    for (int y = 0; y < dim; y++) {
        for (int x = 0; x < dim; x++) {
            mark[x][y] = (data[y * dim + x] > c) ? 1 : 0;
        }
    }

    // iterate over every cell and calculate marching squares
#pragma omp parallel for collapse(2) private(v0, v1, v2, v3, x1, x2, y1, y2)
    for (int y = 0; y < dim - 1; y++) {
        for (int x = 0; x < dim - 1; x++) {
            v3 = mark[x][y];
            v2 = mark[x + 1][y];
            v1 = mark[x + 1][y + 1];
            v0 = mark[x][y + 1];

            // no contour between these 4 points
            if (v0 && v1 && v2 && v3) continue;
            else if (!v0 && !v1 && !v2 && !v3) continue;
            x1 = 0;
            x2 = 0;
            y1 = 0;
            y2 = 0;

            // contour found

            // start on middle left
            if (v0 ^ v3) {
                x1 = x;
                y1 = y + 0.5;
            }
            if (v0 ^ v1) {
                // end in bottom middle
                if (y1 == y + 0.5) {
                    x2 = x + 0.5;
                    y2 = y + 1;
                    // start from bottom middle
                } else {
                    x1 = x + 0.5;
                    y1 = y + 1;
                }
            }
            // end at middle right
            if (v1 ^ v2) {
                x2 = x + 1;
                y2 = y + 0.5;
            }
            if (v2 ^ v3) {
                // start in top middle
                if (y2 == y + 0.5) {
                    x1 = x + 0.5;
                    y1 = y;
                    // end at bottom middle
                } else {
                    x2 = x + 0.5;
                    y2 = y;
                }
            }

            // append vector pair/quadruple
#pragma omp critical
            {
                vectors.append(QVector3D(x1 / (dim - 1), y1 / (dim - 1), 0.001));
                vectors.append(QVector3D(x2 / (dim - 1), y2 / (dim - 1), 0.001));

                // if special case
                if ((!v3 && v2 && !v1 && v0) || (v3 && !v2 && v1 && !v0)) {
                    vectors.append(QVector3D(x / (dim - 1), (y + 0.5) / (dim - 1), 0.001));
                    vectors.append(QVector3D((x + 0.5) / (dim - 1), (y + 1) / (dim - 1), 0.001));
                }
            }
        }
    }

    return vectors;
}


HorizontalSliceToContourLineMapper::HorizontalSliceToContourLineMapper(float *data, int dim, std::vector<float> c): data_(data), dim_(dim), c_(c) {}

QVector <QVector3D>
HorizontalSliceToContourLineMapper::mapSliceToContourLineSegments() {
    QVector <QVector3D> vectors;
    /*
     * call marching squares for every iso value given
     * returns a list of coordinate pairs
     */
    for (float iso : c_) vectors.append(marching_squares(data_, dim_, iso));
    return vectors;
}

