#include <QCoreApplication>
#include <iostream>
#include "flowdatasource.h"

int xs = 16;
int ys = 16;
int zs = 16;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    auto x = FlowDataSource(xs, ys, zs);
    x.createData(0);
    //x.printWindBetrag();
    x.printValuesOfHorizontalSlice(10);

    float min, max, speed;

    for(int l = 0; l < 100; l++) {
        x.createData(l);
        min = 1.0;
        max = -1.0;
        for(int i = 0; i < xs; i++) {
            for(int j = 0; j < ys; j++) {
                for(int k = 0; k < zs; k++) {
                    speed = x.getSpeed(i,j,k);
                    if(speed > max) max = speed;
                    if(speed < min) min = speed;
                }
            }
        }
        //std::cout << "time: " << l << " min: " << min << " max: " << max << std::endl;
    }

    //return a.exec();
    return 1;
}
