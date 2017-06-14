#include <QApplication>

#include "GestureSampleCollector.hpp"
#include "HandDetector.hpp"
#include "SampleCollector.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto h = new HandDetector;
    auto s = new SampleCollector;
    GestureSampleCollector gsc(h,s);
    gsc.run();
    return a.exec();
}