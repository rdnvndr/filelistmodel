#include "filelistmodel.h"

#include <QDir>
#include <QThread>
#include <QFutureWatcher>
#include <QFuture>
#include <QtConcurrent>

FileListModel::FileListModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "Name" << "Size" <<"Directory" << "Date" << "/";
    rootItem = new TreeItem(rootData);
}

FileListModel::~FileListModel()
{
    delete rootItem;
}


int FileListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    TreeItem *parentItem = this->toItem(parent);
    return parentItem->childCount();
}

int FileListModel::columnCount(const QModelIndex &parent) const
{
    TreeItem *parentItem = this->toItem(parent);

    return  parentItem->columnCount();
}

QVariant FileListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = this->toItem(index);
    return item->data(index.column());
}

QModelIndex FileListModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem = this->toItem(parent);
    TreeItem *childItem = parentItem->child(row);

    return this->fromItem(childItem, column);
}

QModelIndex FileListModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = this->toItem(index);
    TreeItem *parentItem = childItem->parentItem();

    return this->fromItem(parentItem, index.column());
}

bool FileListModel::canFetchMore(const QModelIndex &index) const
{
    TreeItem *parentItem = this->toItem(index);

    return !parentItem->isFetched() && !parentItem->isLocked();
}

void FileListModel::fetchMore(const QModelIndex &index)
{
    TreeItem *parentItem = this->toItem(index);

    parentItem->setLocked(true);

    QFutureWatcher<TreeItem *> *watcher = new QFutureWatcher<TreeItem *>();
    connect(watcher, &QFutureWatcher<TreeItem *>::finished,
            this, &FileListModel::finishedFetchMore);

    QFuture<TreeItem *> future = QtConcurrent::run(
                this, &FileListModel::startFetchMore, parentItem);
    watcher->setFuture(future);
}

TreeItem *FileListModel::startFetchMore(TreeItem *parentItem)
{
    // Имитация долгого получения данных
    QThread::msleep(1000);

    QDir dir(parentItem->data(4).toString());
    dir.setSorting(QDir::Type);
    dir.setFilter(dir.filter() | QDir::NoDotAndDotDot);

    for (const QFileInfo &fileInfo : dir.entryInfoList()){
        QList<QVariant> itemData;
        itemData << fileInfo.fileName() << fileInfo.size()
                 << fileInfo.isDir()    << fileInfo.lastModified()
                 << fileInfo.absoluteFilePath();
        TreeItem *childItem = new TreeItem(itemData, parentItem);
        parentItem->appendChild(childItem);
    }

    return parentItem;
}

Qt::ItemFlags FileListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

TreeItem *FileListModel::toItem(const QModelIndex &index) const
{
    if (!index.isValid())
        return  rootItem;
    else
        return  static_cast<TreeItem*>(index.internalPointer());
}

QModelIndex FileListModel::fromItem(TreeItem *item, int column) const
{
    if (item && item != rootItem)
        return createIndex(item->row(), column, item);
    else
        return QModelIndex();
}

QVariant FileListModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

void FileListModel::finishedFetchMore()
{
    QFutureWatcher<TreeItem *> *watcher = dynamic_cast<QFutureWatcher<TreeItem *> *>(sender());
    if(watcher){
        TreeItem *parentItem = watcher->future().result();
        delete  watcher;

        parentItem->setLocked(false);

        QModelIndex parent =  this->fromItem(parentItem);
        beginInsertRows(parent, 0, parentItem->childCount()-1);
        endInsertRows();

        parentItem->setFetched(true);

        // Обновление содержимого узла для представлений
        QList<QPersistentModelIndex> persistenIndexList;
        persistenIndexList << QPersistentModelIndex(parent);
        emit layoutChanged(persistenIndexList);
    }
}

bool FileListModel::hasChildren(const QModelIndex &parent) const
{
    // Если директория
    TreeItem *parentItem = this->toItem(parent);

    return parentItem->data(2).toBool();
}
