#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(800,600);

    this->image = new QImage();
    //connect the button listener event
    connect(ui->pushButton,SIGNAL(clicked()), this, SLOT(on_slotOpenImage_triggered()));

}

void MainWindow::on_slotOpenImage_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, "open image file",
                ".",
                "Image files (*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");
    if(fileName != "")
    {
        if(image->load(fileName))
        {
            //QImage ResizeImage = image->scaled(ui->graphicsView->size(),Qt::KeepAspectRatio);
            QImage ResizeImage = image->scaled(ui->graphicsView->geometry().width() - 10, ui->graphicsView->geometry().height(),Qt::KeepAspectRatio);
            QGraphicsScene *scene = new QGraphicsScene;
            scene->addPixmap(QPixmap::fromImage(ResizeImage));
            ui->graphicsView->setScene(scene);
            //ui->graphicsView->resize(image->width() + 10, image->height() + 10);
            ui->graphicsView->show();
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete image;
}

void MainWindow::on_pushButton_clicked()
{
    /*QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.exec();*/
}
