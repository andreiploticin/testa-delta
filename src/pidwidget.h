#ifndef PIDWIDGET_H
#define PIDWIDGET_H

#include <QGroupBox>
#include <QLineEdit>

class PidWidget : public QGroupBox {
  Q_OBJECT
public:
  explicit PidWidget(QString const &title, QWidget *parent = nullptr);

  void setAddress(uint8_t address) noexcept {
    m_address = address;
  }

  uint8_t getAddress() const noexcept {
    return m_address;
  }

  void setRegisters(std::vector<uint16_t> registers);

  std::vector<uint16_t> getRegisters() const;

signals:
  void getFromController();
  void sendToController(uint8_t, std::vector<uint16_t>);

private:
  uint8_t    m_address;
  QLineEdit *m_p;
  QLineEdit *m_i;
  QLineEdit *m_d;
  QLineEdit *m_period;
};

#endif // PIDWIDGET_H
