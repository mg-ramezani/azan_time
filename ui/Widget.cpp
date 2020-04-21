#include "Widget.h"
#include "ui_Widget.h"

#include <string>

#include <QMediaPlayer>
#include <QFileDialog>
#include <QDateTime>
#include <QTimer>
#include <QTime>
#include <QUrl>

azan_widget::azan_widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::azan_widget)
    , player(new QMediaPlayer(this))
    , first_timer(new QTimer(this))
    , second_timer(new QTimer(this))
    , third_timer(new QTimer(this))
{
    ui->setupUi(this);
    setWindowTitle("Azan-v0.2");

    init_name_of_state();

    connect(first_timer, &QTimer::timeout, this, &azan_widget::play_azan_and_reinit);
    connect(second_timer, &QTimer::timeout, this, &azan_widget::play_azan_and_reinit);
    connect(third_timer, &QTimer::timeout, this, &azan_widget::play_azan_and_reinit);

    set_default_voice();
}

azan_widget::~azan_widget()
{
    delete ui;
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

void azan_widget::play_azan_and_reinit()
{
    player->play();
    check_for_praye_time();
}

void azan_widget::determine_which_one_is_closer(const QTime& current, const QTime& a, const QTime& b, const QTime& c)
{
    const QTime* closer{};

    closer = (a.secsTo(current) < b.secsTo(current)) ? &a : &b;
    closer = (closer->secsTo(current) < c.secsTo(current)) ? closer : &c;

    if (closer == &a)
    {
        ui->label_faraj->setStyleSheet("font-weight: bold; color: red");
        ui->label_dhuhr->setStyleSheet("");
        ui->label_maghrib->setStyleSheet("");
    }
    else if (closer == &b)
    {
        ui->label_faraj->setStyleSheet("");
        ui->label_dhuhr->setStyleSheet("font-weight: bold; color: red");
        ui->label_maghrib->setStyleSheet("");
    }
    else if (closer == &c)
    {
        ui->label_faraj->setStyleSheet("");
        ui->label_dhuhr->setStyleSheet("");
        ui->label_maghrib->setStyleSheet("font-weight: bold; color: red");
    }
}

void azan_widget::set_default_voice()
{
    player->setMedia(QUrl("qrc:/sounds/azan/58-naghshbandi.mp3"));
}

void azan_widget::check_for_praye_time()
{
    auto current_time{QTime::currentTime()};

    auto first_point{QTime::fromString(ui->label_faraj->text(), "HH:mm")};
    auto second_point{QTime::fromString(ui->label_dhuhr->text(), "HH:mm")};
    auto third_point{QTime::fromString(ui->label_maghrib->text(), "HH:mm")};

    determine_which_one_is_closer(current_time, first_point, second_point, third_point);

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
