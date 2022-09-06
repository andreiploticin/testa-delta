#ifndef DELTAWIDGET_H
#define DELTAWIDGET_H

#include <QCheckBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>

#include <QFormLayout>
#include "correctionwidget.h"

class DeltaWidget : public QGroupBox {
  Q_OBJECT
public:
  explicit DeltaWidget(QWidget       *parent      = nullptr,
                       QString const &label       = "",
                       double         setValueDef = 1000,
                       double         corValueDef = 0.0);
  void setCurTemp(double value) {
    if (nullptr != m_curTemp) {
      m_curTemp->setText(QString::number(value, 'f', 1));
    }
  }
  double getSetValue() const {
    return m_setTempEdit->text().toDouble() + m_correction->text().toDouble();
  }
  QVariantMap getInfo() const;
  void        setSetValue(double value) {
           m_setTempEdit->setText(QString::number(value, 'f', 1));
  }
  void fixSetValue(bool value) {
    m_setTempEdit->setEnabled(!value);
  }
  void enableMaster();

signals:
  void masterSelected(double);
  void setValueChanged(double);

private:
  double getSetValueNoCorrection() const {
    return m_setTempEdit->text().toDouble();
  }

  QFormLayout      *m_mainLay{nullptr};
  QLineEdit        *m_setTempEdit{nullptr};
  QLineEdit        *m_curTemp{nullptr};
  CorrectionWidget *m_correction{nullptr};
  QCheckBox        *m_forAllCheck{nullptr};
};

#endif // DELTAWIDGET_H
