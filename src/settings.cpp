#include "settings.h"

#include <QJsonDocument>
Settings::Settings(QObject *parent) : QObject{parent} {
  qInfo() << __PRETTY_FUNCTION__;
}

void Settings::setFilePath(QString const &path) {
  m_filePath = path;
  load();
}

void Settings::save(QVariant variant, QString const &name) {
  if (m_settingsMap.contains(name)) {
    m_settingsMap[name] = variant;
  } else {
    qWarning() << "No section" << name << "in settings file";
  }
  save();
}

void Settings::load() {
  if (m_filePath.isEmpty()) {
    qInfo() << __PRETTY_FUNCTION__ << "No settings file declared";
    return;
  }

  QByteArray rawData;
  auto       result = SettingsFileIO::readFromFile(m_filePath, rawData);

  if (-1 == result) {
    qCritical() << "Can't open settings file " << m_filePath;
    m_filePath = "settings.json";

    result = SettingsFileIO::readFromFile(":/res/defaultSettings", rawData);
    if (0 != result) {
      qCritical() << "Can't open default settings file. Terminate app";
      exit(1);
    }
  }

  // json -> variant
  auto doc      = QJsonDocument::fromJson(rawData);
  m_settingsMap = doc.toVariant().toMap();

  qDebug() << m_settingsMap;
}

void Settings::save() {
  if (m_filePath.isEmpty()) {
    qInfo() << __PRETTY_FUNCTION__ << "No settings file declared";
    return;
  }

  if (!SettingsFileIO::saveToFile(m_filePath, QJsonDocument::fromVariant(m_settingsMap).toJson())) {
    qCritical() << "Can't save settings to file";
    return;
  }
}

std::unique_ptr<Settings> Settings::m_instance{};
