#include "correctionwidget.h"

CorrectionWidget::CorrectionWidget(const QString &value, QWidget *parent) : QWidget{parent} {
  auto mainLay = new QHBoxLayout(this);
  auto dec     = new QPushButton("-", this);
  auto inc     = new QPushButton("+", this);
  m_line       = new QLineEdit(value, this);

  mainLay->setContentsMargins(0,0,0,0);

  mainLay->addWidget(dec);
  mainLay->addWidget(m_line);
  mainLay->addWidget(inc);

  connect(inc, &QPushButton::clicked, this, [this] {
    double old_value = m_line->text().toDouble();
    m_line->setText(QString::number(old_value + 0.1, 'f', 1));
  });

  connect(dec, &QPushButton::clicked, this, [this] {
    double old_value = m_line->text().toDouble();
    m_line->setText(QString::number(old_value - 0.1, 'f', 1));
  });
}
