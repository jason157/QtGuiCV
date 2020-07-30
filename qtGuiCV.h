#pragma once

#include <QMainWindow>
#include "ui_qtGuiCV.h"
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/imgproc/types_c.h>//定义颜色空间转换变量
#include<QString>
#include<QLabel>
#include<QPixmap>
#include<QFileDialog>
#include<QMessageBox>
#include<QLayout>

//using  namespace cv;

namespace Ui {
	class qtGuiCV;

}

class qtGuiCV : public QMainWindow
{
	Q_OBJECT

public:
	qtGuiCV(QWidget* parent = Q_NULLPTR);
	enum logLevel
	{
		INFO, WARNING, ERROR, OKLOG, DEBUG
	};
private:
	Ui::qtGuiCVClass ui;
	cv::Mat image;
	//QLabel label;
	QString currentFile;
	cv::String fileName;

private slots:
	//菜单动作
	void newDocument();
	void openFile();
	void maskOP();
	void maskOPWithKernel();
	void reopenInDealedZone();
	void toGray();  //菜单：灰度变换


private:
	//void openImgFile();
	QImage cvImg2Qimg(cv::Mat img);   //把cv Mat 转为Qimage对象
	void logToTextEdit(logLevel logLevel, QString log_string);   //日志输出在日志区
	//QImage qImgScaled(QImage img, QLabel lbl);
	cv::Size qtGuiCV::qImgScaled(int img_rows, int img_cols, int lbl_height, int lbl_width); //手动实现缩放
	void qtGuiCV::showToQLabelFromCVMat(cv::Mat img, QLabel *label_ptr);  //把Mat直接显示在label上
	cv::Mat maskOperation(cv::Mat src);  // 掩膜操作
	//cv::Mat openImageFromCurrentFile(cv::String filename);
	cv::String getCvFileNameFromCurrentFile();  //从currentFile获取到cv的string

};
