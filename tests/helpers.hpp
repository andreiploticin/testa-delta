#include "../src/icommunication.hpp"
#include "../src/controller.hpp"
#include "../src/idataholder.hpp"

class TestController : public IController {
public:
  void                      setup() override {}
  void                      start(bool cmd = true) {}
  void                      setR(double) override {}
  void                      setCorrection(double, double) override {}
  int                       getStatus() override { return 0; }
  std::pair<double, double> getValue() override { return {1, 2}; }
};

struct TestCommunicationTraits {
  using SetupOptions        = int;
  using CommunicationStatus = int;
  using ControllerType      = TestController;
};

class TestCommunication : public ICommunicationInterface<TestCommunicationTraits> {
public:
  TestCommunication() {
    std::cout << "TestCommunication" << std::endl;
    for (int i{0}; i < 3; ++i) {
      m_controllers.emplace_back(std::make_unique<TController>());
    }
  };

  int  setup(TSetupOptions &options) override { return 0; }
  void setSets(std::vector<double> newSets) override {
    if (newSets.size() > m_controllers.size()) {
      throw std::exception();
    }
    for (int i{0}; i < newSets.size(); ++i) {
      auto item = newSets[i];
      m_controllers[i]->setR(item);
      m_controllers[i]->start();
    }
  }
  void stopAll() override {
    for (auto &controller : m_controllers) {
      controller->stop();
    }
  }
  CommunicationStatus                    getStatus() override { return CommunicationStatus{}; }
  std::vector<std::pair<double, double>> getLastValues() override {
    std::vector<std::pair<double, double>> ret;
    for (auto &controller : m_controllers) {
      ret.push_back(controller->getValue());
    }
    return ret;
  }
};

class TestDataHolder : public IDataHolder {
public:
  TestDataHolder()
      : IDataHolder() {}
  void addDataPoint(std::vector<double> dataPoint) override {
    std::lock_guard lock_guard{m_dataMutex};
    m_data.push_back(dataPoint);
  }
  void load(QString const &filePath) override { ; }
  void save(QString const &filePath) override { ; }
  void exportToFile(QString const &filePath) override { ; }

private:
  void loadInternal(QString const &filePath);
};
