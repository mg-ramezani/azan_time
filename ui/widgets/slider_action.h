#ifndef SLIDER_ACTION_H
#define SLIDER_ACTION_H

#include <QWidgetAction>

class QLabel;
class QSlider;
class QHBoxLayout;

class slider_action : public QWidgetAction
{
private:
    QWidget* win;
    QHBoxLayout* lay;
    QLabel* label;
    QSlider* slider;

public:
    slider_action(QWidget *parent = nullptr);
    QSlider* get_slider(void);
};

#endif // SLIDER_ACTION_H
