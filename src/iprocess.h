#ifndef IPROCESS_H
#define IPROCESS_H

#include <QObject>
#include <memory>
#include <vector>

#include "src/icommunication.h"
#include "src/idataholder.h"

class IProcess : public QObject {
  Q_OBJECT
public:
  IProcess(std::shared_ptr<ICommunication> communication, QObject *parent = nullptr)
      : QObject(parent), m_communication(std::move(communication)) {
    qInfo() << __PRETTY_FUNCTION__ << QThread::currentThread();
  }
  virtual void restart(std::vector<double> newSets) = 0;
  virtual void stop()                               = 0;
  virtual ~IProcess() {
    qInfo() << __PRETTY_FUNCTION__ << QThread::currentThread();
    stopRun();
  }
  void stopRun() {
    m_stopRun = true;
  }
  void setDataHolder(std::shared_ptr<IDataHolder> dataHolder) {
    m_dataHolder = std::move(dataHolder);
  }
  std::shared_ptr<IDataHolder> getDataHolderPtr() const {
    return m_dataHolder;
  }

signals:
  void runStatusChanged(bool);

protected:
  std::shared_ptr<IDataHolder>    m_dataHolder;
  virtual void                    run() = 0;
  std::shared_ptr<ICommunication> m_communication;
  bool                            m_stopRun = false;
};

#endif // IPROCESS_H
