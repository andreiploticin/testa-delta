//
// Created by user on 2023-02-11.
//
#include "initialcorrectionwidget.h"

#include <QGridLayout>
#include <QLabel>
#include <QValidator>

InitialCorrectionWidget::InitialCorrectionWidget(QWidget *parent)
    : QGroupBox("Корректировка уставки при старте", parent) {
  initGui();
  m_default_Style = this->styleSheet();
}

void   InitialCorrectionWidget::setCorrection(double value) { m_cor_edit->setText(QString::number(value, 'f', 1)); }
void   InitialCorrectionWidget::setDuration(double minutes) { m_dur_edit->setText(QString::number(minutes, 'f', 0)); }
double InitialCorrectionWidget::getCorrection() { return m_cor_edit->text().toDouble(); }
double InitialCorrectionWidget::getDuration() { return m_dur_edit->text().toDouble(); }
void   InitialCorrectionWidget::initGui() {
    auto mainLay = new QGridLayout(this);
    m_cor_edit   = new QLineEdit("0.0", this);
    m_dur_edit   = new QLineEdit("10", this);

    m_doubleValidator = new QDoubleValidator(0.1, 300, 1, m_cor_edit);
    m_doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    m_cor_edit->setValidator(m_doubleValidator);
    m_dur_edit->setValidator(new QIntValidator(0, 60 * 5, m_dur_edit));

    m_button = new QPushButton("Задать", this);
    m_button->setMinimumWidth(100);

    mainLay->addWidget(new QLabel("Величина -Δ, °С"), 0, 0);
    mainLay->addWidget(new QLabel("Продолжительность, мин"), 1, 0);
    mainLay->addWidget(m_cor_edit, 0, 1);
    mainLay->addWidget(m_dur_edit, 1, 1);
    mainLay->addWidget(m_button, 0, 2);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &InitialCorrectionWidget::handleTimer);

    connect(m_button, &QPushButton::clicked, this, &InitialCorrectionWidget::buttonHandle);

    setActive(false);
}

void InitialCorrectionWidget::setActive(bool value) {
  m_inWork = value;

  if (m_inWork) {
    setStyleSheet("QGroupBox { background-color: rgba(245,245,50,128); }");
    m_timer->start(1000 * 60 * m_dur_edit->text().toDouble());
  } else {
    setStyleSheet(m_default_Style);
    m_timer->stop();
  }
  m_cor_edit->setEnabled(!m_inWork);
  m_dur_edit->setEnabled(!m_inWork);
  m_button->setText(m_inWork ? "Снять" : "Задать");
}

void InitialCorrectionWidget::buttonHandle() {
  setActive(!m_inWork);
  emit newLowSet(m_inWork ? m_cor_edit->text().toDouble() : -10000);
}
void InitialCorrectionWidget::handleTimer() {
  if (m_inWork) {
    buttonHandle();
  }
}
