#ifndef FILELISTMODEL_H
#define FILELISTMODEL_H

#include <QAbstractItemModel>

#include "treeitem.h"

//! Модель файловой системы
class FileListModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    //! Конструктор класса
    explicit FileListModel(QObject *parent = nullptr);

    //! Деструктор класса
    virtual ~FileListModel() override;

    //! Возвращает количество строк
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    //! Возвращает количество колонок
    int columnCount(const QModelIndex &parent) const override;

    //! Возвращает индекс в родителе
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;

    //! Возвращает индекс родителя
    QModelIndex parent(const QModelIndex &index) const override;

    //! Возвращает данные по индексу
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //! Возвращает заголовок
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    //! Возвращает флаг для индекса
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    //! Получение элемента по индексу
    TreeItem *toItem(const QModelIndex &index) const;

    //! Получение индекса по элементу
    QModelIndex fromItem(TreeItem *item, int column = 0) const;

    //! Возвращает наличие дочерних элементов
    bool hasChildren(const QModelIndex &parent) const override;

    //! Возвращает возможность ленивого чтения данных
    bool canFetchMore(const QModelIndex &parent) const override;

    //! Ленивое чтение данных
    void fetchMore(const QModelIndex &parent) override;

public slots:
    //! Начинает ленивое чтение данных
    TreeItem *startFetchMore(TreeItem *parentItem);

    //! Заканчивает ленивое чтение данных
    void finishedFetchMore();

private:
    //! Корневой элемент данных
    TreeItem *rootItem;
};

#endif // FILELISTMODEL_H
