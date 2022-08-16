#ifndef RS485COMMINICATION_H
#define RS485COMMINICATION_H

#include <QModbusRtuSerialClient>
#include <QTimer>

#include "icommunication.h"

class Rs485Comminication : public ICommunication {
public:
  Rs485Comminication(QObject *parent = nullptr);

  // ICommunication interface
public:
  int  establishConnection(CommunicationSetupOptions const &options) override;
  void closeConnection() override;
  void setSets(std::vector<double> newSets) override;
  void stopAll() override;
  int  getStatus() const override;
  int  getNumberOfControllers() const override;
  void setSettings(QVariant settings) override;

  std::vector<std::pair<double, double>> getLastValues() override;

private:
  std::vector<std::pair<double, double>> m_values;

  void                 makeRequest();
  void                 onReadReady();
  void                 onErrorOccured(QModbusDevice::Error error);
  void                 setModbusState(QModbusDevice::State state);
  bool                 handleErrorLimitation{false};
  QTimer              *m_requestLoopTimer{nullptr};
  QModbusClient       *m_modbusDevice = nullptr;
  QModbusDevice::State m_modbusState{QModbusDevice::State::UnconnectedState};
  uint32_t             m_errorCountTimeout{0U};
  uint32_t             m_errorCount{0U};
  QList<uint32_t>      m_addresses;
  QVariantMap          m_settings;
};

#endif // RS485COMMINICATION_H