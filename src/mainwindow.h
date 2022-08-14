#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <memory>

#include "src/communication.h"
#include "src/deltawidget.h"
#include "src/iprocess.h"

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit MainWindow(std::shared_ptr<ICommunication> communication, QWidget *parent = nullptr);
  ~MainWindow();
private:
  void                            showControllersWidget();
  std::shared_ptr<ICommunication> m_communication;
  std::unique_ptr<IProcess>       m_process;
  QVBoxLayout                    *m_mainLay{nullptr};
  std::vector<DeltaWidget *>      m_controllerWidgets;
  QTimer                         *m_updateValuesTimer{nullptr};
  QPushButton                    *m_startBtn{nullptr};
  QPushButton                    *m_stopBtn{nullptr};
};

#endif // MAINWINDOW_H
