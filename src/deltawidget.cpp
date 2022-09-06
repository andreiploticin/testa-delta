#include "deltawidget.h"

DeltaWidget::DeltaWidget(QWidget *parent, QString const &label, double setValueDef, double corValueDef)
    : QGroupBox{label, parent} {
  m_mainLay = new QFormLayout(this);

  m_curTemp     = new QLineEdit("0.0", this);
  m_setTempEdit = new QLineEdit(QString::number(setValueDef, 'f', 1), this);
  m_correction  = new CorrectionWidget(QString::number(corValueDef, 'f', 1), this);

  m_mainLay->addRow("Текущая температура", m_curTemp);
  m_mainLay->addRow("Уставка", m_setTempEdit);
  m_mainLay->addRow("Корректировка", m_correction);

  connect(m_setTempEdit, &QLineEdit::textChanged, this, [this](QString const &text) {
    if ((nullptr != m_forAllCheck) && m_forAllCheck->isChecked()) {
      emit setValueChanged(text.toDouble());
    }
  });
}

QVariantMap DeltaWidget::getInfo() const {
  QVariantMap ret{};
  ret["correction"] = m_correction->text().toDouble();
  ret["set"]        = m_setTempEdit->text().toDouble();
  ret["label"]      = this->title();
  return ret;
}

void DeltaWidget::enableMaster() {
  if (nullptr == m_forAllCheck) {
    m_forAllCheck = new QCheckBox(this);
    m_mainLay->insertRow(2, "Для всех", m_forAllCheck);
    connect(m_forAllCheck, &QCheckBox::toggled, this, [this](bool value) {
      emit masterSelected(value ? getSetValueNoCorrection() : -10000.0);
    });
  }
}
