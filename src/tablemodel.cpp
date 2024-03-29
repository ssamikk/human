#include "tablemodel.h"

#include <QJsonObject>
#include "constfunction.h"
#include <QDebug>

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent), jsonArray()
{
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section){
        case Columns::CName:
            return utr("Имя");
        case Columns::CSex:
            return utr("Пол");
        case Columns::CAge:
            return utr("Возраст");
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return jsonArray.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return Columns::CHeight;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()){
        return QVariant();
    }
    if (role == Qt::DisplayRole && jsonArray.size() > index.row()) {
        QJsonObject human = jsonArray.at(index.row()).toObject();
        switch (index.column()) {
        case 0:
            return human[columnsName[CName]].toString();
        case 1:
            return human[columnsName[CSex]].toString();
        case 2:
            return human[columnsName[CAge]].toInt();
        default:
            break;
        }
    }
    return QVariant();
}

bool TableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    bool flag = false;
    if (row + count <= jsonArray.size()){
        for(int i = 0; i < count; i++){
            jsonArray.removeAt(row);
        }
        flag = true;
    }
    endRemoveRows();
    return flag;
}

QJsonArray TableModel::getJsonArray() const
{
    return jsonArray;
}

void TableModel::setJsonArray(const QJsonArray &value)
{
    beginResetModel();
    jsonArray = value;
    endResetModel();
}

void TableModel::addObject(const QJsonObject &human)
{
    if(human.isEmpty()){
        return;
    }
    beginResetModel();
    jsonArray.append(human);
    endResetModel();
}

void TableModel::replaceHuman(int row, const QJsonObject &human)
{
    if(human.isEmpty()){
        return;
    }
    beginResetModel();
    jsonArray.replace(row, human);
    endResetModel();
}
