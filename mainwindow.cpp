#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>


#include "gyw.h"

QString filename;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openimage_button_clicked()
{
    filename = QFileDialog::getOpenFileName(this, tr("select image file"), ".",
                                               tr("Image Files(*.jpg *.jpeg *.bmp *.png)"));

     if (filename.isEmpty()) {
         ui->warning_lineEdit->setText("file open does not success !!!");
         return;
     }
     ui->lineEdit->setText(filename);
     ui->warning_lineEdit->clear();
     return;
}

void MainWindow::on_openvideo_button_clicked()
{
    filename = QFileDialog::getOpenFileName(this, tr("select video file"), ".",
                                              tr("Vedio Files(*.mp4 *.avi *.mkv *.yuv)"));

    if (filename.isEmpty()) {
        ui->warning_lineEdit->setText("file open does not success !!!");
        return;
    }
    ui->lineEdit->setText(filename);
    ui->warning_lineEdit->clear();
    return;
}

//单元宽度，高度
int binWidth[4]          ={2,4,8,16};
int binHeight[4]         ={2,4,8,16};
//全局变量==========================================================================
Global_t G;

//初始化标志
int G_class_init_flag=0;
//系统运行时间
int random_num=0;
int run_times=0;
int test=0;
int learning_mode=0;

//抠像
CvPoint pt1 = cvPoint(0,0);
CvPoint pt2 = cvPoint(0,0);
CvPoint pt_Ref_flag = cvPoint(16,500);
CvPoint pt_Chrom_flag = cvPoint(46,500);
bool is_selecting = false;
bool Ref_selecting = false;
bool Chrom_selecting = false;

int key_v=0;


//制作角度转弧度
double tabSin[360];
double tabCos[360];
//开方表
double tabSqrt[TABSQRTNUM];
//全局常量=========================================================================================
yuv_color_t colorRed={81,91,239};
yuv_color_t colorYellow={209,17,146};
yuv_color_t colorBlue={106,187,103};
yuv_color_t colorWhite={255,128,128};
yuv_color_t colorOrange={129,63,204};
yuv_color_t colorBrown={73,116,167};
yuv_color_t colorBlack={16,128,128};
yuv_color_t colorGray={100,128,128};
//字符
unsigned char ASCII_addr[10000]=
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x80,0x03,0xC0,0x03,0xC0,0x01,0xC0,0x01,0xC0,0x01,0x80,
    0x01,0x80,0x01,0x80,0x00,0x80,0x00,0x00,0x03,0xC0,0x03,0xC0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x03,0x9C,0x07,0xB8,0x0E,0x70,0x19,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x18,0x0C,0x18,0x3F,0xFE,0x7F,0xFE,0x0C,0x18,
    0x0C,0x18,0x0C,0x10,0x7F,0xFE,0x3F,0xFE,0x18,0x30,0x18,0x30,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x01,0x80,0x07,0xF0,0x1D,0x9C,0x19,0xBC,0x1F,0x80,0x0F,0xC0,
    0x01,0xF0,0x01,0xF8,0x19,0x9C,0x3D,0x9C,0x19,0x98,0x0F,0xF0,0x01,0x80,0x00,0x80,
    0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x18,0x66,0x10,0xE7,0x20,0xE7,0x60,0x66,0xC0,
    0x3D,0xBC,0x03,0x66,0x02,0x63,0x04,0xE3,0x08,0x66,0x18,0x3E,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xC0,0x18,0xC0,0x18,0xC0,0x19,0xC0,0x1F,0x3C,
    0x3E,0x3C,0x67,0x18,0xE3,0xB0,0xE1,0xE0,0x70,0xF1,0x3F,0xBE,0x00,0x00,0x00,0x00,
    0x00,0x00,0x1C,0x00,0x1E,0x00,0x0C,0x00,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x06,0x00,0x1C,0x00,0x30,0x00,0x60,0x00,0xC0,0x01,0xC0,0x01,0x80,
    0x01,0x80,0x01,0x80,0x01,0xC0,0x00,0xC0,0x00,0x60,0x00,0x38,0x00,0x0C,0x00,0x00,
    0x00,0x00,0x30,0x00,0x18,0x00,0x0E,0x00,0x07,0x00,0x03,0x80,0x01,0x80,0x01,0xC0,
    0x01,0xC0,0x01,0x80,0x01,0x80,0x03,0x00,0x06,0x00,0x0C,0x00,0x38,0x00,0x20,0x00,

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xC0,0x01,0xC0,0x79,0x9E,0x3F,0xFC,
    0x07,0xF0,0x3F,0xBE,0x31,0xCE,0x01,0xC0,0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x01,0x80,0x01,0x80,0x3F,0xFE,
    0x3F,0xFE,0x01,0x80,0x01,0x80,0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x3C,0x00,0x1E,0x00,0x0C,0x00,0x38,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFE,
    0x7F,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x00,0x1C,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x02,0x00,0x06,0x00,0x0C,0x00,0x18,0x00,0x30,0x00,0x60,0x00,0xC0,
    0x01,0x80,0x03,0x00,0x06,0x00,0x0C,0x00,0x18,0x00,0x10,0x00,0x20,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xF0,0x1C,0x18,0x38,0x1C,0x38,0x0E,0x38,0x0E,
    0x78,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x1C,0x1C,0x18,0x07,0xF0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xC0,0x0F,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,
    0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x0F,0xFC,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF8,0x18,0x1C,0x38,0x1C,0x18,0x1C,0x00,0x38,
    0x00,0x70,0x01,0xC0,0x07,0x00,0x0C,0x06,0x3F,0xFC,0x3F,0xFC,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x38,0x38,0x38,0x1C,0x00,0x38,0x03,0xF0,
    0x03,0xF0,0x00,0x1C,0x00,0x0E,0x38,0x1C,0x38,0x1C,0x0F,0xF0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0xF0,0x01,0xF0,0x03,0x70,0x0C,0x70,
    0x18,0x70,0x30,0x70,0x3F,0xFE,0x00,0x70,0x00,0x70,0x03,0xFE,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xFC,0x18,0x00,0x18,0x00,0x18,0xC0,0x1F,0xF8,
    0x18,0x1C,0x00,0x1E,0x10,0x0E,0x38,0x1C,0x38,0x18,0x0F,0xF0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xF8,0x0C,0x3C,0x18,0x08,0x38,0x00,0x3F,0xF8,
    0x7C,0x1C,0x78,0x0E,0x38,0x0E,0x38,0x0E,0x1C,0x1C,0x07,0xF0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xF8,0x3C,0x08,0x30,0x18,0x00,0x30,0x00,0x60,
    0x00,0xC0,0x01,0xC0,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF8,0x38,0x0C,0x30,0x0E,0x3C,0x0C,0x1F,0xF8,
    0x0F,0xF0,0x38,0x3C,0x30,0x0E,0x70,0x0E,0x38,0x0C,0x0F,0xF0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x38,0x18,0x30,0x0C,0x70,0x0E,0x30,0x0E,
    0x38,0x3E,0x1F,0xEE,0x00,0x1C,0x18,0x18,0x3C,0x30,0x1F,0xE0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xC0,0x03,0xC0,
    0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x03,0xC0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x03,0xC0,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x03,0xC0,0x01,0x80,0x01,0x00,
    0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x1C,0x00,0x70,0x01,0xC0,0x07,0x00,0x1C,0x00,
    0x18,0x00,0x0E,0x00,0x03,0x80,0x00,0xE0,0x00,0x38,0x00,0x0C,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFF,0x00,0x00,
    0x00,0x00,0x7F,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x06,0x00,0x01,0xC0,0x00,0x70,0x00,0x1C,
    0x00,0x0C,0x00,0x38,0x00,0xE0,0x03,0x80,0x0C,0x00,0x10,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x01,0xC0,0x0F,0xF8,0x30,0x0C,0x38,0x0E,0x38,0x0E,0x00,0x3C,
    0x00,0xF0,0x01,0x80,0x01,0x80,0x01,0x00,0x03,0xC0,0x03,0xC0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xF8,0x1C,0x0C,0x31,0xFA,0x33,0x3B,0x76,0x33,
    0x76,0x33,0x76,0x72,0x36,0xFC,0x33,0xBE,0x1C,0x0E,0x07,0xF8,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xC0,0x03,0xC0,0x02,0xE0,0x06,0x60,0x0C,0x70,
    0x0C,0x70,0x0F,0xF8,0x18,0x38,0x10,0x18,0x30,0x1C,0xFC,0x3F,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xF8,0x18,0x1C,0x18,0x1E,0x18,0x1C,0x1F,0xF8,
    0x1F,0xFC,0x18,0x0E,0x18,0x0E,0x18,0x0E,0x18,0x1E,0xFF,0xF8,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xFE,0x1C,0x06,0x38,0x02,0x70,0x00,0x70,0x00,
    0x70,0x00,0x70,0x00,0x70,0x02,0x38,0x02,0x1C,0x0C,0x07,0xF8,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xF0,0x18,0x3C,0x18,0x0E,0x18,0x0E,0x18,0x0E,
    0x18,0x0F,0x18,0x0E,0x18,0x0E,0x18,0x0C,0x18,0x38,0x7F,0xE0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFC,0x1C,0x0E,0x1C,0x02,0x1C,0x10,0x1F,0xF0,
    0x1F,0xF0,0x1C,0x10,0x1C,0x00,0x1C,0x02,0x1C,0x06,0x7F,0xFC,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFE,0x1C,0x06,0x1C,0x00,0x1C,0x18,0x1C,0x38,
    0x1F,0xF8,0x1C,0x18,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x7F,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xF8,0x1C,0x1C,0x38,0x0C,0x70,0x00,0x70,0x00,
    0x70,0x00,0x70,0x3F,0x70,0x1C,0x38,0x1C,0x1C,0x1C,0x0F,0xFC,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0x3F,0x38,0x1C,0x38,0x1C,0x38,0x1C,0x38,0x1C,
    0x3F,0xFC,0x38,0x1C,0x38,0x1C,0x38,0x1C,0x38,0x1C,0xFE,0x3F,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xF8,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,
    0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x1F,0xFC,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xFE,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,
    0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x30,0x70,0x78,0xE0,0x3F,0x80,
    0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x3E,0x18,0x30,0x18,0x60,0x19,0xC0,0x1B,0x80,
    0x1F,0xC0,0x18,0xE0,0x18,0x70,0x18,0x38,0x18,0x1C,0x7E,0x3F,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x00,
    0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x03,0x1C,0x06,0x7F,0xFE,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x1F,0x38,0x1E,0x3C,0x3E,0x3C,0x3E,0x2C,0x6E,
    0x2E,0x6E,0x26,0xCE,0x27,0xCE,0x23,0x8E,0x23,0x8E,0xFB,0x3F,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x1F,0x3C,0x04,0x3E,0x04,0x37,0x04,0x33,0x84,
    0x31,0xC4,0x30,0xE4,0x30,0x34,0x30,0x1C,0x30,0x0C,0xFC,0x04,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x1C,0x1C,0x38,0x0E,0x70,0x0E,0x70,0x07,
    0x70,0x07,0x70,0x07,0x30,0x0E,0x38,0x0E,0x1C,0x1C,0x07,0xF0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xF8,0x1C,0x0E,0x1C,0x0E,0x1C,0x0E,0x1C,0x1C,
    0x1F,0xF8,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x7F,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x1C,0x1C,0x38,0x0E,0x70,0x0E,0x70,0x07,
    0x70,0x07,0x70,0x0F,0x73,0x8E,0x3F,0xCE,0x1C,0x7C,0x07,0xFA,0x00,0x3E,0x00,0x08,
    0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xF8,0x1C,0x1C,0x1C,0x0E,0x1C,0x0C,0x1C,0x78,
    0x1F,0xE0,0x1C,0xE0,0x1C,0x70,0x1C,0x38,0x1C,0x1C,0x7E,0x0F,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xFC,0x30,0x1C,0x30,0x04,0x38,0x00,0x1F,0x80,
    0x03,0xF0,0x00,0x3C,0x20,0x0E,0x20,0x0E,0x38,0x0C,0x3F,0xF8,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xFE,0x61,0xC6,0x41,0xC2,0x01,0xC0,0x01,0xC0,
    0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x0F,0xF0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0x1E,0x38,0x04,0x38,0x04,0x38,0x04,0x38,0x04,
    0x38,0x04,0x38,0x04,0x38,0x04,0x38,0x04,0x1C,0x18,0x0F,0xF0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0x1E,0x18,0x0C,0x1C,0x18,0x0C,0x10,0x0E,0x30,
    0x0E,0x20,0x07,0x60,0x07,0x40,0x03,0xC0,0x03,0x80,0x01,0x80,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x7B,0xCF,0x31,0xC6,0x31,0xC4,0x39,0xCC,0x3B,0xEC,
    0x1B,0xF8,0x1F,0x78,0x1E,0x78,0x0E,0x70,0x0C,0x30,0x0C,0x20,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x3E,0x0C,0x18,0x0E,0x30,0x07,0x60,0x03,0xC0,
    0x01,0xC0,0x03,0xE0,0x06,0x70,0x0C,0x38,0x08,0x1C,0x7C,0x3F,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x3E,0x1C,0x08,0x0C,0x10,0x0E,0x30,0x07,0x60,
    0x03,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x0F,0xF0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xFE,0x38,0x1C,0x20,0x38,0x00,0x70,0x00,0xE0,
    0x01,0x80,0x01,0x80,0x00,0x80,0x00,0x00,0x03,0xC0,0x03,0xC0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x01,0xFC,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,
    0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0xFC,0x01,0xFC,
    0x00,0x00,0x00,0x00,0x10,0x00,0x18,0x00,0x0C,0x00,0x06,0x00,0x03,0x00,0x03,0x80,
    0x01,0x80,0x00,0xC0,0x00,0x60,0x00,0x30,0x00,0x38,0x00,0x18,0x00,0x0C,0x00,0x06,
    0x00,0x00,0x1F,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,
    0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x1F,0xC0,0x1F,0xC0,
    0x00,0x00,0x03,0xE0,0x0E,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
    0x00,0x00,0x1F,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xE0,0x1E,0x78,
    0x38,0x1C,0x07,0xFC,0x1E,0x1C,0x30,0x1C,0x30,0x3D,0x1F,0xFE,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x08,0x00,0x78,0x00,0x18,0x00,0x18,0x00,0x19,0xF0,0x1F,0xFC,
    0x1C,0x0C,0x18,0x0E,0x18,0x0E,0x18,0x0E,0x18,0x1C,0x1F,0xF0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xE0,0x0F,0x38,
    0x18,0x1C,0x38,0x00,0x38,0x00,0x38,0x04,0x1C,0x0C,0x0F,0xF8,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x00,0x1C,0x00,0x1C,0x03,0xDC,0x0E,0x7C,
    0x38,0x1C,0x38,0x1C,0x38,0x1C,0x38,0x1C,0x1C,0x3C,0x0F,0xFE,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xE0,0x0F,0x78,
    0x18,0x0C,0x3F,0xFE,0x3F,0xFC,0x38,0x04,0x1C,0x0C,0x07,0xF0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xFE,0x03,0x07,0x07,0x00,0x3F,0xF8,0x3F,0xF8,
    0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x3F,0xF0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xEE,0x0E,0x7E,
    0x18,0x18,0x18,0x18,0x0F,0xF0,0x19,0x80,0x1F,0xFC,0x38,0x3E,0x30,0x0E,0x1F,0xF8,
    0x00,0x00,0x00,0x00,0x08,0x00,0x78,0x00,0x18,0x00,0x18,0x00,0x19,0xF0,0x1F,0xF8,
    0x18,0x1C,0x18,0x1C,0x18,0x1C,0x18,0x1C,0x18,0x1C,0x7E,0x3F,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xC0,0x01,0xC0,0x00,0x00,0x0F,0xC0,0x0F,0xC0,
    0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x1F,0xF8,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x00,0x38,0x00,0x00,0x01,0xF8,0x01,0xF8,
    0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x18,0x30,0x1F,0xE0,
    0x00,0x00,0x00,0x00,0x08,0x00,0x38,0x00,0x18,0x00,0x18,0x00,0x18,0x7C,0x18,0x7C,
    0x18,0xE0,0x1B,0x80,0x1E,0xE0,0x18,0x70,0x18,0x38,0x7E,0x3F,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x80,0x0F,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,
    0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x1F,0xFC,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x77,0x1C,0x7F,0xFE,
    0x31,0xC6,0x31,0xC6,0x31,0xC6,0x31,0xC6,0x31,0xC6,0xFB,0xFF,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x79,0xF0,0x7F,0xF8,
    0x18,0x1C,0x18,0x1C,0x18,0x1C,0x18,0x1C,0x18,0x1C,0x7E,0x3F,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xE0,0x0F,0x78,
    0x38,0x0C,0x30,0x0E,0x70,0x0E,0x38,0x0E,0x18,0x1C,0x0F,0xF0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x79,0xE0,0x7F,0xFC,
    0x18,0x0E,0x18,0x0E,0x18,0x0E,0x18,0x0E,0x1C,0x1C,0x1F,0xF8,0x18,0x00,0x7E,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x1E,0x7C,
    0x38,0x1C,0x38,0x1C,0x30,0x1C,0x38,0x1C,0x18,0x1C,0x0F,0xFC,0x00,0x1C,0x00,0x3E,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x3C,0x7E,0xFE,
    0x0F,0x04,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x7F,0xE0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xE8,0x0E,0x7C,
    0x18,0x0C,0x0F,0x80,0x03,0xF8,0x10,0x1C,0x18,0x0C,0x1F,0xF8,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x03,0x00,0x3F,0xF8,0x3F,0xF8,
    0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x04,0x03,0x0C,0x01,0xF8,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x3C,0x78,0x3C,
    0x18,0x1C,0x18,0x1C,0x18,0x1C,0x18,0x1C,0x18,0x3C,0x0F,0xFE,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x1E,0x3E,0x1E,
    0x0C,0x18,0x0E,0x30,0x07,0x20,0x03,0x60,0x03,0xC0,0x01,0x80,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7B,0xCF,0x7B,0xCF,
    0x39,0xCC,0x39,0xCC,0x1B,0xE8,0x1E,0x78,0x0E,0x70,0x0C,0x30,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x3C,0x3E,0x3C,
    0x07,0x20,0x03,0xC0,0x01,0xC0,0x06,0x70,0x0C,0x38,0x7E,0x7E,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x3E,0x3E,0x3E,
    0x0E,0x18,0x06,0x30,0x03,0x60,0x03,0xE0,0x01,0xC0,0x01,0x80,0x19,0x00,0x3E,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xF8,0x3F,0xF8,
    0x30,0x70,0x01,0xC0,0x03,0x80,0x07,0x04,0x1C,0x0C,0x3F,0xFC,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x1C,0x00,0x20,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0xE0,
    0x01,0xC0,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x38,0x00,0x0C,
    0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,
    0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,
    0x00,0x00,0x3C,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x03,0x00,
    0x01,0x80,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x0C,0x00,0x18,0x00,
    0x0E,0x00,0x3F,0x82,0x61,0xE6,0x00,0x7C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

inline short t_max(short a,short b) { return a > b ? a : b; }
inline short t_min(short a,short b) { return a < b ? a : b; }

inline unsigned char clip(short x)
{
    return (unsigned char) t_max(0, t_min(255, x));
}
void set_pixel(coord_t offset,yuv_color_t color,img_Buffer_t *img)
{
    img_Buffer_t *yuv=img;
    if(offset.x>2 && offset.x<yuv->width-2 && offset.y>2 && offset.y<yuv->height-2)
    {

        switch (G.imageFormat){
            case 0 :
                if((offset.x&1)==0) //%2
                {
                    yuv->ptr[2*offset.y*yuv->width+2*offset.x]= color.u;
                    yuv->ptr[2*offset.y*yuv->width+2*offset.x+1]= color.y;
                    yuv->ptr[2*offset.y*yuv->width+2*offset.x+2]= color.v;
                }
                else
                {
                    yuv->ptr[2*offset.y*yuv->width+2*offset.x]= color.v;
                    yuv->ptr[2*offset.y*yuv->width+2*offset.x+1]= color.y;
                    yuv->ptr[2*offset.y*yuv->width+2*offset.x+2]= color.u;
                }
                break;
            case 1 :
                if((offset.x&1)==0)
                {
                    yuv->ptr[2*offset.y*yuv->width+2*offset.x-1]= color.v;
                    yuv->ptr[2*offset.y*yuv->width+2*offset.x]= color.y;
                    yuv->ptr[2*offset.y*yuv->width+2*offset.x+1]= color.u;
                }
                else
                {
                    yuv->ptr[2*offset.y*yuv->width+2*offset.x-1]= color.u;
                    yuv->ptr[2*offset.y*yuv->width+2*offset.x]= color.y;
                    yuv->ptr[2*offset.y*yuv->width+2*offset.x+1]= color.v;
                }
                break;
            case 2 :
            {
                unsigned char *y_ptr = yuv->ptr;
                unsigned char *u_ptr = y_ptr + yuv->width * yuv->height;
                unsigned char *v_ptr = u_ptr + yuv->width * yuv->height/4;
                y_ptr[offset.y*yuv->width+offset.x]=color.y;
                unsigned int k = (offset.y/2) * (yuv->width/2) + offset.x/2;
                u_ptr[k]=color.u;
                v_ptr[k]=color.v;
                break;
            }
            case 3 :
            {
                unsigned char *y_ptr = yuv->ptr;
                unsigned char *uv_ptr = y_ptr + yuv->width * yuv->height;
                y_ptr[offset.y*yuv->width+offset.x]=color.y;
                unsigned int k = (offset.y/2)*(yuv->width/2) + offset.x/2;
                uv_ptr[k*2]=color.u;
                uv_ptr[k*2+1]=color.v;
                break;
            }

            default:printf("error\n");
        }

    }
}
void black_white_image(coord_t offset,yuv_color_t color,img_Buffer_t *img)
{
    img_Buffer_t *yuv=img;
    if(offset.x>1 && offset.x<yuv->width-1 && offset.y>1 && offset.y<yuv->height-1)
    {

        switch (G.imageFormat){
            case 0 :
                if((offset.x&1)==0)
                {
                    yuv->ptr[2*offset.y*yuv->width+2*offset.x]= color.u;  //u
                  //  yuv->ptr[2*offset.y*yuv->width+2*offset.x+1]= 128;    //y
                }
                else
                {
                    yuv->ptr[2*offset.y*yuv->width+2*offset.x]= color.v;  //v
                  //  yuv->ptr[2*offset.y*yuv->width+2*offset.x+1]= 128;    //y
                }
                break;
            case 1 :
                if((offset.x&1)==0)
                {
                  //  yuv->ptr[2*offset.y*yuv->width+2*offset.x]= color.y;   //y
                    yuv->ptr[2*offset.y*yuv->width+2*offset.x+1]= color.u; //u
                }
                else
                {
                  //  yuv->ptr[2*offset.y*yuv->width+2*offset.x]= color.y;   //y
                    yuv->ptr[2*offset.y*yuv->width+2*offset.x+1]= color.v; //v
                }
                break;
            case 2 :
            {
                unsigned char *y_ptr = yuv->ptr;
                unsigned char *u_ptr = y_ptr + yuv->width * yuv->height;
                unsigned char *v_ptr = u_ptr + yuv->width * yuv->height/4;
          //      y_ptr[offset.y*yuv->width+offset.x]=color.y;
                unsigned int k = (offset.y/2) * (yuv->width/2) + offset.x/2;
                u_ptr[k]=color.u;
                v_ptr[k]=color.v;
                break;
            }
            case 3 :
            {
                unsigned char *y_ptr = yuv->ptr;
                unsigned char *uv_ptr = y_ptr + yuv->width * yuv->height;
           //     y_ptr[offset.y*yuv->width+offset.x]=color.y;
                unsigned int k = (offset.y/2)*(yuv->width/2) + offset.x/2;
                uv_ptr[k*2]=color.u;
                uv_ptr[k*2+1]=color.v;
                break;
            }

            default:printf("error\n");
        }

    }
}
void disp_english_string(coord_t offset,char s_string[],yuv_color_t color,img_Buffer_t *img)
{
    img_Buffer_t *yuv=img;
    unsigned int x,y,point,i;
    unsigned char* addr;
    unsigned short value;
    coord_t display;
    yuv_color_t color_foreground;
    unsigned char string_len;
    string_len = strlen(s_string);
    i=0;

    if(offset.x>(yuv->width-string_len*16))
    {
        offset.x=(yuv->width-string_len*16);
    }
    if(offset.y>(yuv->height-16))
    {
        offset.y = (yuv->height-16);
    }
    color_foreground=color;
    while(i != string_len )
    {
        addr=ASCII_addr+(s_string[i]-32)*32;

        for(y=0;y<32;y+=2)
        {
            value=((*(addr+y))<<8)|(*(addr+y+1));
            for(x=0;x<16;x++)
            {
                point=value&(1<<(15-x));
                display.x=offset.x+x;
                display.y=offset.y+y/2;
                if(point==(1<<(15-x)))
                    set_pixel(display,color_foreground,yuv);
                else
                    set_pixel(display,colorGray,yuv);//背景色
            }
        }
        i++;
        offset.x=offset.x+16;
    }
}
void disp_variable(coord_t offset,int variable,yuv_color_t color,img_Buffer_t *img)
{
    img_Buffer_t *yuv=img;
    char s_string[20];
    sprintf(s_string,"%d",variable);
    disp_english_string(offset,s_string,color,yuv);

}
void disp_variable_f(coord_t offset,double variable,yuv_color_t color,img_Buffer_t *img)
{
    img_Buffer_t *yuv=img;
    char s_string[20];
    sprintf(s_string,"%g",variable);
    disp_english_string(offset,s_string,color,yuv);

}
void display_imageFormat(input_param_t input,coord_t offset,yuv_color_t color,img_Buffer_t *img)
{
    img_Buffer_t *yuv=img;
    coord_t display,disp_temp;

    disp_temp.x=10;
    disp_temp.y=10;
    char name[]={"ImageFormat:"};
    display.x=disp_temp.x;
    display.y=disp_temp.y;
    disp_english_string(display,name,colorWhite,yuv);
    display.x=240;
    display.y=10;
    char f_0[]={"YUV422_Packed_[UYVY]"};
    char f_1[]={"YUV422_Packed_[YUYV]"};
    char f_2[]={"YUV420P_Planar_[Y|U|V]"};
    char f_3[]={"YUV420SP_[Y|UV]"};
    switch (input.imageFormat){
        case 0 : disp_english_string(display,f_0,colorWhite,yuv);break;
        case 1 : disp_english_string(display,f_1,colorWhite,yuv);break;
        case 2 : disp_english_string(display,f_2,colorWhite,yuv);break;
        case 3 : disp_english_string(display,f_3,colorWhite,yuv);break;
        default:printf("error\n");
    }
    display.x=disp_temp.x;
    display.y=disp_temp.y+20;
    char ch[]={"Channel"};
    disp_english_string(display,ch,colorWhite,yuv);

    display.x=disp_temp.x+230;
    disp_variable(display,input.ch,colorWhite,yuv);

}
void All_Right_Reserved(img_Buffer_t *img)
{
    img_Buffer_t *yuv=img;
    coord_t display;
    display.x=8;
    display.y=yuv->height-20;

    char Version[]={"S/N:G20160312A0001"};
    disp_english_string(display,Version,colorWhite,yuv);

    display.x=yuv->width-300;
    display.y=yuv->height-20;
    char Copyright[]={"All Right Reserved"};
    disp_english_string(display,Copyright,colorWhite,yuv);
}
int rgb24_to_yuv422_packed_uyvy(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer)
{
    int i,j;
    img_Buffer_t *rgb,*yuv;
    rgb = in_buffer;
    yuv= out_buffer;
    unsigned char *rgb_p,*yuv_p;
    rgb_p=rgb->ptr;
    yuv_p=yuv->ptr;
    yuv422_t pix;
    int width = rgb->width;
    int height = rgb->height;
    if(rgb == NULL)
    {
        printf("error: input data null!\n");
        return -1;
    }
    for (j=0; j<height; j++)
    {
        for (i=0; i<width; i+=2, rgb->ptr+=6, yuv->ptr+=4)
        {
                  unsigned char b0 = rgb->ptr[0];
                  unsigned char g0 = rgb->ptr[1];
                  unsigned char r0 = rgb->ptr[2];

                  unsigned char b1 = rgb->ptr[3];
                  unsigned char g1 = rgb->ptr[4];
                  unsigned char r1 = rgb->ptr[5];


                  pix.u0 =    (unsigned char)((  -43 * r0 -  85 * g0 + 128 * b0 )>>8)+128;//u0
                  pix.y0 =    (unsigned char)((   77 * r0 + 151 * g0 +  28 * b0 )>>8);//y0  pass

                  pix.v0 =    (unsigned char)((  128 * r1 - 107 * g1 -  21 * b1 )>>8)+128;//v0
                  pix.y1 =    (unsigned char)((   77 * r1 + 151 * g1 +  28 * b1 )>>8);//y1  pass

                  yuv->ptr[0] =    pix.u0;
                  yuv->ptr[1] =    pix.y0;
                  yuv->ptr[2] =    pix.v0;
                  yuv->ptr[3] =    pix.y1;

        }
    //    rgb->ptr+= width;//加上宽度补全  rgb24不需要
    }

    rgb->ptr = rgb_p;
    yuv->ptr = yuv_p;
    return 1;
}
int rgb32_to_yuv422_packed_uyvy(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer)
{
    int i,j;
    img_Buffer_t *rgb,*yuv;
    rgb = in_buffer;
    yuv= out_buffer;
    unsigned char *rgb_p,*yuv_p;
    rgb_p=rgb->ptr;
    yuv_p=yuv->ptr;
    yuv422_t pix;
    int width = rgb->width;
    int height = rgb->height;
    if(rgb == NULL)
    {
        printf("error: input data null!\n");
        return -1;
    }
    for (j=0; j<height; j++)
    {
        for (i=0; i<width; i+=2, rgb->ptr+=6, yuv->ptr+=4)
        {
                  unsigned char b0 = rgb->ptr[0];
                  unsigned char g0 = rgb->ptr[1];
                  unsigned char r0 = rgb->ptr[2];

                  unsigned char b1 = rgb->ptr[3];
                  unsigned char g1 = rgb->ptr[4];
                  unsigned char r1 = rgb->ptr[5];


                  pix.u0 =    (unsigned char)((  -43 * r0 -  85 * g0 + 128 * b0 )>>8)+128;//u0
                  pix.y0 =    (unsigned char)((   77 * r0 + 151 * g0 +  28 * b0 )>>8);//y0  pass

                  pix.v0 =    (unsigned char)((  128 * r1 - 107 * g1 -  21 * b1 )>>8)+128;//v0
                  pix.y1 =    (unsigned char)((   77 * r1 + 151 * g1 +  28 * b1 )>>8);//y1  pass

                  yuv->ptr[0] =    pix.u0;
                  yuv->ptr[1] =    pix.y0;
                  yuv->ptr[2] =    pix.v0;
                  yuv->ptr[3] =    pix.y1;

        }
        rgb->ptr+= width;//加上宽度补全
    }

    rgb->ptr = rgb_p;
    yuv->ptr = yuv_p;
    return 1;
}
int rgb32_to_yuv422_packed_yuyv(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer)
{
    int i,j;
    img_Buffer_t *rgb,*yuv;
    rgb = in_buffer;
    yuv= out_buffer;
    unsigned char *rgb_p,*yuv_p;
    rgb_p=rgb->ptr;
    yuv_p=yuv->ptr;
    yuv422_t pix;
    int width = rgb->width;
    int height = rgb->height;
    if(rgb == NULL)
    {
        printf("error: input data null!\n");
        return -1;
    }
    for (j=0; j<height; j++)
    {
        for (i=0; i<width; i+=2, rgb->ptr+=6, yuv->ptr+=4)
        {
                  unsigned char b0 = rgb->ptr[0];
                  unsigned char g0 = rgb->ptr[1];
                  unsigned char r0 = rgb->ptr[2];
                  unsigned char b1 = rgb->ptr[3];
                  unsigned char g1 = rgb->ptr[4];
                  unsigned char r1 = rgb->ptr[5];


                  pix.u0 =    (unsigned char)((  -43 * r0 -  85 * g0 + 128 * b0 )>>8)+128;//u0
                  pix.y0 =    (unsigned char)((   77 * r0 + 151 * g0 +  28 * b0 )>>8);//y0  pass

                  pix.v0 =    (unsigned char)((  128 * r1 - 107 * g1 -  21 * b1 )>>8)+128;//v0
                  pix.y1 =    (unsigned char)((   77 * r1 + 151 * g1 +  28 * b1 )>>8);//y1  pass


                  yuv->ptr[0] =    pix.y0;
                  yuv->ptr[1] =    pix.u0;
                  yuv->ptr[2] =    pix.y1;
                  yuv->ptr[3] =    pix.v0;

        }
        rgb->ptr+= width;
    }

    rgb->ptr = rgb_p;
    yuv->ptr = yuv_p;
    return 1;
}
int rgb32_to_yuv420p(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer)
{
    int i,j;
    img_Buffer_t *rgb,*yuv;
    rgb = in_buffer;
    yuv = out_buffer;
    unsigned char *rgb_p,*yuv_p;
    rgb_p=rgb->ptr;
    yuv_p=yuv->ptr;
    unsigned char *rgb_ptr_0,*y_ptr_0,*rgb_ptr_1,*y_ptr_1,*u_ptr,*v_ptr;
    unsigned char r[4],g[4],b[4],y[4],u,v;
    if(rgb == NULL)
    {
        printf("error: input data null!\n");
        return -1;
    }
    int width = rgb->width;
    int height =rgb->height;
    int nwidth=width*4;
    u_ptr=yuv->ptr + width*height;
    v_ptr=u_ptr + width*height/4;

    for (j=0; j<height; j+=2)
    {

        rgb_ptr_0  =  rgb->ptr + j * nwidth;   //rgb每个像素3字节,后width字节要补全
        rgb_ptr_1  =  rgb_ptr_0 + nwidth; //下一行

        y_ptr_0    =  yuv->ptr + j * width;       //y每个像素1字节
        y_ptr_1    =  y_ptr_0 + width;//下一行

        for (i = 0;i<nwidth;i+=8) //rgb每个像素3字节,后width字节要补全
        {

            b[0] = *(rgb_ptr_0++);
            g[0] = *(rgb_ptr_0++);
            r[0] = *(rgb_ptr_0++);

            b[1] = *(rgb_ptr_0++);
            g[1] = *(rgb_ptr_0++);
            r[1] = *(rgb_ptr_0++);

            b[2] = *(rgb_ptr_1++);
            g[2] = *(rgb_ptr_1++);
            r[2] = *(rgb_ptr_1++);

            b[3] = *(rgb_ptr_1++);
            g[3] = *(rgb_ptr_1++);
            r[3] = *(rgb_ptr_1++);

            y[0] =    (unsigned char)((   77 * r[0] + 151 * g[0] +  28 * b[0] )>>8);//y0  pass
            y[1] =    (unsigned char)((   77 * r[1] + 151 * g[1] +  28 * b[1] )>>8);//y1  pass
            y[2] =    (unsigned char)((   77 * r[2] + 151 * g[2] +  28 * b[2] )>>8);//y0  pass
            y[3] =    (unsigned char)((   77 * r[3] + 151 * g[3] +  28 * b[3] )>>8);//y1  pass
            u =    (unsigned char)((  -43 * r[0] -  85 * g[0] + 128 * b[0] )>>8)+128;//u0
            v =    (unsigned char)((  128 * r[3] - 107 * g[3] -  21 * b[3] )>>8)+128;//v0

            *(y_ptr_0++)=y[0];
            *(y_ptr_0++)=y[1];

            *(y_ptr_1++)=y[2];
            *(y_ptr_1++)=y[3];

            *(u_ptr++) =u;//存u分量
            *(v_ptr++) =v;//存v分量

        }

    }
    rgb->ptr = rgb_p;
    yuv->ptr = yuv_p;
    return 1;
}
int rgb32_to_yuv420sp(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer)
{
    int i,j;
    img_Buffer_t *rgb,*yuv;
    rgb = in_buffer;
    yuv = out_buffer;
    unsigned char *rgb_p,*yuv_p;
    rgb_p=rgb->ptr;
    yuv_p=yuv->ptr;
    unsigned char *rgb_ptr_0,*y_ptr_0,*rgb_ptr_1,*y_ptr_1,*uv_ptr;
    unsigned char r[4],g[4],b[4],y[4],u,v;
    if(rgb == NULL)
    {
        printf("error: input data null!\n");
        return -1;
    }
    int width = rgb->width;
    int height =rgb->height;
    int nwidth=width*4;
    uv_ptr=yuv->ptr + width*height;
    for (j=0; j<height; j+=2)
    {

        rgb_ptr_0  =  rgb->ptr + j * nwidth;   //rgb每个像素3字节,后width字节要补全
        rgb_ptr_1  =  rgb_ptr_0 + nwidth; //下一行

        y_ptr_0    =  yuv->ptr + j * width;       //y每个像素1字节
        y_ptr_1    =  y_ptr_0 + width;//下一行

        for (i = 0;i<nwidth;i+=8) //rgb每个像素3字节,后width字节要补全
        {

            b[0] = *(rgb_ptr_0++);
            g[0] = *(rgb_ptr_0++);
            r[0] = *(rgb_ptr_0++);

            b[1] = *(rgb_ptr_0++);
            g[1] = *(rgb_ptr_0++);
            r[1] = *(rgb_ptr_0++);

            b[2] = *(rgb_ptr_1++);
            g[2] = *(rgb_ptr_1++);
            r[2] = *(rgb_ptr_1++);

            b[3] = *(rgb_ptr_1++);
            g[3] = *(rgb_ptr_1++);
            r[3] = *(rgb_ptr_1++);

            y[0] =    (unsigned char)((   77 * r[0] + 151 * g[0] +  28 * b[0] )>>8);//y0  pass
            y[1] =    (unsigned char)((   77 * r[1] + 151 * g[1] +  28 * b[1] )>>8);//y1  pass
            y[2] =    (unsigned char)((   77 * r[2] + 151 * g[2] +  28 * b[2] )>>8);//y0  pass
            y[3] =    (unsigned char)((   77 * r[3] + 151 * g[3] +  28 * b[3] )>>8);//y1  pass
            u    =    (unsigned char)((  -43 * r[0] -  85 * g[0] + 128 * b[0] )>>8)+128;//u0
            v    =    (unsigned char)((  128 * r[3] - 107 * g[3] -  21 * b[3] )>>8)+128;//v0

            *(y_ptr_0++)=y[0];
            *(y_ptr_0++)=y[1];

            *(y_ptr_1++)=y[2];
            *(y_ptr_1++)=y[3];

            *(uv_ptr++) =u;//存u分量
            *(uv_ptr++) =v;//存v分量

        }
    }
    rgb->ptr = rgb_p;
    yuv->ptr = yuv_p;
    return 1;
}
int yuv422_packed_uyvy_to_rgb24(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer)
{
    int i,j;
    img_Buffer_t *yuv,*rgb;
    yuv = in_buffer;
    rgb = out_buffer;
    unsigned char *rgb_p,*yuv_p;
    yuv_p=yuv->ptr;
    rgb_p=rgb->ptr;
    yuv422_t pix;
    int width = rgb->width;
    int height = rgb->height;
    if (yuv == NULL )
    {
        printf ("error: input data null!\n");
        return -1;
    }
    for (j=0; j<height; j++)
    {
        for(i = 0; i < width; i+=2, yuv->ptr += 4, rgb->ptr += 6)
        {

            pix.u0  = yuv->ptr[0];
            pix.y0  = yuv->ptr[1];
            pix.v0  = yuv->ptr[2];
            pix.y1  = yuv->ptr[3];


            rgb->ptr[0] = clip( (short)(( 256*pix.y0 + 456*pix.u0 -     pix.v0 - 58112 )>>8) );//b
            rgb->ptr[1] = clip( (short)(( 256*pix.y0 -  86*pix.u0 - 183*pix.v0 + 34547 )>>8) );//g
            rgb->ptr[2] = clip( (short)(( 256*pix.y0 +   3*pix.u0 + 359*pix.v0 - 46191 )>>8) );//r

            rgb->ptr[3] = clip( (short)(( 256*pix.y1 + 456*pix.u0 -     pix.v0 - 58112 )>>8) );//b
            rgb->ptr[4] = clip( (short)(( 256*pix.y1 -  86*pix.u0 - 183*pix.v0 + 34547 )>>8) );//g
            rgb->ptr[5] = clip( (short)(( 256*pix.y1 +   3*pix.u0 + 359*pix.v0 - 46191 )>>8) );//r

        }
     //   rgb->ptr+= width;//补全宽度  rgb24不需要
    }
    yuv->ptr = yuv_p;
    rgb->ptr = rgb_p;

    return 1;
}
int yuv422_packed_uyvy_to_rgb32(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer)
{
    int i,j;
    img_Buffer_t *yuv,*rgb;
    yuv = in_buffer;
    rgb = out_buffer;
    unsigned char *rgb_p,*yuv_p;
    yuv_p=yuv->ptr;
    rgb_p=rgb->ptr;
    yuv422_t pix;
    int width = rgb->width;
    int height = rgb->height;
    if (yuv == NULL )
    {
        printf ("error: input data null!\n");
        return -1;
    }
    for (j=0; j<height; j++)
    {
        for(i = 0; i < width; i+=2, yuv->ptr += 4, rgb->ptr += 6)
        {

            pix.u0  = yuv->ptr[0];
            pix.y0  = yuv->ptr[1];
            pix.v0  = yuv->ptr[2];
            pix.y1  = yuv->ptr[3];


            rgb->ptr[0] = clip( (short)(( 256*pix.y0 + 456*pix.u0 -     pix.v0 - 58112 )>>8) );//b
            rgb->ptr[1] = clip( (short)(( 256*pix.y0 -  86*pix.u0 - 183*pix.v0 + 34547 )>>8) );//g
            rgb->ptr[2] = clip( (short)(( 256*pix.y0 +   3*pix.u0 + 359*pix.v0 - 46191 )>>8) );//r

            rgb->ptr[3] = clip( (short)(( 256*pix.y1 + 456*pix.u0 -     pix.v0 - 58112 )>>8) );//b
            rgb->ptr[4] = clip( (short)(( 256*pix.y1 -  86*pix.u0 - 183*pix.v0 + 34547 )>>8) );//g
            rgb->ptr[5] = clip( (short)(( 256*pix.y1 +   3*pix.u0 + 359*pix.v0 - 46191 )>>8) );//r

        }
        rgb->ptr+= width;//补全宽度
    }
    yuv->ptr = yuv_p;
    rgb->ptr = rgb_p;

    return 1;
}
int yuv422_packed_yuyv_to_rgb32(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer)
{
    int i,j;
    img_Buffer_t *yuv,*rgb;
    yuv = in_buffer;
    rgb = out_buffer;
    unsigned char *rgb_p,*yuv_p;
    yuv_p=yuv->ptr;
    rgb_p=rgb->ptr;
    yuv422_t pix;
    int width = rgb->width;
    int height = rgb->height;

    if (yuv == NULL )
    {
        printf ("error: input data null!\n");
        return -1;
    }
    for (j=0; j<height; j++)
    {
        for(i = 0; i < width; i+=2, yuv->ptr += 4, rgb->ptr += 6)
        {

            pix.y0  = yuv->ptr[0];
            pix.u0  = yuv->ptr[1];
            pix.y1  = yuv->ptr[2];
            pix.v0  = yuv->ptr[3];


            rgb->ptr[0] = clip( (short)(( 256*pix.y0 + 456*pix.u0 -     pix.v0 - 58112 )>>8) );//b
            rgb->ptr[1] = clip( (short)(( 256*pix.y0 -  86*pix.u0 - 183*pix.v0 + 34547 )>>8) );//g
            rgb->ptr[2] = clip( (short)(( 256*pix.y0 +   3*pix.u0 + 359*pix.v0 - 46191 )>>8) );//r

            rgb->ptr[3] = clip( (short)(( 256*pix.y1 + 456*pix.u0 -     pix.v0 - 58112 )>>8) );//b
            rgb->ptr[4] = clip( (short)(( 256*pix.y1 -  86*pix.u0 - 183*pix.v0 + 34547 )>>8) );//g
            rgb->ptr[5] = clip( (short)(( 256*pix.y1 +   3*pix.u0 + 359*pix.v0 - 46191 )>>8) );//r

        }
        rgb->ptr+= width;//补全宽度
    }
    yuv->ptr = yuv_p;
    rgb->ptr = rgb_p;
    return 1;
}
//平面YUV420转RGB32
int yuv420p_to_rgb24(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer)
{
    int i,j;
    img_Buffer_t *rgb,*yuv;
    yuv=in_buffer;
    rgb=out_buffer;
    unsigned char *rgb_p,*yuv_p;
    rgb_p=rgb->ptr;
    yuv_p=yuv->ptr;
    if(yuv == NULL)
    {
        printf("error: input data null!\n");
        return -1;
    }
    int width=in_buffer->width;
    int height=in_buffer->height;

    unsigned char *yuv_ptr=yuv->ptr;
    unsigned char *rgb_ptr_0,*rgb_ptr_1;
    unsigned char *y_ptr_0=yuv_ptr;
    unsigned char *y_ptr_1=y_ptr_0 + width;

    unsigned char *u_ptr=y_ptr_0 + width*height;
    unsigned char *v_ptr=u_ptr + width*height/4;
    unsigned char r[4],g[4],b[4],y[4],u,v;

    int nwidth = width*3;
    for (j=0;j<height;j+=2)
    {

        rgb_ptr_0  =  rgb->ptr + j * nwidth;   //rgb每个像素3字节
        rgb_ptr_1  =  rgb_ptr_0 + nwidth; //下一行

        y_ptr_0    =  yuv->ptr + j * width;       //y每个像素1字节
        y_ptr_1    =  y_ptr_0 + width;//下一行

        for (i=0;i<width;i+=2)
        {
            y[0] = *(y_ptr_0++);
            y[1] = *(y_ptr_0++);
            y[2] = *(y_ptr_1++);
            y[3] = *(y_ptr_1++);

            u = *(u_ptr ++);
            v = *(v_ptr ++);

            b[0] = clip( (short)(( 256*y[0] + 456*u -     v - 58112 )>>8) );
            g[0] = clip( (short)(( 256*y[0] -  86*u - 183*v + 34547 )>>8) );
            r[0] = clip( (short)(( 256*y[0] +   3*u + 359*v - 46191 )>>8) );

            b[1] = clip( (short)(( 256*y[1] + 456*u -     v - 58112 )>>8) );
            g[1] = clip( (short)(( 256*y[1] -  86*u - 183*v + 34547 )>>8) );
            r[1] = clip( (short)(( 256*y[1] +   3*u + 359*v - 46191 )>>8) );

            b[2] = clip( (short)(( 256*y[2] + 456*u -     v - 58112 )>>8) );
            g[2] = clip( (short)(( 256*y[2] -  86*u - 183*v + 34547 )>>8) );
            r[2] = clip( (short)(( 256*y[2] +   3*u + 359*v - 46191 )>>8) );

            b[3] = clip( (short)(( 256*y[3] + 456*u -     v - 58112 )>>8) );
            g[3] = clip( (short)(( 256*y[3] -  86*u - 183*v + 34547 )>>8) );
            r[3] = clip( (short)(( 256*y[3] +   3*u + 359*v - 46191 )>>8) );

            *(rgb_ptr_0++)=b[0];
            *(rgb_ptr_0++)=g[0];
            *(rgb_ptr_0++)=r[0];

            *(rgb_ptr_0++)=b[1];
            *(rgb_ptr_0++)=g[1];
            *(rgb_ptr_0++)=r[1];

            *(rgb_ptr_1++)=b[2];
            *(rgb_ptr_1++)=g[2];
            *(rgb_ptr_1++)=r[2];

            *(rgb_ptr_1++)=b[3];
            *(rgb_ptr_1++)=g[3];
            *(rgb_ptr_1++)=r[3];

        }
    }
    rgb->ptr = rgb_p;
    yuv->ptr = yuv_p;

    return 1;
}
//平面YUV420转RGB32
int yuv420p_to_rgb32(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer)
{
    int i,j;
    img_Buffer_t *rgb,*yuv;
    yuv=in_buffer;
    rgb=out_buffer;
    unsigned char *rgb_p,*yuv_p;
    rgb_p=rgb->ptr;
    yuv_p=yuv->ptr;
    if(yuv == NULL)
    {
        printf("error: input data null!\n");
        return -1;
    }
    int width=in_buffer->width;
    int height=in_buffer->height;

    unsigned char *yuv_ptr=yuv->ptr;
    unsigned char *rgb_ptr_0,*rgb_ptr_1;
    unsigned char *y_ptr_0=yuv_ptr;
    unsigned char *y_ptr_1=y_ptr_0 + width;

    unsigned char *u_ptr=y_ptr_0 + width*height;
    unsigned char *v_ptr=u_ptr + width*height/4;
    unsigned char r[4],g[4],b[4],y[4],u,v;

    int nwidth = width*4;
    for (j=0;j<height;j+=2)
    {

        rgb_ptr_0  =  rgb->ptr + j * nwidth;   //rgb每个像素3字节
        rgb_ptr_1  =  rgb_ptr_0 + nwidth; //下一行

        y_ptr_0    =  yuv->ptr + j * width;       //y每个像素1字节
        y_ptr_1    =  y_ptr_0 + width;//下一行

        for (i=0;i<width;i+=2)
        {
            y[0] = *(y_ptr_0++);
            y[1] = *(y_ptr_0++);
            y[2] = *(y_ptr_1++);
            y[3] = *(y_ptr_1++);

            u = *(u_ptr ++);
            v = *(v_ptr ++);

            b[0] = clip( (short)(( 256*y[0] + 456*u -     v - 58112 )>>8) );
            g[0] = clip( (short)(( 256*y[0] -  86*u - 183*v + 34547 )>>8) );
            r[0] = clip( (short)(( 256*y[0] +   3*u + 359*v - 46191 )>>8) );

            b[1] = clip( (short)(( 256*y[1] + 456*u -     v - 58112 )>>8) );
            g[1] = clip( (short)(( 256*y[1] -  86*u - 183*v + 34547 )>>8) );
            r[1] = clip( (short)(( 256*y[1] +   3*u + 359*v - 46191 )>>8) );

            b[2] = clip( (short)(( 256*y[2] + 456*u -     v - 58112 )>>8) );
            g[2] = clip( (short)(( 256*y[2] -  86*u - 183*v + 34547 )>>8) );
            r[2] = clip( (short)(( 256*y[2] +   3*u + 359*v - 46191 )>>8) );

            b[3] = clip( (short)(( 256*y[3] + 456*u -     v - 58112 )>>8) );
            g[3] = clip( (short)(( 256*y[3] -  86*u - 183*v + 34547 )>>8) );
            r[3] = clip( (short)(( 256*y[3] +   3*u + 359*v - 46191 )>>8) );

            *(rgb_ptr_0++)=b[0];
            *(rgb_ptr_0++)=g[0];
            *(rgb_ptr_0++)=r[0];

            *(rgb_ptr_0++)=b[1];
            *(rgb_ptr_0++)=g[1];
            *(rgb_ptr_0++)=r[1];

            *(rgb_ptr_1++)=b[2];
            *(rgb_ptr_1++)=g[2];
            *(rgb_ptr_1++)=r[2];

            *(rgb_ptr_1++)=b[3];
            *(rgb_ptr_1++)=g[3];
            *(rgb_ptr_1++)=r[3];

        }
    }
    rgb->ptr = rgb_p;
    yuv->ptr = yuv_p;

    return 1;
}
//平面YUV420转RGB24
int yuv420sp_to_rgb24(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer)
{
    int i,j;
    img_Buffer_t *rgb,*yuv;
    yuv=in_buffer;
    rgb=out_buffer;
    unsigned char *rgb_p,*yuv_p;
    rgb_p=rgb->ptr;
    yuv_p=yuv->ptr;
    if(yuv == NULL)
    {
        printf("error: input data null!\n");
        return -1;
    }

    int width=in_buffer->width;
    int height=in_buffer->height;

    unsigned char *yuv_ptr=yuv->ptr;
    unsigned char *rgb_ptr_0,*rgb_ptr_1;
    unsigned char *y_ptr_0=yuv_ptr;
    unsigned char *y_ptr_1=y_ptr_0 + width;

    unsigned char *uv_ptr=y_ptr_0 + width*height;
    unsigned char r[4],g[4],b[4],y[4],u,v;

    int nwidth = width*3;
    for (j=0;j<height;j+=2)
    {

        rgb_ptr_0  =  rgb->ptr + j * nwidth;   //rgb每个像素3字节
        rgb_ptr_1  =  rgb_ptr_0 + nwidth; //下一行

        y_ptr_0    =  yuv->ptr + j * width;       //y每个像素1字节
        y_ptr_1    =  y_ptr_0 + width;//下一行
        for (i=0;i<width;i+=2)
        {
            y[0] = *(y_ptr_0++);
            y[1] = *(y_ptr_0++);
            y[2] = *(y_ptr_1++);
            y[3] = *(y_ptr_1++);

            u = *(uv_ptr ++);
            v = *(uv_ptr ++);

            b[0] = clip( (short)(( 256*y[0] + 456*u -     v - 58112 )>>8) );
            g[0] = clip( (short)(( 256*y[0] -  86*u - 183*v + 34547 )>>8) );
            r[0] = clip( (short)(( 256*y[0] +   3*u + 359*v - 46191 )>>8) );

            b[1] = clip( (short)(( 256*y[1] + 456*u -     v - 58112 )>>8) );
            g[1] = clip( (short)(( 256*y[1] -  86*u - 183*v + 34547 )>>8) );
            r[1] = clip( (short)(( 256*y[1] +   3*u + 359*v - 46191 )>>8) );

            b[2] = clip( (short)(( 256*y[2] + 456*u -     v - 58112 )>>8) );
            g[2] = clip( (short)(( 256*y[2] -  86*u - 183*v + 34547 )>>8) );
            r[2] = clip( (short)(( 256*y[2] +   3*u + 359*v - 46191 )>>8) );

            b[3] = clip( (short)(( 256*y[3] + 456*u -     v - 58112 )>>8) );
            g[3] = clip( (short)(( 256*y[3] -  86*u - 183*v + 34547 )>>8) );
            r[3] = clip( (short)(( 256*y[3] +   3*u + 359*v - 46191 )>>8) );

            *(rgb_ptr_0++)=b[0];
            *(rgb_ptr_0++)=g[0];
            *(rgb_ptr_0++)=r[0];

            *(rgb_ptr_0++)=b[1];
            *(rgb_ptr_0++)=g[1];
            *(rgb_ptr_0++)=r[1];

            *(rgb_ptr_1++)=b[2];
            *(rgb_ptr_1++)=g[2];
            *(rgb_ptr_1++)=r[2];

            *(rgb_ptr_1++)=b[3];
            *(rgb_ptr_1++)=g[3];
            *(rgb_ptr_1++)=r[3];

        }
    }
    rgb->ptr = rgb_p;
    yuv->ptr = yuv_p;

    return 1;
}
//平面YUV420转RGB32
int yuv420sp_to_rgb32(img_Buffer_t *in_buffer,img_Buffer_t *out_buffer)
{
    int i,j;
    img_Buffer_t *rgb,*yuv;
    yuv=in_buffer;
    rgb=out_buffer;
    unsigned char *rgb_p,*yuv_p;
    rgb_p=rgb->ptr;
    yuv_p=yuv->ptr;
    if(yuv == NULL)
    {
        printf("error: input data null!\n");
        return -1;
    }

    int width=in_buffer->width;
    int height=in_buffer->height;

    unsigned char *yuv_ptr=yuv->ptr;
    unsigned char *rgb_ptr_0,*rgb_ptr_1;
    unsigned char *y_ptr_0=yuv_ptr;
    unsigned char *y_ptr_1=y_ptr_0 + width;

    unsigned char *uv_ptr=y_ptr_0 + width*height;
    unsigned char r[4],g[4],b[4],y[4],u,v;

    int nwidth = width*4;
    for (j=0;j<height;j+=2)
    {

        rgb_ptr_0  =  rgb->ptr + j * nwidth;   //rgb每个像素3字节
        rgb_ptr_1  =  rgb_ptr_0 + nwidth; //下一行

        y_ptr_0    =  yuv->ptr + j * width;       //y每个像素1字节
        y_ptr_1    =  y_ptr_0 + width;//下一行
        for (i=0;i<width;i+=2)
        {
            y[0] = *(y_ptr_0++);
            y[1] = *(y_ptr_0++);
            y[2] = *(y_ptr_1++);
            y[3] = *(y_ptr_1++);

            u = *(uv_ptr ++);
            v = *(uv_ptr ++);

            b[0] = clip( (short)(( 256*y[0] + 456*u -     v - 58112 )>>8) );
            g[0] = clip( (short)(( 256*y[0] -  86*u - 183*v + 34547 )>>8) );
            r[0] = clip( (short)(( 256*y[0] +   3*u + 359*v - 46191 )>>8) );

            b[1] = clip( (short)(( 256*y[1] + 456*u -     v - 58112 )>>8) );
            g[1] = clip( (short)(( 256*y[1] -  86*u - 183*v + 34547 )>>8) );
            r[1] = clip( (short)(( 256*y[1] +   3*u + 359*v - 46191 )>>8) );

            b[2] = clip( (short)(( 256*y[2] + 456*u -     v - 58112 )>>8) );
            g[2] = clip( (short)(( 256*y[2] -  86*u - 183*v + 34547 )>>8) );
            r[2] = clip( (short)(( 256*y[2] +   3*u + 359*v - 46191 )>>8) );

            b[3] = clip( (short)(( 256*y[3] + 456*u -     v - 58112 )>>8) );
            g[3] = clip( (short)(( 256*y[3] -  86*u - 183*v + 34547 )>>8) );
            r[3] = clip( (short)(( 256*y[3] +   3*u + 359*v - 46191 )>>8) );

            *(rgb_ptr_0++)=b[0];
            *(rgb_ptr_0++)=g[0];
            *(rgb_ptr_0++)=r[0];

            *(rgb_ptr_0++)=b[1];
            *(rgb_ptr_0++)=g[1];
            *(rgb_ptr_0++)=r[1];

            *(rgb_ptr_1++)=b[2];
            *(rgb_ptr_1++)=g[2];
            *(rgb_ptr_1++)=r[2];

            *(rgb_ptr_1++)=b[3];
            *(rgb_ptr_1++)=g[3];
            *(rgb_ptr_1++)=r[3];

        }
    }
    rgb->ptr = rgb_p;
    yuv->ptr = yuv_p;

    return 1;
}
void LineWithDDA(coord_t start,coord_t end,yuv_color_t color,img_Buffer_t *in_buffer)
{
    coord_t offset;
    img_Buffer_t *yuv = in_buffer;
    int w,h,dx,dy,steps,k;
    double xIn, yIn, x , y;
    w = yuv->width;
    h = yuv->height;

    dx =end.x - start.x;
    dy =end.y - start.y;
    steps=tabSqrt[dx*dx + dy*dy];

    xIn = (double)dx / (double)steps;
    yIn = (double)dy / (double)steps;
    x = start.x, y = start.y;
    for(k=0;k<steps;k++)
    {
        x+=xIn;
        y+=yIn;

        offset.x=x+0.5;
        offset.y=y+0.5;
        if(offset.x>w-2 || offset.x<0 || offset.y<0 || offset.y>h-2 )
            continue;
        set_pixel(offset,color,in_buffer);
    }

}
void yuv_draw_rectangle(coord_t start,coord_t end,yuv_color_t color,img_Buffer_t *in_buffer)
{
    coord_t t1,t2,t3,t4;
    int width=in_buffer->width;
    int height=in_buffer->height;
    if (start.x>width-1)
        start.x=width-1;
    if (start.y>height-1)
        start.y=height-1;
    if (end.x>width-1)
        end.x=width-1;
    if (end.y>height-1)
        end.y=height-1;

    if (start.x<0)
        start.x=0;
    if (start.y<0)
        start.y=0;
    if (end.x<0)
        end.x=0;
    if (end.y<0)
        end.y=0;

    t1=start;
    t3=end;
    t2.x=t1.x;
    t2.y=t3.y;
    t4.x=t3.x;
    t4.y=t1.y;
    LineWithDDA( t1,  t2, color,  in_buffer);
    LineWithDDA( t2,  t3, color,  in_buffer);
    LineWithDDA( t3,  t4, color,  in_buffer);
    LineWithDDA( t4,  t1, color,  in_buffer);
}
void yuv_draw_orthogon(coord_t c, int w, int h,yuv_color_t color,img_Buffer_t *in_buffer)
{
    coord_t start,end;
    int img_w = in_buffer->width;
    int img_h = in_buffer->height;

    if(c.x>img_w||c.y>img_h)
        return;

    start.x=c.x-w/2;
    if(start.x<0)
        start.x=0;

    start.y=c.y-h/2;
    if(start.y<0)
        start.y=0;

    end.x=c.x+w/2;
    if(end.x>img_w-1)
        end.x=img_w-1;

    end.y=c.y+h/2;
    if(end.y>img_h-1)
        end.y=img_h-1;

    yuv_draw_rectangle(start, end, color,in_buffer);

}
void yuv_draw_star(coord_t c, int w, int h,yuv_color_t color,img_Buffer_t *in_buffer)
{
    coord_t start,end;
    int img_w = in_buffer->width;
    int img_h = in_buffer->height;

    if(c.x>img_w||c.y>img_h)
        return;

    start.x=end.x=c.x;
    start.y=c.y-h/2;
    if(start.y<0)
        start.y=0;

    end.y=c.y+h/2;
    if(end.y>img_h-1)
        end.y=img_h-1;

    LineWithDDA( start,  end, color,  in_buffer);

    start.y=end.y=c.y;

    start.x=c.x-w/2;
    if(start.x<0)
        start.x=0;


    end.x=c.x+w/2;
    if(end.x>img_w-1)
        end.x=img_w-1;

    LineWithDDA( start,  end, color,  in_buffer);

}

void yuv_draw_aiming(coord_t c, int w, int h,yuv_color_t color,img_Buffer_t *in_buffer)
{
    coord_t start,end,start1,end1,start2,end2;
    int img_w = in_buffer->width;
    int img_h = in_buffer->height;

    if(c.x>img_w||c.y>img_h)
        return;
    yuv_draw_orthogon(c, w/8, h/8,color,in_buffer);

    start.x=c.x-w/2;
    if(start.x<1)
        start.x=1;

    start.y=c.y;
    if(start.y<1)
        start.y=1;

    end.x=c.x-w/8;
    if(end.x>img_w-2)
        end.x=img_w-2;

    end.y=c.y;
    if(end.y>img_h-2)
        end.y=img_h-2;


    LineWithDDA( start,  end, color,  in_buffer);
/*
    start1=start;
    start2=start;
    end1=end;
    end2=end;
    start1.y=start.y-1;
    end1.y=end.y-1;
    LineWithDDA( start1,  end1, color,  in_buffer);
    start2.y=start.y+1;
    end2.y=end.y+1;
    LineWithDDA( start2,  end2, color,  in_buffer);
*/

    start.y=start.y+1;
    end.y=end.y+1;

    start.x=c.x+w/8;
    if(start.x<1)
        start.x=1;

    start.y=c.y;
    if(start.y<1)
        start.y=1;

    end.x=c.x+w/2;
    if(end.x>img_w-2)
        end.x=img_w-2;

    end.y=c.y;
    if(end.y>img_h-2)
        end.y=img_h-2;


    LineWithDDA( start,  end, color,  in_buffer);
    /*
    start1=start;
    start2=start;
    end1=end;
    end2=end;
    start1.y=start.y-1;
    end1.y=end.y-1;
    LineWithDDA( start1,  end1, color,  in_buffer);
    start2.y=start.y+1;
    end2.y=end.y+1;
    LineWithDDA( start2,  end2, color,  in_buffer);
  */
    start.x=c.x;
    if(start.x<1)
        start.x=1;

    start.y=c.y-h/2;
    if(start.y<1)
        start.y=1;

    end.x=c.x;
    if(end.x>img_w-2)
        end.x=img_w-2;

    end.y=c.y-h/8;
    if(end.y>img_h-2)
        end.y=img_h-2;


    LineWithDDA( start,  end, color,  in_buffer);
    /*
    start1=start;
    start2=start;
    end1=end;
    end2=end;
    start1.x=start.x-1;
    end1.x=end.x-1;
    LineWithDDA( start1,  end1, color,  in_buffer);
    start2.x=start.x+1;
    end2.x=end.x+1;
    LineWithDDA( start2,  end2, color,  in_buffer);
    */
    start.x=c.x;
    if(start.x<1)
        start.x=1;

    start.y=c.y+h/8;
    if(start.y<1)
        start.y=1;

    end.x=c.x;
    if(end.x>img_w-2)
        end.x=img_w-2;

    end.y=c.y+h/2;
    if(end.y>img_h-2)
        end.y=img_h-2;


    LineWithDDA( start,  end, color,  in_buffer);
/*
    start1=start;
    start2=start;
    end1=end;
    end2=end;
    start1.x=start.x-1;
    end1.x=end.x-1;
    LineWithDDA( start1,  end1, color,  in_buffer);
    start2.x=start.x+1;
    end2.x=end.x+1;
    LineWithDDA( start2,  end2, color,  in_buffer);
 */
}

void dis_pyramid_img(int layer)
{
    int w  =G.dis_img->width;
    int w_p=G.pyramid_img[layer]->width;
    int h_p=G.pyramid_img[layer]->height;

    for(int i=0;i<h_p;i++)
    {
        for(int j=0;j<w_p;j++)
        {
            int s = i * w_p + j;
            int k = i * w*2 + j*2;

            if(j%2==0)
            {
                G.dis_img->ptr[k] = G.pyramid_img[layer]->ptr[s].u;
                G.dis_img->ptr[k+1] = G.pyramid_img[layer]->ptr[s].y;
            }
            else
            {
                G.dis_img->ptr[k] = G.pyramid_img[layer]->ptr[s].v;
                G.dis_img->ptr[k+1] = G.pyramid_img[layer]->ptr[s].y;
            }

        }
    }
}

//生成金字塔图像
void creat_pyramid_img(int layer_sum,img_Buffer_t *scr_img)
{
    int w=scr_img->width;
    int h=scr_img->height;

    //原始图像转换为金字塔图像第0层
    for(int i=0;i<h;i+=1)
    {
        for(int j=0;j<w;j+=1)
        {
            int d = i*w+j;
            int s = d+d;

            if(j%2==0)
            {
                G.pyramid_img[0]->ptr[d].u  = scr_img->ptr[s+0];
                G.pyramid_img[0]->ptr[d].y  = scr_img->ptr[s+1];
                G.pyramid_img[0]->ptr[d].v  = scr_img->ptr[s+2];
            }
            else
            {
                G.pyramid_img[0]->ptr[d].v  = scr_img->ptr[s+0];
                G.pyramid_img[0]->ptr[d].y  = scr_img->ptr[s+1];
                G.pyramid_img[0]->ptr[d].u  = scr_img->ptr[s+2];
            }

        }

    }

    img_base_data_t YUV_P[2][4000]={0};
    //金字塔图像第0层生成其余层
    for (int layer=1;layer<=layer_sum;layer++)
    {
        w=G.pyramid_img[layer]->width;
        h=G.pyramid_img[layer]->height;

        for(int i=0;i<h;i+=1)
        {
            int k1= (i*2) * w*2;
            int k2= (i*2+1) * w*2;
            //第一行
            for(int j=0;j<w;j+=1)
            {
                int s = k1 + j*2;
                YUV_P[0][j].y = (G.pyramid_img[layer-1]->ptr[s].y + G.pyramid_img[layer-1]->ptr[s+1].y)/2;
                YUV_P[0][j].u = (G.pyramid_img[layer-1]->ptr[s].u + G.pyramid_img[layer-1]->ptr[s+1].u)/2;
                YUV_P[0][j].v = (G.pyramid_img[layer-1]->ptr[s].v + G.pyramid_img[layer-1]->ptr[s+1].v)/2;
            }
            //第二行
            for(int j=0;j<w;j+=1)
            {
                int s = k2 + j*2;
                YUV_P[1][j].y = (G.pyramid_img[layer-1]->ptr[s].y + G.pyramid_img[layer-1]->ptr[s+1].y)/2;
                YUV_P[1][j].u = (G.pyramid_img[layer-1]->ptr[s].u + G.pyramid_img[layer-1]->ptr[s+1].u)/2;
                YUV_P[1][j].v = (G.pyramid_img[layer-1]->ptr[s].v + G.pyramid_img[layer-1]->ptr[s+1].v)/2;
            }

            for(int p=0;p<w;p++)
            {
                int k = i * w + p;
                G.pyramid_img[layer]->ptr[k].y  = (YUV_P[0][p].y + YUV_P[1][p].y)/2;
                G.pyramid_img[layer]->ptr[k].u  = (YUV_P[0][p].u + YUV_P[1][p].u)/2;
                G.pyramid_img[layer]->ptr[k].v  = (YUV_P[0][p].v + YUV_P[1][p].v)/2;
            }

        }
    }

}

int free_memory()
{
    int success_flag=1;

      //-----------------------------------------dis_img
    if(NULL!=G.dis_img)
    {
        if(NULL!=G.dis_img->ptr)
        {
            free(G.dis_img->ptr);
            G.dis_img->ptr=NULL;
            if(NULL!=G.dis_img->ptr)
            {
                success_flag=0;
            }
        }
        if(NULL!=G.dis_img)
        {
            free(G.dis_img);
            G.dis_img=NULL;
            if(NULL!=G.dis_img)
            {
                success_flag=0;
            }
        }

    }

    //------------------------------------------pyramid_img
    for(int layer=0;layer<LAYER_SUM;layer++)
    {
        if(NULL!=G.pyramid_img[layer])
        {
            if(NULL!=G.pyramid_img[layer]->ptr)
            {
                free(G.pyramid_img[layer]->ptr);
                if(NULL!=G.pyramid_img[layer]->ptr)
                {
                    success_flag=0;
                }
            }
            if(NULL!=G.pyramid_img[layer])
            {
                free(G.pyramid_img[layer]);
                G.pyramid_img[layer]=NULL;
                if(NULL!=G.pyramid_img[layer])
                {
                    success_flag=0;
                }
            }

        }
        //--------------------------------------edge_img
        if(NULL!=G.base_img[layer])
        {

            if(NULL!=G.base_img[layer]->ptr)
            {
                free(G.base_img[layer]->ptr);
                if(NULL!=G.base_img[layer]->ptr)
                {
                    success_flag=0;
                }
            }
            if(NULL!=G.base_img[layer])
            {
                free(G.base_img[layer]);
                G.base_img[layer]=NULL;
                if(NULL!=G.base_img[layer])
                {
                    success_flag=0;
                }
            }
        }
        //--------------------------------------ROIs
        if(NULL!=G.ROIs[layer])
        {

            if(NULL!=G.ROIs[layer]->ptr)
            {
                free(G.ROIs[layer]->ptr);
                if(NULL!=G.ROIs[layer]->ptr)
                {
                    success_flag=0;
                }
            }
            if(NULL!=G.ROIs[layer])
            {
                free(G.ROIs[layer]);
                G.ROIs[layer]=NULL;
                if(NULL!=G.ROIs[layer])
                {
                    success_flag=0;
                }
            }
        }

    }

    if(success_flag==1)
        G.mem_flag=1;
    else
        G.mem_flag=14;
    return 0;

}
int get_zoom(int layer)
{
    int zoom=0;
    switch (layer) {
        case 0:
            zoom=1;
            break;
        case 1:
            zoom=2;
            break;
        case 2:
            zoom=4;
            break;
        case 3:
            zoom=8;
            break;
        case 4:
            zoom=16;
            break;
        case 5:
            zoom=32;
            break;
        default:
            break;
    }

    return zoom;
}

//申请内存
int malloc_memory(img_Buffer_t *scr_img)
{
    int k=0;
    int success_flag=1;
    int img_w = scr_img->width;
    int img_h = scr_img->height;
    int size=img_w * img_h;

    for(int layer=0;layer<LAYER_SUM;layer++)
    {
        int zoom = get_zoom(layer);
        if(zoom==0)
            zoom=1;
        k=size/(zoom*zoom);
        //---------------------------------------------pyramid_img
        if(NULL==G.pyramid_img[layer])
        {
            G.pyramid_img[layer] = (pyramid_img_t*)malloc(sizeof(pyramid_img_t));
            if(NULL==G.pyramid_img[layer])
            {
                //申请不成功
                success_flag=0;
            }

            G.pyramid_img[layer]->ptr = (img_base_data_t *)malloc(sizeof(img_base_data_t)*k);
            if(NULL==G.pyramid_img[layer]->ptr)
            {
                //申请不成功
                success_flag=0;
            }

            if(success_flag==1)
            {
                G.pyramid_img[layer]->width =img_w/zoom;
                G.pyramid_img[layer]->height=img_h/zoom;
            }
        }
        else
        {
            //未释放干净
            success_flag=0;
            break;
        }
        //---------------------------------------------edge_img
        if(NULL==G.base_img[layer])
        {
            G.base_img[layer] = (base_img_t*)malloc(sizeof(base_img_t));
            if(NULL==G.base_img[layer])
            {
                //申请不成功
                success_flag=0;
            }

            G.base_img[layer]->ptr = (base_data_t *)malloc(sizeof(base_data_t)*k);
            if(NULL==G.base_img[layer]->ptr)
            {
                //申请不成功
                success_flag=0;
            }

            if(success_flag==1)
            {
                G.base_img[layer]->width =img_w/zoom;
                G.base_img[layer]->height=img_h/zoom;
            }
        }
        else
        {
            //未释放干净
            success_flag=0;
            break;
        }
        //---------------------------------------------ROIs
        if(NULL==G.ROIs[layer])
        {
            G.ROIs[layer] = (ROI_img_t*)malloc(sizeof(ROI_img_t));
            if(NULL==G.ROIs[layer])
            {
                //申请不成功
                success_flag=0;
            }

            G.ROIs[layer]->ptr = (ROI_data_t *)malloc(sizeof(ROI_data_t)*MAX_ROIS_NUM);
            if(NULL==G.ROIs[layer]->ptr)
            {
                //申请不成功
                success_flag=0;
            }

        }
        else
        {
            //未释放干净
            success_flag=0;
            break;
        }

    }


    //---------------------------------------------dis_img
    k=size*2;
    if(NULL==G.dis_img)
    {
        G.dis_img = (img_Buffer_t*)malloc(sizeof(img_Buffer_t));
        if(NULL==G.dis_img)
        {
            //申请不成功
            success_flag=0;
        }
        G.dis_img->ptr=(unsigned char *)malloc(sizeof(unsigned char)*k);
        if(NULL==G.dis_img->ptr)
        {
            //申请不成功
            success_flag=0;
        }
        if(success_flag==1)
        {
            memset(G.dis_img->ptr,0,sizeof(unsigned char)*k);
            G.dis_img->width =img_w;
            G.dis_img->height=img_h;
        }

    }
    else
    {
        //未释放干净
        success_flag=0;
    }

    if(success_flag==1)
        G.mem_flag=2;
    else
        G.mem_flag=14;
    return 0;
}
//初始化分析参数
int init_param(input_param_t input,img_Buffer_t *yuv)
{
    //第一帧
    if(G_class_init_flag==0)
    {
        //第一帧初始化标志
        G_class_init_flag=1;
        //制作角度转弧度表格
        for(int i=0;i<360;i++)
        {
            double r=i*PI/180;
            tabSin[i]=(double)sinf(r);
            tabCos[i]=(double)cosf(r);
        }
        //制作开方表格
        for(int i=0;i<TABSQRTNUM;i++)
        {
            tabSqrt[i]=sqrt(i);
        }


    }
    return 0;
}

int logfile_init(input_param_t input,char *login_data)
{
    int file_id=random_num*1000+input.ch;
    char filename[20];
    sprintf(filename,"c:\\%d.txt",file_id);
    //获取文件指针
    FILE *pFile = fopen(filename,"w+");
    fwrite(login_data,1,strlen(login_data),pFile);
    fflush(pFile); //数据刷新 数据立即更新
    fclose(pFile);
    pFile=NULL;
    return 0;
}
int logfile_fwrite(input_param_t input,char *login_data)
{
    int file_id=random_num*1000+input.ch;
    char filename[20];
    sprintf(filename,"c:\\%d.txt",file_id);
    //获取文件指针
    FILE *pFile = fopen(filename,"at+");
    fwrite(login_data,1,strlen(login_data),pFile);
    fflush(pFile); //数据刷新 数据立即更新
    fclose(pFile);
    pFile=NULL;
    return 0;
}
void edge_detection(int layer, int Threshold)
{
    /*
     第1步：计算梯度
     首先，由一阶导数算子（一般用sobel模板）计算灰度图像每个像素点在水平和竖直方向上的导数Gx、Gy，得出梯度向量（Gx，Gy），计算梯度的值G和方向theta：
     G=sqrt（Gx*Gx+Gy*Gy)  theta=arctan(Gy/Gx)
     然后，将每个像素点的梯度的值和方向分别放入两个数组中，程序如下：
     */
    coord_t disp_p;
    disp_p.x=0;
    disp_p.y=G.dis_img->height - G.base_img[layer]->height;

    int lowThreshold =Threshold-20; //低阈值
    if(lowThreshold<0)
        lowThreshold=0;
    unsigned char *DisplayBuff=G.dis_img->ptr;
    yuv_color_t color;
    coord_t start,end;
    int img_w=G.base_img[layer]->width;

    //定义倍数
    int zoom = get_zoom(layer);
    //检测区域设定
    start=G.border[layer].area.start;
    end  =G.border[layer].area.end;
    //装载金字塔图像
    img_base_data_t *dat = G.pyramid_img[layer]->ptr;
    //梯度和角度
    int orientation;
    int s[9];
    double gx,gy;
    //定义一个边缘结构
    base_data_t base_data={0};
    for (int i = start.y; i < end.y;i++ )
    {
        for (int j = start.x; j < end.x; j++)
        {
            int k=i * img_w + j;

            s[0]=dat[k-img_w-1].y; s[1]=dat[k-img_w].y; s[2]=dat[k-img_w+1].y;
            s[3]=dat[k-1].y;       s[4]=dat[k].y;       s[5]=dat[k+1].y;
            s[6]=dat[k+img_w-1].y; s[7]=dat[k+img_w].y; s[8]=dat[k+img_w+1].y;

            //求水平和竖直导数
            gx=s[2]+2*s[5]+s[8]-s[0]-2*s[3]-s[6];
            gy=s[0]+2*s[1]+s[2]-s[6]-2*s[7]-s[8];

            //保存灰度值
            int temp = (s[0]+s[1]+s[2]+s[3]+  2*s[4]  +s[5]+s[6]+s[7]+s[8])/10;
            if(temp>255)
                temp=255;
            base_data.gray=temp;

            //保存坐标
            base_data.pix.x=j;
            base_data.pix.y=i;

            //保存方向向量
            base_data.Vt[0]=gx;
            base_data.Vt[1]=gy;

            //计算梯度
            temp = gx * gx + gy * gy;
            base_data.gradients = tabSqrt[temp];
            if(base_data.gradients>lowThreshold)
                base_data.edge_flag = 1;
            else
                base_data.edge_flag = 0;
            //计算角度
            orientation=atan2(gy,gx)*180/PI;
            if(orientation<0)
                orientation=orientation+360;

            base_data.orients = orientation;

            //保存
            base_data.blockID=-1;
            G.base_img[layer]->ptr[k] = base_data;
        }
    }

    base_data_t *edge_dat = G.base_img[layer]->ptr;
    if(key_v=='1')
    {
        color=colorRed;
        for (int i = start.y; i < end.y;i++ )
        {
            for (int j = start.x; j < end.x; j++)
            {
                int k=i*img_w+j;

                if(edge_dat[k].edge_flag==1)
                {
                    k=i*zoom*(img_w+img_w)+j*2;

                    if(j%2==0)
                    {
                        DisplayBuff[k]=color.u;
                        DisplayBuff[k+1]=color.y;
                        DisplayBuff[k+2]=color.v;
                    }
                    else
                    {
                        DisplayBuff[k]=color.v;
                        DisplayBuff[k+1]=color.y;
                        DisplayBuff[k+2]=color.u;
                    }
                }
            }
        }

    }
    /*
     第2步：非极大值抑制
     如果直接把梯度作为边缘的话，将得到一个粗边缘的图像，这不满足上面提到的准则，我们希望得到定位准确的单像素的边缘，所以将每个像素点的梯度与其梯度方向上的相邻像素比较，
     如果不是极大值，将其置0 否则置为某一不大于255的数，程序如下：
     */
    int k;
    int grad[3] = {0};
    int orient[3] = {0};
    for (int i = start.y; i < end.y;i++ )
    {
        for (int j = start.x; j < end.x; j++)
        {
            k=i*img_w+j;
            if(edge_dat[k].edge_flag==0)
                continue;
            grad[0]  =edge_dat[k].gradients;//梯度
            orient[0]=edge_dat[k].orients;//角度

            //根据角度确定方向
            orientation=orient[0];//角度
            if (orientation <= 22.5)
                orientation = 0;
            else if (orientation < 67.5)
                orientation = 45;
            else if (orientation < 112.5)
                orientation = 90;
            else if (orientation < 157.5)
                orientation = 135;
            else if (orientation < 202.5)
                orientation = 180;
            else if (orientation < 247.5)
                orientation = 225;
            else if (orientation < 292.5)
                orientation = 270;
            else if (orientation < 337.5)
                orientation = 315;
            else if (orientation <= 360)
                orientation = 0;

            //获得相邻两像素梯度值
            switch (orientation)//根据角度确定方向
            {
                case 0:
                    grad[1]    =edge_dat[i*img_w+j-1].gradients;
                    grad[2]    =edge_dat[i*img_w+j+1].gradients;

                    orient[1]    =edge_dat[i*img_w+j-1].orients;
                    orient[2]    =edge_dat[i*img_w+j+1].orients;
                    break;
                case 45:
                    grad[1]    =edge_dat[(i+1)*img_w+j-1].gradients;
                    grad[2]    =edge_dat[(i-1)*img_w+j+1].gradients;

                    orient[1]    =edge_dat[(i+1)*img_w+j-1].orients;
                    orient[2]    =edge_dat[(i-1)*img_w+j+1].orients;
                    break;
                case 90:
                    grad[1]    =edge_dat[(i+1)*img_w+j].gradients;
                    grad[2]    =edge_dat[(i-1)*img_w+j].gradients;

                    orient[1]    =edge_dat[(i+1)*img_w+j].orients;
                    orient[2]    =edge_dat[(i-1)*img_w+j].orients;
                    break;
                case 135:
                    grad[1]    =edge_dat[(i+1)*img_w+j+1].gradients;
                    grad[2]    =edge_dat[(i-1)*img_w+j-1].gradients;

                    orient[1]    =edge_dat[(i+1)*img_w+j+1].orients;
                    orient[2]    =edge_dat[(i-1)*img_w+j-1].orients;
                    break;
                case 180:
                    grad[1]    =edge_dat[i*img_w+j+1].gradients;
                    grad[2]    =edge_dat[i*img_w+j-1].gradients;

                    orient[1]    =edge_dat[i*img_w+j+1].orients;
                    orient[2]    =edge_dat[i*img_w+j-1].orients;
                    break;
                case 225:
                    grad[1]    =edge_dat[(i-1)*img_w+j+1].gradients;
                    grad[2]    =edge_dat[(i+1)*img_w+j-1].gradients;

                    orient[1]    =edge_dat[(i-1)*img_w+j+1].orients;
                    orient[2]    =edge_dat[(i+1)*img_w+j-1].orients;
                    break;
                case 270:
                    grad[1]    =edge_dat[(i-1)*img_w+j].gradients;
                    grad[2]    =edge_dat[(i+1)*img_w+j].gradients;

                    orient[1]    =edge_dat[(i-1)*img_w+j].orients;
                    orient[2]    =edge_dat[(i+1)*img_w+j].orients;
                    break;
                case 315:
                    grad[1]    =edge_dat[(i-1)*img_w+j-1].gradients;
                    grad[2]    =edge_dat[(i+1)*img_w+j+1].gradients;

                    orient[1]    =edge_dat[(i-1)*img_w+j-1].orients;
                    orient[2]    =edge_dat[(i+1)*img_w+j+1].orients;
                    break;

                default:
                    break;
            }
            //抑制
            //orients范围（0~360）
            short temp[2]={0};
            int min_offset=10000;
            for(int p=0;p<2;p++)
            {
                temp[p]=abs(orient[0]-orient[p+1]);
                if(temp[p]>180)
                    temp[p]=360-temp[p];
                if(min_offset>temp[p])
                    min_offset=temp[p];

                if(temp[p]<30 && grad[0]<grad[p+1])
                {
                    edge_dat[k].edge_flag = 0;
                    break;
                }

            }


        }
    }

    if(key_v=='2')
    {
        color=colorRed;
        for (int i = start.y; i < end.y;i++ )
        {
            for (int j = start.x; j < end.x; j++)
            {
                int k=i*img_w+j;

                if(edge_dat[k].edge_flag==1)
                {

                    k=i*zoom*(img_w+img_w)+j*2;

                    if(j%2==0)
                    {
                        DisplayBuff[k]=color.u;
                        DisplayBuff[k+1]=color.y;
                        DisplayBuff[k+2]=color.v;
                    }
                    else
                    {
                        DisplayBuff[k]=color.v;
                        DisplayBuff[k+1]=color.y;
                        DisplayBuff[k+2]=color.u;
                    }

                }
            }
        }
    }
    /*
     第3步：双阈值边缘检测
     由上一步得到的边缘还有很多伪边缘，我们通过设置高低双阈值的方法去除它们，具体思想是：梯度值大于高阈值的像素点认为其一定是边缘，置为255，
     梯度值小于低阈值的像素点认为其一定不是边缘置为0，介于两阈值之间的点像素点为待定边缘。然后，考察这些待定边缘点，如果其像素点周围8邻域的梯度值都小于高阈值，
     认为其不是边缘点，置为0；至于，如何设定双阈值大小，我们可以根据实际情况设定，如设成100和20，也可以根据图像梯度值的统计信息设定，一般小阈值是大阈值的0.4倍即可。程序如下：
     */
    for(int i=start.y;i<end.y;i++)
    {
        for(int j=start.x;j<end.x;j++)
        {
            int k=i*img_w+j;
            if(edge_dat[k].edge_flag==0)
                continue;

            int o[9];
            o[0]=edge_dat[k-img_w-1].orients; o[1]=edge_dat[k-img_w].orients; o[2]=edge_dat[k-img_w+1].orients;
            o[3]=edge_dat[k-1].orients;       o[4]=edge_dat[k].orients;       o[5]=edge_dat[k+1].orients;
            o[6]=edge_dat[k+img_w-1].orients; o[7]=edge_dat[k+img_w].orients; o[8]=edge_dat[k+img_w+1].orients;
            int d[9];
            d[0]=edge_dat[k-img_w-1].gradients; d[1]=edge_dat[k-img_w].gradients; d[2]=edge_dat[k-img_w+1].gradients;
            d[3]=edge_dat[k-1].gradients;                                        d[5]=edge_dat[k+1].gradients;
            d[6]=edge_dat[k+img_w-1].gradients; d[7]=edge_dat[k+img_w].gradients; d[8]=edge_dat[k+img_w+1].gradients;

            int count=0;
            int offTH=20;
            int TH=Threshold;
            short temp;

            temp=abs(o[4]-o[0]);
            if(temp>180)
                temp=360-temp;
            if(temp<offTH && d[0]>TH)
                count++;

            temp=abs(o[4]-o[1]);
            if(temp>180)
                temp=360-temp;
            if(temp<offTH && d[1]>TH)
                count++;

            temp=abs(o[4]-o[2]);
            if(temp>180)
                temp=360-temp;
            if(temp<offTH && d[2]>TH)
                count++;

            temp=abs(o[4]-o[3]);
            if(temp>180)
                temp=360-temp;
            if(temp<offTH && d[3]>TH)
                count++;

            temp=abs(o[4]-o[5]);
            if(temp>180)
                temp=360-temp;
            if(temp<offTH && d[5]>TH)
                count++;

            temp=abs(o[4]-o[6]);
            if(temp>180)
                temp=360-temp;
            if(temp<offTH && d[6]>TH)
                count++;

            temp=abs(o[4]-o[7]);
            if(temp>180)
                temp=360-temp;
            if(temp<offTH && d[7]>TH)
                count++;

            temp=abs(o[4]-o[8]);
            if(temp>180)
                temp=360-temp;
            if(temp<offTH && d[8]>TH)
                count++;

            if(count<2)
            {
                edge_dat[k].edge_flag=0;
            }


        }
    }
    if(key_v=='3')
    {
        color=colorRed;
        for (int i = start.y; i < end.y;i++ )
        {
            for (int j = start.x; j < end.x; j++)
            {
                int k=i*img_w+j;

                if(edge_dat[k].edge_flag==1)//梯度
                {
                    k=i*zoom*(img_w+img_w)+j*2;
                    if(j%2==0)
                    {
                        DisplayBuff[k]=color.u;
                        DisplayBuff[k+1]=color.y;
                        DisplayBuff[k+2]=color.v;
                    }
                    else
                    {
                        DisplayBuff[k]=color.v;
                        DisplayBuff[k+1]=color.y;
                        DisplayBuff[k+2]=color.u;
                    }
                }
            }
        }

    }

}

#define MAX_LINES_SIZE 10000
#define MAX_DOTS_SIZE 10000
//广度优先搜索(Vs起点 Vd终点)
void BFS(base_img_t *base_img,int blockID,coord_t Vs,coord_t Vd,block_t *block)
{
    int img_w=base_img->width;
    /*
     //24个方向
     int dir[24][4] = {
     {-2,-2},{-1,-2},{0,-2},{1,-2},{2,-2},
     {-2,-1},{-1,-1},{0,-1},{1,-1},{2,-1},
     {-2, 0},{-1, 0},       {1, 0},{2, 0},
     {-2, 1},{-1, 1},{0, 1},{1, 1},{2, 1},
     {-2, 2},{-1, 2},{0, 2},{1, 2},{2, 2}
     };
     */
    //8个方向
    int step=SEARCHSTEP;
    int dir[8][2] = {
        {-step,-step},{0,-step},{step,-step},
        {-step, 0},             {step, 0},
        {-step, step},{0, step},{step, step}
    };
    int size=0;
    //创建队列
    queue<coord_t> Q;
    //初始状态将起点放进队列Q
    Q.push(Vs);
    //加入到边界图像
    int k=Vs.y*img_w+Vs.x;
    //设置节点已经访问过(不等于-1)
    base_img->ptr[k].blockID=blockID;
    coord_t V={0}, W={0};
    while (!Q.empty()){//队列不为空，继续搜索！
        //取出队列的头Vn
        V = Q.front();
        //弹出队列的第一个元素
        Q.pop();

        for(int i = 0; i < 8; ++i){
            //计算相邻节点
            W.x=V.x+dir[i][0];//x
            W.y=V.y+dir[i][1];//y
            if (W.x < 0 || W.y < 0 || W.x >= Vd.x || W.y >= Vd.y)
            {
                block->size=size;
                return;//到终点返回
            }

            k=W.y*img_w+W.x;
            if (base_img->ptr[k].MatchingDot==true && base_img->ptr[k].blockID==-1)
            {
                //Vw是一个合法的节点并且未访问
                Q.push(W);//新元素加入队列
                //加入到边界图像并设置节点已经访问过
                base_img->ptr[k].blockID=blockID;
                block->ptr[size]=W;
                size++;
                //超出最大尺寸退出
                if(size>MAX_DOTS_SIZE)
                {
                    block->size=MAX_DOTS_SIZE;
                    printf ("error: line->size overflow!\n");
                    return;
                }
            }
        }
    }
    block->size=size;
    return;
}
int seek_flaged(target_unit_t *link,targets_t *targets)
{
    for(int i=0;i<targets->num;i++)
    {
        //新标记（flag=2）
        if(targets->unit[i].flag==1)
        {
            link->w=targets->unit[i].w;
            link->h=targets->unit[i].h;
            link->c_x=targets->unit[i].c_x;
            link->c_y=targets->unit[i].c_y;
            //标记完成（flag=3）
            targets->unit[i].flag=2;
            return 1;
        }

    }
    return 0;
}
//当前所有目标连通成单独块(flag=3，ID=1,2,3...）
int targets_mix(targets_t *targets)
{
    coord_t offset,display;
    int cross_w,cross_h;
    int i,j,w,h,c_x,c_y;
    int ID_NUM=0;
    //连通当前所有关联目标并生成不同ID
    for(i=0;i<targets->num;i++)
    {
        //如果已生成ID则继续，直到全部生成ID
        if(targets->unit[i].GatherID!=0)
            continue;

        ID_NUM++;
        targets->unit[i].GatherID=ID_NUM;
        targets->unit[i].flag=1;
        target_unit_t Link;
        //等待所有flag=2处理完成（一次性处理完所有相交目标）
        while(seek_flaged(&Link,targets))
        {
            //标记所有flag!=3的相交目标
            for(j=0;j<targets->num;j++)
            {
                //如果flag==2表示已经处理过
                if(targets->unit[j].flag==2)
                    continue;
                w=targets->unit[j].w;
                h=targets->unit[j].h;
                c_x=targets->unit[j].c_x;
                c_y=targets->unit[j].c_y;

                //判断是否相交
                cross_w=abs(Link.c_x-c_x)-(Link.w+w)/2;
                cross_h=abs(Link.c_y-c_y)-(Link.h+h)/2;
                if(cross_w<0)//&&cross_h<0)
                {
                    //新标记 flag=2
                    targets->unit[j].flag=1;
                    targets->unit[j].GatherID=ID_NUM;
                }

            }

        }

    }
    return ID_NUM;
}
int creat_target(base_img_t *base_img,area_t area,int dotSize_TH)
{
    coord_t start,end,Vs,Vd;
    int img_w=base_img->width;
    base_data_t *base_dat = base_img->ptr;

    start.x=area.start.x;
    start.y=area.start.y;
    end.x=area.end.x;
    end.y=area.end.y;
    if(end.x==0||end.y==0)
        return 0;
    block_t block={0};
    block.ptr=(coord_t *)malloc(sizeof(coord_t)*MAX_DOTS_SIZE);

    area_t areas[100]={0};
    for(int i=0;i<100;i++)
    {
        areas[i].start.x=20000;
        areas[i].start.y=20000;
    }
    //定义目标
    targets_t target={0};
    //第一个ID编号
    int blockID=0;
    //BFS结束位置
    Vd=end;
    for(int i=start.y;i<end.y;i++)
    {
        Vs.y=i;
        for(int j=start.x;j<end.x;j++)
        {
            Vs.x=j;
            int k=i*img_w+j;

            //如果边界点已标记则查找下一个
            if(base_dat[k].blockID!=-1)
                continue;
            //得到连接块 广度优先搜索(Vs起点 Vd终点)
            BFS(base_img,blockID,Vs,Vd,&block);
            //块中点数量不足继续检测
            if(block.size<dotSize_TH)
                continue;
            //生成初始目标
            for(int s=0;s<block.size;s++)
            {
                coord_t offset=block.ptr[s];

                if(offset.x<areas[blockID].start.x)
                    areas[blockID].start.x=offset.x;
                if(offset.y<areas[blockID].start.y)
                    areas[blockID].start.y=offset.y;
                if(offset.x>areas[blockID].end.x)
                    areas[blockID].end.x=offset.x;
                if(offset.y>areas[blockID].end.y)
                    areas[blockID].end.y=offset.y;
            }
            blockID++;

            //显示初始目标
            for(int t=0;t<blockID;t++)
            {
                coord_t pix0,pix1;
                pix0.x=areas[t].start.x*8;
                pix0.y=areas[t].start.y*8;
                pix1.x=areas[t].end.x*8;
                pix1.y=areas[t].end.y*8;
                yuv_draw_rectangle(pix0,pix1,colorRed,G.dis_img);
            }
            //生成初始目标
            target.num=blockID;
            for(int t=0;t<target.num;t++)
            {
                target.unit[t].w=areas[t].end.x-areas[t].start.x;
                target.unit[t].h=areas[t].end.y-areas[t].start.y;
                target.unit[t].c_x=areas[t].start.x+target.unit[t].w/2;
                target.unit[t].c_y=areas[t].start.y+target.unit[t].h/2;
            }
        }
    }
    //初始目标融合
    //给有相交的目标统一编号(1,2...)
    int ID_SUM = targets_mix(&target);
    //目标融合（相同编号的多目标合成一个目标）
    int count=0;
    for(int ID=1;ID<=ID_SUM;ID++)
    {
        int min_x=10000,min_y=10000,max_x=0,max_y=0;
        for(int t=0;t<target.num;t++)
        {
            if(ID==target.unit[t].GatherID)
            {
                //生成新目标
                int x=target.unit[t].c_x - target.unit[t].w/2;
                int y=target.unit[t].c_y - target.unit[t].h/2;
                if(min_x>x)
                    min_x=x;
                if(min_y>y)
                    min_y=y;
                x=target.unit[t].c_x + target.unit[t].w/2;
                y=target.unit[t].c_y + target.unit[t].h/2;
                if(max_x<x)
                    max_x=x;
                if(max_y<y)
                    max_y=y;

            }
        }
        G.targets[count].w=max_x-min_x;
        G.targets[count].h=max_y-min_y;
        G.targets[count].c_x=min_x+(max_x-min_x)/2;
        G.targets[count].c_y=min_y+(max_y-min_y)/2;
        count++;
        /*
        coord_t pix0,pix1;
        pix0.x=min_x*8;
        pix0.y=min_y*8;
        pix1.x=max_x*8;
        pix1.y=max_y*8;
        yuv_draw_rectangle(pix0,pix1,colorWhite,G.dis_img);
         */
    }
    G.target_num=count;
    free(block.ptr);
    //返回目标数量
    return count;
}
//------------------------------------------------------------------Line_Fit
double Sum_Average(int n, int *c)
{
    double z=0;
    for(int i=0;i<n;i++)
    {
        z += c[i];
    }
    z = z/n;
    return z;
}
double X_By_Y(int n, int *x,int *y)
{
    double z=0;
    for(int i=0;i<n;i++)
    {
        z += x[i] * y[i];
    }
    return z;
}
double Squre_sum(int n,int *c)
{
    double z=0;
    for(int i=0;i<n;i++)
    {
        z += c[i]*c[i];
    }
    return z;
}
void Line_Fit(int n)
{
    int X[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int Y[10] = {2, 3, 3, 4, 5, 6, 7, 8, 9, 6};
    double K=0;               //拟合直线的斜率
    double R=0;               //拟合直线的截距
    double x_sum_average=0;   //数组 X[N] 个元素求和 并求平均值
    double y_sum_average=0;   //数组 Y[N] 个元素求和 并求平均值
    double x_square_sum=0;    //数组 X[N] 个个元素的平均值
    double x_multiply_y=0;    //数组 X[N]和Y[N]对应元素的乘机

    x_sum_average= Sum_Average(n,X);
    y_sum_average= Sum_Average(n,Y);
    x_square_sum = Squre_sum(n,X);
    x_multiply_y = X_By_Y(n,X,Y);
    K = ( x_multiply_y - n * x_sum_average * y_sum_average)/( x_square_sum - n * x_sum_average*x_sum_average );
    R = y_sum_average - K * x_sum_average;

 //   printf("K = %f\n",K);
  //  printf("R = %f\n",R);
}
//计算是否在区域内----------------------------------------------------------------
//TopPoint:存放顶点坐标
//Len:顶点数
//v:输入点坐标（判断此点是否属于区域）
//int gen_polygon (coord_t *intersection, int num, coord_t *v)
int gen_polygon (polygon_t *polygon, coord_t *v)
{
    int i, j, k1,k2, c;
    extreme_t ext;
    coord_t w;
    int num=polygon->intersection_num;
    coord_t *intersection=polygon->intersection;

    if (num < 3)
        return 0;

    //找出顶点中max_x,max_y,min_x,min_y
    vertices_get_extent(intersection, num, &ext);
    //滤除不属于大区域内的点
    if (v->x < ext.min_x || v->x > ext.max_x || v->y < ext.min_y || v->y > ext.max_y)
        return 0;

    /* Set a horizontal beam l(*v, w) from v to the ultra right */
    w.x = ext.max_x + 1;
    w.y = v->y;
    c = 0;        /* Intersection points counter */
    for(i=0; i<num; i++)
    {
        j = (i+1) % num; //j=i+1

        if (intersection[i].y != intersection[j].y)
        {
            if (is_intersect(intersection+i, intersection+j, v, &w)==1) //判断是否有交点
                c++;
        }

        if(intersection[i].y == w.y) //判断顶点是否在线上
        {
            if (intersection[i].x >= v->x)
            {
                k1 =(num+i-1)%num;
                k2 =(num+i+1)%num;
                if (intersection[k1].y > w.y)
                {
                    if (intersection[k2].y < w.y)
                        c++;
                }

                if (intersection[k2].y > w.y)
                {
                    if (intersection[k1].y < w.y)
                        c++;
                }
            }
        }
    }
    return   c%2;
}

void vertices_get_extent (coord_t *intersection, int num, extreme_t *rc)
{
    short int i;
    i = intersection[0].x;
    if (num > 0)
    {
        rc->min_x = rc->max_x = intersection[0].x;
        rc->min_y = rc->max_y = intersection[0].y;
    }
    else
        rc->min_x = rc->min_y = rc->max_x = rc->max_y = 0; // =0 ? no vertices at all

    for(i=1; i<num; i++)//找出极值max,min
    {
        if(intersection[i].x < rc->min_x)
            rc->min_x = intersection[i].x;
        if(intersection[i].y < rc->min_y)
            rc->min_y = intersection[i].y;
        if(intersection[i].x > rc->max_x)
            rc->max_x = intersection[i].x;
        if(intersection[i].y > rc->max_y)
            rc->max_y = intersection[i].y;
    }
}
int is_same_l( coord_t *start,coord_t *end,coord_t *p,coord_t *q)
{

    double dx = end->x - start->x;
    double dy = end->y - start->y;

    double dx1= p->x - start->x;
    double dy1= p->y - start->y;

    double dx2= q->x - end->x;
    double dy2= q->y - end->y;

    if ((dx * dy1-dy * dx1) * (dx * dy2-dy * dx2) > 0)
        return 0;
    else
        return 1;

}
//相交检测
int is_intersect( coord_t *s1_start, coord_t *s1_end, coord_t *s2_start,coord_t *s2_end)
{
    int i,j;
    i = is_same_l(s1_start, s1_end, s2_start, s2_end);
    j = is_same_l(s2_start, s2_end, s1_start, s1_end);
    if ((i&&j) == 0)
        return 0;
    else
        return 1;

}
//-----------------------------------------------------------------------gen_rectangle
int gen_rectangle(rectangle_t *rect,coord_t *v)
{
    int max_x,min_x,max_y,min_y;

    if(rect->start.x<rect->end.x)
    {
        max_x=rect->end.x;
        min_x=rect->start.x;
    }
    else
    {
        min_x=rect->end.x;
        max_x=rect->start.x;
    }
    if(rect->start.y<rect->end.y)
    {
        max_y=rect->end.y;
        min_y=rect->start.y;
    }
    else
    {
        min_y=rect->end.y;
        max_y=rect->start.y;
    }

    if(v->x>min_x && v->x<max_x && v->y>min_y && v->y<max_y)
        return 1;
    else
        return 0;

}
int draw_rectangle(rectangle_t rect)
{
    yuv_draw_rectangle(rect.start,rect.end,colorRed,G.dis_img);
    return 0;
}
int draw_polygon(polygon_t polygon)
{
    coord_t start,end;
    for(int i=0;i<polygon.intersection_num-1;i++)
    {
        start=polygon.intersection[i];
        end=polygon.intersection[i+1];
        LineWithDDA(start,end,colorRed,G.dis_img);
    }
    //最后的点与开始点相连
    start=polygon.intersection[polygon.intersection_num-1];
    end=polygon.intersection[0];
    LineWithDDA(start,end,colorRed,G.dis_img);
    return 0;
}
//判断坐标是否在区域内
bool is_inArea(coord_t v, ROI_data_t ROI)
{
    bool flag=false;
    //形状区域
    switch (ROI.shape) {
        case RECT:
            if(gen_rectangle(&ROI.rect,&v)==1)
            {
                flag=true;
            }
            break;
        case POLYGON:
            if(gen_polygon(&ROI.polygon,&v)==1)
            {
                flag=true;
            }
            break;

        default:
            break;

    }
    return flag;
}
//匹配度计算
double gray_model_matching(gray_model_t *model,base_img_t *base_img,coord_t Offset, double MatchingTH)
{
    int size=model->size;
    gray_model_dat_t *model_dat=model->ptr;
    base_data_t *src_dat=base_img->ptr;
    int img_w = base_img->width;
    int img_h = base_img->height;
    //模板坐标
    coord_t modelPix;
    //图像坐标
    coord_t ImgPix;
    int v,model_acc=0,img_acc=0;
    int modelGray_m,imgGray_m;
    variance_t *modelVar = (variance_t*)malloc(sizeof(variance_t)*size);
    memset(modelVar, 0, sizeof(variance_t)*size);
    variance_t *imgVar = (variance_t*)malloc(sizeof(variance_t)*size);
    memset(imgVar, 0, sizeof(variance_t)*size);
    //求平均值
    for (int i=0;i<size;i++)
    {
        //-----------------------------模板
        //模板的相对坐标
        modelPix=model_dat[i].relPix;
        modelVar[i].v=model_dat[i].gray;
        //模板灰度累加
        model_acc += modelVar[i].v;

        //-----------------------------图像
        ImgPix.x=modelPix.x+Offset.x;
        ImgPix.y=modelPix.y+Offset.y;
        if(ImgPix.x<0 || ImgPix.x>=img_w || ImgPix.y<0 || ImgPix.y>=img_h)
        {
            v=0;
        }
        else
        {
            int k=img_w * ImgPix.y + ImgPix.x;
            //图像灰度累加
            imgVar[i].v=src_dat[k].gray;
            img_acc += imgVar[i].v;
        }
    }
    //模板灰度平均值
    modelGray_m = model_acc/size;
    //图像灰度平均值
    imgGray_m = img_acc/size;
    //-----------------------------求差和方差
    for (int i=0;i<size;i++)
    {
        modelVar[i].diff = modelVar[i].v-modelGray_m;
        modelVar[i].ss   = modelVar[i].diff*modelVar[i].diff;

        imgVar[i].diff   = imgVar[i].v-imgGray_m;
        imgVar[i].ss     = imgVar[i].diff*imgVar[i].diff;
    }


    double matching=0;
    double Acc=0;
    //模板运算
    for (int i=0;i<size;i++)
    {
        double sst=sqrt(double(modelVar[i].ss));
        double ssu=sqrt(double(imgVar[i].ss));
        double temp=sst*ssu;
        if(temp==0)
            Acc +=1;
        //求相关关系
        else
            Acc += (modelVar[i].diff*imgVar[i].diff)/(sst*ssu);
    }

    matching=Acc/double(size);
    free(modelVar);
    free(imgVar);
    return matching;
}
//匹配度计算
double edge_model_matching(edge_model_t *model,base_img_t *base_img,coord_t Offset, double MatchingTH)
{
    int size=model->size;
    edge_model_dat_t *model_dat=model->ptr;
    base_data_t *base_dat=base_img->ptr;
    int img_w = base_img->width;
    int img_h = base_img->height;

    //模板坐标
    coord_t modelPix;
    //图像坐标
    coord_t ImgPix;

    double matching=0;
    double Acc=0;
    int t,u,v,w,p,q,o=0;

    //模板运算
    for (int i=0;i<size;i++)
    {
        //模板的相对坐标
        modelPix=model_dat[i].relPix;

        //模板向量和梯度
        t=model_dat[i].Vt[0];
        u=model_dat[i].Vt[1];
        p=model_dat[i].gradients;

        //以模板中心为原点的正常坐标（区别图像坐标）
        //对应的图像坐标
        ImgPix.x=modelPix.x+Offset.x;
        ImgPix.y=modelPix.y+Offset.y;

        if(ImgPix.x<0 || ImgPix.x>=img_w || ImgPix.y<0 || ImgPix.y>=img_h)
        {
            v=0;
            w=0;
        }
        else
        {
            int k=img_w * ImgPix.y + ImgPix.x;
            //图像向量和梯度
            v=base_dat[k].Vt[0];
            w=base_dat[k].Vt[1];
            q=base_dat[k].gradients;
            //滤波参数
            o+=q;
        }

        //求相关关系
        int e = t*v + u*w;
        int f = p*q;
        if(f!=0)
        {
            double temp = double(e)/f;
            Acc += temp;
        }

    }
    //滤波
    if(o<180)
       return 1;
    matching=Acc/double(size);
    return matching;
}


//生成模型边缘数据且串行化
void creat_edge_model_data(int layer,base_img_t *base_img)
{
    G.edge_matching_dat_num=0;
    coord_t start,end,center;
    start = G.border[layer].area.start;
    end   = G.border[layer].area.end;
    center= G.border[layer].area.center;
    if(center.x==0 && center.y==0)
        return;
    int img_w=base_img->width;
    base_data_t *base_dat = base_img->ptr;
    int num=0;
    int count=0;
    int size=9;
    int step=SEARCHSTEP;
    edge_model_dat_t dat[9];
    for(int i=start.y+step;i<end.y-step;i+=step)
    {
        for(int j=start.x+step;j<end.x-step;j+=step)
        {
            int k=i*img_w+j;

            dat[0].relPix = base_dat[k-img_w-1].pix;         dat[1].relPix = base_dat[k-img_w].pix;            dat[2].relPix = base_dat[k-img_w+1].pix;
            dat[0].Vt[0]  = base_dat[k-img_w-1].Vt[0];       dat[1].Vt[0]  = base_dat[k-img_w].Vt[0];         dat[2].Vt[0] = base_dat[k-img_w+1].Vt[0];
            dat[0].Vt[1]  = base_dat[k-img_w-1].Vt[1];       dat[1].Vt[1]  = base_dat[k-img_w].Vt[1];         dat[2].Vt[1] = base_dat[k-img_w+1].Vt[1];
            dat[0].gradients = base_dat[k-img_w-1].gradients; dat[1].gradients  = base_dat[k-img_w].gradients;  dat[2].gradients = base_dat[k-img_w+1].gradients;

            dat[3].relPix = base_dat[k-1].pix;               dat[4].relPix = base_dat[k].pix;                 dat[5].relPix = base_dat[k+1].pix;
            dat[3].Vt[0]  = base_dat[k-1].Vt[0];             dat[4].Vt[0]  = base_dat[k].Vt[0];               dat[5].Vt[0] = base_dat[k+1].Vt[0];
            dat[3].Vt[1]  = base_dat[k-1].Vt[1];             dat[4].Vt[1]  = base_dat[k].Vt[1];               dat[5].Vt[1] = base_dat[k+1].Vt[1];
            dat[3].gradients  = base_dat[k-1].gradients;      dat[4].gradients  = base_dat[k].gradients;        dat[5].gradients = base_dat[k+1].gradients;

            dat[6].relPix = base_dat[k+img_w-1].pix;          dat[7].relPix = base_dat[k+img_w].pix;           dat[8].relPix = base_dat[k+img_w+1].pix;
            dat[6].Vt[0]  = base_dat[k+img_w-1].Vt[0];        dat[7].Vt[0]  = base_dat[k+img_w].Vt[0];         dat[8].Vt[0] = base_dat[k+img_w+1].Vt[0];
            dat[6].Vt[1]  = base_dat[k+img_w-1].Vt[1];        dat[7].Vt[1]  = base_dat[k+img_w].Vt[1];         dat[8].Vt[1] = base_dat[k+img_w+1].Vt[1];
            dat[6].gradients = base_dat[k+img_w-1].gradients;  dat[7].gradients  = base_dat[k+img_w].gradients;  dat[8].gradients = base_dat[k+img_w+1].gradients;

            memcpy(&G.edge_models[layer][count],&dat[0],sizeof(edge_model_dat_t)*size);
            count+=9;
            num++;

        }
    }
    G.edge_matching_dat_num=num;
}

double edge_model_search(int layer, double MatchingTH)
{
    coord_t start,end,cen;
    int img_w=G.base_img[layer]->width;
    edge_model_t edge_model;
    edge_model.ptr=(edge_model_dat_t *)malloc(sizeof(edge_model_dat_t)*9);
    int size=G.edge_matching_dat_num;
    edge_model.size=9;
    for(int i=0;i<size;i++)
    {
        memcpy(edge_model.ptr,&G.edge_models[layer][i*9],sizeof(edge_model_dat_t)*9);
        coord_t Offset;
        Offset.x=0;
        Offset.y=0;
        double v = edge_model_matching(&edge_model,G.base_img[layer],Offset, MatchingTH);
        if(v<0.2)
        {
            start.x=edge_model.ptr[0].relPix.x * 8+2 ;
            start.y=edge_model.ptr[0].relPix.y * 8+2 ;
            end.x=edge_model.ptr[8].relPix.x * 8-2 ;
            end.y=edge_model.ptr[8].relPix.y * 8-2 ;

            //保存匹配的点
            cen.x=edge_model.ptr[4].relPix.x;
            cen.y=edge_model.ptr[4].relPix.y;
            int k=cen.y * img_w + cen.x;
            G.base_img[layer]->ptr[k].MatchingDot=true;

            yuv_draw_rectangle(start,end,colorRed,G.dis_img);
        }
    }
    free(edge_model.ptr);
    return 0;
}

void display_ROI(int layer)
{
    coord_double_t p[MAX_POLYGON_I_NUM];
    coord_t start,end,center;
    rectangle_t rect;
    center=G.border[layer].area.center;
    int shape=G.border[layer].shape;
    switch (shape) {
        case RECT:
            rect=G.border[layer].rect;
            //生成交点
            p[0].x=rect.start.x-center.x;
            p[0].y=center.y-rect.start.y;

            p[1].x=rect.start.x-center.x;
            p[1].y=center.y-rect.end.y;

            p[2].x=rect.end.x-center.x;
            p[2].y=center.y-rect.end.y;

            p[3].x=rect.end.x-center.x;
            p[3].y=center.y-rect.start.y;

            for(int i=0;i<4;i++)
            {
                start.x=p[i].x;
                start.y=p[i].y;
                if(i==3)
                {
                    end.x  =p[0].x;
                    end.y  =p[0].y;
                }
                else
                {
                    end.x  =p[i+1].x;
                    end.y  =p[i+1].y;
                }
                LineWithDDA(start, end, colorRed, G.dis_img);
            }
            break;


        case POLYGON:
            polygon_t polygon= G.border[layer].polygon;
            int intersection_num=polygon.intersection_num;
            for(int i=0;i<intersection_num;i++)
            {
                p[i].x = polygon.intersection[i].x;
                p[i].y = polygon.intersection[i].y;
            }
            for(int i=0;i<intersection_num;i++)
            {
                start.x=p[i].x;
                start.y=p[i].y;
                if(i==intersection_num-1)
                {
                    end.x  =p[0].x;
                    end.y  =p[0].y;
                }
                else
                {
                    end.x  =p[i+1].x;
                    end.y  =p[i+1].y;
                }
                LineWithDDA(start, end, colorRed, G.dis_img);
            }
            break;
    }
}
//创建ROI和模板
void creat_ROI(int layer, inROI_t in)
{
    //创建模板内存
    coord_t v;
    int num,k,datSize,max_x,min_x,max_y,min_y;

    ROI_data_t ROI={0};
    ROI.shape=in.shape;
    ROI.ID=in.ID;
    ROI.type=in.type;
    //转换为对应层的数据
    int zoom=get_zoom(layer);
    //从外部传递过来的数据中第一个为后面数据的数量
    datSize=in.dat[0];// in[0];
    //转换为相应层的数据
    switch (ROI.shape) {
        case RECT:
            ROI.rect.start.x = in.dat[1]/zoom;
            ROI.rect.start.y = in.dat[2]/zoom;
            ROI.rect.end.x   = in.dat[3]/zoom;
            ROI.rect.end.y   = in.dat[4]/zoom;
            break;
        case POLYGON:
            k=0;
            for(int i=1;i<datSize;i+=2)
            {
                ROI.polygon.intersection[k].x = in.dat[i]/zoom;
                ROI.polygon.intersection[k].y = in.dat[i+1]/zoom;
                k++;
            }
            ROI.polygon.intersection_num=k;
            break;
        default:
            break;
    }

    //计算检测区域中心和外框
    switch (ROI.shape) {

        case RECT:
            if(ROI.rect.start.x<ROI.rect.end.x)
            {
                max_x=ROI.rect.end.x;
                min_x=ROI.rect.start.x;
            }
            else
            {
                min_x=ROI.rect.end.x;
                max_x=ROI.rect.start.x;
            }
            if(ROI.rect.start.y<ROI.rect.end.y)
            {
                max_y=ROI.rect.end.y;
                min_y=ROI.rect.start.y;
            }
            else
            {
                min_y=ROI.rect.end.y;
                max_y=ROI.rect.start.y;
            }
            ROI.area.start.x=min_x;
            ROI.area.start.y=min_y;
            ROI.area.end.x  =max_x;
            ROI.area.end.y  =max_y;
            ROI.area.center.x=min_x + (max_x - min_x)/2;
            ROI.area.center.y=min_y + (max_y - min_y)/2;

            if(ROI.type==BORDER)
            {
                G.border[layer]=ROI;
            }
            else
            {
                //保存ROI
                G.ROIs[layer]->ptr[ROI.ID]=ROI;
                G.ROIs[layer]->num++;


            }
            break;

        case POLYGON:
            max_x=0;min_x=100000;max_y=0;min_y=100000;
            num=ROI.polygon.intersection_num;
            for(int i=0;i<num;i++)
            {

                v.x = ROI.polygon.intersection[i].x;
                v.y = ROI.polygon.intersection[i].y;
                if(v.x<min_x)
                {
                    min_x=v.x;
                }
                if(v.y<min_y)
                {
                    min_y=v.y;
                }
                if(v.x>max_x)
                {
                    max_x=v.x;
                }
                if(v.y>max_y)
                {
                    max_y=v.y;
                }
            }
            ROI.area.start.x=min_x;
            ROI.area.start.y=min_y;
            ROI.area.end.x  =max_x;
            ROI.area.end.y  =max_y;
            ROI.area.center.x=min_x + (max_x - min_x)/2;
            ROI.area.center.y=min_y + (max_y - min_y)/2;
            if(ROI.type==BORDER)
            {
                G.border[layer]=ROI;
            }
            else
            {
                //保存ROI
                G.ROIs[layer]->ptr[ROI.ID]=ROI;
                G.ROIs[layer]->num++;
                if(num>MAX_POLYGON_I_NUM)
                {
                    G.ROIs[layer]->ptr[ROI.ID].polygon.intersection_num = MAX_POLYGON_I_NUM;
                }

            }
            break;

        default:
            break;
    }

}
int get_layer_num(img_Buffer_t *base_img,int width_TH)
{
    int width =base_img->width;
    for(int layer=0;layer<LAYER_SUM;layer++)
    {
         width = width/2;
         if(width<width_TH)
             return layer+1;
    }
    return 0;
}
void display_position_data(position_t position)
{
    coord_t display;
    //显示定位坐标、匹配度---------------------------------------
    if(position.matching >1)
        position.matching =1;
    position.matching=position.matching*100;

    display.x=10;
    display.y=30;
    char Similarity[]={"Similarity: "};
    disp_english_string(display,Similarity,colorWhite,G.dis_img);
    display.x=190;
    disp_variable_f(display,position.matching,colorWhite,G.dis_img);

    display.x=10;
    display.y=50;
    char char_x[]={"X: "};
    disp_english_string(display,char_x,colorWhite,G.dis_img);
    display.x=50;
    disp_variable_f(display,position.focus.x,colorWhite,G.dis_img);

    display.x=10;
    display.y=70;
    char char_y[]={"Y: "};
    disp_english_string(display,char_y,colorWhite,G.dis_img);
    display.x=50;
    disp_variable_f(display,position.focus.y,colorWhite,G.dis_img);

}
//------------------------------------------------------------------------------------主程序入口
int mix_frame_pro(input_param_t input,img_Buffer_t *base_img,output_t *output)
{
    coord_t display;
    int width,height;
    width =base_img->width;
    height=base_img->height;
    //-----------------------测试时间
    struct timeval tsBegin,tsEnd;
    gettimeofday(&tsBegin, NULL);
    //第一帧初始化
    init_param(input,base_img);
    //计数
    run_times++;
    G.imageFormat=input.imageFormat;

    //生成顶层金字塔层数编号
    //金字塔顶层图像宽度小于width_TH
    int width_TH=200;
    G.TOP_LAYER = get_layer_num(base_img,width_TH);
    //生成金字塔图像（顶层编号加1为生成的金字塔图像总层数）
    creat_pyramid_img(G.TOP_LAYER,base_img);
    //显示金字塔图像
    dis_pyramid_img(G.TOP_LAYER);

    if(input.inROI.op==OP_CREAT)
    {
        creat_ROI(G.TOP_LAYER,input.inROI);
        creat_ROI(0,input.inROI);

    }
    int layer=G.TOP_LAYER;
    //边缘检测--------------------------------edge_detection
    int Threshold=50;
    edge_detection(0,Threshold);
    edge_detection(3,Threshold);

    if(key_v=='p')
    {
        creat_edge_model_data(layer,G.base_img[layer]);
        key_v='0';
    }
    //匹配阀值
    double MatchingTH=0.8;
    double start_angel=-180;
    double end_angel=180;
    double scale=1;
    //  output->sum = position_pro(MatchingTH,start_angel,end_angel,scale,output->ptr);
    edge_model_search(layer, MatchingTH);

    int dotSize_TH=2;
    creat_target(G.base_img[layer],G.border[layer].area,dotSize_TH);

    if(G.target_num >100)
        G.target_num=100;

    int zoom=get_zoom(3);
    output->sum=G.target_num;
    for(int i=0;i<G.target_num;i++)
    {
        output->x[i]=G.targets[i].c_x*zoom;
        output->y[i]=G.targets[i].c_y*zoom;
        output->w[i]=G.targets[i].w*zoom;
        output->h[i]=G.targets[i].h*zoom;
        output->v[i]=G.targets[i].v;
        output->d[i]=G.targets[i].d;
        display.x=output->x[i];
        display.y=output->y[i];
        yuv_draw_orthogon(display, output->w[i], output->h[i],colorWhite,G.dis_img);
       // yuv_draw_aiming(display , output->w[i]*3/2, output->h[i]*3/2, colorWhite, G.dis_img);
    }

    display_ROI(3);
    display_ROI(0);

/*
    //生成积分图----------------------------------------------------------------------------
    switch (input.imageFormat){
            case 0 :yuv422_uyvy_integral(base_img,G.integral_image);break;
           // case 0 :yuv422_uyvy_integralSS(yuv,G.integral_image,G.SSintegral_image);break;
            case 1 :yuv422_yuyv_integral(base_img,G.integral_image);break;
            case 2 :yuv420p_integral(base_img,G.integral_image)    ;break;
            case 3 :yuv420sp_integral(base_img,G.integral_image)   ;break;
            default:printf("error\n");
    }

    integral_v_t pix;
    SSintegral_v_t SSpix;
    yuv_integral_value(start,end,&pix,G.integral_image);
    yuv_SSintegral_value(start,end,&SSpix,G.SSintegral_image);

*/
    //--------------------------------------------------------------------------------------
    // 测试图像旋转
    //后向变换
    // IMG_WarpTrans(G.ROIs[0]->ptr[0],*base_img,G.dis_img);
    //前向变换
    // IMG_AffineTrans(G.ROIs[0]->ptr[0],*base_img,G.dis_img);

    //定位-------------------------------------------------

/*
    //显示检测区域
    yuv_draw_rectangle(G.border[0].area.start,G.border[0].area.end,colorOrange,G.dis_img);
    //显示中心十字线
    start.x=0;
    start.y=height/2;
    end.x=width;
    end.y=height/2;
    LineWithDDA(start,end,colorRed,G.dis_img);
    start.x=width/2;
    start.y=0;
    end.x=width/2;;
    end.y=height;
    LineWithDDA(start,end,colorRed,G.dis_img);

*/
    gettimeofday(&tsEnd, NULL);//-----------------------测试时间
    long runtimes;
    runtimes=1000000L*(tsEnd.tv_sec-tsBegin.tv_sec)+tsEnd.tv_usec-tsBegin.tv_usec;
    display.x=10;
    display.y=10;
    char times[]={"Times: "};
    disp_english_string(display,times,colorWhite,G.dis_img);

    display.x=120;
    runtimes=runtimes/1000;
    disp_variable(display,runtimes,colorWhite,G.dis_img);

    //----------------------------
    return 0;
}

void MainWindow::on_gywtrack_button_clicked()
{
    coord_t center;
    //IplImage* frame;
    Mat frame;

    if (filename.isEmpty()) {
        ui->warning_lineEdit->setText("Filename is empty,please open video file first");
        return;
       }

    VideoCapture capture(filename.toStdString());
    if(!capture.isOpened()){
        ui->warning_lineEdit->setText("File open error,please open video file first");
        return ;
    }

    namedWindow("Camera");

    //===这里输入参数====================================================================

    input_param_t input;
    input.imageFormat=0;


    //==============================================================================
    srand(unsigned(time(NULL)));
    random_num=rand()%10000;
    //==============================================================================

    //循环捕捉,知道用户跳出循环
    while(1)
    {

        if(!capture.read(frame)){
            cerr << "Unable to read next frame." << endl;
            cerr << "Exiting..." << endl;
            break;
        }
        int size =frame.cols * frame.rows*3;

        img_Buffer_t *rgb_buffer;
        rgb_buffer= (img_Buffer_t*)malloc(sizeof(img_Buffer_t));
        rgb_buffer->ptr=(unsigned char *)malloc(sizeof(unsigned char)*size);
        rgb_buffer->width=frame.cols;
        rgb_buffer->height=frame.rows;

        memcpy(rgb_buffer->ptr, frame.data, size);

        img_Buffer_t *yuv;
        yuv= (img_Buffer_t*)malloc(sizeof(img_Buffer_t));
        yuv->ptr=(unsigned char *)malloc(sizeof(unsigned char)*size);
        yuv->width=frame.cols;
        yuv->height=frame.rows;



        //========================================================================
        //to do
        switch (input.imageFormat){
            case 0 :rgb24_to_yuv422_packed_uyvy(rgb_buffer,yuv);break;
            case 1 :rgb32_to_yuv422_packed_yuyv(rgb_buffer,yuv);break;
            case 2 :rgb32_to_yuv420p(rgb_buffer,yuv) ;break;
            case 3 :rgb32_to_yuv420sp(rgb_buffer,yuv);break;
            default:printf("error\n");
        }

        if(run_times==0)
        {
            //释放以前内存
       //     free_memory(yuv);
            //重新申请内存
            malloc_memory(yuv);
        }
        //显示内存
        size =yuv->width * frame.rows * 2;
        memcpy(G.dis_img->ptr, yuv->ptr,size);

        //----------------------------------------------
        output_t output;
        if(key_v=='k')
        {
            key_v='0';
            input.inROI.op=OP_CLEAR_ALLID;
            mix_frame_pro(input,yuv,&output);
            input.inROI.op=OP_NULL;

        }

        //设置检测区域
        center.x=yuv->width/2;
        center.y=yuv->height/2;
        if(key_v=='m')
        {
            key_v='0';
            input.inROI.dat=(int *)malloc(sizeof(int)*100);

            input.inROI.ID=0;
            input.inROI.shape=POLYGON;
            input.inROI.type =BORDER;
            input.inROI.dat[0]=8;

            center.y=center.y+100;
            input.inROI.dat[1]=10;
            input.inROI.dat[2]=10;

            input.inROI.dat[3]=10;
            input.inROI.dat[4]=500;

            input.inROI.dat[5]=1270;
            input.inROI.dat[6]=500;

            input.inROI.dat[7]=1270;
            input.inROI.dat[8]=10;


            input.inROI.op = OP_CREAT;
            mix_frame_pro(input,yuv,&output);
            input.inROI.op = OP_NULL;

            free(input.inROI.dat);
        }

      //  mix_frame_pro(input,yuv,&output);


        switch (input.imageFormat){
            case 0 :yuv422_packed_uyvy_to_rgb24(G.dis_img,rgb_buffer);break;
            case 1 :yuv422_packed_yuyv_to_rgb32(G.dis_img,rgb_buffer);break;
            case 2 :yuv420p_to_rgb32(G.dis_img,rgb_buffer)           ;break;
            case 3 :yuv420sp_to_rgb32(G.dis_img,rgb_buffer)          ;break;
            default:printf("error\n");
        }


        //========================================================================
        size =frame.cols * frame.rows*3;
        memcpy(frame.data, rgb_buffer->ptr, size);

        imshow("Camera",frame);
      //  cvMoveWindow("Camera", 0, 0);
        char key = (char)waitKey(100);
        if(key==27)  //27==ESC
            break;

        switch(key)
        {
            case '0': key_v='0';
                break;

            case '1': key_v='1';
                break;

            case '2': key_v='2';
                break;

            case '3': key_v='3';
                break;

            case '4': key_v='4';
                break;

            case '5': key_v='5';
                break;

            case '6': key_v='6';
                break;

            case '7': key_v='7';
                break;

            case '8': key_v='8';
                break;

            case '9': key_v='9';
                break;


            case 'm': key_v='m';
                break;

            case 'k': key_v='k';
                break;

            case 'p': key_v='p';
                break;

            default:
                break;

        }



 //       free(rgb_buffer->ptr);
 //       free(rgb_buffer);
 //       free(yuv->ptr);
 //       free(yuv);
        //     free(output);


    }

    capture.release();
    destroyAllWindows();

    return ;
}
