#include "settingsdialog.h"

#include <QLayout>

SettingsDialog::SettingsDialog(QVariantMap settings, QWidget *parent) : QDialog{parent, Qt::Dialog} {
  setWindowTitle("Настройки");

  m_calWidget       = new CalibrationWidget(this);
  m_deltaAddress1   = new QLineEdit("", this);
  m_deltaAddress2   = new QLineEdit("", this);
  m_deltaAddress3   = new QLineEdit("", this);
  m_timeoutMax      = new QLineEdit("", this);
  m_errorMax        = new QLineEdit("", this);
  m_acqFreq         = new QLineEdit("", this);
  m_autosaveMinutes = new QLineEdit("", this);
  m_comPort         = new QComboBox(this);
  m_saveBtn         = new QPushButton("Сохранить", this);
  m_closeBtn        = new QPushButton("Закрыть", this);

  auto mainLay = new QVBoxLayout(this);

  auto formLay = new QFormLayout();
  mainLay->addLayout(formLay);

  formLay->addRow("COM-порт для соединения", m_comPort);
  formLay->addRow("Адрес регулятора верней зоны", m_deltaAddress1);
  formLay->addRow("Адрес регулятора средней зоны", m_deltaAddress2);
  formLay->addRow("Адрес регулятора нижней зоны", m_deltaAddress3);
  formLay->addRow("Частота сохранения данных, сек", m_acqFreq);
  formLay->addRow("Частота автосохранения данных, мин", m_autosaveMinutes);
  formLay->addRow("Допустимое число ошибок соединения", m_errorMax);
  formLay->addRow("Допустимое число ошибок соединения по таймауту", m_timeoutMax);

  mainLay->addWidget(m_calWidget);

  auto btnLay = new QHBoxLayout();
  mainLay->addLayout(btnLay);

  btnLay->addWidget(m_saveBtn);
  btnLay->addWidget(m_closeBtn);

  connect(m_closeBtn, &QPushButton::clicked, this, &QDialog::reject);
  connect(m_saveBtn, &QPushButton::clicked, this, [this]() {
    emit getSettings();
    qDebug() << getSettings();
  });

  if (!settings.empty()) {
    setVariant(settings);
  }
}

void SettingsDialog::setVariant(QVariantMap settings) {
  auto communication = settings["communication"].toMap();
  if (!communication.empty()) {
    auto delta_address = communication["delta_address"];
    if (delta_address.isValid()) {
      auto delta_address_list = delta_address.toList();
      if (delta_address_list.size() > 0) {
        m_deltaAddress1->setText(QString::number(delta_address_list[0].toUInt()));
      }
      if (delta_address_list.size() > 1) {
        m_deltaAddress2->setText(QString::number(delta_address_list[1].toUInt()));
      }
      if (delta_address_list.size() > 2) {
        m_deltaAddress3->setText(QString::number(delta_address_list[2].toUInt()));
      }
    }
    if (communication.contains("max_timeout_errors")) {
      m_timeoutMax->setText(QString::number(communication["max_timeout_errors"].toUInt()));
    }
    if (communication.contains("max_errors")) {
      m_errorMax->setText(QString::number(communication["max_errors"].toUInt()));
    }
    if (communication.contains("serial_port")) {
      auto        curCom = communication["serial_port"].toString();
      QStringList coms{"COM1", "COM2", "COM3", "COM4", "COM5"};
      if (!coms.contains(curCom)) {
        coms.push_back(curCom);
      }
      m_comPort->addItems(coms);
      m_comPort->setCurrentText(curCom);
    }
  }

  if (settings.contains("data_acquisition_frequency")) {
    m_acqFreq->setText(QString::number(settings["data_acquisition_frequency"].toUInt()));
  }
  if (settings.contains("autosave_period")) {
    m_autosaveMinutes->setText(QString::number(settings["autosave_period"].toUInt()));
  }
  if (settings.contains("calibration")) {
    m_calWidget->setData(settings["calibration"].toList());
  }
}

QVariantMap SettingsDialog::getSettings() const {
  QVariantList addresses{};
  if (!m_deltaAddress1->text().isEmpty()) {
    addresses.push_back(m_deltaAddress1->text().toUInt());
  }
  if (!m_deltaAddress2->text().isEmpty()) {
    addresses.push_back(m_deltaAddress2->text().toUInt());
  }
  if (!m_deltaAddress3->text().isEmpty()) {
    addresses.push_back(m_deltaAddress3->text().toUInt());
  }

  QVariantMap communication{};

  communication["delta_address"] = addresses;

  communication["max_timeout_errors"] = m_timeoutMax->text().toUInt();
  communication["max_errors"]         = m_errorMax->text().toUInt();
  communication["serial_port"]        = m_comPort->currentText();

  QVariantMap settings{};

  settings["communication"] = communication;

  settings["data_acquisition_frequency"] = m_acqFreq->text().toUInt();
  settings["autosave_period"]            = m_autosaveMinutes->text().toUInt();
  settings["calibration"]                = m_calWidget->getData();

  return settings;
}
