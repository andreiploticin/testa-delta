//
// Created by Andrey on 10.02.2022.
//

#ifndef ABCD2_FILEWRITER_H
#define ABCD2_FILEWRITER_H

#include <QFile>
#include <QIODevice>

class FileWriter {
public:
  explicit FileWriter(QString filePath) : m_filePath(std::move(filePath)) {
  }

  unsigned write(QByteArray &rawData) {
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly)) {
      return -1;
    }
    auto size = file.write(rawData);
    if (rawData.size() != size) {
      return -1;
    }

    file.close();

    return 0;
  }

private:
  QString m_filePath;
};

#endif // ABCD2_FILEWRITER_H
