#pragma once

#include <QMainWindow>
#include "ui_qtGuiCV.h"
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/imgproc/types_c.h>//������ɫ�ռ�ת������
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
	//�˵�����
	void newDocument();
	void openFile();
	void maskOP();
	void maskOPWithKernel();
	void reopenInDealedZone();
	void toGray();  //�˵����Ҷȱ任


private:
	//void openImgFile();
	QImage cvImg2Qimg(cv::Mat img);   //��cv Mat תΪQimage����
	void logToTextEdit(logLevel logLevel, QString log_string);   //��־�������־��
	//QImage qImgScaled(QImage img, QLabel lbl);
	cv::Size qtGuiCV::qImgScaled(int img_rows, int img_cols, int lbl_height, int lbl_width); //�ֶ�ʵ������
	void qtGuiCV::showToQLabelFromCVMat(cv::Mat img, QLabel *label_ptr);  //��Matֱ����ʾ��label��
	cv::Mat maskOperation(cv::Mat src);  // ��Ĥ����
	//cv::Mat openImageFromCurrentFile(cv::String filename);
	cv::String getCvFileNameFromCurrentFile();  //��currentFile��ȡ��cv��string

};
