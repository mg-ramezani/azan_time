#include "slider_action.h"

#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>

slider_action::slider_action(QWidget* parent)
    : QWidgetAction(parent)
    , win(new QWidget)
    , lay(new QHBoxLayout(win))
    , label(new QLabel(win))
    , slider(new QSlider(Qt::Orientation::Horizontal, win))
{
    connect(slider, &QSlider::valueChanged, this, [this](auto i) {label->setText(QString::number(i));});

    slider->setMaximum(100);

    lay->addWidget(label);
    lay->addWidget(slider);

    setDefaultWidget(win);
}

QSlider *slider_action::get_slider()
{
    return slider;
}
