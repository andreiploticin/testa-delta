#ifndef DATAHOLDERPLOTWIDGET_H
#define DATAHOLDERPLOTWIDGET_H

#include <QWidget>

#include "src/dataholderplot.h"

class DataHolderPlotWidget : public QWidget {
  Q_OBJECT
public:
  explicit DataHolderPlotWidget(QWidget *parent = nullptr);
  void setDataHolder(std::shared_ptr<IDataHolder> dataHolder) {
    m_plot->setDataHolder(dataHolder);
  }
  void setSettings(QVariant settings) {
    m_plot->setSettings(settings);
  }

private:
  void updateViewModeBtn(DataHolderPlot::ViewMode viewMode);
  DataHolderPlot *m_plot{nullptr};
  QPushButton    *m_cursorBtn{nullptr};
  QPushButton    *m_viewModeBtn{nullptr};
};

#endif // DATAHOLDERPLOTWIDGET_H
