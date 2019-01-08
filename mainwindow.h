#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTreeView>
#include <QMainWindow>

#include "filelistmodel.h"

//! Главное окно приложения
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
    //! Конструктор класса
    MainWindow(QWidget *parent = nullptr);

    //! Деструктор класса
    ~MainWindow();

private:
    //! Дерево
    QTreeView     *m_treeView;

    //! Модель
    FileListModel *m_model;
};

#endif // MAINWINDOW_H
