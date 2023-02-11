#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void read_data();
    int byte_to_int(char byte);
    void bmp_4();
    void bmp_8();
    void bmp_16();
    void bmp_24();
    void bmp_32();
    int sum_Byts(QByteArray arr, int begin, int length);
private slots:
    void start_slot();
private:
    Ui::MainWindow *ui;
    QByteArray arr;
    int inf_size;
    int inf_wight;
    int inf_height;
    int inf_depth;
    int inf_num_col;
    int inf_begin_arr;
    int inf_impot_colot;
};
#endif // MAINWINDOW_H
