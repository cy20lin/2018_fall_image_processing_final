#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <array>

#include <bits/stl_vector.h>
#include "core.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_slotOpenImage_triggered();

private:
    Ui::MainWindow *ui;
    QImage *image;
};

#endif // MAINWINDOW_H
