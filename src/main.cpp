#include <QApplication>
#include <QList>
#include <QPushButton>
#include <QVariant>

#include "src/mainwindow.h"
#include "src/rs485comminication.h"
#include "src/settings.h"

int main(int argc, char **argv) {
  auto app = std::make_unique<QApplication>(argc, argv);
  qSetMessagePattern("%{function}(line %{line}): %{message}");
  auto g_comm = std::make_shared<Rs485Comminication>();
  auto      binaryFullDirPath    = QCoreApplication::applicationDirPath();
  auto      settingsPath         = binaryFullDirPath + "/settings.json";

  Settings::getInstance().setFilePath(settingsPath);
  auto appWindow = std::make_unique<MainWindow>(g_comm);
  appWindow->show();
  return app->exec();
}
