#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFrame>
#include <QLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <memory>

#include "src/dataholder.h"
#include "src/dataholderplot.h"
#include "src/deltawidget.h"
#include "src/icommunication.h"
#include "src/iprocess.h"

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit MainWindow(std::shared_ptr<ICommunication> communication, QWidget *parent = nullptr);
  ~MainWindow() override;

private:
  void                            initGui();
  void                            enableControlWidgets(bool value = true);
  void                            handleProcessChanges(bool runStatus);

  QAction *m_openSettingsAction{nullptr};
  void openSettings();
  QAction *m_openDataAction{nullptr};
  void openData();

  std::shared_ptr<ICommunication> m_communication;
  std::unique_ptr<IProcess>       m_process;
  std::vector<DeltaWidget *>      m_controllerWidgets;
  std::shared_ptr<DataHolder>     m_activeDataHolder;
  QTimer                         *m_updateValuesTimer{nullptr};
  QFrame                         *m_controlWidget{nullptr};
  QLabel                         *m_statusLabel{nullptr};
  QPushButton                    *m_startBtn{nullptr};
  QPushButton                    *m_stopBtn{nullptr};
  DataHolderPlot                 *m_plot{nullptr};
  bool                            m_controlWidgetsEnabled{true};
};

#endif // MAINWINDOW_H
