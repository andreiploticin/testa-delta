#ifndef CALIBRATIONWIDGET_H
#define CALIBRATIONWIDGET_H

#include <QComboBox>
#include <QGroupBox>
#include <QTableView>

#include "src/calibrationmodel.h"

class CalibrationWidget : public QGroupBox {
  Q_OBJECT
public:
  explicit CalibrationWidget(QWidget *parent = nullptr);
  void         setData(QVariantList data);
  QVariantList getData() const;
  ~CalibrationWidget() override;

private:
  void                        setModelToView(int index);
  QTableView                 *m_tableView{nullptr};
  QComboBox                  *m_selectBox{nullptr};
  QVector<CalibrationModel *> m_models;
  bool                        m_dataReady{false};
};

#endif // CALIBRATIONWIDGET_H
