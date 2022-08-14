#ifndef DATAHOLDERPLOT_H
#define DATAHOLDERPLOT_H

#include <QVariant>

#include "idataholder.h"
#include "qcustomplot.h"

class DataHolderPlot : public QCustomPlot {
  Q_OBJECT
public:
  enum ViewMode {
    HOLD_ALL = 1,
    HOLD_X   = 2,
    FREE     = 3
  };
  explicit DataHolderPlot(QWidget *parent = nullptr);
  ~DataHolderPlot();
  void setDataHolder(std::shared_ptr<IDataHolder> dataHolder);
  void setSettings(QVariant settings);
  void setViewMode(ViewMode mode);
  void enableCursor(bool value = true);

signals:
  void viewModeChanged(ViewMode);
  void cursorEnabled(bool);

private:
  /**
   * @brief Update plot after data changed
   */
  void replotDataUpdated();
  void updateGraphsData();
  void initGraphs();
  void setupGraphs();

  ViewMode                     m_viewMode{HOLD_ALL};
  bool                         m_cursorIsOn{false};
  bool                         m_settingsIsSet{true};
  QVariant                     m_settings;
  std::shared_ptr<IDataHolder> m_dataHolder;
};

#endif // DATAHOLDERPLOT_H
