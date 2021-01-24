//---------------------------------��ͷ�ļ��ռ��������֡�---------------------------------------
#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/features2d/features2d.hpp>
#include <iostream>
#include "opencv2/imgproc/imgproc_c.h"
//#include "omp.h"
//-----------------------------------�������ռ��������֡�---------------------------------------
using namespace cv;
using namespace std;

#pragma region ȫ�ֱ����ͺ�����������
//-----------------------------------��ȫ�ֺ������������֡�--------------------------------------
Point2f srcTriangle[3];
Point2f dstTriangle[3];//��������㣬�������������Σ��ڷ���任���õ�
Mat warpMat(2, 3, CV_32FC1);//����任�õ��ľ���

//-----------------------------------��ȫ�ֺ����������֡�--------------------------------------
static void ShowText();
static Mat strelDisk(int Radius);
static Mat thinImage(const Mat & src, const int maxIterations);
//static Mat remove_block(double minarea, Mat& src);
static void remove_block(Mat &Src, Mat &Dst, int AreaLimit, int CheckMode, int NeihborMode);
static int xmin(Mat& inputImage);
static int xmax(Mat& inputImage);
static IplImage* Mat_to_IplImage(const Mat Img);
static Mat IplImage_to_Mat(const IplImage* pBinary);
static void Niblack(const IplImage *srcImg, IplImage *binImg, double k,int t);
static Mat NiBlack(const Mat Img, int t, double k, int l);
static void chao_thinimage(Mat &srcimage);
static Mat xuanzhuan(const Mat src, double angle, int a, int b);
static int xxmin(Mat& inputImage);
static int xxmax(Mat& inputImage);
static int yymin(Mat& inputImage);
static int yymax(Mat& inputImage);
static Mat claheGO(Mat src, int _step);

#pragma endregion


//-----------------------------------��main( )������------------------------------------------
int main(int argc, char** argv)
{
#pragma region ��������	

	double time0 = static_cast<double>(getTickCount());//��¼��ʼʱ��
	system("color 2F");//�ı�console������ɫ	
	ShowText();//��ʾ��������

#pragma endregion

	/******************************* ͼ��Ԥ����׶� *******************************/

#pragma region ����ͼ��

	cout << "******* ͼ��Ԥ����׶ο�ʼ *******" << endl;

	//����һ��ͼƬ������ͼ��
	Mat picture = imread("..\\1.bmp");
	if (!picture.data) { printf("��ȡͼƬ����\n"); return false; }
	//��ʾ�����ͼƬ
	namedWindow("��ԭʼͼ��");
	imshow("��ԭʼͼ��", picture);
	cout<<"ͼ��������ɣ�"<<endl;

#pragma endregion

#pragma region ��ʱ��90����תͼ��

	//��תͼ��
	Mat picture0, t;
	transpose(picture, t);
	flip(t, picture0, 0); //��ʱ��90����תͼ��
	namedWindow("��0ͼ����ת��");
	imshow("��0ͼ����ת��", picture0);//��ʾ�����ͼƬ
	cout << "ͼ����ת��ɣ�" << endl;

#pragma endregion

#pragma region ת��Ϊ�Ҷ�ͼ	

	Mat picture1;//��������	
	cvtColor(picture0, picture1, COLOR_BGR2GRAY);//��ԭͼ��ת��Ϊ�Ҷ�ͼ��	
	namedWindow("1ͼ��ҶȻ�");
	imshow("1ͼ��ҶȻ�", picture1);//��ʾ�����ͼƬ
	cout<<"ͼ��ҶȻ���ɣ�"<<endl;

#pragma endregion

#pragma region ͼ���С�ͻҶȹ�һ��

	//��ֵ�˲�
	Mat picture2;
	blur(picture1, picture2, Size(3,3));
	namedWindow("2ͼ���ֵ�˲�");
	imshow("2ͼ���ֵ�˲�", picture2);
	cout<<"ͼ���ֵ�˲���ɣ�"<<endl;

	imwrite("..\\biye\\1��ֵ�˲�.bmp", picture2);

	//Sobel���ӱ�Ե���
	Mat picture3;
	Sobel(picture2, picture3, -1, 2, 0, 3, 1, 0, BORDER_DEFAULT);//ʹ��ˮƽX����Sobel
	namedWindow("3ͼ��Sobel���ӱ�Ե���");
	imshow("3ͼ��Sobel���ӱ�Ե���", picture3);
	cout << "ͼ��Sobel���ӱ�Ե�����ɣ�" << endl;
	//matlab���ر�Ե�����ֵ����tv��ֲ��opencv��������û�н��
	//��Եͼ����������ͨ���ж���ͨ��������Ĵ�С������������Ƚ���ȥ��
	imwrite("..\\biye\\2��Ե���.bmp", picture3);

	Mat picture4, picture5, picture6 ,picture7;
	Mat core1 = strelDisk(2);//������̬ѧ�����ں˰뾶Ϊ2��Բ
	morphologyEx(picture3, picture4, MORPH_CLOSE, core1);//ʵ�ֱ�����
	threshold(picture4, picture5, 20, 255, THRESH_BINARY);//���ж�ֵ����������20Ϊ��ֵ�����԰���Ҫ����
	//Mat picture6 = thinImage(picture5,-1);//������ͼ����й�������ϸ����������������
	//Mat picture7 = remove_block(100.0, picture6);//ȥ����ֵͼ�������
	//Mat picture8 = thinImage(picture7, -1);//������ͼ����й�������ϸ����������������
	//Mat picture9 = remove_block(100.0, picture8);//ȥ����ֵͼ���������ps���˴���û���ã����ǹ�������Ҫ�˾䣬����ɾ����
	chao_thinimage(picture5);
	picture6 = picture5;//������ͼ����й�������ϸ����������������
	//imshow("6", picture6);
	remove_block(picture6, picture7, 100, 1, 1);//ȥ����ֵͼ�������
	chao_thinimage(picture7);
	Mat picture8 = picture7;//������ͼ����й�������ϸ����������������
	namedWindow("4ͼ��ȥ����");
	imshow("4ͼ������", picture4);
	imshow("4ͼȥ����", picture5);
	imshow("4��ȥ����", picture6);
	imshow("4ͼ��ȥ����", picture8);
	cout << "ͼ��ȥ������ɣ�" << endl;
	imwrite("..\\biye\\3������.bmp", picture8);


	//Ѱ����������ָ�߽���������ֵ
	int x_min = xmin(picture8);
	cout << "��ָ��߽�ĺ�����ֵ��" << x_min << endl;
	//Ѱ����������ָ�߽��������Сֵ
	int x_max = xmax(picture8);
	cout << "��ָ�ұ߽�ĺ�����ֵ��" << x_max << endl;
	//��ȡROIͼ������
	Mat imageROI = picture1(Range(40, picture1.rows-10),Range(x_min+6, x_max-6));//�����Ҷ������˸ı�
	//Mat imageROI = picture1(Rect(x_min, 0, x_max - x_min, picture1.rows));//Ч��һ��
	cout << "��ȡROIͼ��������ɣ�" << endl;
	namedWindow("5��ȡ����ROIͼ��");
	imshow("5��ȡ����ROIͼ��", imageROI);

	imwrite("..\\biye\\4ROI.bmp", imageROI);

	//���гߴ��һ��
	Mat picture10;
	int chang = imageROI.rows;
	int kuan = imageROI.cols;
	cout << "ROI����δ��һ���ߴ�ǰ��Ϊ" << chang << endl;
	cout << "ROI����δ��һ���ߴ�ǰ��Ϊ" << kuan << endl;

	//����Դͼ���Ŀ��ͼ���ϵ�������Լ������任
	srcTriangle[0] = Point2f(0, 0);
	srcTriangle[1] = Point2f(static_cast<float>(imageROI.cols ), 0);
	srcTriangle[2] = Point2f(0, static_cast<float>(imageROI.rows ));
	dstTriangle[0] = Point2f(static_cast<float>(imageROI.cols*0.0), static_cast<float>(imageROI.rows*0.0));
	dstTriangle[1] = Point2f(static_cast<float>(imageROI.cols*180.0 / kuan), static_cast<float>(imageROI.rows*0.0));
	dstTriangle[2] = Point2f(static_cast<float>(imageROI.cols*0.0), static_cast<float>(imageROI.rows * 250.0 / chang));
	warpMat = getAffineTransform(srcTriangle, dstTriangle);//��÷���任����	
	warpAffine(imageROI, picture10, warpMat, Size(180, 250), INTER_LINEAR);//��Դͼ��Ӧ�÷���任������˫���Բ�ֵ�����Ҫ�޸Ĺ�һ��ͼ���Сһ��Ҫ�޸�����ѡ�������任ֵ
	
	//srcTriangle[0] = Point2f(0, 0);
	//srcTriangle[1] = Point2f(static_cast<float>(imageROI.cols), 0);
	//srcTriangle[2] = Point2f(0, static_cast<float>(imageROI.rows));
	//dstTriangle[0] = Point2f(static_cast<float>(imageROI.cols*0.0), static_cast<float>(imageROI.rows*0.0));
	//dstTriangle[1] = Point2f(static_cast<float>(imageROI.cols*80.0 / kuan), static_cast<float>(imageROI.rows*0.0));
	//dstTriangle[2] = Point2f(static_cast<float>(imageROI.cols*0.0), static_cast<float>(imageROI.rows * 310 / chang));
	//warpMat = getAffineTransform(srcTriangle, dstTriangle);//��÷���任����	
	//warpAffine(imageROI, picture10, warpMat, Size(80, 310), INTER_LINEAR);//��Դͼ��Ӧ�÷���任������˫���Բ�ֵ�����Ҫ�޸Ĺ�һ��ͼ���Сһ��Ҫ�޸�����ѡ�������任ֵ
	////���������ִ�С 60*210 & 80*310 ͼƬ����ѡ�񣬺��ָ�����

	cout << "ROIͼ��ߴ��һ����ɣ�" << endl;
	cout << "ROI����ߴ��һ���󳤣�" << picture10.rows << endl;
	cout << "ROI����ߴ��һ�����" << picture10.cols << endl;

	imwrite("..\\biye\\5��С��һ��.bmp", picture10);

	//���лҶȹ�һ��
	Mat picture11;
	equalizeHist(picture10, picture11);//ֱ��ͼ���⻯,���Ҷȹ�һ��
	cout << "ROIͼ��Ҷȹ�һ����ɣ�" << endl;
	namedWindow("6ͼ����гߴ�Ҷȹ�һ��");
	imshow("6ͼ����гߴ�Ҷȹ�һ��", picture11);

	cout << "******* ͼ��Ԥ����׶���� *******" << endl;

#pragma endregion

	/******************************* ͼ����ǿ�ָ�׶� *******************************/

#pragma region ���о�ֵ�˲�����ǿͼ��

	/*	���׶���Ҫ���û���ģ���ķָ�ͼ���㷨������NiBlack�㷨�Ծ�������������ȡ*/
	cout << "******* ͼ����ǿ�ָ�׶ο�ʼ *******" << endl;

	//Mat picture11 = imread("..\\4.bmp");�˴�����ֱ����ȡ����ROI�����ͼƬ��ȡ�ã������Դ�����
	Mat picture12;
	//cvtColor(picture11, picture12, COLOR_BGR2GRAY);//��ԭͼ��ת��Ϊ�Ҷ�ͼ��	//�����ֱ�Ӷ�ȡROI����ͼƬ��Ҫ�ȻҶȻ�
	blur(picture11, picture12, Size(3, 3));//���о�ֵ�˲�����
	cout << "ͼ���ֵ�˲���ɣ�" << endl;
	equalizeHist(picture12, picture12);//ֱ��ͼ���⻯
	imwrite("..\\biye\\6ֱ��ͼ���⻯.bmp", picture12);
	//picture12 = claheGO(picture11, 5);//CLAHE�㷨
	//imwrite("..\\biye\\7claheֱ��ͼ���⻯.bmp", picture12);
	cout << "ͼ����ǿ��ɣ�" << endl;
	imshow("6ͼ���ֵ�˲�", picture12);
	
#pragma endregion

#pragma region ͼ��ָ�

	//IplImage* picture13_1=Mat_to_IplImage(picture10);
	IplImage* picture13_1 = Mat_to_IplImage(picture12);//��Mat��ת��ΪIplImage��
	IplImage* picture13_2 = cvCloneImage(picture13_1);//������ͬ��С��IplImage��
	Niblack(picture13_1, picture13_2, -0.925 , 5 );//����NiBlack�㷨����ͼ�񣬿ɵ��ڲ���������ϵ���Լ�ѡ�񴰿ڣ����ݵĵ������ö�ʶ��Ӱ�������Ҫ�������ں���������������д����ص��������������۲�����ֵ
	Mat picture13 = IplImage_to_Mat(picture13_2);////��IplImage��ת��ΪMat��
	imshow("7ͼ�����NiBlack�㷨����", picture13);
	cout << "ͼ��NiBlack�㷨������ɣ�" << endl;

	imwrite("..\\biye\\8niblack.bmp", picture13);

	Mat picture14;
	remove_block(picture13, picture14, 15, 1, 1);//ȥ����ֵͼ���������ȥ�����ֵ�ɵ���Ӱ���
	//Mat picture14 = remove_block(10.0, picture13);//ȥ����ֵͼ���������ȥ�����ֵ�ɵ���Ӱ���
	//morphologyEx(picture14, picture14, MORPH_CLOSE, MORPH_ELLIPSE);//ʵ�ֱ�����
	imshow("8ͼ��ָ��", picture14);
	cout << "ͼ��ָ����ɣ�" << endl;
	imwrite("..\\biye\\9ȥ������.bmp", picture14);

	//Mat M = NiBlack(picture12, 2, 50, INT_MAX);//ò��д��Mat��ĵ�niblank�㷨��̫���ã�ѡ�����
	//imshow("����", M);

	cout << "******* ͼ����ǿ�ָ�׶���� *******" << endl;

#pragma endregion

	/******************************* ͼ�����׶� *******************************/

#pragma region ͼ���˲�ȥ��

	cout << "******* ͼ�����������ʼ *******" << endl;


	Mat picture15, picture16;
	medianBlur(picture14, picture15, 5);//��ֵ�˲�

	remove_block(picture15, picture15, 40, 1, 1);//ȥ������

	//Mat picture15, picture16;
	//medianBlur(picture14, picture15, 5);//��ֵ�˲�
	//picture15 = remove_block(22.0, picture15);//ȥ������
	//morphologyEx(picture15, picture16, MORPH_CLOSE, NULL);//ʵ�ֱ�����
	imshow("9ͼ���˲�ȥ�봦��", picture15);
	cout << "ͼ���˲�ȥ�봦����ɣ�" << endl;

	imwrite("..\\biye\\10�˲�ȥ������.bmp", picture15);


#pragma endregion

#pragma region ͼ�������

	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(8, 25));//������Բ���ӣ������ɵ���Ӱ���
	dilate(picture15, picture16, element);//������̬ѧ����
	imwrite("..\\biye\\11��̬ѧ����.bmp", picture16);

	Mat picture17;
	medianBlur(picture16, picture17, 7);//��ֵ�˲�
	remove_block(picture17, picture17, 200, 1, 1);//ȥ��������������ⲻʵ��,��ע��
	//picture17 = remove_block(120, picture17);//ȥ��������������ⲻʵ��,��ע��
	imwrite("..\\biye\\12ȥ�������.bmp", picture17);
	//Canny(picture17, picture18, 3, 9, 3);
	//morphologyEx(picture17, picture18, MORPH_ERODE, Size(6, 6));//ʵ�ֱ�����
//	Mat picture19, picture20;
//	Mat picture19 = thinImage(picture17, -1);//������ͼ����й�������ϸ����������������
	imshow("10ͼ�����������1", picture15);
	imshow("10ͼ�����������2", picture16);
	imshow("10ͼ�����������3", picture17);
	chao_thinimage(picture17);
	//blur(picture17, picture18,Size(3,3));
	imshow("10ͼ�����������4", picture17);
	//imshow("ͼ�����������", picture18);
	cout << "ͼ�������������ɣ�" << endl;
	imwrite("..\\tu_data\\������ͼƬ.bmp", picture17);
	imwrite("..\\biye\\13������.bmp", picture17);

	cout << "ͼ�񱣴���ɣ�" << endl;
	
	cout << "******* ͼ�������������� *******" << endl;

#pragma endregion


time0 = ((double)getTickCount() - time0) / getTickFrequency();//��������ʱ�䲢���
cout << "\n������ʱ��Ϊ�� " << time0 << "��" << endl << endl;  //�������ʱ��

	//�ȴ����ⰴ������
	waitKey(0);
	return 0;
}
//-----------------------------------�����ù����ȫ���Ӻ�����------------------------------------------
#pragma region �����Ϣ����ShowText( )

//-----------------------------------��ShowHelpText( )������----------------------------------  
static void ShowText()//���һЩ������Ϣ  
{
	//���������Ϣ��OpenCV�汾
	printf("\n\t\t\t	OpenCVָ�����Ǽ���ȡ��Ŀ����1.0�汾  \n");
	printf("\n\t\t\t	2018��3��  CIDP_425_Lab	 \n"); 
	printf("\n\t\t\t	By@JXH   github:j1o2h3n\n");
	printf("\n\t\t\t	��������IDE�汾��Visual Studio 2013	 \n");
	printf("\n\t\t\t	��ǰʹ�õ�OpenCV�汾Ϊ��" CV_VERSION);
	printf("\n\n\n  -----------------------------------------------------------------------------------------------------------\n\n\n");
}

#pragma endregion

#pragma region ������̬ѧ�����ں˰뾶ΪR��Բ����strelDisk()

//ʵ��matlab��strel('disk',Radius)���� 
static Mat strelDisk(int Radius)
{
	int borderWidth; Mat sel; int m, n;
	switch (Radius){
	case 1:
	case 2:
		if (Radius == 1)
			borderWidth = 1;
		else
			borderWidth = 2;
		sel = Mat((2 * Radius + 1), (2 * Radius + 1), CV_8U, cv::Scalar(1));
		break;//���뾶Ϊ1ʱ��3X3�� ,���뾶Ϊ2ʱ��5X5��  
	case 3:
		borderWidth = 0;
		sel = Mat((2 * Radius - 1), (2 * Radius - 1), CV_8U, cv::Scalar(1));
		break;
	default:
		n = Radius / 7; m = Radius % 7;
		if (m == 0 || m >= 4)
			borderWidth = 2 * (2 * n + 1);
		else
			borderWidth = 2 * 2 * n;
		sel = Mat((2 * Radius - 1), (2 * Radius - 1), CV_8U, cv::Scalar(1));
		break;
	}
	for (int i = 0; i < borderWidth; i++){
		for (int j = 0; j < borderWidth; j++){
			if (i + j < borderWidth){
				sel.at<uchar>(i, j) = 0;
				sel.at<uchar>(i, sel.cols - 1 - j) = 0;
				sel.at<uchar>(sel.rows - 1 - i, j) = 0;
				sel.at<uchar>(sel.rows - 1 - i, sel.cols - 1 - j) = 0;
			}
		}
	}
	return sel;
}

#pragma endregion

#pragma region ������ͼ�����ϸ���������ĺ���thinImage()

/**
* @brief ������ͼ�����ϸ����������
* @param srcΪ����ͼ��,��cvThreshold�����������8λ�Ҷ�ͼ���ʽ��Ԫ����ֻ��0��1,1������Ԫ�أ�0����Ϊ�հ�
* @param maxIterations���Ƶ���������������������ƣ�Ĭ��Ϊ-1���������Ƶ���������ֱ��������ս��
* @return Ϊ��srcϸ��������ͼ��,��ʽ��src��ʽ��ͬ��Ԫ����ֻ��0��1,1������Ԫ�أ�0����Ϊ�հ�
*/
static Mat thinImage(const Mat &src, const int maxIterations = -1)
{
	assert(src.type() == CV_8UC1);
	Mat dst;
	int width = src.cols;
	int height = src.rows;
	src.copyTo(dst);
	int count = 0;  //��¼��������    
	while (true)
	{
		count++;
		if (maxIterations != -1 && count > maxIterations) //���ƴ������ҵ�����������    
			break;
		std::vector<uchar *> mFlag; //���ڱ����Ҫɾ���ĵ�    
		//�Ե���    
		for (int i = 0; i < height; ++i)
		{
			uchar * p = dst.ptr<uchar>(i);
			for (int j = 0; j < width; ++j)
			{
				//��������ĸ����������б��    
				//  p9 p2 p3    
				//  p8 p1 p4    
				//  p7 p6 p5    
				uchar p1 = p[j];
				if (p1 != 1) continue;
				uchar p4 = (j == width - 1) ? 0 : *(p + j + 1);
				uchar p8 = (j == 0) ? 0 : *(p + j - 1);
				uchar p2 = (i == 0) ? 0 : *(p - dst.step + j);
				uchar p3 = (i == 0 || j == width - 1) ? 0 : *(p - dst.step + j + 1);
				uchar p9 = (i == 0 || j == 0) ? 0 : *(p - dst.step + j - 1);
				uchar p6 = (i == height - 1) ? 0 : *(p + dst.step + j);
				uchar p5 = (i == height - 1 || j == width - 1) ? 0 : *(p + dst.step + j + 1);
				uchar p7 = (i == height - 1 || j == 0) ? 0 : *(p + dst.step + j - 1);
				if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6)
				{
					int ap = 0;
					if (p2 == 0 && p3 == 1) ++ap;
					if (p3 == 0 && p4 == 1) ++ap;
					if (p4 == 0 && p5 == 1) ++ap;
					if (p5 == 0 && p6 == 1) ++ap;
					if (p6 == 0 && p7 == 1) ++ap;
					if (p7 == 0 && p8 == 1) ++ap;
					if (p8 == 0 && p9 == 1) ++ap;
					if (p9 == 0 && p2 == 1) ++ap;

					if (ap == 1 && p2 * p4 * p6 == 0 && p4 * p6 * p8 == 0)
					{
						//���    
						mFlag.push_back(p + j);
					}
				}
			}
		}

		//����ǵĵ�ɾ��    
		for (std::vector<uchar *>::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
		{
			**i = 0;
		}

		//ֱ��û�е����㣬�㷨����    
		if (mFlag.empty())
		{
			break;
		}
		else
		{
			mFlag.clear();//��mFlag���    
		}

		//�Ե���    
		for (int i = 0; i < height; ++i)
		{
			uchar * p = dst.ptr<uchar>(i);
			for (int j = 0; j < width; ++j)
			{
				//��������ĸ����������б��    
				//  p9 p2 p3    
				//  p8 p1 p4    
				//  p7 p6 p5    
				uchar p1 = p[j];
				if (p1 != 1) continue;
				uchar p4 = (j == width - 1) ? 0 : *(p + j + 1);
				uchar p8 = (j == 0) ? 0 : *(p + j - 1);
				uchar p2 = (i == 0) ? 0 : *(p - dst.step + j);
				uchar p3 = (i == 0 || j == width - 1) ? 0 : *(p - dst.step + j + 1);
				uchar p9 = (i == 0 || j == 0) ? 0 : *(p - dst.step + j - 1);
				uchar p6 = (i == height - 1) ? 0 : *(p + dst.step + j);
				uchar p5 = (i == height - 1 || j == width - 1) ? 0 : *(p + dst.step + j + 1);
				uchar p7 = (i == height - 1 || j == 0) ? 0 : *(p + dst.step + j - 1);

				if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6)
				{
					int ap = 0;
					if (p2 == 0 && p3 == 1) ++ap;
					if (p3 == 0 && p4 == 1) ++ap;
					if (p4 == 0 && p5 == 1) ++ap;
					if (p5 == 0 && p6 == 1) ++ap;
					if (p6 == 0 && p7 == 1) ++ap;
					if (p7 == 0 && p8 == 1) ++ap;
					if (p8 == 0 && p9 == 1) ++ap;
					if (p9 == 0 && p2 == 1) ++ap;

					if (ap == 1 && p2 * p4 * p8 == 0 && p2 * p6 * p8 == 0)
					{
						//���    
						mFlag.push_back(p + j);
					}
				}
			}
		}

		//����ǵĵ�ɾ��    
		for (std::vector<uchar *>::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
		{
			**i = 0;
		}

		//ֱ��û�е����㣬�㷨����    
		if (mFlag.empty())
		{
			break;
		}
		else
		{
			mFlag.clear();//��mFlag���    
		}
	}
	return dst;
}

#pragma endregion

//#pragma region ɾ����ֵ��ͼ���������С������remove_block()
//
//static Mat remove_block(double minarea, Mat& src)
//{
//	CvSeq* contour = NULL;
//	double tmparea = 0.0;
//	CvMemStorage* storage = cvCreateMemStorage(0);
//	//const char* tmpaddsum1 = tmp_string.c_str();  
//
//	//IplImage* img_src = cvLoadImage(tmpaddsum1, CV_LOAD_IMAGE_ANYCOLOR);  
//	IplImage* img_src = &IplImage(src);
//	IplImage* img_Clone = cvCloneImage(img_src);
//	//���ʶ�ֵͼ��ÿ�����ֵ  
//	uchar *pp;
//	IplImage* img_dst = cvCreateImage(cvGetSize(img_src), IPL_DEPTH_8U, 1);
//
//	//-----����2ֵͼ�е�������������������ɾ�����С��minarea������-----//  
//	//CvScalar color = cvScalar(255, 0, 0);//CV_RGB(128,0,0)  
//	CvContourScanner scanner = NULL;
//	scanner = cvStartFindContours(img_src, storage, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));
//	//��ʼ����������  
//	CvRect rect;
//
//	while (contour = cvFindNextContour(scanner)){
//		tmparea = fabs(cvContourArea(contour));
//		rect = cvBoundingRect(contour, 0);
//
//		if (tmparea < minarea){
//			//����ͨ��������ĵ�Ϊ��ɫʱ�����������С���ú�ɫ�������  
//			pp = (uchar*)(img_Clone->imageData + img_Clone->widthStep*(rect.y + rect.height / 2) + rect.x + rect.width / 2);
//			if (pp[0] == 255){
//				for (int y = rect.y; y<rect.y + rect.height; y++)
//				{
//					for (int x = rect.x; x<rect.x + rect.width; x++)
//					{
//						pp = (uchar*)(img_Clone->imageData + img_Clone->widthStep*y + x);
//
//						if (pp[0] == 255)
//						{
//							pp[0] = 0;
//						}
//					}
//				}
//			}
//
//		}
//	}
//	Mat dst_img = cvarrToMat(img_Clone);
//	if (dst_img.channels() == 3)
//		cvtColor(dst_img, dst_img, CV_RGB2GRAY);
//	return dst_img;
//}
//
//#pragma endregion

#pragma region ɾ����ֵ��ͼ���������С������remove_block()

static void remove_block(Mat &Src, Mat &Dst, int AreaLimit, int CheckMode, int NeihborMode)
{
	int RemoveCount = 0;
	Dst = Src;
	//�½�һ����ǩͼ���ʼ��Ϊ0���ص㣬Ϊ�˼�¼ÿ�����ص����״̬�ı�ǩ��0����δ��飬1�������ڼ��,2�����鲻�ϸ���Ҫ��ת��ɫ����3������ϸ������   
	//��ʼ����ͼ��ȫ��Ϊ0��δ���  
	Mat PointLabel = Mat::zeros(Src.size(), CV_8UC1);
	if (CheckMode == 1)//ȥ��С��ͨ����İ�ɫ��  
	{
		//cout << "ȥ��С��ͨ��.";
		for (int i = 0; i < Src.rows; i++)
		{
			for (int j = 0; j < Src.cols; j++)
			{
				if (Src.at<uchar>(i, j) < 10)
				{
					PointLabel.at<uchar>(i, j) = 3;//��������ɫ����Ϊ�ϸ�����Ϊ3  
				}
			}
		}
	}
	else//ȥ���׶�����ɫ������  
	{
		//cout << "ȥ���׶�";
		for (int i = 0; i < Src.rows; i++)
		{
			for (int j = 0; j < Src.cols; j++)
			{
				if (Src.at<uchar>(i, j) > 10)
				{
					PointLabel.at<uchar>(i, j) = 3;//���ԭͼ�ǰ�ɫ���򣬱��Ϊ�ϸ�����Ϊ3  
				}
			}
		}
	}


	vector<Point2i>NeihborPos;//������ѹ������  
	NeihborPos.push_back(Point2i(-1, 0));
	NeihborPos.push_back(Point2i(1, 0));
	NeihborPos.push_back(Point2i(0, -1));
	NeihborPos.push_back(Point2i(0, 1));


	if (NeihborMode == 1)
	{
		//cout << "Neighbor mode: 8����." << endl;
		NeihborPos.push_back(Point2i(-1, -1));
		NeihborPos.push_back(Point2i(-1, 1));
		NeihborPos.push_back(Point2i(1, -1));
		NeihborPos.push_back(Point2i(1, 1));
	}
	else int a = 0;//cout << "Neighbor mode: 4����." << endl;
	int NeihborCount = 4 + 4 * NeihborMode;
	int CurrX = 0, CurrY = 0;
	//��ʼ���  
	for (int i = 0; i < Src.rows; i++)
	{
		for (int j = 0; j < Src.cols; j++)
		{
			if (PointLabel.at<uchar>(i, j) == 0)//��ǩͼ�����ص�Ϊ0����ʾ��δ���Ĳ��ϸ��  
			{   //��ʼ���  
				vector<Point2i>GrowBuffer;//��¼������ص�ĸ���  
				GrowBuffer.push_back(Point2i(j, i));
				PointLabel.at<uchar>(i, j) = 1;//���Ϊ���ڼ��  
				int CheckResult = 0;

				for (int z = 0; z < GrowBuffer.size(); z++)
				{
					for (int q = 0; q < NeihborCount; q++)
					{
						CurrX = GrowBuffer.at(z).x + NeihborPos.at(q).x;
						CurrY = GrowBuffer.at(z).y + NeihborPos.at(q).y;
						if (CurrX >= 0 && CurrX < Src.cols&&CurrY >= 0 && CurrY < Src.rows)  //��ֹԽ��    
						{
							if (PointLabel.at<uchar>(CurrY, CurrX) == 0)
							{
								GrowBuffer.push_back(Point2i(CurrX, CurrY));  //��������buffer    
								PointLabel.at<uchar>(CurrY, CurrX) = 1;           //���������ļ���ǩ�������ظ����    
							}
						}
					}
				}
				if (GrowBuffer.size() > AreaLimit) //�жϽ�����Ƿ񳬳��޶��Ĵ�С����1Ϊδ������2Ϊ����    
					CheckResult = 2;
				else
				{
					CheckResult = 1;
					RemoveCount++;//��¼�ж�������ȥ��  
				}

				for (int z = 0; z < GrowBuffer.size(); z++)
				{
					CurrX = GrowBuffer.at(z).x;
					CurrY = GrowBuffer.at(z).y;
					PointLabel.at<uchar>(CurrY, CurrX) += CheckResult;//��ǲ��ϸ�����ص㣬����ֵΪ2  
				}
				//********�����õ㴦�ļ��**********    
			}
		}
	}

	CheckMode = 255 * (1 - CheckMode);
	//��ʼ��ת�����С������    
	for (int i = 0; i < Src.rows; ++i)
	{
		for (int j = 0; j < Src.cols; ++j)
		{
			if (PointLabel.at<uchar>(i, j) == 2)
			{
				Dst.at<uchar>(i, j) = CheckMode;
			}
			else if (PointLabel.at<uchar>(i, j) == 3)
			{

				Dst.at<uchar>(i, j) = Src.at<uchar>(i, j);

			}
		}
	}


}

#pragma endregion

#pragma region Ѱ��������߽����ֵ������xmin()

static int xmin(Mat& inputImage)
{
	int x_min=0;
	for (int i = 0; i < inputImage.rows; ++i)//��
	{
		for (int j = 0; j < inputImage.cols / 2; ++j)//��
		{
			if (inputImage.at<uchar>(i, j))
			{
				if (x_min<j)
				{
					x_min = j;
				}
			}
		}
	}
	return x_min;
}


#pragma endregion

#pragma region Ѱ��������߽���Сֵ������xmax()

static int xmax(Mat& inputImage)
{
	int x_max = inputImage.cols;
	for (int i = 0; i < inputImage.rows; ++i)//��
	{
		for (int j = inputImage.cols / 2; j < inputImage.cols; ++j)//��
		{
			if (inputImage.at<uchar>(i, j))
			{
				if (x_max>j)
				{
					x_max = j;
				}
			}
		}
	}
	return x_max;
}


#pragma endregion

#pragma region ����IplImage���ʹ���ͼ��Nilblack��ֵ���㷨����Niblack()
/*************************************************************
Nilblack��ֵ���㷨�����ȫ����ֵ����ͼ��ͱ����Ҷ��������ʱ
���׺���ϸ�ڵ���������Ż�����һ�ֳ��õıȽ���Ч�ľֲ���ֵ
�㷨�������㷨�Ļ���˼���Ƕ�Ԥ���е�ÿһ���㣬������R*R�����ڣ�
��������������ص�ľ�ֵ�ͷ��Ȼ����ʽ������ֵ���ж�ֵ����
T(x,y) = m(x,y) + k* s(x,y)���ж������ص�����(x,y),T(x,y) Ϊ��
����ֵ��m(x,y)Ϊ�õ��R*R���������ص�����ؾ�ֵ��s(x,y)Ϊ�õ�
R*R���������ر�׼���kΪ����ϵ��(ͨ��ȡ-0.1),t��ѡ���㷨���ڴ�С��
*************************************************************/
static void Niblack(const IplImage *srcImg, IplImage *binImg, double k,int t)
{
	int m = srcImg->width;//��
	int n = srcImg->height;//��
	int Niblack_q=t;//������
	int Niblack_s=t;//������
	

	for (int i = 0; i < n; i++)//��ѭ��
	{
		uchar *srcptr = (uchar*)(srcImg->imageData + srcImg->widthStep * i);//�����i����ߵ����ص�ָ��srcptr
		uchar *dstptr = (uchar*)(binImg->imageData + binImg->widthStep * i);
		for (int j = 0; j < m; j++)//��ѭ��
		{
			int begin_y = i - Niblack_q, begin_x = j - Niblack_s;
			int end_y = i + Niblack_q, end_x = j + Niblack_s;
			if (begin_y < 0) begin_y = 0;
			if (begin_x < 0) begin_x = 0;
			if (end_y > n) end_y = n;
			if (end_x > m) end_x = m;

			double mean = 0;
			double std = 0;
			int total = (end_y - begin_y) * (end_x - begin_x);  //���������ܵ����ص������˴�Ӧ��Ϊend-begin+1��ֵ���������ø���  

			for (int y = begin_y; y<= end_y; y++)//��
			{
				uchar *ptr = (uchar*)(srcImg->imageData + srcImg->widthStep * y);
				for (int x = begin_x; x <= end_x; x++)//��
				{
					mean += ptr[x];
				}
			}  //�����ڸ�С�����ڻҶ�ֵ�ܺ�  

			mean = mean / total;  //�������ƽ���Ҷ�  

			for (int y = begin_y; y <= end_y; y++)
			{
				uchar *ptr = (uchar*)(srcImg->imageData + srcImg->widthStep * y);
				for (int x = begin_x; x <= end_x; x++)
				{
					double sq = (mean - ptr[x]) * (mean - ptr[x]);
					std += sq;
				}
			}   //������������ܵķ����  

			std /= total;
			std = sqrt(std);  //�����׼��  

			double threshold = mean + k * std;  //������õ��ĵ���ֵ  

			if (srcptr[j] > threshold-1)
				dstptr[j] = 0;
			else
				dstptr[j] = 255;
		}
	}
}

#pragma endregion

#pragma region ��Matת��ΪIplImage���͵ĺ���Mat_to_IplImage()

static IplImage* Mat_to_IplImage(const Mat Img)
{
	//Mat��>IplImage  
	//ǳ����:  
	IplImage* pBinary = &IplImage(Img);
	//���ֻҪ�ټ�һ�θ�������  
	IplImage *input = cvCloneImage(pBinary);
	return input;
}

#pragma endregion

#pragma region ��IplImageת��ΪMat���͵ĺ���IplImage_to_Mat()

static Mat IplImage_to_Mat(const IplImage *pBinary)
{
	//IplImage��>Mat  
	//ǳ������  
	Mat Img;
	Img = cvarrToMat(pBinary);
	//���ֻ��Ҫ����Mat�ﴴ��һ���µ�Mat����Ȼ��������ݵĸ��ƣ����������ĺ�����������ͷ�ĸ��ƣ�ǳ������  
	Mat ImgTemp;
	ImgTemp = Img.clone();
	return ImgTemp;
}

#pragma endregion

#pragma region ����Mat���Ͳ���ͼ��д��NiBlack��ֵ���㷨����NiBlack()

//�˷����Ҳ�����ʹ�ã���������ѡ���Խ����β��������û��ɷβ��
static  Mat NiBlack(const Mat Img, int t, double k, int l)
{
	int m = Img.rows;//��
	int n = Img.cols;//��
	//Mat M = Mat::zeros((Img.rows + 2 * t),( Img.cols + 2 * t),CV_8UC1);//���������
	//Mat yanmo = M(Rect(t , t , Img.cols, Img.rows));
	//Img.copyTo(yanmo, Img);//ͼ����Ĥ
	Mat M;
	cv::copyMakeBorder(Img, M, t, t, t, t, cv::BORDER_CONSTANT, 0);//����ͼ��߽磬����Ĥ����
	Mat b = Mat::zeros(Img.rows, Img.cols, CV_8UC1);
	Mat h = Mat::zeros(Img.rows, Img.cols, CV_8UC1);
	for (int i = t; i<(m + t); i++)//at<uchar>(0, 0)��ʼ
	{
		for (int j = t ; j <(n + t); j++)
		{
			int begin_y = i - t, begin_x = j - t;
			int end_y = i +t, end_x = j + t;
			double mean = 0;
			double std = 0;
			int total = ((2*t)^2);  //���������ܵ����ص���,����matlab���� 
			
			for (int y = begin_y; y<= end_y; y++)
			{
				for (int x = begin_x; x <= end_x; x++)
				{
					mean = mean + M.at<uchar>(y, x);
				}
			}  //�����ڸ�С�����ڻҶ�ֵ�ܺ�  


			mean = mean / total;  //�������ƽ���Ҷ�  

			for (int y = begin_y; y <= end_y; y++)
			{
				for (int x = begin_x; x <= end_x; x++)
				{
					double sq = (M.at<uchar>(y, x) - mean) * (M.at<uchar>(y, x) - mean);
					std = std + sq;
				}
			}   //������������ܵķ����  

			std = std / total;
			std = sqrt(std);  //�����׼��  

			double threshold = k * std +  mean;  //������õ��ĵ���ֵ
			//M.at<uchar>(i-t, j-t) = threshold;
			M.at<uchar>(i , j ) = threshold;
			//M.at<uchar>(i, j) = mean;
		}
	}
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (Img.at<uchar>(i, j) <= M.at<uchar>(i, j))
			{
				/*h.at<uchar>(i, j) = 255;*/ h.at<uchar>(i, j) = Img.at<uchar>(i, j)*double((Img.at<uchar>(i, j) / M.at<uchar>(i, j)) ^ l);
			}
			else //if (M.at<uchar>(i, j) != 255)
			{
				/*h.at<uchar>(i, j) = 0;*/ h.at<uchar>(i, j) = 256 - ((255 - Img.at<uchar>(i, j))*double(((255 - Img.at<uchar>(i, j)) / (255 - M.at<uchar>(i, j))) ^ l));
			}
			//else if (M.at<uchar>(i, j) == 255)
			//{
				///*h.at<uchar>(i, j) = 0;*/ h.at<uchar>(i, j) = 0;
			//}
		}
	}

	//Mat shuchu;	
	return M;
}

#pragma endregion

#pragma region ��������������chao_thinimage()

//�����������������ǰ����Ǹ��滻����ǰ���Ǹ����Ǻܺã����һ���С����

static void chao_thinimage(Mat &srcimage)//��ͨ������ֵ�����ͼ��  
{
	vector<Point> deletelist1;
	int Zhangmude[9];
	int nl = srcimage.rows;
	int nc = srcimage.cols;
	while (true)
	{
		for (int j = 1; j < (nl - 1); j++)
		{
			uchar* data_last = srcimage.ptr<uchar>(j - 1);
			uchar* data = srcimage.ptr<uchar>(j);
			uchar* data_next = srcimage.ptr<uchar>(j + 1);
			for (int i = 1; i < (nc - 1); i++)
			{
				if (data[i] == 255)
				{
					Zhangmude[0] = 1;
					if (data_last[i] == 255) Zhangmude[1] = 1;
					else  Zhangmude[1] = 0;
					if (data_last[i + 1] == 255) Zhangmude[2] = 1;
					else  Zhangmude[2] = 0;
					if (data[i + 1] == 255) Zhangmude[3] = 1;
					else  Zhangmude[3] = 0;
					if (data_next[i + 1] == 255) Zhangmude[4] = 1;
					else  Zhangmude[4] = 0;
					if (data_next[i] == 255) Zhangmude[5] = 1;
					else  Zhangmude[5] = 0;
					if (data_next[i - 1] == 255) Zhangmude[6] = 1;
					else  Zhangmude[6] = 0;
					if (data[i - 1] == 255) Zhangmude[7] = 1;
					else  Zhangmude[7] = 0;
					if (data_last[i - 1] == 255) Zhangmude[8] = 1;
					else  Zhangmude[8] = 0;
					int whitepointtotal = 0;
					for (int k = 1; k < 9; k++)
					{
						whitepointtotal = whitepointtotal + Zhangmude[k];
					}
					if ((whitepointtotal >= 2) && (whitepointtotal <= 6))
					{
						int ap = 0;
						if ((Zhangmude[1] == 0) && (Zhangmude[2] == 1)) ap++;
						if ((Zhangmude[2] == 0) && (Zhangmude[3] == 1)) ap++;
						if ((Zhangmude[3] == 0) && (Zhangmude[4] == 1)) ap++;
						if ((Zhangmude[4] == 0) && (Zhangmude[5] == 1)) ap++;
						if ((Zhangmude[5] == 0) && (Zhangmude[6] == 1)) ap++;
						if ((Zhangmude[6] == 0) && (Zhangmude[7] == 1)) ap++;
						if ((Zhangmude[7] == 0) && (Zhangmude[8] == 1)) ap++;
						if ((Zhangmude[8] == 0) && (Zhangmude[1] == 1)) ap++;
						if (ap == 1)
						{
							if ((Zhangmude[1] * Zhangmude[7] * Zhangmude[5] == 0) && (Zhangmude[3] * Zhangmude[5] * Zhangmude[7] == 0))
							{
								deletelist1.push_back(Point(i, j));
							}
						}
					}
				}
			}
		}
		if (deletelist1.size() == 0) break;
		for (size_t i = 0; i < deletelist1.size(); i++)
		{
			Point tem;
			tem = deletelist1[i];
			uchar* data = srcimage.ptr<uchar>(tem.y);
			data[tem.x] = 0;
		}
		deletelist1.clear();

		for (int j = 1; j < (nl - 1); j++)
		{
			uchar* data_last = srcimage.ptr<uchar>(j - 1);
			uchar* data = srcimage.ptr<uchar>(j);
			uchar* data_next = srcimage.ptr<uchar>(j + 1);
			for (int i = 1; i < (nc - 1); i++)
			{
				if (data[i] == 255)
				{
					Zhangmude[0] = 1;
					if (data_last[i] == 255) Zhangmude[1] = 1;
					else  Zhangmude[1] = 0;
					if (data_last[i + 1] == 255) Zhangmude[2] = 1;
					else  Zhangmude[2] = 0;
					if (data[i + 1] == 255) Zhangmude[3] = 1;
					else  Zhangmude[3] = 0;
					if (data_next[i + 1] == 255) Zhangmude[4] = 1;
					else  Zhangmude[4] = 0;
					if (data_next[i] == 255) Zhangmude[5] = 1;
					else  Zhangmude[5] = 0;
					if (data_next[i - 1] == 255) Zhangmude[6] = 1;
					else  Zhangmude[6] = 0;
					if (data[i - 1] == 255) Zhangmude[7] = 1;
					else  Zhangmude[7] = 0;
					if (data_last[i - 1] == 255) Zhangmude[8] = 1;
					else  Zhangmude[8] = 0;
					int whitepointtotal = 0;
					for (int k = 1; k < 9; k++)
					{
						whitepointtotal = whitepointtotal + Zhangmude[k];
					}
					if ((whitepointtotal >= 2) && (whitepointtotal <= 6))
					{
						int ap = 0;
						if ((Zhangmude[1] == 0) && (Zhangmude[2] == 1)) ap++;
						if ((Zhangmude[2] == 0) && (Zhangmude[3] == 1)) ap++;
						if ((Zhangmude[3] == 0) && (Zhangmude[4] == 1)) ap++;
						if ((Zhangmude[4] == 0) && (Zhangmude[5] == 1)) ap++;
						if ((Zhangmude[5] == 0) && (Zhangmude[6] == 1)) ap++;
						if ((Zhangmude[6] == 0) && (Zhangmude[7] == 1)) ap++;
						if ((Zhangmude[7] == 0) && (Zhangmude[8] == 1)) ap++;
						if ((Zhangmude[8] == 0) && (Zhangmude[1] == 1)) ap++;
						if (ap == 1)
						{
							if ((Zhangmude[1] * Zhangmude[3] * Zhangmude[5] == 0) && (Zhangmude[3] * Zhangmude[1] * Zhangmude[7] == 0))
							{
								deletelist1.push_back(Point(i, j));
							}
						}
					}
				}
			}
		}
		if (deletelist1.size() == 0) break;
		for (size_t i = 0; i < deletelist1.size(); i++)
		{
			Point tem;
			tem = deletelist1[i];
			uchar* data = srcimage.ptr<uchar>(tem.y);
			data[tem.x] = 0;
		}
		deletelist1.clear();
	}
}

#pragma endregion

#pragma region ͼ����ת����xuanzhuan()
//��һ����������ͼ�񣬵ڶ���������ת�Ƕȣ������Ĳ����ǲü���תͼ������ĳ�����,����ӦС��ԭ��
static Mat xuanzhuan(const Mat src,double angle,int a,int b)
{
	Point2f center(src.cols / 2, src.rows / 2);
	Mat rot = getRotationMatrix2D(center, angle, 1);
	Rect bbox = RotatedRect(center, src.size(), angle).boundingRect();

	rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
	rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;
	Mat dst;
	//��ת
	warpAffine(src, dst, rot, bbox.size());
	//�ü�
	dst = dst(Range(((dst.rows - a) / 2), ((dst.rows + a) / 2)), Range(((dst.cols - b) / 2), ((dst.cols + b) / 2)));
	
	return dst;
}

#pragma endregion

#pragma region ����߽���Сֵ������xxmin()

static int xxmin(Mat& inputImage)
{
	int xx_min = inputImage.cols;
	for (int i = 0; i < inputImage.rows; ++i)//��
	{
		for (int j = 0; j < inputImage.cols; ++j)//��
		{
			if (inputImage.at<uchar>(i, j))
			{
				if (j<xx_min)
				{
					xx_min = j;
				}
			}
		}
	}
	return xx_min;
}


#pragma endregion

#pragma region ����߽����ֵ������xxmax()

static int xxmax(Mat& inputImage)
{
	int xx_max = 0;
	for (int i = 0; i < inputImage.rows; ++i)//��
	{
		for (int j = 0; j < inputImage.cols; ++j)//��
		{
			if (inputImage.at<uchar>(i, j))
			{
				if (j>xx_max)
				{
					xx_max = j;
				}
			}
		}
	}
	return xx_max;
}


#pragma endregion

#pragma region ����߽���Сֵ������yymin()

static int yymin(Mat& inputImage)
{
	int yy_min = inputImage.rows;
	for (int i = 0; i < inputImage.rows; ++i)//��
	{
		for (int j = 0; j < inputImage.cols; ++j)//��
		{
			if (inputImage.at<uchar>(i, j))
			{
				if (i<yy_min)
				{
					yy_min = i;
				}
			}
		}
	}
	return yy_min;
}

#pragma endregion

#pragma region ����߽���Сֵ������yymax()

static int yymax(Mat& inputImage)
{
	int yy_max = 0;
	for (int i = 0; i < inputImage.rows; ++i)//��
	{
		for (int j = 0; j < inputImage.cols; ++j)//��
		{
			if (inputImage.at<uchar>(i, j))
			{
				if (i>yy_max)
				{
					yy_max = i;
				}
			}
		}
	}
	return yy_max;
}

#pragma endregion

#pragma region CLAHE�㷨claheGO()

static Mat claheGO(Mat src, int _step)//_stepС�ڵ���8
{
	//int _step = 5;//�˲����ɵ�
	Mat CLAHE_GO = src.clone();
	int block = _step;//pblock
	int width = src.cols;
	int height = src.rows;
	int width_block = width / block; //ÿ��С���ӵĳ��Ϳ�
	int height_block = height / block;
	//�洢����ֱ��ͼ  
	int tmp2[8 * 8][256] = { 0 };
	float C2[8 * 8][256] = { 0.0 };
	//�ֿ�
	int total = width_block * height_block;
	for (int i = 0; i < block; i++)
	{
		for (int j = 0; j < block; j++)
		{
			int start_x = i * width_block;
			int end_x = start_x + width_block;
			int start_y = j * height_block;
			int end_y = start_y + height_block;
			int num = i + block * j;
			//����С��,����ֱ��ͼ
			for (int ii = start_x; ii < end_x; ii++)
			{
				for (int jj = start_y; jj < end_y; jj++)
				{
					int index = src.at<uchar>(jj, ii);
					tmp2[num][index]++;
				}
			}
			//�ü������Ӳ�����Ҳ����clahe�е�cl����
			//����Ĳ��� ��Ӧ��Gem������ fCliplimit  = 4  , uiNrBins  = 255
			int average = width_block * height_block / 255;
			//���ڲ������ѡ����Ҫ�������ۡ���ͬ�Ľ����������
			//����ȫ�ֵ�ʱ����������cl����㣬��Ҫ�������� 
			int LIMIT = 40 * average;
			int steal = 0;
			for (int k = 0; k < 256; k++)
			{
				if (tmp2[num][k] > LIMIT) {
					steal += tmp2[num][k] - LIMIT;
					tmp2[num][k] = LIMIT;
				}
			}
			int bonus = steal / 256;
			//hand out the steals averagely  
			for (int k = 0; k < 256; k++)
			{
				tmp2[num][k] += bonus;
			}
			//�����ۻ��ֲ�ֱ��ͼ  
			for (int k = 0; k < 256; k++)
			{
				if (k == 0)
					C2[num][k] = 1.0f * tmp2[num][k] / total;
				else
					C2[num][k] = C2[num][k - 1] + 1.0f * tmp2[num][k] / total;
			}
		}
	}
	//����任�������ֵ  
	//�������ص��λ�ã�ѡ��ͬ�ļ��㷽��  
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			//four coners  
			if (i <= width_block / 2 && j <= height_block / 2)
			{
				int num = 0;
				CLAHE_GO.at<uchar>(j, i) = (int)(C2[num][CLAHE_GO.at<uchar>(j, i)] * 255);
			}
			else if (i <= width_block / 2 && j >= ((block - 1)*height_block + height_block / 2)) {
				int num = block * (block - 1);
				CLAHE_GO.at<uchar>(j, i) = (int)(C2[num][CLAHE_GO.at<uchar>(j, i)] * 255);
			}
			else if (i >= ((block - 1)*width_block + width_block / 2) && j <= height_block / 2) {
				int num = block - 1;
				CLAHE_GO.at<uchar>(j, i) = (int)(C2[num][CLAHE_GO.at<uchar>(j, i)] * 255);
			}
			else if (i >= ((block - 1)*width_block + width_block / 2) && j >= ((block - 1)*height_block + height_block / 2)) {
				int num = block * block - 1;
				CLAHE_GO.at<uchar>(j, i) = (int)(C2[num][CLAHE_GO.at<uchar>(j, i)] * 255);
			}
			//four edges except coners  
			else if (i <= width_block / 2)
			{
				//���Բ�ֵ  
				int num_i = 0;
				int num_j = (j - height_block / 2) / height_block;
				int num1 = num_j * block + num_i;
				int num2 = num1 + block;
				float p = (j - (num_j*height_block + height_block / 2)) / (1.0f*height_block);
				float q = 1 - p;
				CLAHE_GO.at<uchar>(j, i) = (int)((q*C2[num1][CLAHE_GO.at<uchar>(j, i)] + p * C2[num2][CLAHE_GO.at<uchar>(j, i)]) * 255);
			}
			else if (i >= ((block - 1)*width_block + width_block / 2)) {
				//���Բ�ֵ  
				int num_i = block - 1;
				int num_j = (j - height_block / 2) / height_block;
				int num1 = num_j * block + num_i;
				int num2 = num1 + block;
				float p = (j - (num_j*height_block + height_block / 2)) / (1.0f*height_block);
				float q = 1 - p;
				CLAHE_GO.at<uchar>(j, i) = (int)((q*C2[num1][CLAHE_GO.at<uchar>(j, i)] + p * C2[num2][CLAHE_GO.at<uchar>(j, i)]) * 255);
			}
			else if (j <= height_block / 2) {
				//���Բ�ֵ  
				int num_i = (i - width_block / 2) / width_block;
				int num_j = 0;
				int num1 = num_j * block + num_i;
				int num2 = num1 + 1;
				float p = (i - (num_i*width_block + width_block / 2)) / (1.0f*width_block);
				float q = 1 - p;
				CLAHE_GO.at<uchar>(j, i) = (int)((q*C2[num1][CLAHE_GO.at<uchar>(j, i)] + p * C2[num2][CLAHE_GO.at<uchar>(j, i)]) * 255);
			}
			else if (j >= ((block - 1)*height_block + height_block / 2)) {
				//���Բ�ֵ  
				int num_i = (i - width_block / 2) / width_block;
				int num_j = block - 1;
				int num1 = num_j * block + num_i;
				int num2 = num1 + 1;
				float p = (i - (num_i*width_block + width_block / 2)) / (1.0f*width_block);
				float q = 1 - p;
				CLAHE_GO.at<uchar>(j, i) = (int)((q*C2[num1][CLAHE_GO.at<uchar>(j, i)] + p * C2[num2][CLAHE_GO.at<uchar>(j, i)]) * 255);
			}
			//˫���Բ�ֵ
			else {
				int num_i = (i - width_block / 2) / width_block;
				int num_j = (j - height_block / 2) / height_block;
				int num1 = num_j * block + num_i;
				int num2 = num1 + 1;
				int num3 = num1 + block;
				int num4 = num2 + block;
				float u = (i - (num_i*width_block + width_block / 2)) / (1.0f*width_block);
				float v = (j - (num_j*height_block + height_block / 2)) / (1.0f*height_block);
				CLAHE_GO.at<uchar>(j, i) = (int)((u*v*C2[num4][CLAHE_GO.at<uchar>(j, i)] +
					(1 - v)*(1 - u)*C2[num1][CLAHE_GO.at<uchar>(j, i)] +
					u * (1 - v)*C2[num2][CLAHE_GO.at<uchar>(j, i)] +
					v * (1 - u)*C2[num3][CLAHE_GO.at<uchar>(j, i)]) * 255);
			}
			//����ⲽ�����Ƹ�˹ƽ��
			CLAHE_GO.at<uchar>(j, i) = CLAHE_GO.at<uchar>(j, i) + (CLAHE_GO.at<uchar>(j, i) << 8) + (CLAHE_GO.at<uchar>(j, i) << 16);
		}
	}
	return CLAHE_GO;
}

#pragma endregion