#include "pidwidget.h"

#include <QFormLayout>
#include <QMessageBox>
#include <QPushButton>

PidWidget::PidWidget(QString const &title, QWidget *parent) : QGroupBox{title, parent} {
  auto mainLay  = new QVBoxLayout(this);
  auto paramLay = new QFormLayout();

  auto sendBtn = new QPushButton("Загрузить в прибор");
  auto getBtn  = new QPushButton("Получить из прибора");

  m_p      = new QLineEdit(this);
  m_i      = new QLineEdit(this);
  m_d      = new QLineEdit(this);
  m_period = new QLineEdit(this);

  paramLay->addRow("PB Proportional band [0.1~999.9]", m_p);
  paramLay->addRow("Ti Integral time [0~9999]", m_i);
  paramLay->addRow("Td Derivative time [0~9999]", m_d);
  paramLay->addRow("Control cycle, s [0.1~99]", m_period);

  mainLay->addLayout(paramLay);
  mainLay->addWidget(sendBtn);
  mainLay->addWidget(getBtn);

  connect(sendBtn, &QPushButton::clicked, this, [this]() {
    emit sendToController(m_address, getRegisters());
  });
  connect(getBtn, &QPushButton::clicked, this, &PidWidget::getFromController);
}

/**
 * @brief PidWidget::setRegisters
 * @param registers vector starts with address of the first register
 */
void PidWidget::setRegisters(std::vector<uint16_t> registers) {
  if (registers.size() != 6) {
    QMessageBox::information(this, "Внимание", "Полученные данные имеют неверный размер!");
    return;
  }

  if (0x1007 == registers[0]) {
    m_period->setText(QString::number(0.1 * static_cast<double>(registers[1]), 'f', 1));
    // skip 2
    m_p->setText(QString::number(0.1 * static_cast<double>(registers[3]), 'f', 1));
    m_i->setText(QString::number(static_cast<double>(registers[4]), 'f', 1));
    m_d->setText(QString::number(static_cast<double>(registers[5]), 'f', 1));
  } else {
    QMessageBox::information(this, "Внимание", "Полученные данные имеют неверный начальный адрес значений!");
  }
}

std::vector<uint16_t> PidWidget::getRegisters() const {
  std::vector<uint16_t> ret{};
  ret.push_back(static_cast<uint16_t>(10 * m_period->text().toDouble()));
  ret.push_back(static_cast<uint16_t>(10 * m_p->text().toDouble()));
  ret.push_back(static_cast<uint16_t>(m_i->text().toUInt()));
  ret.push_back(static_cast<uint16_t>(m_d->text().toUInt()));

  return ret;
}
