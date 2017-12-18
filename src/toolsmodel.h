#ifndef TOOLSMODEL_H
#define TOOLSMODEL_H

#include <QAbstractItemModel>

class ToolsModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit ToolsModel(QObject *parent = nullptr);
};

#endif // TOOLSMODEL_H
