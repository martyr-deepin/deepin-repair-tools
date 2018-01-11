#ifndef PLUGINNAVGATION_H
#define PLUGINNAVGATION_H

#include <QPushButton>

class PluginNavgation : public QPushButton
{
    Q_OBJECT

public:
    explicit PluginNavgation(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // PLUGINNAVGATION_H
