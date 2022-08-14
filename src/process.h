#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <QTimer>
#include <thread>

#include "communication.h"
#include "iprocess.h"

class Process : public IProcess {
public:
  Process(std::shared_ptr<ICommunication> com, QObject *parent = nullptr);
  ~Process() override {
    qInfo() << __PRETTY_FUNCTION__ << QThread::currentThread();
  }
  void     restart(std::vector<double> newSets) override;
  void     stop() override;
  void     run() override;
  uint64_t a = 0;

private:
  bool                         m_runStatus{false};
  void                         setRunStatus(bool value);
  std::shared_ptr<IDataHolder> m_dataHolder;
  QTimer                      *m_runTimer{nullptr};
};

#endif // PROCESS_HPP
