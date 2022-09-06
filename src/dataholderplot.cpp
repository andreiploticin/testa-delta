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
  //    m_leg->updateLayout();
}

void Legend::setValuesVisible(bool value) {
  if (value == m_valuesVisible) {
    return;
  }
  m_valuesVisible = value;
  m_cursorLay->setVisible(value);
  m_cursorLay->replot();

  if (value) {
    setup();
  } else {
    for (int i{0}; i < m_values.size(); ++i) {
      auto &item = m_values[i];
      m_leg->remove(item);
      //      m_leg->removeItem(m_leg->rowColToIndex(i, 1));
    }
    m_values.clear();
    m_leg->updateLayout();
  }

  m_valuesLay->replot();
}

void Legend::setup() {
  auto itemCount = m_plot->graphCount();

  for (int i{0}; i < itemCount; ++i) {
    auto item = new QCPTextElement(m_plot, "           ");
    item->setTextColor(Qt::black);
    m_leg->addElement(i, 2, item);
    item->setLayer(m_valuesLay);
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

  //  auto cLegend = new QCPLayoutGrid();
  //  this->axisRect()->insetLayout()->addElement(cLegend, Qt::AlignLeft | Qt::AlignTop);
  //  cLegend->addElement(new QCPTextElement{this, "AAAAAAAAAA"});
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
  //  m_legend.setup();
  replot();
}

void DataHolderPlot::setViewMode(ViewMode mode) {
  if (m_viewMode == mode) {
    return;
  }

  if (ViewMode::FREE == m_viewMode) {
//    m_currentView.setWidth(this->xAxis->range().size());
//    this->yAxis->rescale(true);
//    this->yAxis->setRangeLower(this->yAxis->range().lower - 10);
//    this->yAxis->setRangeUpper(this->yAxis->range().upper + 10);
    m_currentView = getRange();
  }

  m_viewMode = mode;
  emit viewModeChanged(m_viewMode);
  replotDataUpdated();
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
  updateRange();
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

void DataHolderPlot::updateRange() {
  switch (m_viewMode) {
    case HOLD_ALL:
      this->xAxis->rescale();
      this->yAxis->rescale(true);

      if (this->yAxis->range().size() > 4000)
        this->yAxis->setRange(-2000, 2000);

      if (this->yAxis->range().size() < 20) {
        this->yAxis->setRangeLower(this->yAxis->range().lower - 10);
        this->yAxis->setRangeUpper(this->yAxis->range().upper + 10);
      }
      m_currentView = getRange();
      break;
    case HOLD_X: {
      this->xAxis->rescale();
      this->xAxis->setRangeLower(this->xAxis->range().upper - m_currentView.width());
      m_currentView.setLeft(this->xAxis->range().lower);
      m_currentView.setRight(this->xAxis->range().upper);
      break;
    }
    default:
      break;
  }

  if ((m_viewMode == HOLD_ALL) || (m_viewMode == HOLD_X)) {
    this->xAxis->setRange(m_currentView.left(), m_currentView.right());
    this->yAxis->setRange(m_currentView.bottom(), m_currentView.top());
  }
}

inline QRectF DataHolderPlot::getRange() {
  QPointF tl(this->xAxis->range().lower, this->yAxis->range().upper);
  QPointF br(this->xAxis->range().upper, this->yAxis->range().lower);
  return QRectF(tl, br);
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

void DataHolderPlot::mousePressEvent(QMouseEvent *event) {
  if (QEvent::MouseButtonPress == event->type()) {
    setViewMode(ViewMode::FREE);
  }
  QCustomPlot::mousePressEvent(event);
}

void DataHolderPlot::wheelEvent(QWheelEvent *event) {
  setViewMode(ViewMode::FREE);
  QCustomPlot::wheelEvent(event);
}
