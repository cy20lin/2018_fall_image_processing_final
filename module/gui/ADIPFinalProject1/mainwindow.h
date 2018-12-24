#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <array>

#include <bits/stl_vector.h>
#include "widget.h"

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
    api::gui::std_image_type qimage_to_std_image(QImage *in);
    QImage std_image_to_qimage(api::gui::std_image_type in);

    api::gui::std_keypoints_type get_points();
};

#endif // MAINWINDOW_H
