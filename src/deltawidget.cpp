#include "deltawidget.h"

#include <QFormLayout>

DeltaWidget::DeltaWidget(QWidget *parent, QString const &label, double setValueDef, double corValueDef)
    : QGroupBox{label, parent} {
  auto mainLay = new QFormLayout(this);

  m_curTemp     = new QLineEdit("0.0", this);
  m_setTempEdit = new QLineEdit(QString::number(setValueDef, 'f', 1), this);
  m_correction  = new CorrectionWidget(QString::number(corValueDef, 'f', 1), this);

  mainLay->addRow("Текущая температура", m_curTemp);
  mainLay->addRow("Уставка", m_setTempEdit);
  mainLay->addRow("Корректировка", m_correction);
}

QVariantMap DeltaWidget::getInfo() const {
  QVariantMap ret{};
  ret["correction"] = m_correction->text().toDouble();
  ret["set"]        = m_setTempEdit->text().toDouble();
  ret["label"]      = this->title();
  return ret;
}
