#include "calibrationwidget.h"

#include <QDebug>
#include <QDoubleSpinBox>
#include <QHeaderView>
#include <QLayout>
#include <QStyledItemDelegate>

#include "src/calibrationmodel.h"

class SpinBoxDelegate : public QStyledItemDelegate {
public:
  QWidget *
    createEditor(QWidget *parent, QStyleOptionViewItem const &option, QModelIndex const &index) const Q_DECL_OVERRIDE {
    auto w = QStyledItemDelegate::createEditor(parent, option, index);

    auto sp = qobject_cast<QDoubleSpinBox *>(w);
    if (sp) {
      sp->setDecimals(1);
      sp->setSingleStep(0.1);
    }
    return w;
  }
};

CalibrationWidget::CalibrationWidget(QWidget *parent) : QGroupBox{parent} {
  setTitle("Калибровка");

  auto mainLay = new QVBoxLayout(this);

  m_tableView = new QTableView(this);
  m_selectBox = new QComboBox(this);

  mainLay->addWidget(m_selectBox);
  mainLay->addWidget(m_tableView);

  auto del = new SpinBoxDelegate();
  m_tableView->setItemDelegate(del);
  m_tableView->setSortingEnabled(true);

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

  m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

QVariantList CalibrationWidget::getData() const {
  QVariantList ret{};

  for (int i{0}; i < m_models.size(); ++i) {
    auto model   = m_models[i];
    auto map     = model->getVariant().toMap();
    map["label"] = m_selectBox->itemText(i);
    ret.push_back(map);
  }

  return ret;
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
