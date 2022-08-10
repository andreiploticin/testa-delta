#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <utility>

class IController {
public:
  virtual void setup()                = 0;
  virtual void start(bool cmd = true) = 0;
  void         stop() {
            start(false);
  }
  virtual void                      setR(double)                  = 0;
  virtual void                      setCorrection(double, double) = 0;
  virtual int                       getStatus()                   = 0;
  virtual std::pair<double, double> getValue()                    = 0;
};
#endif // CONTROLLER_H
