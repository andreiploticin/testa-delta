#ifndef IPROCESS_H
#define IPROCESS_H

#include <memory>
#include <thread>
#include <vector>

#include "src/communication.h"

class IProcess {
public:
  IProcess(std::shared_ptr<ICommunication> communication) : m_communication(std::move(communication)) {
    qInfo() << __PRETTY_FUNCTION__ << QThread::currentThread();
    m_thread = std::thread(IProcess::run, this);
  }
  virtual void restart(std::vector<double> newSets) = 0;
  virtual void stop()                               = 0;
  virtual ~IProcess() {
    qInfo() << __PRETTY_FUNCTION__ << QThread::currentThread();
    stopRun();
    if (m_thread.joinable()) {
      m_thread.join();
    }
  }
  void stopRun() {
    m_stopRun = true;
  }

protected:
  virtual void                    run() = 0;
  std::shared_ptr<ICommunication> m_communication;
  bool                            m_stopRun = false;

private:
  std::thread m_thread;
};

#endif // IPROCESS_H
