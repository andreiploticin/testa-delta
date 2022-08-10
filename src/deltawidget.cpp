#include "deltawidget.h"

DeltaWidget::DeltaWidget(QWidget *parent) : QWidget{parent} {

  auto groupBoxLayout = new QHBoxLayout(this);
  auto groupBox = new QGroupBox(this);
  groupBoxLayout->addWidget(groupBox);

  m_mainLay = new QFormLayout(groupBox);
  m_curTemp = new QLineEdit("0.0", this);
  m_setTempEdit = new QLineEdit("0.0", this);

  m_mainLay->addRow("Текущая температура", m_curTemp);
  m_mainLay->addRow("Уставка", m_setTempEdit);
}
