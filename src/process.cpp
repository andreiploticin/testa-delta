#include "process.h"

#include "dataholder.h"

Process::Process(std::shared_ptr<ICommunication> com, QObject *parent) : IProcess(std::move(com), parent) {
  qInfo() << __PRETTY_FUNCTION__ << QThread::currentThread();

  m_runTimer = new QTimer(this);
  connect(m_runTimer, &QTimer::timeout, this, [this] {
    if (!m_stopRun && m_runStatus) {
      if ((nullptr != m_dataHolder.get()) && (nullptr != m_communication.get())) {
        auto                lastPointsPairs = m_communication->getLastValues();
        QVector<double> lastPointVector{};
        for (auto const &pair : lastPointsPairs) {
          lastPointVector.push_back(pair.first);
          lastPointVector.push_back(pair.second);
        }
        m_dataHolder->addDataPoint(QDateTime::currentDateTime().toSecsSinceEpoch(), lastPointVector);
      }
    }
  });
}

void Process::restart(std::vector<double> newSets) {
  if (!m_runStatus) {
    m_dataHolder = std::make_shared<DataHolder>();
    m_dataHolder->setAutoSave(true, 3);
  }

  this->m_communication->setSets(newSets);
  m_runTimer->start(1000);
  setRunStatus(true);
}

void Process::stop() {
  m_dataHolder->setAutoSave(false);
  this->m_communication->stopAll();
  m_runTimer->stop();
  setRunStatus(false);
}

void Process::run() {
}

void Process::setRunStatus(bool value) {
  if (m_runStatus == value) {
    return;
  }
  m_runStatus = value;
  emit runStatusChanged(m_runStatus);
}
