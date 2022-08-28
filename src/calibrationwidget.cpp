#include "calibrationwidget.h"

#include <QLayout>

#include "src/calibrationmodel.h"

CalibrationWidget::CalibrationWidget(QWidget *parent) : QGroupBox{parent} {
  setTitle("Калибровка");

  auto mainLay = new QVBoxLayout(this);

  m_tableView = new QTableView(this);
  m_selectBox = new QComboBox(this);

  mainLay->addWidget(m_tableView);
  mainLay->addWidget(m_selectBox);

  connect(m_selectBox, &QComboBox::currentIndexChanged, this, &CalibrationWidget::setModelToView);
}

void CalibrationWidget::setData(QVariantList data) {
  m_dataReady = false;
  for (auto &item : m_models) {
    item->deleteLater();
  }
  m_models.clear();
  m_selectBox->clear();

  for (int i{0}; i < data.size(); ++i) {
    auto modelMap = data[i].toMap();
    if (modelMap.contains("label")) {
      m_selectBox->addItem(modelMap["label"].toString());
    } else {
      m_selectBox->addItem(QString::number(i + 1));
    }

    auto model = new CalibrationModel{};
    model->setVariant(modelMap);
    m_models.push_back(model);
  }
  m_dataReady = true;
  if (!m_models.empty()) {
    setModelToView(0);
  }
}

QVariantList CalibrationWidget::getData() const {
  return {1};
}

CalibrationWidget::~CalibrationWidget() {
  for (auto &item : m_models) {
    item->deleteLater();
  }
}

void CalibrationWidget::setModelToView(int index) {
  if (m_dataReady) {
    m_tableView->setModel(m_models[index]);
  }
}
