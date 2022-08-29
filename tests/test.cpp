#include <gtest/gtest.h>

#include <QApplication>
#include <QDebug>
//#include <QModbusRtuSerialServer>
//#include <QSerialPort>
//#include <QSerialPortInfo>
#include <memory>

//#include "helpers.hpp"
//#include "src/process.hpp"
#include "communication_helper.h"
#include "src/calibrationwidget.h"
#include "src/dataholder.h"
#include "src/dataholderplotwidget.h"
#include "src/mainwindow.h"
#include "src/rs485comminication.h"
#include "src/settings.h"
#include "src/settingsdialog.h".h "

class testBase : public ::testing::Test {
protected:
  int                           argc{1};
  char                          name1[5]{"asdf"};
  char                         *argv[2] = {name1};
  std::unique_ptr<QApplication> app;
  void                          SetUp() override {
    ::testing::Test::SetUp();
    app = std::make_unique<QApplication>(argc, argv);
    qSetMessagePattern("%{function}(line %{line}): %{message}");
  }
  void TearDown() override {
    ::testing::Test::TearDown();
  }
};

// TEST_F(testBase, testApp) {
//   auto g_testCommunication = std::make_shared<TestCommunication>();
//   Settings::getInstance().setFilePath("c:/dev/testa-delta/settings.json");
//   auto appWindow = std::make_unique<MainWindow>(g_testCommunication);
//   appWindow->show();

//  app->exec();
//}

TEST_F(testBase, testCorrection) {
  Settings::getInstance().setFilePath("c:/dev/testa-delta/settings.json");
  //  auto calibrationWidget = std::make_unique<CalibrationWidget>(nullptr);
  //  calibrationWidget->show();
  //  calibrationWidget->setData(Settings::getInstance().getSettingsMap()["settings"].toMap()["calibration"].toList());

  auto settingsDialog = std::make_unique<SettingsDialog>(Settings::getInstance().getSettingsMap()["settings"].toMap());

  Settings::getInstance().


  settingsDialog->show();
  app->exec();
}

// TEST_F(testBase, testPlot) {
//   auto g_plotWidget = std::make_shared<DataHolderPlotWidget>();
//   auto g_dataHolder = std::make_shared<DataHolder>();
//   Settings::getInstance().setFilePath("c:/dev/testa-delta/settings.json");

//  g_plotWidget->setDataHolder(g_dataHolder);
//  g_dataHolder->load(
//    "c:/dev/build-testa-delta-Desktop_Qt_6_3_1_MinGW_64_bit-Debug/data/autosave2022-08-19_17-46-27_r22.dat");

//  g_plotWidget->setSettings(Settings::getInstance().getSettingsMap().value("plotLines"));
//  g_plotWidget->show();
//  g_plotWidget->setGeometry(500, 300, 600, 400);
//  app->exec();
//}

// TEST_F(testBase, testCommunication) {
//   auto g_comm = std::make_shared<Rs485Comminication>();
//   Settings::getInstance().setFilePath("c:/dev/testa-delta/settings.json");
//   g_comm->setSettings(Settings::getInstance().getSettingsMap()["settings].toMap()["communication"]);
//   g_comm->establishConnection({});
//   QTimer::singleShot(4000, [&g_comm]() {
//     g_comm->setSets({1000});
//   });
//   app->exec();
// }
