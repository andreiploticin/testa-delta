#include "mainwindow.h"

#include <QApplication>

#include "config.h"
#include "src/process.h"
#include "src/settings.h"
#include "src/settingsdialog.h"
#include "src/utils.h"

MainWindow::MainWindow(std::shared_ptr<ICommunication> communication, QWidget *parent)
    : QMainWindow{parent}
    , m_communication(std::move(communication)) {
  m_process           = std::make_unique<Process>(m_communication);
  m_updateValuesTimer = new QTimer(this);

  initGui();

  connect(m_communication.get(), &ICommunication::connectedChanged, this, &MainWindow::handleConnectionChanges);

  m_communication->setSettings(Settings::getInstance().getSettingsMap()["settings"].toMap()["communication"]);

  connectToDelta();

  connect(m_startBtn, &QPushButton::clicked, this, [this] {
    if ((nullptr != m_process.get()) && (nullptr != m_communication.get()) && (1 == m_communication->getStatus())) {
      std::vector<double> newSets{};
      std::vector<double> newCorrections{};
      auto correctionsList = Settings::getInstance().getSettingsMap()["settings"].toMap()["calibration"].toList();

      for (int i{0}; i < m_controllerWidgets.size(); ++i) {
        auto const &item       = m_controllerWidgets[i];
        auto        correction = utils::correction(item->getSetValue(), correctionsList[i].toMap());
        newSets.push_back(item->getSetValue() - correction);
        newCorrections.push_back(correction);
      }
      qDebug() << newSets;
      qDebug() << newCorrections;

      // update corrections
      m_communication->setCorrections(newCorrections);
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

  m_controlWidget = new QFrame(this);
  //  m_controlWidget->setContentsMargins(0,3,0,3);
  auto controlWidgetLay = new QVBoxLayout();
  controlWidgetLay->setContentsMargins(0, 0, 0, 0);
  m_controlWidget->setLayout(controlWidgetLay);
  m_controlWidget->setMaximumWidth(400);

  m_statusLabel = new QLabel("Статус: ", this);

  m_plotWidget = new DataHolderPlotWidget(m_controlWidget);

  m_startBtn = new QPushButton("Пуск", this);
  m_stopBtn  = new QPushButton("Остановка", this);

  mainLay->addWidget(m_controlWidget);
  mainLay->addWidget(m_plotWidget);

  controlWidgetLay->addWidget(m_statusLabel);
  auto controllers = Settings::getInstance().getSettingsMap()["controllers"].toList();
  for (int i{0}; i < controllers.size(); ++i) {
    auto controller = controllers[i].toMap();
    auto w          = new DeltaWidget(this, controller["label"].toString(), controller["set"].toDouble(),
                                      controller["correction"].toDouble());
    if (0 == i) {
      w->enableMaster();
    } else {
      controlWidgetLay->addSpacing(10);
    }
    m_controllerWidgets.push_back(w);
    controlWidgetLay->addWidget(w);
  }
  for (int i = 1; i < m_controllerWidgets.size(); ++i) {
    auto &controller = m_controllerWidgets[i];
    connect(m_controllerWidgets[0], &DeltaWidget::masterSelected, this, [&controller](double value) {
      if (value > 0) {
        controller->fixSetValue(true);
        controller->setSetValue(value);
      } else {
        controller->fixSetValue(false);
      }
    });
    connect(m_controllerWidgets[0], &DeltaWidget::setValueChanged, controller, &DeltaWidget::setSetValue);
  }
  controlWidgetLay->addWidget(m_startBtn);
  controlWidgetLay->addWidget(m_stopBtn);
  controlWidgetLay->addStretch();

  m_plotWidget->setSettings(Settings::getInstance().getSettingsMap().value("plotLines"));

  setGeometry(geometry() + QMargins{0, 0, 300, 0});

  enableControlWidgets(false);

  QString version = PROJECT_VER;
  setWindowTitle("Testa Delta v" + version);

  /*
   * Setup menu.
   */
  auto menu            = menuBar();
  m_openSettingsAction = menu->addAction("Настройки");
  connect(m_openSettingsAction, &QAction::triggered, this, &MainWindow::openSettings);
  m_openDataAction = menu->addAction("Открыть данные");
  connect(m_openDataAction, &QAction::triggered, this, &MainWindow::openData);
  m_connectAction = menu->addAction("Подключиться");
  connect(m_connectAction, &QAction::triggered, this, &MainWindow::connectToDelta);

  QFont font = menuBar()->font();
  font.setPointSize(Settings::getInstance().getFontSize());
  QApplication::setFont(font, "QWidget");
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

void MainWindow::handleConnectionChanges(bool isConnected) {
  m_connectAction->setEnabled(!isConnected);
  enableControlWidgets(isConnected);
}

void MainWindow::saveSetsAndCorrections() {
  QVariantList controllersList{};
  for (auto const &item : m_controllerWidgets) {
    controllersList.push_back(item->getInfo());
  }
  Settings::getInstance().save(controllersList, "controllers");
}

void MainWindow::openSettings() {
  auto sd = new SettingsDialog(Settings::getInstance().getSettingsMap()["settings"].toMap(), m_communication, this);

  QObject::connect(sd, &SettingsDialog::saveSettings, [this](QVariantMap map) {
    Settings::getInstance().save(map, "settings");
    QFont font = menuBar()->font();
    font.setPointSize(Settings::getInstance().getFontSize());
    QApplication::setFont(font, "QWidget");
  });

  sd->exec();
}

void MainWindow::openData() {
  auto binaryFullDirPath = QCoreApplication::applicationDirPath();
  auto targetDir         = binaryFullDirPath + "/" + "data";
  auto filePath          = QFileDialog::getOpenFileName(this, tr("Open Data"), targetDir, tr("data file (*.dat)"));
  if (!filePath.isEmpty()) {
    auto dataHolder = std::make_shared<DataHolder>(6);
    auto plotWidget = new DataHolderPlotWidget();
    plotWidget->setDataHolder(dataHolder);
    if (0 > dataHolder->load(filePath)) {
      QMessageBox::critical(this, "Ошибка",
                            "Открываемый файл повреждён или имеет неверный формат. Отображение невозможно.");
      plotWidget->deleteLater();
      return;
    } else {
      plotWidget->setSettings(Settings::getInstance().getSettingsMap().value("plotLines"));
      plotWidget->setAttribute(Qt::WA_DeleteOnClose);
      plotWidget->setWindowTitle(filePath);
      plotWidget->show();
      plotWidget->setMinimumSize(600, 400);
    }
  }
}

void MainWindow::connectToDelta() { m_communication->establishConnection(CommunicationSetupOptions{}); }

MainWindow::~MainWindow() {}

void MainWindow::closeEvent(QCloseEvent *event) {
  saveSetsAndCorrections();
  QMainWindow::closeEvent(event);
}
