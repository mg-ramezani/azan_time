#include "Widget.h"
#include "ui_Widget.h"

#include <string>

#include <QSystemTrayIcon>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QDateTime>
#include <QSettings>
#include <QAction>
#include <QTimer>
#include <QMenu>
#include <QTime>
#include <QUrl>

azan_widget::azan_widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::azan_widget)
    , player(new QMediaPlayer(this))
    , first_timer(new QTimer(this))
    , second_timer(new QTimer(this))
    , third_timer(new QTimer(this))
    , action_minimize(new QAction("کوچک‌نمایی", this))
    , action_maximize(new QAction("بزرگ‌نمایی", this))
    , action_mute(new QAction("بی‌صدا", this))
    , action_quite(new QAction("خروج", this))
    , tray_icon(new QSystemTrayIcon(this))
    , tray_menu(new QMenu(this))
{
    ui->setupUi(this);

    QCoreApplication::setOrganizationName("Ghasem");
    QCoreApplication::setOrganizationDomain("ghasem.org");
    QCoreApplication::setApplicationName("Azan");
    QCoreApplication::setApplicationVersion("v0.3");

    setWindowTitle("Azan-" + QCoreApplication::applicationVersion());

    init_name_of_state();

    create_timer_connects();

    connect(player, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status)
    {
        if(status == QMediaPlayer::MediaStatus::EndOfMedia)
        {
            check_for_praye_time();

            ui->label_faraj->setStyleSheet("");
            ui->label_dhuhr->setStyleSheet("");
            ui->label_maghrib->setStyleSheet("");
        }
    });

    set_default_voice();

    create_actions_connects();
    create_menus();
    set_menus_to_tray_icon();
    config_tray_icon();

    load_all_settings();
}

azan_widget::~azan_widget()
{
    QSettings settings;
    settings.setValue("default_cordinate", ui->groupBox_3->isChecked());
    settings.setValue("default_cordinate_lat", ui->lineEdit_lat->text());
    settings.setValue("default_cordinate_long", ui->lineEdit_2_long->text());
    settings.setValue("default_azan_sound", ui->checkBox_default_sound->isChecked());
    settings.setValue("user_azan_sound", player->media().request().url());
    settings.setValue("azan_sound_value", ui->horizontalSlider_player_volume->value());
    settings.setValue("play_faraj_azan", ui->checkBox_faraj->isChecked());
    settings.setValue("play_dhuhr_azan", ui->checkBox_dhuhr->isChecked());
    settings.setValue("play_maghrib_azan", ui->checkBox_maghrib->isChecked());
    settings.setValue("state", ui->comboBox_state->currentIndex());
    settings.setValue("city", ui->comboBox_city->currentIndex());

    delete ui;
}

void azan_widget::load_all_settings()
{
    QSettings settings;

    if(settings.value("default_cordinate").toBool())
    {
        ui->groupBox_3->setChecked(true);
        ui->lineEdit_lat->setText(settings.value("default_cordinate_lat").toString());
        ui->lineEdit_2_long->setText(settings.value("default_cordinate_long").toString());

        on_groupBox_3_clicked();
    }

    if(!settings.value("default_azan_sound").toBool())
    {
        ui->checkBox_default_sound->setChecked(settings.value("default_azan_sound").toBool());
        player->setMedia(settings.value("user_azan_sound").toUrl());
    }

    ui->horizontalSlider_player_volume->setValue(settings.value("azan_sound_value").toInt());
    player->setVolume(settings.value("azan_sound_value").toInt());
    ui->checkBox_faraj->setChecked(settings.value("play_faraj_azan").toBool());
    ui->checkBox_dhuhr->setChecked(settings.value("play_dhuhr_azan").toBool());
    ui->checkBox_maghrib->setChecked(settings.value("play_maghrib_azan").toBool());

    ui->comboBox_state->setCurrentIndex(settings.value("state").toInt());
    ui->comboBox_city->setCurrentIndex(settings.value("city").toInt());

    on_pushButton_clicked();
}

void azan_widget::create_timer_connects()
{
    connect(first_timer, &QTimer::timeout, this, [this]()
    {
        ui->label_faraj->setStyleSheet("font-weight: bold; color: red");

        play_azan();
    });

    connect(second_timer, &QTimer::timeout, this, [this]()
    {
        ui->label_dhuhr->setStyleSheet("font-weight: bold; color: red");

        play_azan();
    });

    connect(third_timer, &QTimer::timeout, this, [this]()
    {
        ui->label_maghrib->setStyleSheet("font-weight: bold; color: red");

        play_azan();
    });
}

void azan_widget::closeEvent(QCloseEvent* event)
{
#ifdef Q_OS_OSX
    if (!event->spontaneous() || !isVisible()) {
        return;
    }
#endif
    if (tray_icon->isVisible()) {
        hide();
        event->ignore();
    }

}

void azan_widget::init_name_of_state()
{
    const auto arr_lenght{khode_azon.get_length_of_availabel_cordinate()};
    const auto& list{khode_azon.get_availabel_cordinate()};

    std::string last_state;
    for (size_t i{}; i < arr_lenght; ++i)
    {
        if (!list.at(i).at(0).empty() && list.at(i).at(0) != last_state)
        {
            ui->comboBox_state->addItem(QString::fromStdString(list.at(i).at(0)));
            last_state = list.at(i).at(0);
        }
        else
        {
            last_state = list.at(i).at(0);
        }
    }
    ui->comboBox_state->addItem("شهرهای دیگر");
}

void azan_widget::play_azan()
{
    player->play();
}

void azan_widget::set_default_voice()
{
    player->setMedia(QUrl("qrc:/sounds/azan/58-naghshbandi.mp3"));
}

inline void azan_widget::create_actions_connects()
{
    action_mute->setCheckable(true);
    action_mute->setChecked(false);

    connect(action_minimize, &QAction::triggered, this, &QWidget::hide);
    connect(action_maximize, &QAction::triggered, this, &QWidget::showNormal);
    connect(action_mute, &QAction::triggered, this, [&, this](bool check){(check) ? player->setVolume(0) : player->setVolume(100);});
    connect(action_quite, &QAction::triggered, this, &QCoreApplication::quit);
}

inline void azan_widget::create_menus()
{
    tray_menu->addActions({action_mute, action_minimize, action_maximize});
    tray_menu->addSeparator();
    tray_menu->addAction(action_quite);
}

inline void azan_widget::set_menus_to_tray_icon()
{
    tray_icon->setContextMenu(tray_menu);
}

void azan_widget::config_tray_icon()
{
    tray_icon->setIcon(QIcon(":/icon/icon[pngwing.com].png"));
    tray_icon->setVisible(true);
}

void azan_widget::check_for_praye_time()
{
    auto current_time{QTime::currentTime()};

    auto first_point{QTime::fromString(ui->label_faraj->text(), "HH:mm")};
    auto second_point{QTime::fromString(ui->label_dhuhr->text(), "HH:mm")};
    auto third_point{QTime::fromString(ui->label_maghrib->text(), "HH:mm")};

    if (ui->checkBox_faraj->isChecked())
    {
        first_timer->start(std::abs(current_time.secsTo(first_point)) * 1000);
    }

    if (ui->checkBox_dhuhr->isChecked())
    {
        second_timer->start(std::abs(current_time.secsTo(second_point)) * 1000);
    }

    if (ui->checkBox_maghrib->isChecked())
    {
        third_timer->start(std::abs(current_time.secsTo(third_point)) * 1000);
    }
}

void azan_widget::on_comboBox_state_currentIndexChanged(const QString& arg1)
{
    ui->comboBox_city->clear();

    const auto& list{khode_azon.get_availabel_cordinate()};

    const auto arr_length{khode_azon.get_length_of_availabel_cordinate()};
    for (size_t i{}; i < arr_length; ++i)
    {
        if (list.at(i).at(0) == arg1.toStdString() ||
             (arg1 == "شهرهای دیگر" && list.at(i).at(0).empty()))
        {
            ui->comboBox_city->addItem(QString::fromStdString(list.at(i).at(1)));
        }
    }
}

void azan_widget::on_pushButton_clicked()
{
    auto set_azan_time{
         [&, this](const auto& times) {
#define s(x) QString::fromStdString(x)
             ui->label_faraj->setText(s(times.at(AzanTime::Fajr)));
             ui->label_sunrise->setText(s(times.at(AzanTime::Sunrise)));
             ui->label_dhuhr->setText(s(times.at(AzanTime::Dhuhr)));
             ui->label_sunset_3->setText(s(times.at(AzanTime::Sunset)));
             ui->label_maghrib->setText(s(times.at(AzanTime::Maghrib)));
#undef s
             check_for_praye_time();
         }};

    if (ui->groupBox_3->isChecked())
    {
        const auto cordinate{std::make_pair<double>(ui->lineEdit_lat->text().toDouble(),
             ui->lineEdit_2_long->text().toDouble())};

        khode_azon.set_new_cordinates(cordinate);
        const auto prayer_times{khode_azon.get_prayer_times()};
        set_azan_time(prayer_times);
    }
    else
    {
        const auto& list{khode_azon.get_availabel_cordinate()};
        const auto state_city{((ui->comboBox_state->currentText() == "شهرهای دیگر") ? std::string() : ui->comboBox_state->currentText().toStdString()) + ":" + ui->comboBox_city->currentText().toStdString()};
        const auto& arr_list{khode_azon.get_length_of_availabel_cordinate()};

        for (size_t i{}; i < arr_list; ++i)
        {
            const auto t{list.at(i).at(0) + ":" + list.at(i).at(1)};

            if (state_city == t)
            {
                khode_azon.set_new_cordinates(t);
                const auto prayer_times{khode_azon.get_prayer_times()};

                set_azan_time(prayer_times);
                break;
            }
        }
    }
}

void azan_widget::on_groupBox_3_clicked()
{
    ui->comboBox_state->setEnabled(!ui->groupBox_3->isChecked());
    ui->comboBox_city->setEnabled(!ui->groupBox_3->isChecked());
}

void azan_widget::on_checkBox_default_sound_toggled(bool checked)
{
    ui->pushButton_select_sound->setEnabled(!checked);
    if (checked)
    {
        set_default_voice();
    }
}

void azan_widget::on_pushButton_select_sound_clicked()
{
    auto path{QFileDialog::getOpenFileUrl(this, "انتخاب صوت اذان", QUrl(), "*.mp3")};
    player->setMedia(path);
}

void azan_widget::on_toolButton_sournd_test_clicked(bool checked)
{
    if(checked)
    {
        player->play();
    }
    else
    {
        player->stop();
    }
}

void azan_widget::on_horizontalSlider_player_volume_sliderMoved(int position)
{
    player->setVolume(position);
}

void azan_widget::on_toolButton_reset_default_clicked()
{
    constexpr auto title{"تأییدیه"};
    constexpr auto text{"آیا واقعاً مطمئن هستید که می‌خواید تنظیمات پیش‌فرض رابرگردانید ؟"};
    const auto result {QMessageBox::question(this, title, text)};

    if(result == QMessageBox::No)
    {
        return;
    }

    ui->label_faraj->clear();
    ui->label_sunrise->clear();
    ui->label_dhuhr->clear();
    ui->label_sunset_3->clear();
    ui->label_maghrib->clear();

    ui->comboBox_city->setCurrentIndex(0);
    ui->comboBox_state->setCurrentIndex(0);
    ui->comboBox_city->setEnabled(true);
    ui->comboBox_state->setEnabled(true);

    ui->groupBox_3->setChecked(false);
    ui->lineEdit_lat->clear();
    ui->lineEdit_2_long->clear();

    ui->checkBox_default_sound->setChecked(true);
    player->setMedia(QUrl("qrc:/sounds/azan/58-naghshbandi.mp3"));
    ui->horizontalSlider_player_volume->setValue(100);

    ui->checkBox_faraj->setChecked(true);
    ui->checkBox_dhuhr->setChecked(true);
    ui->checkBox_maghrib->setChecked(true);
}
