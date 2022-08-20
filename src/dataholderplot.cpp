#include "dataholderplot.h"

Legend::Legend(DataHolderPlot *plot) : m_plot{plot}, m_leg{plot->legend} {
  setVisible(true);
  m_leg->setBrush(QBrush(QColor(255, 255, 255, 230)));
  m_plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft | Qt::AlignTop);
  m_valuesLay = m_plot->layer("legend");
  m_valuesLay->setMode(QCPLayer::LayerMode::lmBuffered);

  m_cursor = new QCPItemStraightLine(m_plot);
//  m_cursor->setVisible(false);
  m_plot->addLayer("cursor");
  m_cursorLay = m_plot->layer("cursor");
  m_cursorLay->setMode(QCPLayer::LayerMode::lmBuffered);
  m_cursorLay->setVisible(false);
  m_cursor->setLayer(m_cursorLay);
  //  m_plot->moveLayer(valuesLay, m_plot->layer("legend"));
}

void Legend::setVisible(bool value) {
  m_leg->setVisible(value);
}

void Legend::setValuesVisible(bool value) {
  if (value == m_valuesVisible) {
    return;
  }
  m_valuesVisible = value;
  m_cursorLay->setVisible(value);
//  if (m_valuesVisible) {
    m_valuesLay->replot();
    m_cursorLay->replot();
//  }
}

void Legend::setup() {
  auto itemCount = m_plot->graphCount();

  for (int i{0}; i < itemCount; ++i) {
    auto item = new QCPTextElement(m_plot, "0.0");
    item->setTextColor(Qt::black);
    m_leg->addElement(i, 2, item);
    item->setLayer(m_valuesLay);
    //    item->setVisible(false);
    qInfo() << item->layer()->name();
    m_values.push_back(item);
  }
}

void Legend::clear() {
  for (int i{0}; i < m_values.size(); ++i) {
    auto &item = m_values[i];
    m_leg->removeItem(m_leg->rowColToIndex(i, 2));
    delete item;
  }
  m_values.clear();
  m_valuesLay->replot();
}

void Legend::action() {
  setValuesVisible(true);
  setValues(m_plot->m_dataHolder->getDataAtSample(0));
}

void Legend::setValues(QVector<double> values) {
  for (int i{0}; i < qMin(values.size(), m_values.size()); ++i) {
    m_values[i]->setText(QString::number(values[i], 'f', 1));
  }
  if (m_valuesVisible) {
    m_valuesLay->replot();
  }
}

void Legend::setCursorPosition(double timeValue) {
  m_cursor->point1->setCoords(timeValue, 0);
  m_cursor->point2->setCoords(timeValue, 1000);
  m_cursorLay->replot();
}

DataHolderPlot::DataHolderPlot(QWidget *parent) : QCustomPlot(parent), m_legend(this) {
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
  if (dataHolder == m_dataHolder) {
    return;
  }
  m_dataHolder = dataHolder;
  initGraphs();
  setSettings(m_settings);
  updateGraphsData();

  connect(m_dataHolder.get(), &IDataHolder::pointAdded, this, [this] {
    updateGraphsData();
    replotDataUpdated();
  });
}

void DataHolderPlot::setSettings(QVariant settings) {
  if (!settings.isValid()) {
    return;
  }

  m_settings = settings;

  for (int i = 0; i < qMin(graphCount(), settings.toList().size()); ++i) {
    auto   map = settings.toList()[i].toMap();
    QPen   pen;
    QColor color(map["color"].toString());
    pen.setColor(color);
    qDebug() << color;
    graph(i)->setPen(pen);
    graph(i)->setName(map["label"].toString());
  }
  m_legend.setup();
  replot();
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

  if (nullptr == m_dataHolder.get()) {
    return;
  }

  // action

  m_cursorIsOn = value;
  m_legend.setValuesVisible(value);
  emit cursorEnabled(value);
}

void DataHolderPlot::enableLegend(bool value) {
  if (m_legendIsOn == value) {
    return;
  }

  // action

  m_legendIsOn = value;
  emit legendEnabled(m_legendIsOn);
}

void DataHolderPlot::action() {
  m_legend.action();
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
  m_legend.clear();
  clearGraphs();
  auto c_data = m_dataHolder->getData();

  for (int i{0}; i < c_data.size(); ++i) {
    addGraph();
  }
}

void DataHolderPlot::setupGraphs() {
}

void DataHolderPlot::mouseMoveEvent(QMouseEvent *event) {
  if (m_cursorIsOn && (nullptr != m_dataHolder.get())) {
    auto   mouseCord = event->pos();
    double xCord     = xAxis->pixelToCoord(mouseCord.x());
    auto   data      = m_dataHolder->getDataAtTime(xCord);
    m_legend.setCursorPosition(xCord);
    m_legend.setValues(data);
  }
  QCustomPlot::mouseMoveEvent(event);
}
