#include "calibrationmodel.h"

CalibrationModel::CalibrationModel() : QAbstractTableModel{} {
}

void CalibrationModel::setVariant(QVariant data) {
  if (data.toMap().contains("data_temp") && data.toMap().contains("data_correct")) {
    m_data.clear();

    auto getVectorByName = [&data](QString const &name) -> QVector<double> {
      QVector<double> ret;
      for (auto item : data.toMap()[name].toList()) {
        ret.push_back(item.toDouble());
      }
      return ret;
    };

    auto temps = getVectorByName("data_temp");
    auto cors  = getVectorByName("data_correct");

    for (int i = 0; i < qMin(temps.size(), cors.size()); ++i) {
      m_data.push_back({temps[i], cors[i]});
    }
  }
}

QVariant CalibrationModel::getVariant() const {
  return 1;
}

int CalibrationModel::rowCount(QModelIndex const &parent) const {
  return m_data.size();
}

int CalibrationModel::columnCount(QModelIndex const &parent) const {
  return 2;
}

QVariant CalibrationModel::data(QModelIndex const &index, int role) const {
  auto const &point = m_data[index.row()];
  return double{(0 == index.column()) ? point.first : point.second};
}

QVariant CalibrationModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    switch (section) {
      case 0:
        return QString("Температура, С");
      case 1:
        return QString("Корректировка, С");
    }
  }
  return QVariant();
}

bool CalibrationModel::insertRows(int row, int count, QModelIndex const &parent) {
  return false;
}

bool CalibrationModel::removeRows(int row, int count, QModelIndex const &parent) {
  return false;
}

void CalibrationModel::sort(int column, Qt::SortOrder order) {
}
