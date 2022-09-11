#include "dataholderplotwidget.h"

#include <QLayout>

DataHolderPlotWidget::DataHolderPlotWidget(QWidget *parent) : QWidget{parent} {
  auto mainLay = new QVBoxLayout(this);
  auto btnLay  = new QHBoxLayout();
  m_plot       = new DataHolderPlot(this);

  m_cursorBtn   = new QPushButton("Показать курсор", this);
  m_viewModeBtn = new QPushButton(this);

  m_viewModeBtn->setFixedSize(100, 50);
  m_viewModeBtn->setIconSize(QSize(90, 45));

  m_cursorBtn->setFixedSize(160, 50);

  mainLay->addLayout(btnLay);
  mainLay->addWidget(m_plot);
  btnLay->addWidget(m_cursorBtn);
  btnLay->addWidget(m_viewModeBtn);
  btnLay->addStretch();

  m_plot->enableCursor();

  connect(m_cursorBtn, &QPushButton::clicked, m_plot, &DataHolderPlot::switchCursor);
  connect(m_plot, &DataHolderPlot::cursorEnabled, this, [this](bool value) {
    m_cursorBtn->setText(value ? "Убрать курсор" : "Показать курсор");
  });
  connect(m_viewModeBtn, &QPushButton::clicked, m_plot, &DataHolderPlot::switchViewMode);
  connect(m_plot, &DataHolderPlot::viewModeChanged, this, &DataHolderPlotWidget::updateViewModeBtn);

  m_plot->setViewMode(DataHolderPlot::ViewMode::HOLD_ALL);
  m_viewModeBtn->setIcon(QIcon(":/imgs/graph_lead_full.png"));
}

void DataHolderPlotWidget::updateViewModeBtn(DataHolderPlot::ViewMode viewMode) {
  switch (viewMode) {
    case DataHolderPlot::ViewMode::HOLD_ALL:
      m_viewModeBtn->setIcon(QIcon(":/imgs/graph_lead_full.png"));
      break;
    case DataHolderPlot::ViewMode::HOLD_X:
      m_viewModeBtn->setIcon(QIcon(":/imgs/graph_lead_x.png"));
      break;
    default:
      m_viewModeBtn->setIcon(QIcon(":/imgs/graph_lead_zoom.png"));
      break;
  }
}
