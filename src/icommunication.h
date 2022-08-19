#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QDebug>
#include <QEventLoop>
#include <QObject>
#include <QRunnable>
#include <QThread>
#include <memory>
#include <QVariant>
#include <vector>

#include "controller.h"

struct CommunicationSetupOptions {};

class ICommunication : public QObject {
  Q_OBJECT
public:
  //  using TSetupOptions       = typename Traits::SetupOptions;
  //  using TController         = typename Traits::ControllerType;
  //  using CommunicationStatus = std::pair<int, std::vector<int>>;

  ICommunication(QObject *parent = nullptr) : QObject(parent) {
    qInfo() << __PRETTY_FUNCTION__ << QThread::currentThread();
  }
  virtual ~ICommunication() {
    qDebug() << __PRETTY_FUNCTION__ << QThread::currentThread();
    emit finished();
  }

  virtual int                                    establishConnection(CommunicationSetupOptions const &options) = 0;
  virtual void                                   closeConnection()                                             = 0;
  virtual void                                   setSets(std::vector<double> newSets)                          = 0;
  virtual void                                   stopAll()                                                     = 0;
  virtual int                                    getStatus() const                                             = 0;
  virtual std::vector<std::pair<double, double>> getLastValues()                                               = 0;
  virtual int                                    getNumberOfControllers() const                                = 0;
  virtual void                                   setSettings(QVariant settings)                                = 0;

signals:
  void connected();
  void finished();

protected:
  bool                                      m_connectedStatus{false};
  std::vector<std::unique_ptr<IController>> m_controllers;

  void setConnectedStatus(bool value) {
    if (value == m_connectedStatus) {
      return;
    }
    m_connectedStatus = value;
    if (m_connectedStatus) {
      emit connected();
    }
  }
};

#endif // COMMUNICATION_H
