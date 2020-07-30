#include "qtGuiCV.h"
#include <ui_qtGuiCV.h>

qtGuiCV::qtGuiCV(QWidget *parent)
	: QMainWindow(parent)
	//ui(new Ui::qtGuiCV)
{
	ui.setupUi(this);
	
	connect(ui.action_New, &QAction::triggered, this, &qtGuiCV::newDocument);
	connect(ui.action_Open, &QAction::triggered, this, &qtGuiCV::openFile);
	connect(ui.action_toGray, &QAction::triggered, this, &qtGuiCV::toGray);
	connect(ui.actionMask_OP, &QAction::triggered, this, &qtGuiCV::maskOP);
	connect(ui.actionMask_with_kernel, &QAction::triggered, this, &qtGuiCV::maskOPWithKernel);
	connect(ui.actionreopen_in_B_zone, &QAction::triggered, this, &qtGuiCV::reopenInDealedZone);
	
	//connect(ui.action_OpenImg, &QAction::triggered, this, &qtGuiCV::openImgFile);
	

}
void qtGuiCV::newDocument()
{
	currentFile.clear();
	ui.o_label->clear();
	
}


void qtGuiCV::openFile()
{	
	currentFile = QFileDialog::getOpenFileName(this, "open the file");
	image = cv::imread(getCvFileNameFromCurrentFile(), cv::IMREAD_COLOR);
	if (!image.empty())
	{
		ui.o_label->clear();
		ui.a_label->clear();
		showToQLabelFromCVMat(image, ui.o_label);
		logToTextEdit(OKLOG, "OpenCV open file done!");
	}
	else
	{
		logToTextEdit(ERROR, "OpenCV open file error!");
	}
}

void qtGuiCV::maskOP()
{
	cv::Mat src = cv::imread(getCvFileNameFromCurrentFile(), cv::IMREAD_COLOR);
	if (!src.empty())
	{
		showToQLabelFromCVMat(maskOperation(src), ui.a_label);
		logToTextEdit(OKLOG, "Mask operation is done !");
	}
	else
	{
		logToTextEdit(ERROR, "The image is empty, please reopen it");
	}
}

//ʹ��filter2D������Ĥ����
void qtGuiCV::maskOPWithKernel()
{
	cv::Mat src = cv::imread(getCvFileNameFromCurrentFile(), cv::IMREAD_COLOR);
	if (!(src.empty()))
	{
		cv::Mat dst;
		cv::Mat k = (cv::Mat_<char>(3, 3) << 0, -1, 0, -1, 6, -1, 0, -1, 0);
		cv::filter2D(src, dst, -1, k);
		showToQLabelFromCVMat(dst, ui.a_label);
		logToTextEdit(OKLOG, "Mask operation with kernel (0, -1, 0, -1, 5, -1, 0, -1, 0) is done !");

		//test code
		if ( 0 )
		{
			cv::namedWindow("sharped kernel", cv::WINDOW_FULLSCREEN);
			cv::imshow("sharped kernel", dst);
			cv::waitKey(0);
		}
		
	}
	else
	{
		logToTextEdit(ERROR, "The image is empty, please reopen it");
	}
	

}


void qtGuiCV::reopenInDealedZone()
{
	cv::Mat src = cv::imread(getCvFileNameFromCurrentFile(), cv::IMREAD_COLOR);
	if (!(src.empty()))
	{
		showToQLabelFromCVMat(src, ui.a_label);
		logToTextEdit(OKLOG, "reopen  is done !");
	}
	else
	{
		logToTextEdit(ERROR, "The image is empty, please reopen it");
	}

}

//cv Mat ת��Ϊ QIamge����
QImage qtGuiCV::cvImg2Qimg(cv::Mat img)
{
	//cv::Mat simg = img.clone();  //���ɴ������image�ĵ�ַ��ÿ�α任����image�ϱ任��Ӧ�ñ���imageһֱ����
	cv::Mat simg = img;
	//img.copyTo(simg);
	QImage dimg;
	if (simg.channels() == 3)    // RGB image
	{	
		bool flag = true; //debug flag
		if (flag)
		{
			cv::cvtColor(simg, simg, CV_BGR2RGB);
			dimg = QImage((const uchar*)(simg.data),
				simg.cols, simg.rows,
				simg.cols * simg.channels(),
				QImage::Format_RGB888);
		}
		else
		{
			cv::Mat dst(simg.size(), simg.type());
			cv::cvtColor(simg, dst, CV_BGR2RGB);
			dimg = QImage((const uchar*)(dst.data),
				dst.cols, dst.rows,
				dst.cols * dst.channels(),
				QImage::Format_RGB888);
		}
		logToTextEdit(DEBUG, "3 channels  image is converted");
	}
	else                     // gray image
	{
		logToTextEdit(DEBUG, "START TURN A GRAY IMAGE TO QImage");
		dimg = QImage((const uchar*)(simg.data),
			simg.cols, simg.rows,
			simg.cols * simg.channels(),
			QImage::Format_Indexed8);
		logToTextEdit(DEBUG, "A gray  image is converted");
	}
	return dimg;
}

//�Ҷȱ任
void qtGuiCV::toGray() 
{
	if (!(this->image.empty()))
	{
		cv::Mat gray_imag;
		cvtColor(image, gray_imag, CV_BGR2GRAY);//�Ҷ�ת��
		showToQLabelFromCVMat(gray_imag, ui.a_label);
		logToTextEdit(OKLOG, "to gray action is done !");
	}
	else
	{
		logToTextEdit(ERROR, "The image is empty, please reopen it");
	}

}

//��־��¼
void qtGuiCV::logToTextEdit(logLevel logLevel, QString log_string)
{	
	QColor color;
	//case INFO, WARNING, ERROR, OKLOG, DEBUG
	switch (logLevel) {
	case INFO:
		color = Qt::gray;
		log_string = "INFO:" + log_string;
		break;
	case WARNING:
		color = Qt::yellow;
		log_string = "INFO:" + log_string;
		break;
	case ERROR:
		color = Qt::red;
		log_string = "ERROR:" + log_string;
		break;
	case OKLOG:
		color = Qt::green;
		log_string = "INFO:" + log_string;
		break;
	case DEBUG:
		color = Qt::gray;
		log_string = "DEBUG:" + log_string;
		break;
	default:
		color = Qt::gray;
		log_string = "WRONG_TYPE:" + log_string;
	}
	ui.logTextEdit->setTextColor(color);
	ui.logTextEdit->append(log_string);

}

//�Զ������ţ�QT�Ѿ�ʵ�֣�����Ҫ
cv::Size qtGuiCV::qImgScaled(int img_rows, int img_cols, int lbl_height, int lbl_width)
{
	//ͼƬ����
	//1.ͼƬ�ȿ�ߣ�ͼƬ���ڿ�ߣ���������С
	//2.��������С��ͼƬ��С�ڿ������ͼƬ��
	float x_img, x_label;
	int d_height, d_width; //ͼƬ���ճߴ�
	int i_height = img_rows;
	int i_width = img_cols;
	int l_height = lbl_height;
	int l_width = lbl_width;
	x_img = i_height * 1.0 / i_width;  //int / int ��׼ȷ
	x_label = l_height * 1.0 / l_width;
	if (x_img > x_label)
	{
		d_height = l_height;
		d_width = d_height * 1.0 / i_height * i_width;
	}
	else
	{
		d_width = l_width;
		d_height = d_width * 1.0 / i_width * i_height;
	}
	return cv::Size(d_width, d_height);
}
//��MatͼƬ��ʾ��ָ����Label��
//��Ҫ��Ӧ����;�����ʾ
void qtGuiCV::showToQLabelFromCVMat(cv::Mat simg, QLabel* label_ptr)
{
	QImage img = cvImg2Qimg(simg);// openvc Mat ת QImage
	img = img.scaled(label_ptr->width(), label_ptr->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation); //���������� smoot����ͼƬ����
	label_ptr->setAlignment(Qt::AlignCenter); // label���ݾ���
	label_ptr->setPixmap(QPixmap::fromImage(img)); // ����ͼƬ��label��
}


//�ֶ���Ĥ����
cv::Mat qtGuiCV::maskOperation(cv::Mat src)
{
	/*kernel
	A=  0 -1   0
	   -1  5  -1
	    0 -1   0
	*/
	//int rows = (src.rows - 1) * src.channels(); //��Ĥ�������ӵڶ������ؿ�ʼ��ÿ������*channels���ֽ�**����
	int rows = src.rows - 1;

	int cols = src.cols * src.channels();
	int x = src.channels();  //������һ�����أ���Ҫ��ƫ����

	cv::Mat dst = cv::Mat::zeros(src.size(), src.type());  //Ӧ�ó�ʼ��Ϊһ��ȫ0 Mat,��ԭͼһ��

	for (int row = x; row < rows; row++)
	{
		const uchar* current = src.ptr<uchar>(row);
		const uchar* previous = src.ptr<uchar>(row - 1); //��ôȡ��һ�����ص㣿 src.ptr<uchar>(row - offset)
		const uchar* next = src.ptr<uchar>(row + 1);

		uchar* outputImageptr = dst.ptr<uchar>(row);

		for (int col = 1; col < cols; col++)
		{
			//��ô��ͼ��ĵ㸳ֵ��
			outputImageptr[col] = cv::saturate_cast<uchar>( 5 * current[col] - (current[col - x] + current[col + x] + previous[col] + next[col]));

		}
	}

	return dst;
}


//���ļ����ļ����ַ�����Qstring ���ͣ��˺�����currentFile���QStringתΪcv::string.
cv::String qtGuiCV::getCvFileNameFromCurrentFile()
{
	return this->currentFile.toLocal8Bit().toStdString();
}
