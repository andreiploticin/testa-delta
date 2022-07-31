#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <memory>
#include <tuple>
#include <vector>

#include "controller.hpp"

template <typename Traits> class CommunicationInterface {
public:
  using TSetupOptions       = typename Traits::SetupOptions;
  using TController         = typename Traits::ControllerType;
  using CommunicationStatus = std::pair<int, std::vector<int>>;

  CommunicationInterface()                                                     = default;
  virtual int                                    setup(TSetupOptions &options) = 0;
  virtual CommunicationStatus                    getStatus()                   = 0;
  virtual std::vector<std::pair<double, double>> getLastValues()               = 0;

protected:
  std::vector<std::unique_ptr<TController>> m_controllers;
};

#endif // COMMUNICATION_H