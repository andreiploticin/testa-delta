#ifndef CORRECTIONWIDGET_H
#define CORRECTIONWIDGET_H

#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class CorrectionWidget : public QWidget {
  Q_OBJECT
public:
  explicit CorrectionWidget(QString const &value = "0.0", QWidget *parent = nullptr);
  QString text() const {
    return m_line->text();
  }
  void setText(QString const &text) {
    m_line->setText(text);
  }

private:
  QLineEdit *m_line{nullptr};
};

#endif // CORRECTIONWIDGET_H
