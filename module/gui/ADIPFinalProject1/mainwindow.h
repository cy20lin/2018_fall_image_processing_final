#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <array>

#include <bits/stl_vector.h>
#include "widget.h"
#include <api/opk/kp_extractor.hpp>


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

    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    QImage *image;
    api::segmenter::std_image_type qimage_to_std_image(QImage *in);
    QImage std_image_to_qimage(api::segmenter::std_image_type in);
    api::opk::kpExtractor kp;


};

#endif // MAINWINDOW_H
