#include "dataholderplotwidget.h"
#include <QLayout>

DataHolderPlotWidget::DataHolderPlotWidget(QWidget *parent) : QWidget{parent} {
  auto mainLay = new QVBoxLayout(this);
  m_plot = new DataHolderPlot(this);

  auto btn = new QPushButton("Motor, Action!", this);
  mainLay->addWidget(m_plot);
  mainLay->addWidget(btn);

  connect(btn, &QPushButton::clicked, m_plot, &DataHolderPlot::action);
}
