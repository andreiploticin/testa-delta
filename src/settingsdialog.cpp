#include "settingsdialog.h"
#include "QSerialPortInfo"
#include <QLayout>
#include <QTimer>

SettingsDialog::SettingsDialog(QVariantMap settings, std::shared_ptr<ICommunication> communication, QWidget *parent)
    : QDialog{parent, Qt::Dialog}, m_communication{communication} {
  setWindowTitle("Настройки");

  m_calWidget       = new CalibrationWidget(this);
  m_deltaAddress1   = new QLineEdit("", this);
  m_deltaAddress2   = new QLineEdit("", this);
  m_deltaAddress3   = new QLineEdit("", this);
  m_timeoutMax      = new QLineEdit("", this);
  m_errorMax        = new QLineEdit("", this);
  m_acqFreq         = new QLineEdit("", this);
  m_autosaveMinutes = new QLineEdit("", this);
  m_fontSize        = new QSpinBox(this);
  m_comPort         = new QComboBox(this);
  m_saveBtn         = new QPushButton("Сохранить", this);
  m_closeBtn        = new QPushButton("Закрыть", this);

  m_fontSize->setRange(8,20);
  m_fontSize->setSingleStep(1);

  auto mainLay = new QHBoxLayout(this);

  auto systemLay = new QVBoxLayout();

  auto formLay = new QFormLayout();
  systemLay->addLayout(formLay);

  formLay->addRow("COM-порт для соединения", m_comPort);
  formLay->addRow("Адрес регулятора верхней зоны", m_deltaAddress1);
  formLay->addRow("Адрес регулятора средней зоны", m_deltaAddress2);
  formLay->addRow("Адрес регулятора нижней зоны", m_deltaAddress3);
  formLay->addRow("Частота сохранения данных, сек", m_acqFreq);
  formLay->addRow("Частота автосохранения данных, мин", m_autosaveMinutes);
  formLay->addRow("Допустимое число ошибок соединения", m_errorMax);
  formLay->addRow("Допустимое число ошибок соединения по таймауту", m_timeoutMax);
  formLay->addRow("Размер шрифта интерфейса, пт.", m_fontSize);

  systemLay->addWidget(m_calWidget);

  auto btnLay = new QHBoxLayout();
  systemLay->addLayout(btnLay);

  btnLay->addWidget(m_saveBtn);
  btnLay->addWidget(m_closeBtn);

  auto pidLay = new QVBoxLayout();
  m_pid1      = new PidWidget("Настройки регулятора верхней зоны");
  m_pid2      = new PidWidget("Настройки регулятора средней зоны");
  m_pid3      = new PidWidget("Настройки регулятора нижней зоны");

  pidLay->addWidget(m_pid1);
  pidLay->addStretch();
  pidLay->addWidget(m_pid2);
  pidLay->addStretch();
  pidLay->addWidget(m_pid3);

  mainLay->addLayout(systemLay);
  mainLay->addLayout(pidLay);

  connect(m_closeBtn, &QPushButton::clicked, this, &QDialog::reject);
  connect(m_saveBtn, &QPushButton::clicked, this, [this]() {
    emit saveSettings(getSettings());
  });

  if (!settings.empty()) {
    setVariant(settings);
  }

  connect(m_communication.get(), &ICommunication::requestResult, this, &SettingsDialog::handleCommunicationRequest);

  connect(m_pid1, &PidWidget::getFromController, this, [this]() {
    m_communication->makeCustomRequest(m_pid1->getAddress(), 0x1007, 5);
  });
  connect(m_pid2, &PidWidget::getFromController, this, [this]() {
    m_communication->makeCustomRequest(m_pid2->getAddress(), 0x1007, 5);
  });
  connect(m_pid3, &PidWidget::getFromController, this, [this]() {
    m_communication->makeCustomRequest(m_pid3->getAddress(), 0x1007, 5);
  });

  connect(m_pid1, &PidWidget::sendToController, m_communication.get(), &ICommunication::sendRegisters);
  connect(m_pid2, &PidWidget::sendToController, m_communication.get(), &ICommunication::sendRegisters);
  connect(m_pid3, &PidWidget::sendToController, m_communication.get(), &ICommunication::sendRegisters);

  QTimer::singleShot(100, [this](){m_communication->makeCustomRequest(m_pid1->getAddress(), 0x1007, 5);});
  QTimer::singleShot(200, [this](){m_communication->makeCustomRequest(m_pid2->getAddress(), 0x1007, 5);});
  QTimer::singleShot(300, [this](){m_communication->makeCustomRequest(m_pid3->getAddress(), 0x1007, 5);});
}

void SettingsDialog::setVariant(QVariantMap settings) {
  auto communication = settings["communication"].toMap();
  if (!communication.empty()) {
    auto delta_address = communication["delta_address"];
    if (delta_address.isValid()) {
      auto delta_address_list = delta_address.toList();
      if (delta_address_list.size() > 0) {
        m_deltaAddress1->setText(QString::number(delta_address_list[0].toUInt()));
        m_pid1->setAddress(delta_address_list[0].toUInt());
      }
      if (delta_address_list.size() > 1) {
        m_deltaAddress2->setText(QString::number(delta_address_list[1].toUInt()));
        m_pid2->setAddress(delta_address_list[1].toUInt());
      }
      if (delta_address_list.size() > 2) {
        m_deltaAddress3->setText(QString::number(delta_address_list[2].toUInt()));
        m_pid3->setAddress(delta_address_list[2].toUInt());
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
      for(auto & port : QSerialPortInfo::availablePorts()) {
          auto portName = port.portName();
          if (!coms.contains(portName)) {
            coms.push_back(portName);
          }
      }
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
  if (settings.contains("font_size")) {
    m_fontSize->setValue(settings["font_size"].toInt());
  }
}

void SettingsDialog::handleCommunicationRequest(uint8_t addsress, std::vector<uint16_t> registers) {
  if (registers.size() < 6) {
    return;
  }
  QVector<uint8_t> currAddresses{m_pid1->getAddress(), m_pid2->getAddress(), m_pid3->getAddress()};

  if (currAddresses.contains(addsress)) {
    QList<PidWidget *> list = {m_pid1, m_pid2, m_pid3};
    list[currAddresses.indexOf(addsress)]->setRegisters(registers);
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
  settings["font_size"]                  = m_fontSize->value();
  settings["autosave_period"]            = m_autosaveMinutes->text().toUInt();
  settings["calibration"]                = m_calWidget->getData();

  return settings;
}
