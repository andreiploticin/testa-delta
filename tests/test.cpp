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
#include "src/settingsdialog.h"
#include "src/utils.h"

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

TEST_F(testBase, testApp) {
  auto g_testCommunication = std::make_shared<TestCommunication>();
  Settings::getInstance().setFilePath("c:/dev/testa-delta/settings.json");
  auto appWindow = std::make_unique<MainWindow>(g_testCommunication);
  appWindow->show();

  app->exec();
}

// TEST_F(testBase, testCorrection) {
//   Settings::getInstance().setFilePath("c:/dev/testa-delta/settings.json");
//   //  auto calibrationWidget = std::make_unique<CalibrationWidget>(nullptr);
//   //  calibrationWidget->show();
//   //
//   calibrationWidget->setData(Settings::getInstance().getSettingsMap()["settings"].toMap()["calibration"].toList());

//  auto settingsDialog =
//  std::make_unique<SettingsDialog>(Settings::getInstance().getSettingsMap()["settings"].toMap());

//  QObject::connect(settingsDialog.get(), &SettingsDialog::saveSettings, [this](QVariantMap map) {
//    Settings::getInstance().save(map, "settings");
//  });
//  settingsDialog->show();
//  app->exec();
//}

//TEST_F(testBase, testPlot) {
//  auto g_plotWidget = std::make_shared<DataHolderPlotWidget>();
//  auto g_dataHolder = std::make_shared<DataHolder>(6);
//  Settings::getInstance().setFilePath("c:/dev/testa-delta/settings.json");

//  g_plotWidget->setDataHolder(g_dataHolder);
//  g_dataHolder->load(
//    "c:/dev/build-testa-delta-Desktop_Qt_6_3_1_MinGW_64_bit-Debug/data/autosave2022-08-20_17-14-57_r514.dat");

//  g_plotWidget->setSettings(Settings::getInstance().getSettingsMap().value("plotLines"));
//  g_plotWidget->show();
//  g_plotWidget->setGeometry(500, 300, 600, 400);
//  app->exec();
//}

//TEST_F(testBase, testLegend) {
//  auto g_plotWidget = std::make_shared<DataHolderPlotWidget>();
//  auto g_dataHolder = std::make_shared<DataHolder>(6);
//  Settings::getInstance().setFilePath("c:/dev/testa-delta/settings.json");

//  g_plotWidget->setDataHolder(g_dataHolder);
//  g_dataHolder->load(
//    "c:/dev/build-testa-delta-Desktop_Qt_6_3_1_MinGW_64_bit-Debug/data/autosave2022-08-20_17-14-57_r514.dat");

//  g_plotWidget->setSettings(Settings::getInstance().getSettingsMap().value("plotLines"));
//  g_plotWidget->show();
//  g_plotWidget->setGeometry(500, 300, 600, 400);
//  app->exec();
//}


// TEST_F(testBase, testCommunication) {
//   auto g_comm = std::make_shared<Rs485Comminication>();
//   Settings::getInstance().setFilePath("c:/dev/testa-delta/settings.json");
//   g_comm->setSettings(Settings::getInstance().getSettingsMap()["settings"].toMap()["communication"]);
//   g_comm->establishConnection({});
//   QTimer::singleShot(4000, [&g_comm]() {
//     g_comm->setSets({1000});
//   });
//   app->exec();
// }

/*
TEST(utils, correction) {
  QVector<double> temps{100, 200, 300, 400, 500};
  QVector<double> cors{-1, 0, 1, 10, -20};
  EXPECT_EQ(49, correct(50, temps, cors));
  EXPECT_EQ(98, correct(99, temps, cors));
  EXPECT_EQ(250.5, correct(250, temps, cors));
  EXPECT_EQ(445, correct(450, temps, cors));
  EXPECT_EQ(99, correct(100, temps, cors));
  EXPECT_EQ(200, correct(200, temps, cors));
  EXPECT_EQ(301, correct(300, temps, cors));
  EXPECT_EQ(410, correct(400, temps, cors));
  EXPECT_EQ(480, correct(500, temps, cors));
  EXPECT_EQ(580, correct(600, temps, cors));

  QVariantMap  map{};
  QVariantList t(temps.cbegin(), temps.cend());
  QVariantList c(cors.cbegin(), cors.cend());
  map["data_temp"]    = t;
  map["data_correct"] = c;
  EXPECT_EQ(445, correct(450, map));

  temps = {100, 200};
  cors  = {1, 0};
  EXPECT_EQ(51, correct(50, temps, cors));
  EXPECT_EQ(101, correct(100, temps, cors));
  EXPECT_EQ(150.5, correct(150, temps, cors));
  EXPECT_EQ(200, correct(200, temps, cors));
  EXPECT_EQ(250, correct(250, temps, cors));

  temps = {100};
  cors  = {1};
  EXPECT_EQ(51, correct(50, temps, cors));
  EXPECT_EQ(101, correct(100, temps, cors));
  EXPECT_EQ(151, correct(150, temps, cors));
  EXPECT_EQ(201, correct(200, temps, cors));
  EXPECT_EQ(251, correct(250, temps, cors));

  temps = {};
  cors  = {};
  EXPECT_EQ(50, correct(50, temps, cors));
  EXPECT_EQ(100, correct(100, temps, cors));
  EXPECT_EQ(150, correct(150, temps, cors));
  EXPECT_EQ(200, correct(200, temps, cors));
  EXPECT_EQ(250, correct(250, temps, cors));

  QVariantList t2(temps.cbegin(), temps.cend());
  QVariantList c2(cors.cbegin(), cors.cend());
  map["data_temp"]    = t2;
  map["data_correct"] = c2;
  qDebug() << map;
  EXPECT_EQ(450, correct(450, map));

  temps = {100, 100, 200};
  cors  = {1, 1, 0};
  EXPECT_EQ(51, correct(50, temps, cors));
  EXPECT_EQ(101, correct(100, temps, cors));
  EXPECT_EQ(150.5, correct(150, temps, cors));
  EXPECT_EQ(200, correct(200, temps, cors));
  EXPECT_EQ(250, correct(250, temps, cors));
}
*/
