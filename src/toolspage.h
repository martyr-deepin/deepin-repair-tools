#ifndef TOOLSPAGE_H
#define TOOLSPAGE_H

#include "toolsmodel.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QStackedLayout>

class ToolsPage : public QWidget
{
    Q_OBJECT

public:
    explicit ToolsPage(QWidget *parent = nullptr);

    void setModel(const QPointer<ToolsModel> &model);

private slots:
    void refreshNavBar();

private:
    QPointer<ToolsModel> m_model;
    QVBoxLayout *m_navigationLayout;
    QStackedLayout *m_pluginsLayout;
    QLabel *m_bottomTips;
    QPushButton *m_bottomButton;
};

#endif // TOOLSPAGE_H
