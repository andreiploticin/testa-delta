#ifndef DATAHOLDERPLOT_H
#define DATAHOLDERPLOT_H

#include <QVariant>

#include "idataholder.h"
#include "qcustomplot.h"

class DataHolderPlot;

class Legend {
public:
  explicit Legend(DataHolderPlot *plot);
  void setVisible(bool value);
  void setValuesVisible(bool value);
  /**
   * @brief Add all need drawable object.
   * Apply color settings.
   *
   */
  void setup();

  void clear();
  void action();
  void setValues(QVector<double> values);
  void setCursorPosition(double timeValue);

private:
  QCPLegend                *m_leg{nullptr};
  DataHolderPlot *const     m_plot{nullptr};
  QCPLayer                 *m_valuesLay{nullptr};
  QCPLayer                 *m_cursorLay{nullptr};
  QVector<QCPTextElement *> m_values;
  bool                      m_valuesVisible{false};
  QCPItemStraightLine      *m_cursor{nullptr};
};

class DataHolderPlot : public QCustomPlot {
  Q_OBJECT
  friend Legend;

public:
  enum ViewMode {
    HOLD_ALL = 1,
    HOLD_X   = 2,
    FREE     = 3
  };
  explicit DataHolderPlot(QWidget *parent = nullptr);
  ~DataHolderPlot() override;
  void setDataHolder(std::shared_ptr<IDataHolder> dataHolder);
  void setSettings(QVariant settings);
  void setViewMode(ViewMode mode);
  void enableCursor(bool value = true);
  void enableLegend(bool value = true);
  void switchCursor() {
    enableCursor(!m_cursorIsOn);
  }
  void action();

signals:
  void viewModeChanged(DataHolderPlot::ViewMode);
  void cursorEnabled(bool);
  void legendEnabled(bool);

  // QWidget interface
protected:
  void mouseMoveEvent(QMouseEvent *event) override;

private:
  /**
   * @brief Update plot after data changed
   */
  void replotDataUpdated();
  void updateGraphsData();
  void initGraphs();
  void setupGraphs();

  Legend                       m_legend;
  ViewMode                     m_viewMode{HOLD_ALL};
  bool                         m_legendIsOn{false};
  bool                         m_cursorIsOn{false};
  bool                         m_settingsIsSet{true};
  QVariant                     m_settings;
  std::shared_ptr<IDataHolder> m_dataHolder;
};

#endif // DATAHOLDERPLOT_H
