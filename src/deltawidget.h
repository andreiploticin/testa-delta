#ifndef DELTAWIDGET_H
#define DELTAWIDGET_H

#include <QCheckBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class DeltaWidget : public QWidget {
  Q_OBJECT
public:
  explicit DeltaWidget(QWidget *parent = nullptr);
  void setCurTemp(double value) {
    if (nullptr != m_curTemp) {
      m_curTemp->setText(QString::number(value, 'f', 2));
    }
  }

private:
  QFormLayout *m_mainLay{nullptr};
  QLineEdit   *m_setTempEdit{nullptr};
  QLineEdit   *m_curTemp{nullptr};
};

#endif // DELTAWIDGET_H
