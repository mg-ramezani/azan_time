#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "calcpraye.h"

class QMediaPlayer;
class QTimer;

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
    void on_groupBox_3_clicked();
    void on_checkBox_default_sound_toggled(bool checked);
    void on_pushButton_select_sound_clicked();

private:
    void init_name_of_state(void);
    void check_for_praye_time(void);
    void play_azan_and_reinit(void);

private:
    void set_default_voice(void);

private:
    Ui::azan_widget *ui;
    Azan::CalculateAzan khode_azon;
    QMediaPlayer *player{nullptr};

    QTimer *first_timer;
    QTimer *second_timer;
    QTimer *third_timer;
};
#endif // WIDGET_H
