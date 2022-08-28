#include "dataholder.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QThread>

DataHolder::DataHolder(int size, QObject *parent) : IDataHolder(parent), m_paramSize{size} {
  qInfo() << __PRETTY_FUNCTION__ << QThread::currentThread();
  m_autosaveTimer = new QTimer(this);
  connect(m_autosaveTimer, &QTimer::timeout, this, &DataHolder::autoSave);

  // prepare dir to save
  m_saveDir.setPath(QCoreApplication::applicationDirPath() + "/data");
  if (!m_saveDir.exists()) {
    m_saveDir.mkpath(".");
  }
  // get delimiter from settings

  // create m_data vectors
  for (int i = 0; i < m_paramSize; ++i) {
    m_data.push_back(QVector<double>());
  }
}

DataHolder::~DataHolder() {
  qInfo() << __PRETTY_FUNCTION__ << QThread::currentThread();
  DataHolder::setAutoSave(false);
}

void DataHolder::addDataPoint(uint64_t pointTime, QVector<double> point) {
  if (point.size() != m_data.size()) {
    qInfo() << "point.size() != m_data.size()";
    return;
  }
  m_time.push_back(pointTime);
  for (int i = 0; i < point.size(); ++i) {
    m_data[i].push_back(point[i]);
  }
  emit pointAdded();
}

QVector<double> DataHolder::getDataAtTime(double time) const {
  auto pos    = std::upper_bound(m_time.cbegin(), m_time.cend(), time);
  auto sample = pos - m_time.cbegin();

  return getDataAtSample(sample);
}

QVector<double> DataHolder::getDataAtSample(uint64_t sample) const {
  if (m_time.empty() || (sample >= m_time.size())) {
    return {};
  }
  QVector<double> ret;
  for (int i{0}; i < m_data.size(); ++i) {
    ret.push_back(m_data[i][sample]);
  }
  return ret;
}

int DataHolder::load(QString const &filePath) {
  QFile file(filePath);
  auto  ret = loadData(file);
  if (0 == ret) {
    emit pointAdded();
  }
  return ret;
}

int DataHolder::save(QString const &, bool temporary) {
  if (m_time.empty() || m_data.empty() || m_data[0].empty()) {
    qInfo() << __PRETTY_FUNCTION__ << "incompleate data";
    return -1;
  }

  auto baseName     = QDateTime::fromSecsSinceEpoch(m_time[0]).toString("yyyy-MM-dd_HH-mm-ss");
  auto pathName     = QString("autosave") + baseName + "_r" + QString::number(m_lastSaveRev + 1) + ".dat";
  auto pathNamePrev = QString("autosave") + baseName + "_r" + QString::number(m_lastSaveRev) + ".dat";

  if (!temporary) {
    if (0 == saveData(QFile(m_saveDir.path() + "/" + baseName + ".dat"))) {
      m_saveDir.remove(pathNamePrev);
      qInfo() << "save data success:" << baseName + ".dat";
      return 0;
    } else {
      qInfo() << "save data failed:" << baseName + ".dat";
      return -1;
    }
  } else {
    if (0 == saveData(QFile(m_saveDir.path() + "/" + pathName))) {
      auto res = m_saveDir.remove(pathNamePrev);
      qInfo() << "update autosave data";
      m_lastSaveRev++;
      return res ? 0 : -1;
    } else {
      qInfo() << "fail autosave data";
      return -1;
    }
  }

  //  if (!save(pathName, true)) {
  //    m_saveDir.remove(pathNamePrev);
  //    qInfo() << "update autosave data";
  //  }
  //  return saveData(file);
}

void DataHolder::exportToFile(QString const &) {
}

void DataHolder::setAutoSave(bool value, uint32_t periodInSec) {
  if (nullptr == m_autosaveTimer) {
    return;
  }

  if (value) {
    m_autosaveTimer->start(1000 * periodInSec);
  } else {
    m_autosaveTimer->stop();
  }
}

void DataHolder::autoSave() {
  save("", true);
}

int DataHolder::saveData(QFile file) {
  if (file.open(QIODevice::WriteOnly)) {
    QTextStream stream(&file);

    for (int i = 0; i < m_time.size(); ++i) {
      stream << QDateTime::fromSecsSinceEpoch(m_time[i]).toString("yyyy-MM-dd_HH-mm-ss");
      for (int j = 0; j < m_data.size(); ++j) {
        stream << m_csvDelimiter << QString::number(m_data[j][i], 'f', 1);
      }
      stream << Qt::endl;
    }

    file.close();
  } else {
    return -1;
  }
  return 0;
}

int DataHolder::loadData(QFile &file) {
  if (file.open(QIODevice::ReadOnly)) {
    m_data.clear();
    // create m_data vectors
    for (int i = 0; i < m_paramSize; ++i) {
      m_data.push_back(QVector<double>());
    }
    m_time.clear();
    QTextStream stream(&file);

    int prevFiledsSize = -1;
    while (!stream.atEnd()) {
      QString line   = stream.readLine();
      auto    fields = line.split(m_csvDelimiter);
      if ((prevFiledsSize >= 0) && (fields.size() != prevFiledsSize)) {
        qInfo() << "Invalid format of input file" << file.fileName();
        return -1;
      }

      // process time
      auto timeString = fields.first();
      m_time.push_back(QDateTime::fromString(timeString, "yyyy-MM-dd_HH-mm-ss").toSecsSinceEpoch());
      fields.pop_front();

      // process data values
      for (int i = 0; i < m_data.size(); ++i) {
        bool isOkFlag{false};
        m_data[i].push_back(fields[i].toDouble(&isOkFlag));
        if (!isOkFlag) {
          qInfo() << "Invalid format of input file" << file.fileName();
          return -1;
        }
      }
    }
    file.close();
  } else {
    return -1;
  }
  return 0;
}

QDir DataHolder::m_saveDir{};
