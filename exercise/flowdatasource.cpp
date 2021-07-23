#include "flowdatasource.h"

void gen_tornado(int xs, int ys, int zs, int time, float *tornado) {
    float x, y, z;
    int ix, iy, iz;
    float r, xc, yc, scale, temp, z0;
    float r2 = 8;
    float SMALL = 0.00000000001;
    float xdelta = 1.0 / (xs - 1.0);
    float ydelta = 1.0 / (ys - 1.0);
    float zdelta = 1.0 / (zs - 1.0);

#pragma omp parallel for private(z, xc, yc, r, r2, y, x, temp, scale, z0)
    for (iz = 0; iz < zs; iz++) {
        z = iz * zdelta;                        // map z to 0->1
        xc = 0.5 + 0.1 * std::sin(0.04 * time + 10.0 * z);   // For each z-slice, determine the spiral circle.
        yc = 0.5 + 0.1 * cos(0.03 * time + 3.0 * z);    //    (xc,yc) determine the center of the circle.
        r = 0.1 + 0.4 * z * z + 0.1 * z * std::sin(8.0 * z); //  The radius also changes at each z-slice.
        r2 = 0.2 + 0.1 * z;                           //    r is the center radius, r2 is for damping
        for (iy = 0; iy < ys; iy++) {
            y = iy * ydelta;
#pragma omp simd
            for (ix = 0; ix < xs; ix++) {
                x = ix * xdelta;
                temp = std::sqrt((y - yc) * (y - yc) + (x - xc) * (x - xc));
                scale = std::fabs(r - temp);

                if (scale > r2)
                    scale = 0.8 - scale;
                else
                    scale = 1.0;
                z0 = 0.1 * (0.1 - temp * z);
                if (z0 < 0.0) z0 = 0.0;
                temp = std::sqrt(temp * temp + z0 * z0);
                scale = (r + r2 - temp) * scale / (temp + SMALL);
                scale = scale / (1 + z);
                tornado[3 * (ix + ys * (iy + zs * iz))] = scale * (y - yc) + 0.1 * (x - xc);
                tornado[3 * (ix + ys * (iy + zs * iz)) + 1] = scale * -(x - xc) + 0.1 * (y - yc);
                tornado[3 * (ix + ys * (iy + zs * iz)) + 2] = scale * z0;
            }
        }
    }
}

FlowDataSource::FlowDataSource(int dim) {
    cartesianDataGrid_ = new float[dim * dim * dim * 3];
    slice_ = new float[dim * dim];
    dimensions_ = dim;
}

FlowDataSource::~FlowDataSource() {
    delete[] cartesianDataGrid_;
    delete[] slice_;
}

float* FlowDataSource::generateTornadoAtTime(int time) {
    gen_tornado(dimensions_, dimensions_, dimensions_, time, cartesianDataGrid_);
    return cartesianDataGrid_;
}


void FlowDataSource::printValuesOfHorizontalSlice(int iz) {
    for(int y = 0; y < dimensions_; y++) {
        for(int x = 0; x < dimensions_; x++) {
            printf("(%f %f %f)\t", getDataValue(x, y, iz, 0), getDataValue(x, y, iz, 1), getDataValue(x, y, iz, 2));
        }
        printf("\n");
    }
}

float *FlowDataSource::getZSlice(int slice) {
    // copy 2d slice of x-wind out of the 4d array
#pragma omp parallel for collapse(2)
    for (int y = 0; y < dimensions_; y++)
        for (int x = 0; x < dimensions_; x++)
            slice_[y * dimensions_ + x] = getDataValue(x, y, slice, 0);

    return slice_;
}


float FlowDataSource::getDataValue(int ix, int iy, int iz, int ic) {
    return cartesianDataGrid_[3 * (ix + dimensions_ * (iy + dimensions_ * iz)) + ic];
}


float FlowDataSource::getSpeed(int ix, int iy, int iz) {
    return sqrt(pow(getDataValue(ix, iy, iz, 0), 2) +
                pow(getDataValue(ix, iy, iz, 1), 2) +
                pow(getDataValue(ix, iy, iz, 2), 2));
}
