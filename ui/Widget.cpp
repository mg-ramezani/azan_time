#include "Widget.h"
#include "ui_Widget.h"

#include <string>

azan_widget::azan_widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::azan_widget)
{
    ui->setupUi(this);
    setWindowTitle("Azan-v0.1");

    init_name_of_state();
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

void azan_widget::on_comboBox_state_currentIndexChanged(const QString& arg1)
{
    ui->comboBox_city->clear();

    const auto& list{khode_azon.get_availabel_cordinate()};

    const auto arr_length{khode_azon.get_length_of_availabel_cordinate()};
    for (size_t i{}; i < arr_length; ++i)
    {
        if (list.at(i).at(0) == arg1.toStdString())
        {
            ui->comboBox_city->addItem(QString::fromStdString(list.at(i).at(1)));
        }
    }
}
