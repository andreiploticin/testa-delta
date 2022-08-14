#include <gtest/gtest.h>

#include <QApplication>
#include <QDebug>
//#include <QModbusRtuSerialServer>
//#include <QSerialPort>
//#include <QSerialPortInfo>
#include <memory>

//#include "helpers.hpp"
//#include "src/process.hpp"
//#include "communication_helper.h"
//#include "src/mainwindow.h"
#include "src/dataholder.h"
#include "src/dataholderplot.h"

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

//TEST_F(testBase, testApp) {
//  auto g_testCommunication = std::make_shared<TestCommunication>();

//  auto appWindow = std::make_unique<MainWindow>(g_testCommunication);
//  appWindow->show();

//  app->exec();
//}

TEST_F(testBase, testPlot) {
  auto g_plot = std::make_shared<DataHolderPlot>();
  auto g_dataHolder = std::make_shared<DataHolder>();

  g_plot->setDataHolder(g_dataHolder);
  g_dataHolder->load("c:/dev/build-testa-delta-Desktop_Qt_6_3_1_MinGW_64_bit-Debug/data/autosave2022-08-12_22-26-19_r171.dat");

  g_plot->show();
  g_plot->setGeometry(500,300,600,400);
  app->exec();
}
