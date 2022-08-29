#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QComboBox>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QtSerialPort/QSerialPortInfo>

#include "src/calibrationwidget.h"

class SettingsDialog : public QDialog {
  Q_OBJECT
public:
  explicit SettingsDialog(QVariantMap settings, QWidget *parent = nullptr);

signals:
  void saveSettings(QVariantMap);

private:
  void               setVariant(QVariantMap settings);
  QVariantMap        getSettings() const;
  CalibrationWidget *m_calWidget;
  QLineEdit         *m_deltaAddress1;
  QLineEdit         *m_deltaAddress2;
  QLineEdit         *m_deltaAddress3;
  QLineEdit         *m_timeoutMax;
  QLineEdit         *m_errorMax;
  QLineEdit         *m_acqFreq;
  QLineEdit         *m_autosaveMinutes;
  QComboBox         *m_comPort;
  QPushButton       *m_saveBtn;
  QPushButton       *m_closeBtn;
};

#endif // SETTINGSDIALOG_H
