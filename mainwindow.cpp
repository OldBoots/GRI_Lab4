#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_2->setPixmap(QPixmap("C:\\Study2023\\GRI\\IMG_20230131_101330.jpg").scaled(400,
                                                                                         400,
                                                                                         Qt::KeepAspectRatio,
                                                                                         Qt::SmoothTransformation));
    connect(ui->pushButton, SIGNAL(clicked()), SLOT(start_slot()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::read_data()
{
    inf_size = sum_Byts(arr, 2, 4);
    inf_begin_arr = 14 + sum_Byts(arr, 14, 4);
    inf_wight = sum_Byts(arr, 18, 4);
    inf_height = sum_Byts(arr, 22, 4);
    inf_depth = sum_Byts(arr, 28, 2);
    inf_num_col = sum_Byts(arr, 46, 4);
    inf_impot_colot = sum_Byts(arr, 50, 4);
}

void MainWindow::bmp_4()
{
    read_data();
    QImage pict(inf_wight, inf_height, QImage::Format_Indexed8);
    for(int i = 0, index = inf_begin_arr; i < 16; i++){
        pict.setColor(i, qRgb(byte_to_int(arr[index + 2]),
                              byte_to_int(arr[index + 1]),
                              byte_to_int(arr[index])));
        index+=4;
    }
    for(int i = 0, index = arr.size() - 1; i < inf_height; i++){
        for(int j = 0; j < inf_wight; j += 2){
            pict.setPixel(j, i, (byte_to_int(arr[index]) >> 4) & 0x0f);
            pict.setPixel(j + 1, i, (byte_to_int(arr[index])) & 0x0f);
            index--;
        }
    }
    pict.mirror(true, false);
    ui->label->setPixmap(QPixmap::fromImage(pict).scaled(400,
                                                         400,
                                                         Qt::KeepAspectRatio,
                                                         Qt::SmoothTransformation));
}

void MainWindow::bmp_8()
{
    read_data();
    QImage pict(inf_wight, inf_height, QImage::Format_Indexed8);
    for(int i = 0, index = inf_begin_arr; i < 256; i++){
        pict.setColor(i, qRgb(byte_to_int(arr[index + 2]),
                              byte_to_int(arr[index + 1]),
                              byte_to_int(arr[index])));
        index+=4;
    }
    for(int i = 0, index = arr.size() - 1; i < inf_height; i++){
        for(int j = 0; j < inf_wight; j++){

            pict.setPixel(j, i, byte_to_int(arr[index]));
            index--;
        }
    }
    pict.mirror(true, false);
    ui->label->setPixmap(QPixmap::fromImage(pict).scaled(400,
                                                         400,
                                                         Qt::KeepAspectRatio,
                                                         Qt::SmoothTransformation));
}

void MainWindow::bmp_16()
{
    read_data();
    QImage pict(inf_wight, inf_height, QImage::Format_RGB666);
    int pix;
    for(int i = 0, index = arr.size() - 1; i < inf_height; i++){
        for(int j = 0; j < inf_wight; j++){
            pix = QByteArray::fromRawData(&arr[index - 1], 1).toHex().toInt(nullptr, 16) +
                  (QByteArray::fromRawData(&arr[index], 1).toHex().toInt(nullptr, 16) << 8);
            pict.setPixel(j, i, qRgb(((pix >> 11) & 0x001f) + 80,
                                     ((pix >> 5) & 0x003f) + 80,
                                     (pix & 0x001f) + 80));
            index-=2;
        }
    }
    pict.mirror(true, false);
    ui->label->setPixmap(QPixmap::fromImage(pict).scaled(400,
                                                         400,
                                                         Qt::KeepAspectRatio,
                                                         Qt::SmoothTransformation));
}

void MainWindow::bmp_24()
{
    read_data();
    QImage pict(inf_wight, inf_height, QImage::Format_RGB666);
    for(int i = 0, index = arr.size() - 1; i < inf_height; i++){
        for(int j = 0; j < inf_wight; j++){
            pict.setPixel(j, i, qRgb(byte_to_int(arr[index]),
                                     byte_to_int(arr[index - 1]),
                                     byte_to_int(arr[index - 2])));
            index-=3;
        }
    }
    pict.mirror(true, false);
    ui->label->setPixmap(QPixmap::fromImage(pict).scaled(400,
                                                         400,
                                                         Qt::KeepAspectRatio,
                                                         Qt::SmoothTransformation));
}

void MainWindow::bmp_32()
{
    read_data();
    QImage pict(inf_wight, inf_height,  QImage::Format_ARGB32);
    for(int i = 0, index = arr.size() - 1; i < inf_height; i++){
        for(int j = 0; j < inf_wight; j++){
            pict.setPixel(j, i, qRgba(byte_to_int(arr[index - 1]),
                                      byte_to_int(arr[index - 2]),
                                      byte_to_int(arr[index - 3]),
                                      byte_to_int(arr[index])));
            index-=4;
        }
    }
    pict.mirror(true, false);
    ui->label->setPixmap(QPixmap::fromImage(pict).scaled(400,
                                                         400,
                                                         Qt::KeepAspectRatio,
                                                         Qt::SmoothTransformation));
}

int MainWindow::sum_Byts(QByteArray array, int begin, int length)
{
    int rez = 0;
    for(int i = begin, shift = 0; i < begin + length; i++, shift += 8){
        rez += QByteArray::fromRawData(&array[i], 1).toHex().toInt(nullptr, 16) << shift;
    }
    return rez;
}

void MainWindow::start_slot()
{
    QFile file(QFileDialog::getOpenFileName());
    file.open(QIODevice::ReadOnly);
    arr = file.readAll();
    file.close();
    if(byte_to_int(arr[0]) != 66 || byte_to_int(arr[1]) != 77){
        qDebug("Это не БМП!");
        return;
    }
    switch (sum_Byts(arr, 28, 2)) {
    case 32:
        qDebug("Формат 32 бита");
        bmp_32();
        break;
    case 24:
        qDebug("Формат 24 бита");
        bmp_24();
        break;
    case 16:
        qDebug("Формат 16 бит");
        bmp_16();
        break;
    case 8:
        qDebug("Формат 8 бит");
        bmp_8();
        break;
    case 4:
        qDebug("Формат 4 бита");
        bmp_4();
        break;
    default:
        qDebug("Этот БМП файл не подходит!");
        break;
    }
}

int MainWindow::byte_to_int(char byte)
{
    return QByteArray::fromRawData(&byte, 1).toHex().toInt(nullptr, 16);
}
