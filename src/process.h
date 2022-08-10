#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "communication.h"
#include "iprocess.h"

class Process : public IProcess {
public:
  Process(std::shared_ptr<ICommunication> com) : IProcess(std::move(com)) {
    qInfo() << __PRETTY_FUNCTION__ << QThread::currentThread();
  }
  ~Process() override {
    qInfo() << __PRETTY_FUNCTION__ << QThread::currentThread();
  }
  void     restart(std::vector<double> newSets) override;
  void     stop() override;
  void     run() override;
  uint64_t a = 0;
};

void Process::restart(std::vector<double> newSets) {
  this->m_communication->setSets(newSets);
}

void Process::stop() {
  this->m_communication->stopAll();
}

void Process::run() {
  qInfo() << __PRETTY_FUNCTION__ << QThread::currentThread();
  while (!this->m_stopRun) {
    a++;
  }
}

#endif // PROCESS_HPP
