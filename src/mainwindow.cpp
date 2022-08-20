#include "mainwindow.h"
//#include <QThreadPool>
#include "src/process.h"
#include "src/settings.h"
#include "src/settingsdialog.h"

MainWindow::MainWindow(std::shared_ptr<ICommunication> communication, QWidget *parent)
    : QMainWindow{parent}, m_communication(std::move(communication)) {
  m_process           = std::make_unique<Process>(m_communication);
  m_updateValuesTimer = new QTimer(this);

  initGui();

  connect(m_communication.get(), &ICommunication::connected, this, [this] {
    enableControlWidgets();
  });

  m_communication->establishConnection(CommunicationSetupOptions{});

  connect(m_startBtn, &QPushButton::clicked, this, [this] {
    if ((nullptr != m_process.get()) && (nullptr != m_communication.get()) && (1 == m_communication->getStatus())) {
      std::vector<double> newSets{};
      for (auto const &item : m_controllerWidgets) {
        newSets.push_back(item->getSetValue());
      }

      // process create dataHolder
      m_process->restart(newSets);
      // we work with created or old dataHolder
      m_plotWidget->setDataHolder(m_process->getDataHolderPtr());
    }
  });

  connect(m_stopBtn, &QPushButton::clicked, this, [this] {
    if ((nullptr != m_process.get()) && (nullptr != m_communication.get()) && (1 == m_communication->getStatus())) {
      m_process->stop();
    }
  });

  connect(m_process.get(), &IProcess::runStatusChanged, this, &MainWindow::handleProcessChanges);

  connect(m_updateValuesTimer, &QTimer::timeout, this, [this] {
    auto lastValues = m_communication->getLastValues();

    if (lastValues.size() != m_controllerWidgets.size()) {
      qInfo() << "invalide sizes";
      return;
    }

    for (int i = 0; i < lastValues.size(); ++i) {
      m_controllerWidgets[i]->setCurTemp(lastValues[i].second);
    };
  });

  handleProcessChanges(false);
}

void MainWindow::initGui() {
  auto widget = new QWidget();
  setCentralWidget(widget);
  auto mainLay = new QHBoxLayout(widget);

  m_controlWidget       = new QFrame(this);
  //  m_controlWidget->setContentsMargins(0,3,0,3);
  auto controlWidgetLay = new QVBoxLayout();
  controlWidgetLay->setContentsMargins(0, 0, 0, 0);
  m_controlWidget->setLayout(controlWidgetLay);
  m_controlWidget->setMaximumWidth(300);

  m_statusLabel = new QLabel("Статус: ", this);

  m_plotWidget = new DataHolderPlotWidget(m_controlWidget);

  m_startBtn = new QPushButton("Start", this);
  m_stopBtn  = new QPushButton("Stop", this);

  mainLay->addWidget(m_controlWidget);
  mainLay->addWidget(m_plotWidget);

  controlWidgetLay->addWidget(m_statusLabel);
  auto controllers = Settings::getInstance().getSettingsMap()["controllers"].toList();
  for (int i{0}; i < controllers.size(); ++i) {
    auto controller = controllers[i].toMap();
    auto w          = new DeltaWidget(
      this, controller["label"].toString(), controller["set"].toDouble(), controller["correction"].toDouble());
    m_controllerWidgets.push_back(w);
    controlWidgetLay->addWidget(w);
  }
  controlWidgetLay->addWidget(m_startBtn);
  controlWidgetLay->addWidget(m_stopBtn);
  controlWidgetLay->addStretch();

  m_plotWidget->setSettings(Settings::getInstance().getSettingsMap().value("plotLines"));

  setGeometry(geometry() + QMargins{0, 0, 300, 0});

  enableControlWidgets(false);

  /*
   * Setup menu.
   */
  auto menu            = menuBar();
  m_openSettingsAction = menu->addAction("Настройки");
  connect(m_openSettingsAction, &QAction::triggered, this, &MainWindow::openSettings);
  m_openDataAction = menu->addAction("Открыть данные");
  connect(m_openDataAction, &QAction::triggered, this, &MainWindow::openData);
}

void MainWindow::enableControlWidgets(bool value) {
  if (value == m_controlWidgetsEnabled) {
    return;
  }
  m_controlWidgetsEnabled = value;

  if (m_controlWidgetsEnabled && (m_controllerWidgets.size() > 0)) {
    m_updateValuesTimer->start(1000);
  } else {
    m_updateValuesTimer->stop();
  }

  m_controlWidget->setEnabled(m_controlWidgetsEnabled);
}

void MainWindow::handleProcessChanges(bool runStatus) {
  m_statusLabel->setText(QString("Статус: ") + (runStatus ? QString("работа") : QString("ожидание")));
  m_stopBtn->setEnabled(runStatus);
}

void MainWindow::openSettings() {
  auto sd = new SettingsDialog(this);
  sd->exec();
}

void MainWindow::openData()
{

}

MainWindow::~MainWindow() {
}
