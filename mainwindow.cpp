#include "mainwindow.h"

#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
    m_model = new FileListModel(this);

    m_treeView = new QTreeView(this);
    m_treeView->setAlternatingRowColors(true);
    m_treeView->setModel(m_model);

    setWindowTitle(tr("File List Model"));
    setCentralWidget(m_treeView);
}

MainWindow::~MainWindow()
{
    delete m_treeView;
    delete m_model;
}
