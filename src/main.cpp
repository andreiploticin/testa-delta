#include <QApplication>
#include <QPushButton>
#include <QVariant>
#include <QList>

int main(int argc, char **argv)
{
  QList<int> vint = {1, 2, 3, 4, 5, 6};
  QList<double> vd = {45.5, 67.89};
  QList<QString> vs = {"asdf", "fffff", "dfsdf"};

  QVariantList a;

  QApplication app(argc, argv);

  QPushButton button("Hello world !");
  button.show();

  return app.exec();

  int b = 5;
  b = 7;
}