#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QImage>
#include <array>
#include <api/segmenter/cv_bridge.hpp>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //resize(800,600);

    this->image = new QImage();
    //connect the button listener event
    connect(ui->pushButton,SIGNAL(clicked()), this, SLOT(on_slotOpenImage_triggered()));
}

void MainWindow::on_slotOpenImage_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, "open image file",
                ".",
                "Image files (*.bmp *.jpg *.jpeg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");
    if(fileName != "")
    {
        if(image->load(fileName))
        {
            //***Clear keypoints***
            ui->widget1->foregroundPoints.clear();
            ui->widget1->backgroundPoints.clear();

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
            // data[xn*y+x][red] = (unsigned char)in->pixelColor(x, y).red();
            // data[xn*y+x][green] = (unsigned char)in->pixelColor(x, y).green();
            // data[xn*y+x][blue] = (unsigned char)in->pixelColor(x, y).blue();
            // data[xn*y+x][alpha] = 255;
            data[xn*y+x][red] = (unsigned char)QColor(in->pixel(x, y)).red();
            data[xn*y+x][green] = (unsigned char)QColor(in->pixel(x, y)).green();
            data[xn*y+x][blue] = (unsigned char)QColor(in->pixel(x, y)).blue();
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
    int boundaryX = 5;
    int boundaryY = 5;
    QImage MarkedImage = image->scaled(ui->graphicsView_2->geometry().width() - 2 * boundaryX, ui->graphicsView_2->geometry().height() - 2 * boundaryY, Qt::KeepAspectRatio);
    // MarkedImage size = fit graphicsView_2 size

    std::vector<std::pair<int,int>> newForegroundPoints;
    std::vector<std::pair<int,int>> newBackgroundPoints;
    for (const auto & pt : ui->widget1->foregroundPoints)
    {
        for(int loopNumber1 = -2; loopNumber1 <= 2; loopNumber1++)
        {
            for(int loopNumber2 = -2; loopNumber2 <= 2; loopNumber2++)
            {
                if(pt.first + loopNumber1 < 0 || pt.first + loopNumber1 > MarkedImage.width() - 1 ||
                   pt.second + loopNumber2 < 0 || pt.second + loopNumber2 > MarkedImage.height() - 1  )
                {
                    //throw std::invalid_argument("keypoint out of range");
                }
                else
                {
                    MarkedImage.setPixel(pt.first + loopNumber1, pt.second + loopNumber2, qRgb(255,0,0));
                }
            }
        }
        newForegroundPoints.push_back(std::pair<int,int>(pt.first * this->image->width() / ui->widget1->width(), pt.second * this->image->height() / ui->widget1->height() ));
    }
    for (const auto & pt : ui->widget1->backgroundPoints)
    {
        for(int loopNumber1 = -2; loopNumber1 <= 2; loopNumber1++)
        {
            for(int loopNumber2 = -2; loopNumber2 <= 2; loopNumber2++)
            {
                if(pt.first + loopNumber1 < 0 || pt.first + loopNumber1 > MarkedImage.width() - 1 ||
                   pt.second + loopNumber2 < 0 || pt.second + loopNumber2 > MarkedImage.height() - 1  )
                {
                    //throw std::invalid_argument("keypoint out of range");
                }
                else
                {
                    MarkedImage.setPixel(pt.first + loopNumber1, pt.second + loopNumber2, qRgb(0,0,255));
                }
            }
        }
        newBackgroundPoints.push_back(std::pair<int,int>(pt.first * this->image->width() / ui->widget1->width() , pt.second * this->image->height() / ui->widget1->height() ));
    }
    QGraphicsScene *scene1 = new QGraphicsScene;
    scene1->addPixmap(QPixmap::fromImage(MarkedImage));
    ui->graphicsView_2->setScene(scene1);
    ui->graphicsView_2->show();

    api::segmenter::std_keypoints_type keyPoints1;
    keyPoints1.push_back(newForegroundPoints);
    keyPoints1.push_back(newBackgroundPoints);
    api::segmenter::segmenter_cimg obj1;
    QImage segmentedImage = std_image_to_qimage(obj1.segment(qimage_to_std_image(this->image), keyPoints1));    //  segmentedImage size = raw image


    QGraphicsScene *scene2 = new QGraphicsScene;
    scene2->addPixmap(QPixmap::fromImage(segmentedImage.scaled(ui->graphicsView_3->geometry().width() - 2 * boundaryX, ui->graphicsView_3->geometry().height() - 2 * boundaryY, Qt::KeepAspectRatio)));
    ui->graphicsView_3->setScene(scene2);
    ui->graphicsView_3->show();

    QImage *resultImage = new QImage(image->size().width(), image->size().height(), QImage::Format_RGB888); //  resultImage size = raw image
    for(int loopNumber1 = 0; loopNumber1 < resultImage->height(); loopNumber1++)
    {
        for(int loopNumber2 = 0; loopNumber2 < resultImage->width(); loopNumber2++)
        {
            // if((unsigned char)segmentedImage.pixelColor(loopNumber2, loopNumber1).red() != 0)
            if((unsigned char)QColor(segmentedImage.pixel(loopNumber2, loopNumber1)).red() != 0)
            {
                resultImage->setPixel(
                    loopNumber2,
                    loopNumber1,
                    qRgb((unsigned char)QColor(this->image->pixel(loopNumber2, loopNumber1)).red(),
                         (unsigned char)QColor(this->image->pixel(loopNumber2, loopNumber1)).green(),
                         (unsigned char)QColor(this->image->pixel(loopNumber2, loopNumber1)).blue()));
            }
            else
            {
                resultImage->setPixel(
                    loopNumber2,
                    loopNumber1,
                    qRgb(0, 0, 0));
            }
        }
    }
    QGraphicsScene *scene3 = new QGraphicsScene;
    scene3->addPixmap(QPixmap::fromImage(resultImage->scaled(ui->graphicsView_4->geometry().width() - 2 * boundaryX, ui->graphicsView_4->geometry().height() - 2 * boundaryY, Qt::KeepAspectRatio)));
    ui->graphicsView_4->setScene(scene3);
    ui->graphicsView_4->show();

}


void MainWindow::on_pushButton_3_clicked()
{
    ui->widget1->foregroundPoints.clear();
    ui->widget1->backgroundPoints.clear();
}

void MainWindow::on_pushButton_4_clicked()
{
    //api::opk::kpExtractor kp;
    auto image = api::segmenter::std_image_to_cv_mat(qimage_to_std_image(this->image));
    int xn = image.cols;
    int yn = image.rows;
    kp.computeKp(image);
    // std::cout << xn << " " << yn << std::endl;
    auto fgpts = kp.getKeypoints(0.1)[0];
    for (auto & pt : fgpts) {
        int x = pt.first;
        int y = pt.second;
        if (x < 0 || x >= xn || y < 0 || y >= yn) {
            continue;
        }
        //std::cout << pt.first << " " << pt.second << std::endl;
        ui->widget1->foregroundPoints.push_back(std::pair<int,int>(pt.first * ui->widget1->width() / this->image->width(), pt.second * ui->widget1->height() / this->image->height()));
    }
    QMessageBox msgBox;
    msgBox.setText("Openpose keypoints generated");
    msgBox.exec();
}
