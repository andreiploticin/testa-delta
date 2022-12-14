#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDebug>
#include <QFile>
#include <QObject>
#include <QVariant>
#include <memory>

class SettingsFileIO {
public:
  static unsigned saveToFile(QString filePath, QByteArray const &data) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
      return -1;
    }
    auto writtenByte = file.write(data);
    file.close();

    if (data.size() == writtenByte) {
      return 0;
    }

    return -1;
  }

  static unsigned readFromFile(QString filePath, QByteArray &data) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
      qInfo() << __PRETTY_FUNCTION__ << "Can not open settings file";
      return -1;
    }
    data = file.readAll();
    file.close();

    if (data.isEmpty()) {
      qInfo() << __PRETTY_FUNCTION__ << "Invalid settings format";
      return -1;
    }
    return 0;
  }
};

class Settings : public QObject {
  Q_OBJECT
public:
  ~Settings() override {
    qInfo() << __PRETTY_FUNCTION__;
  }
  static Settings &getInstance() {
    if (nullptr == m_instance.get()) {
      m_instance = std::unique_ptr<Settings>(new Settings(nullptr));
    }
    return *m_instance;
  }

  void        setFilePath(QString const &path);
  void        save(QVariant variant, QString const &name);
  QVariantMap getSettingsMap() const {
    return m_settingsMap;
  }

  int getFontSize() const {
    if (m_settingsMap.contains("settings") && m_settingsMap["settings"].toMap().contains("font_size")) {
      return m_settingsMap["settings"].toMap()["font_size"].toInt();
    }
    return 10;
  }

protected:
  QVariantMap m_settingsMap;

private:
  void load();
  void save();
  explicit Settings(QObject *parent = nullptr);
  QString                          m_filePath;
  static std::unique_ptr<Settings> m_instance;
signals:
};

#endif // SETTINGS_H
