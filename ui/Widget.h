#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "calcpraye.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class azan_widget;
}
QT_END_NAMESPACE

class azan_widget : public QWidget
{
    Q_OBJECT

public:
    azan_widget(QWidget *parent = nullptr);
    ~azan_widget();

private slots:
    void on_comboBox_state_currentIndexChanged(const QString &arg1);
    void on_pushButton_clicked();

private:
    void init_name_of_state(void);

private:
    Ui::azan_widget *ui;
    Azan::CalculateAzan khode_azon;
};
#endif // WIDGET_H
