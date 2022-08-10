#ifndef IDATAHOLDER_HPP
#define IDATAHOLDER_HPP

#include <QString>
#include <mutex>
#include <thread>
#include <vector>

class IDataHolder {
public:
  virtual ~IDataHolder();
  virtual void addDataPoint(std::vector<double>) = 0;
  virtual void load(QString const &)             = 0;
  virtual void save(QString const &)             = 0;
  virtual void exportToFile(QString const &)     = 0;

protected:
  std::mutex                       m_dataMutex;
  std::thread                      m_thread;
  std::vector<std::vector<double>> m_data;
};

IDataHolder::~IDataHolder() {
  if (m_thread.joinable()) {
    m_thread.join();
  }
}

#endif // IDATAHOLDER_HPP