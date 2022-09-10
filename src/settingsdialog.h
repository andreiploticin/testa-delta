#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QComboBox>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QtSerialPort/QSerialPortInfo>
#include <src/pidwidget.h>

#include "src/calibrationwidget.h"
#include "src/icommunication.h"

class SettingsDialog : public QDialog {
  Q_OBJECT
public:
  explicit SettingsDialog(QVariantMap                     settings,
                          std::shared_ptr<ICommunication> communication,
                          QWidget                        *parent = nullptr);

signals:
  void saveSettings(QVariantMap);

private:
  std::shared_ptr<ICommunication> m_communication;

  void               setVariant(QVariantMap settings);
  void               handleCommunicationRequest(uint8_t addsress, std::vector<uint16_t> registers);
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
  PidWidget         *m_pid1;
  PidWidget         *m_pid2;
  PidWidget         *m_pid3;
};

#endif // SETTINGSDIALOG_H
