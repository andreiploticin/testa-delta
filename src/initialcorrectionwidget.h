//
// Created by user on 2023-02-11.
//

#ifndef TESTA_DELTA_INITIALCORRECTIONWIDGET_H
#define TESTA_DELTA_INITIALCORRECTIONWIDGET_H

#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QValidator>
#include <QTimer>

class InitialCorrectionWidget : public QGroupBox {
  Q_OBJECT
public:
  InitialCorrectionWidget(QWidget *parent = nullptr);
  void   setCorrection(double value);
  void   setDuration(double minutes);
  double getCorrection();
  double getDuration();
  void   setActive(bool value);

signals:
  /**
   * Chane to work or low set work
   */
  void newLowSet(double correction);

private:
  void              initGui();
  void              buttonHandle();
  void              handleTimer();
  QLineEdit        *m_cor_edit{nullptr};
  QLineEdit        *m_dur_edit{nullptr};
  QPushButton      *m_button{nullptr};
  QTimer           *m_timer{nullptr};
  QString           m_default_Style{};
  QDoubleValidator *m_doubleValidator{nullptr};
  bool              m_inWork{false};
};

#endif // TESTA_DELTA_INITIALCORRECTIONWIDGET_H
