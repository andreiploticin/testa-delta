#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QDebug>
#include <QEventLoop>
#include <QObject>
#include <QRunnable>
#include <QThread>
#include <QVariant>
#include <memory>
#include <vector>

#include "controller.h"

struct CommunicationSetupOptions {};

class ICommunication : public QObject {
  Q_OBJECT
public:
  //  using TSetupOptions       = typename Traits::SetupOptions;
  //  using TController         = typename Traits::ControllerType;
  //  using CommunicationStatus = std::pair<int, std::vector<int>>;

  ICommunication(QObject *parent = nullptr)
      : QObject(parent) {
    qInfo() << __PRETTY_FUNCTION__ << QThread::currentThread();
  }
  virtual ~ICommunication() {
    qDebug() << __PRETTY_FUNCTION__ << QThread::currentThread();
    //    emit disconnected();
  }

  virtual int  establishConnection(CommunicationSetupOptions const &options) = 0;
  virtual void closeConnection()                                             = 0;
  virtual void setSets(std::vector<double> newSets)                          = 0;
  virtual void setCorrections(std::vector<double> newCorrections)            = 0;
  virtual void stopAll()                                                     = 0;
  virtual int  getStatus() const                                             = 0;
  virtual int  getNumberOfControllers() const                                = 0;
  virtual void setSettings(QVariant settings)                                = 0;

  virtual std::vector<std::pair<double, double>> getLastValues()  = 0;
  virtual std::vector<double>                    getCorrections() = 0;

  virtual void makeCustomRequest(uint8_t address, uint16_t offset, uint8_t size)                = 0;
  virtual void sendRegisters(uint8_t address, uint16_t offset, std::vector<uint16_t> registers) = 0;

signals:
  void connectedChanged(bool);
  void requestResult(uint8_t, std::vector<uint16_t>);

protected:
  bool                                      m_connectedStatus{false};
  std::vector<std::unique_ptr<IController>> m_controllers;

  void setConnectedStatus(bool value) {
    if (value == m_connectedStatus) {
      return;
    }
    m_connectedStatus = value;
    connectedChanged(value);
  }
};

#endif // COMMUNICATION_H
