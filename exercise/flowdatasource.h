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
     */
    void generateTornadoAtTime(int time);

    /**
     * @brief getData
     * @return float* - pointer to the tornado data
     */
    float *getData();

    /**
     * @brief getSpeeds
     * @return float* - pointer to the 2d speed slice
     */
    float *getSpeeds();

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
    float *speeds_;
    float *slice_;
    int dimensions_;
};

#endif // FLOWDATASOURCE_H
