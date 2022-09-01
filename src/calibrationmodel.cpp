#include "calibrationmodel.h"

CalibrationModel::CalibrationModel() : QAbstractTableModel{} {
}

void CalibrationModel::setVariant(QVariant data) {
  if (data.toMap().contains("data_temp") && data.toMap().contains("data_correct")) {
    m_data.clear();

    auto getVectorByName = [&data](QString const &name) -> QVector<double> {
      QVector<double> ret;
      for (auto const item : data.toMap()[name].toList()) {
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
  QList<QVariant> temps;
  QList<QVariant> cors;
  for (auto const item : m_data) {
    temps.push_back(item.first);
    cors.push_back(item.second);
  }
  QVariantMap ret{};
  ret["data_temp"]    = temps;
  ret["data_correct"] = cors;
  return ret;
}

int CalibrationModel::rowCount(QModelIndex const &parent) const {
  return m_data.size();
}

int CalibrationModel::columnCount(QModelIndex const &parent) const {
  return 2;
}

QVariant CalibrationModel::data(QModelIndex const &index, int role) const {
  if ((Qt::DisplayRole == role) || (Qt::EditRole == role)) {
    auto const &point = m_data[index.row()];
    return double{(0 == index.column()) ? point.first : point.second};
  }
  return {};
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
  return {};
}

bool CalibrationModel::insertRows(int row, int count, QModelIndex const &parent) {
  return false;
}

bool CalibrationModel::removeRows(int row, int count, QModelIndex const &parent) {
  return false;
}

void CalibrationModel::sort(int column, Qt::SortOrder order) {
}

Qt::ItemFlags CalibrationModel::flags(QModelIndex const &index) const {
  if (!index.isValid()) {
    return Qt::ItemIsEnabled;
  }
  return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool CalibrationModel::setData(QModelIndex const &index, QVariant const &value, int role) {
  auto row = index.row();
  auto col = index.column();
  if ((row >= m_data.size()) || (col > 1)) {
    return false;
  }

  if (0 == col) {
    m_data[row].first = value.toDouble();
  } else {
    m_data[row].second = value.toDouble();
  }
  return true;
}
