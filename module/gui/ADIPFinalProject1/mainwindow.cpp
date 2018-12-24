#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <array>

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
            int boundaryX = 5;
            int boundaryY = 5;

            //***Resize image for display***
            QImage ResizeImage = image->scaled(ui->graphicsView->geometry().width() - 2 * boundaryX, ui->graphicsView->geometry().height() - 2 * boundaryY, Qt::KeepAspectRatio);

            //***Show origin image***
            QGraphicsScene *scene = new QGraphicsScene;
            scene->addPixmap(QPixmap::fromImage(ResizeImage));
            ui->graphicsView->setScene(scene);
            ui->graphicsView->show();

            //***Show image for choose points***
            ui->graphicsView_2->setScene(scene);
            ui->graphicsView_2->show();
            int graphicsView2Width = ui->graphicsView_2->geometry().width();
            int graphicsView2Height = ui->graphicsView_2->geometry().height();
            int graphicsView2X = ui->graphicsView_2->geometry().x();
            int graphicsView2Y = ui->graphicsView_2->geometry().y();
            int graphicsView2WidthHalf = graphicsView2Width / 2;
            int graphicsView2HeightHalf = graphicsView2Height / 2;
            ui->widget1->setGeometry(graphicsView2X + ( graphicsView2WidthHalf - ResizeImage.width() / 2), graphicsView2Y + ( graphicsView2HeightHalf - ResizeImage.height() / 2), ResizeImage.width(), ResizeImage.height());

            ui->widget1->show();

            api::segmenter::std_image_type StdImageType1 = qimage_to_std_image(&ResizeImage);
            QImage ResizeImage2 = std_image_to_qimage(StdImageType1);
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


api::segmenter::std_image_type MainWindow::qimage_to_std_image(QImage *in) {
    api::segmenter::std_image_type out;
    auto & size = std::get<0>(out);
    auto & data = std::get<1>(out);
    int xn;
    int yn;
    xn = in->width();
    yn = in->height();
    size.first = xn;
    size.second = yn;
    data.resize(xn * yn);
    for (int x = 0; x < xn; ++x) {
        for (int y = 0; y < yn; ++y) {
            // NOTE:
            // OpenCV use BGR format as default, our format is RGB
            // Be careful!!
            enum { cv_blue = 0, cv_green, cv_red, cv_alpha};
            enum { red = 0, green, blue, alpha};
            data[xn*y+x][red] = (unsigned char)in->pixelColor(x, y).red();
            data[xn*y+x][green] = (unsigned char)in->pixelColor(x, y).green();
            data[xn*y+x][blue] = (unsigned char)in->pixelColor(x, y).blue();
            data[xn*y+x][alpha] = 255;
        }
    }
    return out;
}

QImage MainWindow::std_image_to_qimage(api::segmenter::std_image_type in)
{
    QImage out;
    auto & size = std::get<0>(in);
    auto & data = std::get<1>(in);
    int xn;
    int yn;
    xn = size.first;
    yn = size.second;
    out = *new QImage(xn, yn, QImage::Format_RGB888);
    for (int x = 0; x < xn; ++x) {
        for (int y = 0; y < yn; ++y) {
            enum { cv_blue = 0, cv_green, cv_red, cv_alpha};
            enum { red = 0, green, blue, alpha};
            out.setPixel(x, y, qRgb((int)data[xn*y+x][red], (int)data[xn*y+x][green], (int)data[xn*y+x][blue]));
        }
    }
    return out;
}




void MainWindow::on_pushButton_2_clicked()
{
    api::segmenter::std_keypoints_type keyPoints1;
    keyPoints1.push_back(ui->widget1->foregroundPoints);
    keyPoints1.push_back(ui->widget1->backgroundPoints);
    api::segmenter::segmenter_cimg obj1;
    obj1.segment(qimage_to_std_image(this->image), keyPoints1);
}

