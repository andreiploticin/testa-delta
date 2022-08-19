#ifndef IDATAHOLDER_HPP
#define IDATAHOLDER_HPP

#include <QDebug>
#include <QObject>
#include <QString>
#include <QVector>

class IDataHolder : public QObject {
  Q_OBJECT
public:
  IDataHolder(QObject *parent = nullptr) : QObject(parent) {
  }
  virtual ~IDataHolder() override {
    qInfo() << __PRETTY_FUNCTION__;
  }
  virtual void            addDataPoint(uint64_t, QVector<double>)             = 0;
  virtual int             load(QString const &)                               = 0;
  virtual int             save(QString const &, bool temporary = false)       = 0;
  virtual void            exportToFile(QString const &)                       = 0;
  virtual void            setAutoSave(bool value, uint32_t periodInSec = 120) = 0;
  virtual QVector<double> getDataAtTime(double timer) const                   = 0;
  virtual QVector<double> getDataAtSample(uint64_t sample) const              = 0;

  virtual QVector<QVector<double>> const &getData() const = 0;
  virtual QVector<double> const          &getTime() const = 0;
signals:
  void pointAdded();
};
#endif // IDATAHOLDER_HPP
