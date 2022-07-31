#include "../src/communication.hpp"
#include "../src/controller.hpp"

class TestController : public IController {
public:
  void                      setup() override {}
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

class TestCommunication : public CommunicationInterface<TestCommunicationTraits> {
public:
  TestCommunication() {
    for (int i{0}; i < 3; ++i) {
      m_controllers.emplace_back(std::make_unique<TController>());
    }
  };

  int                                    setup(TSetupOptions &options) override { return 0; }
  CommunicationStatus                    getStatus() override { return CommunicationStatus{}; }
  std::vector<std::pair<double, double>> getLastValues() override {
    std::vector<std::pair<double, double>> ret;
    for (auto &controller : m_controllers) {
      ret.push_back(controller->getValue());
    }
    return ret;
  }
};
