#ifndef COMMUNICATION_HELPER_H
#define COMMUNICATION_HELPER_H

#include <QTimer>
#include <random>

#include "src/communication.h"

static std::mt19937                           generator(0);
static std::uniform_real_distribution<double> distribution(1000, 1200);

class TestCommunication : public ICommunication {
  // ICommunication interface
public:
  TestCommunication() : ICommunication() {
  }

  int setup(CommunicationSetupOptions const &options) override {
    QTimer::singleShot(1000, [this]() {
      this->setConnectedStatus(true);
      m_updateTimer = new QTimer(this);
      connect(
        m_updateTimer,
        &QTimer::timeout,
        this,
        [this]() {
          for (int i = 0; i < getNumberOfControllers(); ++i) {
            std::pair<double, double> pair1{1000, distribution(generator)};
            std::pair<double, double> pair2{1050, distribution(generator)};
            std::pair<double, double> pair3{1100, distribution(generator)};
            m_values = {pair1, pair2, pair3};
          }
        },
        Qt::QueuedConnection);
      m_updateTimer->start(300);
    });
    return 0;
  }
  void setSets(std::vector<double> newSets) override {
  }
  void stopAll() override {
  }
  int getStatus() const override {
    return static_cast<int>(m_connectedStatus);
  }
  std::vector<std::pair<double, double>> getLastValues() override {
    //    qInfo() << __PRETTY_FUNCTION__ << QThread::currentThread();
    return m_values;
  }
  int getNumberOfControllers() const override {
    return 3;
  }

private:
  std::vector<std::pair<double, double>> m_values;
  QTimer                                *m_updateTimer{nullptr};
};

#endif // COMMUNICATION_HELPER_H
