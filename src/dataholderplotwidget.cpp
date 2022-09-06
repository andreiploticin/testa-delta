#include "dataholderplotwidget.h"

#include <QLayout>

DataHolderPlotWidget::DataHolderPlotWidget(QWidget *parent) : QWidget{parent} {
  auto mainLay = new QVBoxLayout(this);
  auto btnLay  = new QHBoxLayout();
  m_plot       = new DataHolderPlot(this);

  m_cursorBtn   = new QPushButton("Cursor", this);
  m_viewModeBtn = new QPushButton("", this);

  mainLay->addLayout(btnLay);
  mainLay->addWidget(m_plot);
  btnLay->addWidget(m_cursorBtn);
  btnLay->addWidget(m_viewModeBtn);

  m_plot->enableCursor();

  connect(m_cursorBtn, &QPushButton::clicked, m_plot, &DataHolderPlot::switchCursor);
  connect(m_plot, &DataHolderPlot::cursorEnabled, this, [this](bool value) {
    m_cursorBtn->setText(value ? "Turn off cursor" : "Turn on cursor");
  });
  connect(m_viewModeBtn, &QPushButton::clicked, m_plot, &DataHolderPlot::switchViewMode);
  connect(m_plot, &DataHolderPlot::viewModeChanged, this, &DataHolderPlotWidget::updateViewModeBtn);

  m_plot->setViewMode(DataHolderPlot::ViewMode::HOLD_ALL);
  m_viewModeBtn->setText("HOLD_ALL");
}

void DataHolderPlotWidget::updateViewModeBtn(DataHolderPlot::ViewMode viewMode) {
  switch (viewMode) {
    case DataHolderPlot::ViewMode::HOLD_ALL:
      m_viewModeBtn->setText("HOLD_ALL");
      break;
    case DataHolderPlot::ViewMode::HOLD_X:
      m_viewModeBtn->setText("HOLD_X");
      break;
    default:
      m_viewModeBtn->setText("FREE");
      break;
  }
}
