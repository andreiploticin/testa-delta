#include "mainwindow.h"

#include <QThreadPool>

#include "src/process.h"

MainWindow::MainWindow(std::shared_ptr<ICommunication> communication, QWidget *parent)
    : QMainWindow{parent}, m_communication(std::move(communication)) {
  auto widget = new QWidget();
  setCentralWidget(widget);
  m_mainLay = new QVBoxLayout(widget);

  m_startBtn = new QPushButton("Start", this);
  m_stopBtn  = new QPushButton("Stop", this);

  m_mainLay->addWidget(m_startBtn);
  m_mainLay->addWidget(m_stopBtn);

  m_process = std::make_unique<Process>(m_communication);

  connect(m_communication.get(), &ICommunication::connected, this, &MainWindow::showControllersWidget);

  //  auto communicationThread = new QThread;
  //  m_communication.get()->moveToThread(communicationThread);
  //  communicationThread->start();

  m_communication->setup(CommunicationSetupOptions{});

  connect(m_startBtn, &QPushButton::clicked, this, [this] {
    if ((nullptr != m_process.get()) && (nullptr != m_communication.get()) && (1 == m_communication->getStatus())) {
      m_process->restart(std::vector<double>{1, 2, 3});
    }
  });
}

void MainWindow::showControllersWidget() {
  for (int i = 0; i < m_communication->getNumberOfControllers(); ++i) {
    auto w = new DeltaWidget(this);
    m_controllerWidgets.push_back(w);

    m_mainLay->addWidget(w);
  }

  m_updateValuesTimer = new QTimer(this);

  connect(
    m_updateValuesTimer,
    &QTimer::timeout,
    this,
    [this]() {
      auto lastValues = m_communication->getLastValues();

      if (lastValues.size() != m_controllerWidgets.size()) {
        qInfo() << "invalide sizes";
        return;
      }

      for (int i = 0; i < lastValues.size(); ++i) {
        m_controllerWidgets[i]->setCurTemp(lastValues[i].second);
      }
    },
    Qt::QueuedConnection);
  m_updateValuesTimer->start(1000);
}

MainWindow::~MainWindow() {
  m_communication->finished();
}
