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

    for (iz = 0; iz < zs; iz++) {
        z = iz * zdelta;                        // map z to 0->1
        xc = 0.5 + 0.1 * std::sin(0.04 * time + 10.0 * z);   // For each z-slice, determine the spiral circle.
        yc = 0.5 + 0.1 * cos(0.03 * time + 3.0 * z);    //    (xc,yc) determine the center of the circle.
        r = 0.1 + 0.4 * z * z + 0.1 * z * std::sin(8.0 * z); //  The radius also changes at each z-slice.
        r2 = 0.2 + 0.1 * z;                           //    r is the center radius, r2 is for damping
        for (iy = 0; iy < ys; iy++) {
            y = iy * ydelta;
            for (ix = 0; ix < xs; ix++) {
                x = ix * xdelta;
                temp = std::sqrt((y - yc) * (y - yc) + (x - xc) * (x - xc));
                scale = std::fabs(r - temp);
                /*
                 *  I do not like this next line. It produces a discontinuity
                 *  in the magnitude. Fix it later.
                 *
                 */
                if (scale > r2)
                    scale = 0.8 - scale;
                else
                    scale = 1.0;
                z0 = 0.1 * (0.1 - temp * z);
                if (z0 < 0.0) z0 = 0.0;
                temp = std::sqrt(temp * temp + z0 * z0);
                scale = (r + r2 - temp) * scale / (temp + SMALL);
                scale = scale / (1 + z);
                *tornado++ = scale * (y - yc) + 0.1 * (x - xc);
                *tornado++ = scale * -(x - xc) + 0.1 * (y - yc);
                *tornado++ = scale * z0;
            }
        }
    }
}

FlowDataSource::FlowDataSource(int xs, int ys, int zs) {
    cartesianDataGrid_ = new float[xs*ys*zs*3];
    windBetrag_ = new float[xs*ys*zs];
    sizes_.xs = xs;
    sizes_.ys = ys;
    sizes_.zs = zs;
}

void FlowDataSource::createData(int time) {
    gen_tornado(sizes_.xs, sizes_.ys, sizes_.zs, time, cartesianDataGrid_);

    for(int i = 0; i < sizes_.xs; i++) {
        for(int j = 0; j < sizes_.ys; j++) {
            for(int k = 0; k < sizes_.zs; k++) {
                windBetrag_[i*sizes_.xs*sizes_.ys + j*sizes_.ys + k] = getSpeed(i, j, k);
            }
        }
    }
}


float FlowDataSource::getDataValue(int ix, int iy, int iz, int ic) {
    return cartesianDataGrid_[3 * (ix*sizes_.xs*sizes_.ys + iy*sizes_.zs + iz) + ic];
}

void FlowDataSource::printValuesOfHorizontalSlice(int iz) {
    for(int i = 0; i < sizes_.xs; i++) {
        for(int j = 0; j < sizes_.ys; j++) {
            std::cout << getDataValue(i, j, iz, 0) << " " <<
                         getDataValue(i, j, iz, 1) << " " <<
                         getDataValue(i, j, iz, 2) << std::endl;
        }
    }
}

float FlowDataSource::getSpeed(int ix, int iy, int iz) {
    return sqrt(pow(getDataValue(ix, iy, iz, 0), 2) +
                pow(getDataValue(ix, iy, iz, 1), 2) +
                pow(getDataValue(ix, iy, iz, 2), 2));
}

void FlowDataSource::printWindBetrag() {
    for(int i = 0; i < sizes_.zs; i++) {
        for(int j = 0; j < sizes_.ys; j++) {
            for(int k = 0; k < sizes_.xs; k++) {
                std::cout << " " << windBetrag_[i*sizes_.xs + j*sizes_.ys + k];
            }
            std::cout << std::endl;
        }
        std::cout << "_____________________" << std::endl;
    }
}

