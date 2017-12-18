#ifndef REPAIRTOOLS_H
#define REPAIRTOOLS_H

#include <QWidget>

class RepairTools : public QWidget
{
    Q_OBJECT

public:
    explicit RepairTools(QWidget *parent = nullptr);

private:
    void keyPressEvent(QKeyEvent *e);
};

#endif // REPAIRTOOLS_H
