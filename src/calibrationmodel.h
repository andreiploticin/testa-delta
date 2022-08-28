#ifndef CALIBRATIONMODEL_H
#define CALIBRATIONMODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include <QPair>
#include <QVector>

class CalibrationModel : public QAbstractTableModel {
  Q_OBJECT
public:
  explicit CalibrationModel();
  void     setVariant(QVariant data);
  QVariant getVariant() const;

  // QAbstractItemModel interface
  int      rowCount(QModelIndex const &parent) const override;
  int      columnCount(QModelIndex const &parent) const override;
  QVariant data(QModelIndex const &index, int role) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  bool     insertRows(int row, int count, QModelIndex const &parent) override;
  bool     removeRows(int row, int count, QModelIndex const &parent) override;
  void     sort(int column, Qt::SortOrder order) override;

private:
  QVector<QPair<double, double>> m_data;
};

#endif // CALIBRATIONMODEL_H
