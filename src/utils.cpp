#include "utils.h"
namespace utils {
double correction(double value, QVector<double> x, QVector<double> deltas) {
  if ((x.size() != deltas.size()) || (0 == x.size())) {
    return 0;
  }
  auto iter = std::upper_bound(x.cbegin(), x.cend(), value);
  int  i    = std::distance(x.cbegin(), iter);

  if (0 == i) {
    return deltas.first();
  } else if (x.size() == i) {
    return deltas.last();
  } else {
    return deltas[i - 1] + (value - x[i - 1]) * (deltas[i] - deltas[i - 1]) / (x[i] - x[i - 1]);
  }
}

double correction(double value, QVariantMap map) {
  auto tempVar = map["data_temp"].toList().toVector();
  auto corsVar = map["data_correct"].toList().toVector();
  if (tempVar.isEmpty() || corsVar.isEmpty()) {
    return 0;
  }

  QVector<double> temp{};
  QVector<double> cors{};

  for (int i{0}; i < tempVar.size(); ++i) {
    temp.push_back(tempVar[i].toDouble());
    cors.push_back(corsVar[i].toDouble());
  }
  return correction(value, temp, cors);
}
} // namespace utils
