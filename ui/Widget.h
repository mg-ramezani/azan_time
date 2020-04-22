#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "calcpraye.h"

class QSystemTrayIcon;
class QMediaPlayer;
class QAction;
class QTimer;
class QMenu;

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

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void on_comboBox_state_currentIndexChanged(const QString &arg1);
    void on_pushButton_clicked();
    void on_groupBox_3_clicked();
    void on_checkBox_default_sound_toggled(bool checked);
    void on_pushButton_select_sound_clicked();
    void on_toolButton_sournd_test_clicked(bool checked);
    void on_horizontalSlider_player_volume_sliderMoved(int position);
    void on_toolButton_reset_default_clicked();
    void on_toolButton_enable_try_icon_clicked(bool checked);

private:
    void init_name_of_state(void);
    void check_for_praye_time(void);
    void play_azan(void);

private:
    void set_default_voice(void);
    inline void create_actions_connects(void);
    inline void create_menus(void);
    inline void set_menus_to_tray_icon(void);
    inline void config_tray_icon(void);
    void load_all_settings(void);
    inline void create_timer_connects(void);

private:
    Ui::azan_widget *ui;
    Azan::CalculateAzan khode_azon;
    QMediaPlayer *player{nullptr};

    QTimer *first_timer;
    QTimer *second_timer;
    QTimer *third_timer;

    QAction* action_minimize;
    QAction* action_maximize;
    QAction* action_mute;
    QAction* action_quite;

    QSystemTrayIcon *tray_icon;
    QMenu* tray_menu;
};
#endif // WIDGET_H
