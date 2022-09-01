#ifndef UTILS_H
#define UTILS_H

#include <QVariantMap>
#include <QVector>
namespace utils {
double correction(double value, QVector<double> x, QVector<double> deltas);

double correction(double, QVariantMap map);
} // namespace utils

#endif // UTILS_H
