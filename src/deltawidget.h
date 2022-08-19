#ifndef DELTAWIDGET_H
#define DELTAWIDGET_H

#include <QCheckBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>

#include "correctionwidget.h"

class DeltaWidget : public QGroupBox {
  Q_OBJECT
public:
  explicit DeltaWidget(QWidget       *parent = nullptr,
                       QString const &label = "",
                       double         setValueDef = 1000,
                       double         corValueDef = 0.0);
  void setCurTemp(double value) {
    if (nullptr != m_curTemp) {
      m_curTemp->setText(QString::number(value, 'f', 2));
    }
  }
  double getSetValue() const {
    return m_setTempEdit->text().toDouble() + m_correction->text().toDouble();
  }

private:
  QLineEdit        *m_setTempEdit{nullptr};
  QLineEdit        *m_curTemp{nullptr};
  CorrectionWidget *m_correction{nullptr};
};

#endif // DELTAWIDGET_H
