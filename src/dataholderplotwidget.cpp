#include "dataholderplotwidget.h"

#include <QLayout>

DataHolderPlotWidget::DataHolderPlotWidget(QWidget *parent) : QWidget{parent} {
  auto mainLay = new QVBoxLayout(this);
  auto btnLay  = new QHBoxLayout();
  m_plot       = new DataHolderPlot(this);

  auto btn    = new QPushButton("Motor, Action!", this);
  m_cursorBtn = new QPushButton("Cursor", this);

  mainLay->addLayout(btnLay);
  mainLay->addWidget(m_plot);
  btnLay->addWidget(btn);
  btnLay->addWidget(m_cursorBtn);

  m_plot->enableCursor();

  connect(m_cursorBtn, &QPushButton::clicked, m_plot, &DataHolderPlot::switchCursor);
  connect(btn, &QPushButton::clicked, m_plot, &DataHolderPlot::action);
  connect(m_plot, &DataHolderPlot::cursorEnabled, this, [this](bool value) {
    m_cursorBtn->setText(value ? "Turn off cursor" : "Turn on cursor");
  });
}
