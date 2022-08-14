#include "dataholderplot.h"

DataHolderPlot::DataHolderPlot(QWidget *parent) : QCustomPlot(parent) {
  QSharedPointer<QCPAxisTickerDateTime> timeTicker(new QCPAxisTickerDateTime);
  timeTicker->setDateTimeFormat("hh:mm:ss");
  xAxis->setTicker(timeTicker);

  this->setInteraction(QCP::iRangeDrag, true);
  this->setInteraction(QCP::iRangeZoom, true);
  this->setNoAntialiasingOnDrag(true);
}

DataHolderPlot::~DataHolderPlot() {
}

void DataHolderPlot::setDataHolder(std::shared_ptr<IDataHolder> dataHolder) {
  m_dataHolder = dataHolder;
  initGraphs();
  updateGraphsData();

  connect(m_dataHolder.get(), &IDataHolder::pointAdded, this, [this] {
    updateGraphsData();
    replotDataUpdated();
  });
}

void DataHolderPlot::setSettings(QVariant settings) {
}

void DataHolderPlot::setViewMode(ViewMode mode) {
  if (m_viewMode == mode) {
    return;
  }

  // action

  m_viewMode = mode;
  emit viewModeChanged(m_viewMode);
}

void DataHolderPlot::enableCursor(bool value) {
  if (m_cursorIsOn == value) {
    return;
  }

  // action

  m_cursorIsOn = value;
  emit cursorEnabled(m_cursorIsOn);
}

void DataHolderPlot::replotDataUpdated() {
  xAxis->rescale();
  yAxis->rescale();
  replot();
}

void DataHolderPlot::updateGraphsData() {
  auto c_time = m_dataHolder->getTime();
  auto c_data = m_dataHolder->getData();

  if (graphCount() != c_data.size()) {
    qInfo() << __PRETTY_FUNCTION__ << "graphs count does not match data sets number";
    return;
  }

  for (int i{0}; i < c_data.size(); ++i) {
    graph(i)->setData(c_time, c_data[i]);
  }
}

void DataHolderPlot::initGraphs() {
  if (0 == m_dataHolder->getData().size()) {
    qInfo() << __PRETTY_FUNCTION__ << "No data sets";
    return;
  }
  clearGraphs();
  auto c_data = m_dataHolder->getData();

  for (int i{0}; i < c_data.size(); ++i) {
    addGraph();
  }
}

void DataHolderPlot::setupGraphs() {
}
