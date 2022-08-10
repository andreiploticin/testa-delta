#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QDebug>
#include <QEventLoop>
#include <QObject>
#include <QRunnable>
#include <QThread>
#include <memory>
#include <tuple>
#include <vector>

#include "controller.h"

struct CommunicationSetupOptions {};

class ICommunication
    : public QObject
    , public QRunnable {
  Q_OBJECT
public:
  //  using TSetupOptions       = typename Traits::SetupOptions;
  //  using TController         = typename Traits::ControllerType;
  //  using CommunicationStatus = std::pair<int, std::vector<int>>;

  ICommunication() : QObject(nullptr) {
    qInfo() << __PRETTY_FUNCTION__ << QThread::currentThread();
  }
  virtual ~ICommunication() {
    qDebug() << __PRETTY_FUNCTION__ << QThread::currentThread();
    emit finished();
  }

  void run() override;

  virtual int                                    setup(CommunicationSetupOptions const &options) = 0;
  virtual void                                   setSets(std::vector<double> newSets)            = 0;
  virtual void                                   stopAll()                                       = 0;
  virtual int                                    getStatus()                                     = 0;
  virtual std::vector<std::pair<double, double>> getLastValues()                                 = 0;
  virtual int                                    getNumberOfControllers() const                  = 0;

signals:
  void connected();
  void finished();

protected:
  std::vector<std::unique_ptr<IController>> m_controllers;
};

inline void ICommunication::run() {
  qInfo() << __PRETTY_FUNCTION__ << QThread::currentThread();
  auto communicationLoop = std::make_unique<QEventLoop>();
  connect(this, &ICommunication::finished, communicationLoop.get(), &QEventLoop::quit);
  communicationLoop->exec();
}
#endif // COMMUNICATION_H
