#include "process.h"

#include "dataholder.h"
#include "settings.h"

Process::Process(std::shared_ptr<ICommunication> com, QObject *parent)
    : IProcess(std::move(com), parent) {
  qInfo() << __PRETTY_FUNCTION__ << QThread::currentThread();

  m_runTimer = new QTimer(this);
  connect(m_runTimer, &QTimer::timeout, this, [this] {
    if (!m_stopRun && m_runStatus) {
      if ((nullptr != m_dataHolder.get()) && (nullptr != m_communication.get())) {
        auto            lastPointsPairs = m_communication->getLastValues();
        auto            corrections     = m_communication->getCorrections();
        QVector<double> lastPointVector{};
        for (int i{0}; i < lastPointsPairs.size(); ++i) {
          auto const &pair = lastPointsPairs[i];
          lastPointVector.push_back(pair.first + corrections[i]);
          lastPointVector.push_back(pair.second);
        }
        m_dataHolder->addDataPoint(QDateTime::currentDateTime().toSecsSinceEpoch(), lastPointVector);
      }
    }
  });

  connect(m_communication.get(), &ICommunication::connectedChanged, this, [this](bool connectionStatus) {
    if (!connectionStatus) {
      stop();
    }
  });
}

void Process::restart(std::vector<double> newSets) {
  if (!m_runStatus) {
    m_dataHolder = std::make_shared<DataHolder>(2 * newSets.size());

    auto autosave_period = Settings::getInstance().getSettingsMap()["settings"].toMap()["autosave_period"].toUInt();
    m_dataHolder->setAutoSave(true, 60 * autosave_period);
  }

  this->m_communication->setSets(newSets);
  auto period = Settings::getInstance().getSettingsMap()["settings"].toMap()["data_acquisition_frequency"].toUInt();
  m_runTimer->start(1000 * period);
  setRunStatus(true);
}

void Process::stop() {
  if (nullptr != m_dataHolder.get()) {
    m_dataHolder->setAutoSave(false);
    m_dataHolder->save("aaa", false);
  }
  this->m_communication->stopAll();
  m_runTimer->stop();
  setRunStatus(false);
}

void Process::run() {}

void Process::setRunStatus(bool value) {
  if (m_runStatus == value) {
    return;
  }
  m_runStatus = value;
  emit runStatusChanged(m_runStatus);
}
