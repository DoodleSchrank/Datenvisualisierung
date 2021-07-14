#ifndef FLOWDATASOURCE_H
#define FLOWDATASOURCE_H

#include <array>
#include <cmath>
#include <iostream>

typedef enum dimension {
    x,
    y,
    z
} dimension;

class FlowDataSource
{
public:
    FlowDataSource(int dim);
    ~FlowDataSource();
    void generateTime(int time);
    float* getData();
    float* getSpeeds();
    float* getSlice(dimension dim, int slice);

private:
    float getDataValue(int ix, int iy, int iz, int ic);
    float getSpeed(int ix, int iy, int iz);
    float* cartesianDataGrid_;
    float* speeds_;
    float* slice_;
    int dimensions_;
};

#endif // FLOWDATASOURCE_H
