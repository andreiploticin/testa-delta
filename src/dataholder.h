#ifndef DATAHOLDER_H
#define DATAHOLDER_H

#include <QDir>
#include <QObject>
#include <QTimer>

#include "idataholder.h"

class DataHolder : public IDataHolder {
  Q_OBJECT
public:
  // IDataHolder interface
  DataHolder(int size, QObject *parent = nullptr);
  ~DataHolder() override;
  void                            addDataPoint(uint64_t pointTime, QVector<double>) override;
  int                             load(QString const &) override;
  int                             save(QString const &, bool temporary = false) override;
  void                            exportToFile(QString const &) override;
  void                            setAutoSave(bool value, uint32_t periodInSec = 120) override;
  QVector<double>                 getDataAtTime(double time) const override;
  QVector<double>                 getDataAtSample(uint64_t sample) const override;
  QVector<QVector<double>> const &getData() const override {
    return m_data;
  }
  QVector<double> const &getTime() const override {
    return m_time;
  }

private:
  /**
   * @brief vector of parameter's values.
   * Values - are vector itself too.
   */
  QVector<QVector<double>> m_data;
  QVector<double>          m_time;

  void        autoSave();
  int         saveData(QFile file);
  int         loadData(QFile &file);
  QChar       m_csvDelimiter{';'};
  QTimer     *m_autosaveTimer{nullptr};
  static QDir m_saveDir;
  uint32_t    m_lastSaveRev{0};
  int         m_paramSize{0};

public:
};

#endif // DATAHOLDER_H
