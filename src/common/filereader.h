//
// Created by Andrey on 17.10.2021.
//

#ifndef ABCD2_FILEREADER_H
#define ABCD2_FILEREADER_H
#include <QFile>
#include <utility>

class FileReader {
public:
  explicit FileReader(QString filePath) : m_filePath(std::move(filePath)) {
  }

  unsigned read(QByteArray &rawData) {
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly)) {
      return -1;
    }
    rawData = file.readAll();
    file.close();

    if (rawData.isEmpty()) {
      return -1;
    }
    return 0;
  }

private:
  QString m_filePath;
};

#endif // ABCD2_FILEREADER_H
