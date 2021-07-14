#ifndef FLOWDATASOURCE_H
#define FLOWDATASOURCE_H

#include <array>
#include <cmath>
#include <iostream>

class FlowDataSource
{
public:
    FlowDataSource(int xs, int ys, int zs);
    void createData(int time);
    float getDataValue(int ix, int iy, int iz, int ic);
    void printValuesOfHorizontalSlice(int iz);
    float getSpeed(int ix, int iy, int iz);
    void printWindBetrag();

private:
    float* cartesianDataGrid_;
    float* windBetrag_;
    struct sizes {
        int xs;
        int ys;
        int zs;
    } sizes_;
};

#endif // FLOWDATASOURCE_H
