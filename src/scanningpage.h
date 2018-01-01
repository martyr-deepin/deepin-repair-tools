#ifndef SCANNINGPAGE_H
#define SCANNINGPAGE_H

#include <QWidget>
#include <QLabel>

class ScanningPage : public QWidget
{
    Q_OBJECT

public:
    explicit ScanningPage(QWidget *parent = nullptr);

private:
    QLabel *m_centerTips;
    QLabel *m_bottomTips;
};

#endif // SCANNINGPAGE_H
