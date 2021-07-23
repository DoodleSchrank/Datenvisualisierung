#ifndef FLOWDATASOURCE_H
#define FLOWDATASOURCE_H

#include <array>
#include <cmath>
#include <iostream>

/**
 * @brief The FlowDataSource class - generates tornados and 2d slices of it
 */
class FlowDataSource {
public:
    /**
     * @brief FlowDataSource
     * @param dim - dimensionality of data
     */
    FlowDataSource(int dim);

    ~FlowDataSource();

    /**
     * @brief generateTornadoAtTime - generates tornado at given timepoint
     * @param time - timepoint
     * @return float* - returns pointer to data
     */
    float *generateTornadoAtTime(int time);

    /**
     * @brief printValuesOfHorizontalSlice - prints out the 3d vectors on a given z
     */
    void printValuesOfHorizontalSlice(int iz);

    /**
     * @brief getZSlice
     * @param slice - slice number
     * @return float* - pointer to the 2d slice
     */
    float *getZSlice(int slice);

private:
    float getDataValue(int ix, int iy, int iz, int ic);

    float getSpeed(int ix, int iy, int iz);

    float *cartesianDataGrid_;
    float *slice_;
    int dimensions_;
};

#endif // FLOWDATASOURCE_H
