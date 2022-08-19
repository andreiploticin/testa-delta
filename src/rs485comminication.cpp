#include "rs485comminication.h"

#include <QModbusDataUnit>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "settings.h"

Rs485Comminication::Rs485Comminication(QObject *parent) : ICommunication(parent) {
  m_values           = {};
  m_modbusDevice     = new QModbusRtuSerialClient(this);
  m_requestLoopTimer = new QTimer(this);

  connect(m_requestLoopTimer, &QTimer::timeout, this, &Rs485Comminication::makeRequest);
  connect(m_modbusDevice, &QModbusClient::stateChanged, this, &Rs485Comminication::setModbusState);
  connect(m_modbusDevice, &QModbusDevice::errorOccurred, this, &Rs485Comminication::onErrorOccured);
}

int Rs485Comminication::establishConnection(CommunicationSetupOptions const &options) {
  // clear errors
  handleErrorLimitation = false;
  m_errorCount          = 0U;
  m_errorCountTimeout   = 0U;

  auto portName
    = Settings::getInstance().getSettingsMap()["settings"].toMap()["communication"].toMap()["serial_port"].toString();
  m_modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter, portName);
  m_modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::NoParity);
  m_modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, QSerialPort::Baud9600);
  m_modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QSerialPort::Data8);
  m_modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QSerialPort::OneStop);

  m_modbusDevice->setTimeout(1000);
  m_modbusDevice->setNumberOfRetries(3);

  m_modbusDevice->connectDevice();

  m_requestLoopTimer->start(2000);

  return 0;
}

void Rs485Comminication::closeConnection() {
  stopAll();
  m_modbusDevice->disconnectDevice();
}

void Rs485Comminication::setSets(std::vector<double> newSets) {
  if (newSets.size() != m_addresses.size()) {
    qInfo() << "newSets.size() != m_addresses.size()";
    return;
  }
  for (int i = 0; i < newSets.size(); ++i) {
    auto            device = m_addresses[i];
    QModbusDataUnit req{QModbusDataUnit::HoldingRegisters, 0x1001, 1};
    req.setValue(0, 10 * newSets[i]);
    auto replay = m_modbusDevice->sendWriteRequest(req, device);
    if (nullptr != replay) {
      connect(replay, &QModbusReply::finished, this, &Rs485Comminication::onReadReady);
    }
  }
}

void Rs485Comminication::stopAll() {
  m_requestLoopTimer->stop();
}

int Rs485Comminication::getStatus() const {
  return static_cast<int>(QModbusDevice::State::ConnectedState == m_modbusState);
}

std::vector<std::pair<double, double>> Rs485Comminication::getLastValues() {
  return m_values;
}

void Rs485Comminication::onReadReady() {
  auto reply = qobject_cast<QModbusReply *>(sender());
  if (nullptr == reply) {
    return;
  }

  if (reply->error() == QModbusDevice::NoError) {
    const QModbusDataUnit unit = reply->result();
    for (qsizetype i = 0, total = unit.valueCount(); i < total; ++i) {
      const QString entry = tr("Device: %1, Address: %2, Value: %3")
                              .arg(reply->serverAddress())
                              .arg(unit.startAddress() + i)
                              .arg(0.1 * unit.value(i));
      qInfo() << entry;
      if (m_addresses.contains(reply->serverAddress())) {
        auto &pair = m_values[m_addresses.indexOf(reply->serverAddress())];
        if (0x1000 == (unit.startAddress() + i)) {
          pair.first = 0.1 * unit.value(i);
        } else if (0x1001 == (unit.startAddress() + i)) {
          pair.second = 0.1 * unit.value(i);
        }
      }
    }
  } else {
    onErrorOccured(reply->error());
  }
  //  if (reply->error() == QModbusDevice::ProtocolError) {
  //     qInfo() << tr("Read response error: %1 (Modbus exception: 0x%2)")
  //                  .arg(reply->errorString())
  //                  .arg(reply->rawResult().exceptionCode(), -1, 16);
  //   } else {
  //     qInfo() << tr("Read response error: %1 (code: 0x%2)").arg(reply->errorString()).arg(reply->error(), -1, 16);
  //   }
  reply->deleteLater();
}

void Rs485Comminication::onErrorOccured(QModbusDevice::Error error) {
  if (QModbusDevice::Error::NoError == error) {
    return;
  }

  // no need to handle error if already in error limiting state
  if (handleErrorLimitation) {
    return;
  }

  ++m_errorCount;
  qInfo() << error;

  if (QModbusDevice::Error::TimeoutError == error) {
    if ((++m_errorCountTimeout) >= m_settings["max_timeout_errors"].toUInt()) {
      qInfo() << "RS-485 communication timeout error limit reached:" << m_errorCountTimeout;
      handleErrorLimitation = true;
      closeConnection();
      return;
    }
  }
  if (m_errorCount >= m_settings["max_errors"].toUInt()) {
    qInfo() << "RS-485 communication error limit reached:" << m_errorCount;
    handleErrorLimitation = true;
    closeConnection();
    return;
  }
}

void Rs485Comminication::makeRequest() {
  for (auto device : m_addresses) {
    QModbusDataUnit req{QModbusDataUnit::HoldingRegisters, 0x1000, 2};
    auto            replay = m_modbusDevice->sendReadRequest(req, device);
    if (nullptr != replay) {
      connect(replay, &QModbusReply::finished, this, &Rs485Comminication::onReadReady);
    }
  }
}

void Rs485Comminication::setModbusState(QModbusDevice::State state) {
  if (QModbusDevice::ClosingState == state || QModbusDevice::ConnectingState == state) {
    return;
  }
  if (state == m_modbusState) {
    return;
  }
  m_modbusState = state;

  if (QModbusDevice::ConnectedState == m_modbusState) {
    qInfo() << __PRETTY_FUNCTION__ << "Connected";
    emit connected();
  }

  if (QModbusDevice::UnconnectedState == m_modbusState) {
    qInfo() << __PRETTY_FUNCTION__ << "Disconnected";
    emit connected();
  }
}

int Rs485Comminication::getNumberOfControllers() const {
  return m_addresses.size();
}

void Rs485Comminication::setSettings(QVariant settings) {
  if (QModbusDevice::ConnectedState == m_modbusState) {
    qInfo() << "Can not apply settings during established connection";
    return;
  }
  m_settings = settings.toMap();

  m_addresses.clear();
  m_values.clear();
  auto addresses = m_settings["delta_address"].toList();
  for (auto const &item : addresses) {
    m_addresses.push_back(item.toUInt());
    m_values.push_back({0, 0});
  }
}
