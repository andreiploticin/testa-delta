#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <QTimer>
#include <thread>

#include "icommunication.h"
#include "iprocess.h"

class Process : public IProcess {
public:
  Process(std::shared_ptr<ICommunication> com, QObject *parent = nullptr);
  ~Process() override {
    qInfo() << __PRETTY_FUNCTION__ << QThread::currentThread();
  }
  void restart(std::vector<double> newSets) override;
  void stop() override;
  void run() override;
  int  getStatus() const override {
    return static_cast<int>(m_runStatus);
  }
  std::shared_ptr<IDataHolder> getDataHolder() const {
    return m_dataHolder;
  }

private:
  bool                         m_runStatus{false};
  void                         setRunStatus(bool value);
  QTimer                      *m_runTimer{nullptr};
};

#endif // PROCESS_HPP
