#ifndef DISKCLEANWIDGET_H
#define DISKCLEANWIDGET_H

#include <QWidget>

class DiskCleanWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DiskCleanWidget(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *e);
};

#endif // DISKCLEANWIDGET_H
