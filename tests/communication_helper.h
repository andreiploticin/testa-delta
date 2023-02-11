#ifndef COMMUNICATION_HELPER_H
#define COMMUNICATION_HELPER_H

#include <QTimer>
#include <random>

#include "src/icommunication.h"

static std::mt19937                           generator(0);
static std::uniform_real_distribution<double> distribution(-1, 1);

class TestCommunication : public ICommunication {
  // ICommunication interface
public:
  TestCommunication()
      : ICommunication() {
    m_values        = {{25, 25}, {24, 24}, {26, 26}};
    m_tempCalcTimer = new QTimer(this);
    connect(m_tempCalcTimer, &QTimer::timeout, this, &TestCommunication::updateTemps);
    m_tempCalcTimer->start(100);
  }

  int establishConnection(CommunicationSetupOptions const &options) override {
    QTimer::singleShot(1000, [this]() {
      this->setConnectedStatus(true);
      m_updateTimer = new QTimer(this);
      connect(
        m_updateTimer, &QTimer::timeout, this,
        [this]() {
          for (int i = 0; i < getNumberOfControllers(); ++i) {
            //            double temp1 =
            //            std::pair<double, double> pair1{m_sets[0], m_sets[0]+distribution(generator)};
            //            std::pair<double, double> pair2{m_sets[1], m_sets[1]+distribution(generator)};
            //            std::pair<double, double> pair3{m_sets[2], m_sets[2]+distribution(generator)};
            //              m_values = {pair1, pair2, pair3};
          }
        },
        Qt::QueuedConnection);
      m_updateTimer->start(300);
    });
    return 0;
  }
  void                                   setSets(std::vector<double> newSets) override { m_sets = newSets; }
  void                                   stopAll() override {}
  int                                    getStatus() const override { return static_cast<int>(m_connectedStatus); }
  std::vector<std::pair<double, double>> getLastValues() override {
    //    qInfo() << __PRETTY_FUNCTION__ << QThread::currentThread();
    return m_values;
  }
  int getNumberOfControllers() const override { return 3; }

private:
  double                                 Tconst     = 30;
  double                                 abientTemp = 25;
  std::vector<std::pair<double, double>> m_values;
  QTimer                                *m_updateTimer{nullptr};
  std::vector<double>                    m_sets{0, 0, 0};
  QTimer                                *m_tempCalcTimer{nullptr};
  void                                   updateTemps();

  // ICommunication interface
public:
  void closeConnection() override;
  void setSettings(QVariant settings) override;
  void makeCustomRequest(uint8_t address, uint16_t offset, uint8_t size) override;
  void sendRegisters(uint8_t address, uint16_t offset, std::vector<uint16_t> registers) override;
};

void TestCommunication::sendRegisters(uint8_t address, uint16_t offset, std::vector<uint16_t> registers) {}

void TestCommunication::makeCustomRequest(uint8_t address, uint16_t offset, uint8_t size) {}

void TestCommunication::closeConnection() {}

void TestCommunication::setSettings(QVariant settings) {}

void TestCommunication::updateTemps() {
  for (int i = 0; i < getNumberOfControllers(); ++i) {
    if (m_sets[i] > 0) {
      m_values[i].first = m_sets[i];
      m_values[i].second += (m_values[i].first - m_values[i].second) / Tconst + distribution(generator);
    }
  }
}

#endif // COMMUNICATION_HELPER_H
