#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFrame>
#include <QLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <memory>

#include "src/dataholder.h"
#include "src/dataholderplotwidget.h"
#include "src/deltawidget.h"
#include "src/icommunication.h"
#include "src/initialcorrectionwidget.h"
#include "src/iprocess.h"

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit MainWindow(std::shared_ptr<ICommunication> communication, QWidget *parent = nullptr);
  ~MainWindow() override;

private:
  void initGui();
  void enableControlWidgets(bool value = true);
  void handleProcessChanges(bool runStatus);
  void handleInitCorrectionChanges(double correction);
  void handleConnectionChanges(bool isConnected);
  void saveSetsAndCorrections();
  void sentNewSet(double globalCorrection = -10000);
  void handleStop();

  QAction *m_openSettingsAction{nullptr};
  void     openSettings();
  QAction *m_openDataAction{nullptr};
  void     openData();
  QAction *m_connectAction{nullptr};
  void     connectToDelta();

  std::shared_ptr<ICommunication> m_communication;
  std::unique_ptr<IProcess>       m_process;
  std::vector<DeltaWidget *>      m_controllerWidgets;
  QTimer                         *m_updateValuesTimer{nullptr};
  QFrame                         *m_controlWidget{nullptr};
  QLabel                         *m_statusLabel{nullptr};
  InitialCorrectionWidget        *m_initCorrection{nullptr};
  QPushButton                    *m_startBtn{nullptr};
  QPushButton                    *m_stopBtn{nullptr};
  DataHolderPlotWidget           *m_plotWidget{nullptr};
  bool                            m_controlWidgetsEnabled{true};

  // QWidget interface
protected:
  void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
