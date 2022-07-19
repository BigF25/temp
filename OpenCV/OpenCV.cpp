// OpenCV.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <windows.h>
#include <fstream>  
#include <string>  
using namespace cv;
using namespace std;
//-------------------------------------------------------------------------
//结构体
//-------------------------------------------------------------------------
struct Cube
{
	//UF UR UB UL DF DR DB DL FR FL BR BL UFR URB UBL ULF DRF DFL DLB DBR F R U B L D
	int CubeData[3][54];
	int Idx[54];
	int GetValueFlag[54];
};
//====================================================================函数声明
void RectInit(void);
string my_i2s(int i);
Cube KmeanCluster(Cube cube);
Cube CutOverNum(Cube cube);
Cube GetCubeData(Mat src0, Mat src1);
bool CubeCanSolve(Cube cube);
bool CubeIdxIs6_9(Cube cube);
void PrinfCube(Cube cube);
Cube Cluster(Cube cube);
bool IsPictureCaped(void);
#define CENTER_MOVE      true
#define CENTER_NO_MOVE   false
Cube CenterGrab(Cube cube, bool CenterMove);
//====================================================================全局变量
Rect rect[50];
Rect rect0[7];
Rect rect1[18];
Rect rect2[7];
Rect rect3[18];
Scalar scalar[50];
char str[6] = { 'F', 'R', 'U', 'B', 'L', 'D' };
//====================================================================标定
//标定量，光照强度、
//北科大光强数据
//double light[50] = {
//	428, 460, 452, 514, 430, 477, 463, 427, 470, 549, 542, 456, 420, 430, 489, 572, 561, 470, 428, 424, 475, 547, 532, 461, 411, 420, 461, 408, 451, 500, 419, 424, 394, 436, 481, 481, 437, 392, 408, 463, 512, 513, 469, 411, 409, 458, 509, 518, 463, 415
//};
//全套代码测试 全白色ps生成//可以用的
//double light[50] = {
//643,703,687,705,701,706,697,570,601,661,660,663,427,570,562,701,702,600,466,560,597,707,706,614,538,698,708,659,685,706,704,701,543,471,626,633,364,460,487,599,543,464,404,404,450,430,529,459,311,620, };

//double light[50] = {
//704,705,705,705,706,706,705,595,457,560,550,657,591,569,666,560,593,702,576,577,530,610,632,700,602,642,681,665,684,705,682,708,567,496,689,652,337,467,552,699,667,636,527,469,553,580,652,574,299,589, };

//11.29 3:00
double light[50] = {
690,704,703,705,697,705,697,590,583,672,705,649,538,565,675,695,705,669,524,560,609,688,705,636,553,625,663,652,666,705,667,709,603,602,631,653,638,647,618,651,668,696,675,673,634,657,643,709,703,689, };

double LightScale[54];  //光照强度比例
//=====================================================================宏定义
#define F  0
#define R  1
#define U  2
#define B  3
#define L  4
#define D  5
/**
*	@brief		opencv主函数入口
*/
int _tmain(int argc, _TCHAR* argv[])
{
	//处理光强数据
	double LightSum = 0;
	for (int i = 0; i < 50; i++)
	{
		LightSum += light[i];
	}
	double LightAverage = LightSum / 50.0;
	for (int i = 0; i < 50; i++)
	{
		//LightScale[i] = light[i] / LightAverage;
		LightScale[i] = 1;
	}
	//初始化采集位置
	RectInit();
	//计时
	//while (true)
	{
		cout << "等待图像...." << endl;
		while (!IsPictureCaped())  //等待问题被放入
		{
			Sleep(5);
		}
		double start = (double)cvGetTickCount();
		//读取图像
		Mat src00 = imread("../bridge/image/00.jpg");  //352*288
		Mat src01 = imread("../bridge/image/01.jpg");
		Mat src02 = imread("../bridge/image/02.jpg");
		Mat src03 = imread("../bridge/image/03.jpg");
		Mat src10 = imread("../bridge/image/10.jpg");  //352*288
		Mat src11 = imread("../bridge/image/11.jpg");
		Mat src12 = imread("../bridge/image/12.jpg");
		Mat src13 = imread("../bridge/image/13.jpg");
		//我写的调试代码
		{
			//////////////////////////////////显示图片代码
			//{
			//	imshow("a", src03);
			//	waitKey(0);
			//}
			///////////////////////////////显示像素信息代码
			//{
			//	for (int i = 0; i < src00.rows; i++) {
			//		for (int j = 0; j < src00.cols; j++) {
			//			int b = src00.at<Vec3b>(i, j)[0];
			//			int g = src00.at<Vec3b>(i, j)[1];
			//			int r = src00.at<Vec3b>(i, j)[2];
			//			cout << "(" << i << "," << j << "):" << b << "," << g << "," << r << ";";
			//		}
			//		cout << endl;
			//	}
			//}
			//////////////////////////////////修改像素颜色
			{
				//for (int i = 0; i < src00.rows; i++) {
				//	for (int j = 0; j < src00.cols; j++) {
				//		src00.at<Vec3b>(i, j) = Vec3b(255, 255, 255);//白色
				//		src01.at<Vec3b>(i, j) = Vec3b(255, 255, 255);//白色
				//		src02.at<Vec3b>(i, j) = Vec3b(255, 255, 255);//白色
				//		src03.at<Vec3b>(i, j) = Vec3b(255, 255, 255);//白色
				//		src10.at<Vec3b>(i, j) = Vec3b(255, 255, 255);//白色
				//		src11.at<Vec3b>(i, j) = Vec3b(255, 255, 255);//白色
				//		src12.at<Vec3b>(i, j) = Vec3b(255, 255, 255);//白色
				//		src13.at<Vec3b>(i, j) = Vec3b(255, 255, 255);//白色
				//	}
				//}
				////00.bmp
				//for (int k = 0; k < 7; k++) {
				//	for (int i = rect0[k].y; i < rect0[k].y + rect0[k].height; i++) {
				//		for (int j = rect0[k].x; j < rect0[k].x + rect0[k].width; j++) {
				//			//src00.at<Vec3b>(i, j) = Vec3b(39, 127, 255);//橙色
				//			src00.at<Vec3b>(i, j) = Vec3b(0, 0, 0);//黑色
				//		}
				//	}
				//}
				////01.bmp
				//for (int k = 0; k < 18; k++) {
				//	for (int i = rect1[k].y; i < rect1[k].y + rect1[k].height; i++) {
				//		for (int j = rect1[k].x; j < rect1[k].x + rect1[k].width; j++) {
				//			if (k % 6 == 0 || k % 6 == 1 || k % 6 == 2)
				//				//src01.at<Vec3b>(i, j) = Vec3b(255, 0, 0);//蓝色
				//				src01.at<Vec3b>(i, j) = Vec3b(0, 0, 0);//黑色
				//			else
				//				//src01.at<Vec3b>(i, j) = Vec3b(0, 255, 255);//黄色
				//				src01.at<Vec3b>(i, j) = Vec3b(0, 0, 0);//黑色
				//		}
				//	}
				//}
				////02.bmp
				//for (int k = 0; k < 7; k++) {
				//	for (int i = rect2[k].y; i < rect2[k].y + rect2[k].height; i++) {
				//		for (int j = rect2[k].x; j < rect2[k].x + rect2[k].width; j++) {
				//			//src02.at<Vec3b>(i, j) = Vec3b(0, 0, 255);//红色
				//			src02.at<Vec3b>(i, j) = Vec3b(0, 0, 0);//黑色
				//		}
				//	}
				//}
				////03.bmp
				//for (int k = 0; k < 18; k++) {
				//	for (int i = rect3[k].y; i < rect3[k].y + rect3[k].height; i++) {
				//		for (int j = rect3[k].x; j < rect3[k].x + rect3[k].width; j++) {
				//			if (k % 6 == 0 || k % 6 == 1 || k % 6 == 2)
				//				//src03.at<Vec3b>(i, j) = Vec3b(0, 255, 0);//绿色
				//				src03.at<Vec3b>(i, j) = Vec3b(0, 0, 0);//黑色
				//			else
				//				//src03.at<Vec3b>(i, j) = Vec3b(255, 255, 255);//白色
				//				src03.at<Vec3b>(i, j) = Vec3b(0, 0, 0);//黑色
				//		}
				//	}
				//}
				////10.bmp
				//for (int k = 0; k < 7; k++) {
				//	for (int i = rect0[k].y; i < rect0[k].y + rect0[k].height; i++) {
				//		for (int j = rect0[k].x; j < rect0[k].x + rect0[k].width; j++) {
				//			//src10.at<Vec3b>(i, j) = Vec3b(255, 0, 0);//蓝色
				//			src10.at<Vec3b>(i, j) = Vec3b(0, 0, 0);//黑色
				//		}
				//	}
				//}
				////11.bmp
				//for (int k = 0; k < 18; k++) {
				//	for (int i = rect1[k].y; i < rect1[k].y + rect1[k].height; i++) {
				//		for (int j = rect1[k].x; j < rect1[k].x + rect1[k].width; j++) {
				//			if (k % 6 == 0 || k % 6 == 1 || k % 6 == 2)
				//				//src11.at<Vec3b>(i, j) = Vec3b(0, 0, 255);//红色
				//				src11.at<Vec3b>(i, j) = Vec3b(0, 0, 0);//黑色
				//			else
				//				//src11.at<Vec3b>(i, j) = Vec3b(0, 255, 255);//黄色
				//				src11.at<Vec3b>(i, j) = Vec3b(0, 0, 0);//黑色
				//		}
				//	}
				//}
				////12.bmp
				//for (int k = 0; k < 7; k++) {
				//	for (int i = rect2[k].y; i < rect2[k].y + rect2[k].height; i++) {
				//		for (int j = rect2[k].x; j < rect2[k].x + rect2[k].width; j++) {
				//			//src12.at<Vec3b>(i, j) = Vec3b(0, 255, 0);//绿色
				//			src12.at<Vec3b>(i, j) = Vec3b(0, 0, 0);//黑色
				//		}
				//	}
				//}
				////13.bmp
				//for (int k = 0; k < 18; k++) {
				//	for (int i = rect3[k].y; i < rect3[k].y + rect3[k].height; i++) {
				//		for (int j = rect3[k].x; j < rect3[k].x + rect3[k].width; j++) {
				//			if (k % 6 == 0 || k % 6 == 1 || k % 6 == 2)
				//				//src13.at<Vec3b>(i, j) = Vec3b(39, 127, 255);//橙色
				//				src13.at<Vec3b>(i, j) = Vec3b(0, 0, 0);//黑色
				//			else
				//				//src13.at<Vec3b>(i, j) = Vec3b(255, 255, 255);//白色
				//				src13.at<Vec3b>(i, j) = Vec3b(0, 0, 0);//黑色
				//		}
				//	}
				//}
				//imshow("src00", src00);
				//imshow("src01", src01);
				//imshow("src02", src02);
				//imshow("src03", src03);
				//imshow("src10", src10);
				//imshow("src11", src11);
				//imshow("src12", src12);
				//imshow("src13", src13);
				//imwrite("00.bmp", src00);
				//imwrite("01.bmp", src01);
				//imwrite("02.bmp", src02);
				//imwrite("03.bmp", src03);
				//imwrite("10.bmp", src10);
				//imwrite("11.bmp", src11);
				//imwrite("12.bmp", src12);
				//imwrite("13.bmp", src13);
				//waitKey(0);
			}
		}

		Sleep(100);

		Mat src0;
		src0.push_back(src00);
		src0.push_back(src01);
		src0.push_back(src02);
		src0.push_back(src03);
		Mat src1;
		src1.push_back(src10);
		src1.push_back(src11);
		src1.push_back(src12);
		src1.push_back(src13);
		//imwrite("33.bmp", src0);///////////////////////
		//imwrite("44.bmp", src1);///////////////////////
		//////获取魔方颜色信息
		Cube cube = GetCubeData(src0, src1);
		//////聚类
		cube = Cluster(cube);
		//////输出魔方状态信息
		cout << cube.CubeData << endl;
		PrinfCube(cube);
		//北科调试代码
		{
			////写光强
			//ofstream dout("../bridge/file/result.txt");
			//dout << "double light[50]={" << endl;
			//for (int i = 0; i < 50; i++)
			//{
			//	dout << (int)(scalar[i].val[0] + scalar[i].val[1] + scalar[i].val[2]) << ",";
			//}
			//dout << "};" << endl;
			//dout.close();
			//写数据
			//for (int i = 0; i < 3; i++)
			//{
			//	dout << "d" << i << "=[" << endl;
			//	for (int j = 0; j < 54; j++)
			//	{
			//		dout << cube.CubeData[i][j] << ";";
			//	}
			//	dout << "];" << endl;
			//}
			////写中心点
			//dout << "s=[" << endl;
			//for (int i = 48; i < 54; i++)
			//{
			//	for (int j = 0; j < 3; j++)
			//	{
			//		dout << cube.CubeData[j][i] << " ";
			//	}
			//	dout << ";" << endl;
			//}
			//dout << "];" << endl;
			//dout.close();
			//for (int i = 0; i < 50; i++)
			//{
			//	rectangle(src1, rect[i], Scalar(0, 0, 0), CV_FILLED);
			//}
			//namedWindow("src", CV_WINDOW_NORMAL);
			//imshow("src", src1);
			//waitKey(0);
		}
		double end = (double)cvGetTickCount();
		cout << "消耗时间:" << (end - start) / cvGetTickFrequency() / 1000 << endl;
	}
	return 0;
}
//namedWindow("src", CV_WINDOW_NORMAL);
//createTrackbar("x", "src", &x, 380, onTrackBar);
//createTrackbar("y", "src", &y, 380, onTrackBar);
//createTrackbar("dx", "src", &dx, 380, onTrackBar);
//createTrackbar("dy", "src", &dy, 380, onTrackBar);
//void onTrackBar(int, void*)
//{
//	Mat src0;
//	src0 = imread("03.bmp");
//	rectangle(src0, Rect(x,y,dx,dy), Scalar(0,0,0), CV_FILLED);
//	imshow("src", src0);
//}
//初始化信息采集位置
void RectInit(void)
{
	//北科大的原数据
	{
		////00.bmp
		//rect0[0] = Rect(49, 29, 25, 25);
		//rect0[1] = Rect(275, 27, 25, 25);
		//rect0[2] = Rect(41, 142, 25, 25);
		//rect0[3] = Rect(160, 142, 25, 25);
		//rect0[4] = Rect(49, 246, 25, 25);
		//rect0[5] = Rect(160, 246, 25, 25);
		//rect0[6] = Rect(277, 242, 25, 25);
		////01.bmp
		//rect1[0] = Rect(55, 64, 12, 21);
		//rect1[1] = Rect(88, 57, 19, 21);
		//rect1[2] = Rect(148, 25, 25, 25);
		//rect1[3] = Rect(214, 25, 25, 25);
		//rect1[4] = Rect(271, 55, 19, 21);
		//rect1[5] = Rect(308, 64, 12, 21);
		//rect1[6] = Rect(51, 131, 12, 25);
		//rect1[7] = Rect(88, 129, 19, 25);
		//rect1[8] = Rect(144, 120, 21, 25);
		//rect1[9] = Rect(215, 125, 25, 25);
		//rect1[10] = Rect(271, 125, 20, 25);
		//rect1[11] = Rect(310, 131, 12, 25);
		//rect1[12] = Rect(53, 199, 12, 21);
		//rect1[13] = Rect(86, 207, 19, 19);
		//rect1[14] = Rect(140, 230, 25, 35);
		//rect1[15] = Rect(212, 230, 25, 35);
		//rect1[16] = Rect(267, 207, 19, 19);
		//rect1[17] = Rect(308, 199, 12, 19);
		////02.bmp
		//rect2[0] = Rect(43, 19, 25, 25);
		//rect2[1] = Rect(166, 19, 25, 25);
		//rect2[2] = Rect(290, 21, 25, 25);
		//rect2[3] = Rect(29, 138, 25, 25);
		//rect2[4] = Rect(166, 148, 21, 21);
		//rect2[5] = Rect(41, 249, 21, 21);
		//rect2[6] = Rect(294, 249, 21, 21);
		////03.bmp
		//rect3[0] = Rect(68 - 15, 70, 12, 25);
		//rect3[1] = Rect(103 - 15, 53, 16, 25);
		//rect3[2] = Rect(152 - 15, 35, 19, 25);
		//rect3[3] = Rect(212 - 15, 35, 19, 25);
		//rect3[4] = Rect(261 - 15, 51, 16, 25);
		//rect3[5] = Rect(298 - 15, 68, 12, 25);
		//rect3[6] = Rect(66 - 15, 140, 12, 25);
		//rect3[7] = Rect(101 - 15, 134, 16, 25);
		//rect3[8] = Rect(152 - 15, 125, 19, 25);
		//rect3[9] = Rect(214 - 15, 125, 19, 25);
		//rect3[10] = Rect(263 - 15, 131, 16, 25);
		//rect3[11] = Rect(302 - 15, 136, 12, 25);
		//rect3[12] = Rect(68 - 15, 210, 12, 25);
		//rect3[13] = Rect(103 - 15, 216, 16, 25);
		//rect3[14] = Rect(152 - 15, 222, 19, 25);
		//rect3[15] = Rect(214 - 15, 218, 19, 25);
		//rect3[16] = Rect(265 - 15, 212, 16, 25);
		//rect3[17] = Rect(302 - 15, 205, 12, 25);
	}
	////2-----
	{
		////00.bmp
		//rect0[0] = Rect(58, 131, 25, 25);//32
		//rect0[1] = Rect(163, 23, 25, 25);//34
		//rect0[2] = Rect(110, 188, 25, 25);//23
		//rect0[3] = Rect(161, 133, 25, 25);//52
		//rect0[4] = Rect(163, 243, 25, 25);//43
		//rect0[5] = Rect(216, 183, 25, 25);//15
		//rect0[6] = Rect(276, 129, 25, 25);//41
		////01.bmp

		//rect1[0] = Rect(273, 198, 12, 25);//27
		//rect1[1] = Rect(235, 212, 16, 25);//2
		//rect1[2] = Rect(176, 212, 19, 25);//24
		//rect1[3] = Rect(122, 222, 19, 25);//25
		//rect1[4] = Rect(73, 196, 16, 25);//16
		//rect1[5] = Rect(41, 192, 12, 25);//38
		//rect1[6] = Rect(274, 124, 12, 25);//4
		//rect1[7] = Rect(233, 131, 16, 25);//50
		//rect1[8] = Rect(186, 125, 19, 25);//0
		//rect1[9] = Rect(122, 125, 19, 25);//1
		//rect1[10] = Rect(71, 134, 16, 25);//48
		//rect1[11] = Rect(41, 125, 12, 25);//9
		//rect1[12] = Rect(276, 50, 12, 25);//30
		//rect1[13] = Rect(231, 51, 16, 25);//6
		//rect1[14] = Rect(182, 26, 19, 25);//33
		//rect1[15] = Rect(122, 35, 19, 25);//35
		//rect1[16] = Rect(73, 53, 16, 25);//18
		//rect1[17] = Rect(39, 53, 12, 25);//40
		////02.bmp
		//rect2[0] = Rect(162, 39, 25, 25);//47
		//rect2[1] = Rect(215, 88, 25, 25);//11
		//rect2[2] = Rect(264, 138, 21, 21);//37
		//rect2[3] = Rect(116, 90, 25, 25);//21
		//rect2[4] = Rect(170, 140, 21, 21);//49
		//rect2[5] = Rect(69, 145, 25, 25);//28
		//rect2[6] = Rect(167, 247, 21, 21);//26
		////03.bmp
		//rect3[0] = Rect(302, 197, 12, 19);//36
		//rect3[1] = Rect(262, 202, 19, 19);//10
		//rect3[2] = Rect(205, 214, 25, 35);//45
		//rect3[3] = Rect(144, 213, 25, 35);//46
		//rect3[4] = Rect(96, 199, 19, 19);//20
		//rect3[5] = Rect(59, 199, 12, 21);//29
		//rect3[6] = Rect(299, 124, 12, 25);//8
		//rect3[7] = Rect(263, 120, 20, 25);//53
		//rect3[8] = Rect(208, 116, 25, 25);//12
		//rect3[9] = Rect(148, 120, 21, 25);//13
		//rect3[10] = Rect(97, 124, 19, 25);//51
		//rect3[11] = Rect(56, 123, 12, 25);//5
		//rect3[12] = Rect(305, 46, 12, 21);//39
		//rect3[13] = Rect(261, 35, 19, 21);//14
		//rect3[14] = Rect(208, 25, 25, 25);//42
		//rect3[15] = Rect(147, 25, 25, 25);//44
		//rect3[16] = Rect(95, 38, 19, 21);//22
		//rect3[17] = Rect(59, 50, 12, 21);//31
	}
	////3------测试成功的
	{
		////00.bmp
		//rect0[0] = Rect(276, 129, 25, 25);//32
		//rect0[1] = Rect(163, 23, 25, 25);//34
		//rect0[2] = Rect(216, 183, 25, 25);//23
		//rect0[3] = Rect(161, 133, 25, 25);//52
		//rect0[4] = Rect(163, 243, 25, 25);//43
		//rect0[5] = Rect(110, 188, 25, 25);//15
		//rect0[6] = Rect(58, 131, 25, 25);//41
		////01.bmp
		//rect1[0] = Rect(41, 192, 12, 25);//27
		//rect1[1] = Rect(73, 196, 16, 25);//2
		//rect1[2] = Rect(122, 222, 19, 25);//24
		//rect1[3] = Rect(176, 212, 19, 25);//25
		//rect1[4] = Rect(235, 212, 16, 25);//16
		//rect1[5] = Rect(273, 198, 12, 25);//38
		//rect1[6] = Rect(41, 125, 12, 25);//4
		//rect1[7] = Rect(71, 134, 16, 25);//50
		//rect1[8] = Rect(122, 125, 19, 25);//0
		//rect1[9] = Rect(186, 125, 19, 25);//1
		//rect1[10] = Rect(233, 131, 16, 25);//48
		//rect1[11] = Rect(274, 124, 12, 25);//9
		//rect1[12] = Rect(39, 53, 12, 25);//30
		//rect1[13] = Rect(73, 53, 16, 25);//6
		//rect1[14] = Rect(122, 35, 19, 25);//33
		//rect1[15] = Rect(182, 26, 19, 25);//35
		//rect1[16] = Rect(231, 51, 16, 25);//18
		//rect1[17] = Rect(276, 50, 12, 25);//40
		////02.bmp
		//rect2[0] = Rect(162, 39, 25, 25);//47
		//rect2[1] = Rect(116, 90, 25, 25);//11
		//rect2[2] = Rect(69, 145, 25, 25);//37
		//rect2[3] = Rect(215, 88, 25, 25);//21
		//rect2[4] = Rect(170, 140, 21, 21);//49
		//rect2[5] = Rect(264, 138, 21, 21);//28
		//rect2[6] = Rect(167, 247, 21, 21);//26
		////03.bmp
		//rect3[0] = Rect(59, 199, 12, 21);//36
		//rect3[1] = Rect(96, 199, 19, 19);//10
		//rect3[2] = Rect(144, 213, 25, 35);//45
		//rect3[3] = Rect(205, 214, 25, 35);//46
		//rect3[4] = Rect(262, 202, 19, 19);//20
		//rect3[5] = Rect(302, 197, 12, 19);//29
		//rect3[6] = Rect(56, 123, 12, 25);//8
		//rect3[7] = Rect(97, 124, 19, 25);//53
		//rect3[8] = Rect(148, 120, 21, 25);//12
		//rect3[9] = Rect(208, 116, 25, 25);//13
		//rect3[10] = Rect(263, 120, 20, 25);//51
		//rect3[11] = Rect(299, 124, 12, 25);//5
		//rect3[12] = Rect(59, 50, 12, 21);//39
		//rect3[13] = Rect(95, 38, 19, 21);//14
		//rect3[14] = Rect(147, 25, 25, 25);//42
		//rect3[15] = Rect(208, 25, 25, 25);//44
		//rect3[16] = Rect(261, 35, 19, 21);//22
		//rect3[17] = Rect(305, 46, 12, 21);//31
	}
	////第一次测试全套代码
	{
		//rect0[6] = Rect(50, 130, 25, 25);//41
		//rect0[5] = Rect(100, 180, 25, 25);//15

		//rect0[1] = Rect(154, 23, 25, 25);//34
		//rect0[3] = Rect(154, 133, 25, 25);//52
		//rect0[4] = Rect(154, 235, 25, 25);//43

		//rect0[2] = Rect(200, 180, 25, 25);//23
		//rect0[0] = Rect(255, 130, 25, 25);//32

		////01.bmp
		//rect1[12] = Rect(55, 75, 10, 25);//30
		//rect1[6] = Rect(55, 140, 10, 25);//4
		//rect1[0] = Rect(55, 205, 10, 25);//27

		//rect1[13] = Rect(95, 53, 15, 20);//6
		//rect1[7] = Rect(95, 140, 15, 25);//50
		//rect1[1] = Rect(95, 223, 15, 20);//2

		//rect1[14] = Rect(140, 45, 20, 25);//33
		//rect1[8] = Rect(140, 135, 20, 25);//0
		//rect1[2] = Rect(140, 225, 20, 25);//24
		//rect1[15] = Rect(205, 40, 20, 25);//35
		//rect1[9] = Rect(205, 133, 20, 25);//1
		//rect1[3] = Rect(205, 220, 20, 25);//25

		//rect1[16] = Rect(255, 55, 15, 20);//18
		//rect1[10] = Rect(255, 133, 15, 25);//48
		//rect1[4] = Rect(255, 212, 15, 20);//16

		//rect1[17] = Rect(296, 60, 10, 25);//40
		//rect1[11] = Rect(296, 135, 10, 25);//9
		//rect1[5] = Rect(296, 210, 10, 25);//38
		////02.bmp
		//rect2[2] = Rect(60, 150, 25, 25);//37
		//rect2[1] = Rect(110, 95, 25, 25);//11

		//rect2[0] = Rect(157, 40, 25, 25);//47
		//rect2[4] = Rect(160, 148, 25, 25);//49
		//rect2[6] = Rect(163, 255, 25, 25);//26

		//rect2[3] = Rect(208, 88, 25, 25);//21
		//rect2[5] = Rect(254, 140, 25, 25);//28
		////03.bmp
		//rect3[12] = Rect(60, 63, 10, 25);//39
		//rect3[6] = Rect(60, 135, 10, 25);//8
		//rect3[0] = Rect(60, 210, 10, 25);//36
		//rect3[13] = Rect(99, 53, 15, 25);//14
		//rect3[7] = Rect(99, 134, 15, 25);//53
		//rect3[1] = Rect(99, 218, 15, 25);//10

		//rect3[14] = Rect(146, 44, 20, 25);//42
		//rect3[8] = Rect(146, 137, 20, 25);//12
		//rect3[2] = Rect(146, 230, 20, 25);//45
		//rect3[15] = Rect(208, 44, 20, 25);//44
		//rect3[9] = Rect(208, 137, 20, 25);//13
		//rect3[3] = Rect(208, 230, 20, 25);//46

		//rect3[16] = Rect(264, 53, 15, 25);//22
		//rect3[10] = Rect(264, 139, 15, 25);//51
		//rect3[4] = Rect(264, 223, 15, 25);//20
		//rect3[17] = Rect(303, 60, 10, 25);//31
		//rect3[11] = Rect(303, 133, 10, 25);//5
		//rect3[5] = Rect(303, 207, 10, 25);//29
	}

	rect0[0] = Rect(356, 65, 34, 38);
	rect0[1] = Rect(215, 220, 33, 37);
	rect0[2] = Rect(425, 139, 41, 43);
	rect0[3] = Rect(355, 215, 36, 40);
	rect0[4] = Rect(495, 211, 36, 37);
	rect0[5] = Rect(435, 284, 33, 42);
	rect0[6] = Rect(362, 356, 37, 36);

	rect1[0] = Rect(280, 116, 25, 17);
	rect1[1] = Rect(284, 154, 17, 22);
	rect1[2] = Rect(262, 200, 32, 27);
	rect1[3] = Rect(263, 252, 35, 26);
	rect1[4] = Rect(281, 296, 21, 15);
	rect1[5] = Rect(288, 329, 19, 11);
	rect1[6] = Rect(340, 114, 23, 17);
	rect1[7] = Rect(344, 154, 21, 19);
	rect1[8] = Rect(343, 197, 26, 27);
	rect1[9] = Rect(340, 254, 27, 19);
	rect1[10] = Rect(340, 298, 21, 13);
	rect1[11] = Rect(342, 329, 21, 9);
	rect1[12] = Rect(403, 118, 18, 15);
	rect1[13] = Rect(402, 153, 15, 20);
	rect1[14] = Rect(409, 196, 34, 27);
	rect1[15] = Rect(413, 252, 30, 22);
	rect1[16] = Rect(401, 295, 16, 20);
	rect1[17] = Rect(401, 327, 13, 11);

	rect2[0] = Rect(141, 229, 42, 37);
	rect2[1] = Rect(215, 307, 40, 39);
	rect2[2] = Rect(283, 386, 37, 41);
	rect2[3] = Rect(219, 153, 40, 41);
	rect2[4] = Rect(296, 237, 38, 41);
	rect2[5] = Rect(302, 84, 41, 45);
	rect2[6] = Rect(440, 252, 43, 41);

	rect3[0] = Rect(443, 384, 35, 16);
	rect3[1] = Rect(462, 335, 34, 23);
	rect3[2] = Rect(497, 260, 44, 32);
	rect3[3] = Rect(478, 154, 51, 45);
	rect3[4] = Rect(450, 88, 38, 27);
	rect3[5] = Rect(431, 51, 39, 18);
	rect3[6] = Rect(345, 381, 33, 20);
	rect3[7] = Rect(348, 332, 38, 29);
	rect3[8] = Rect(347, 255, 54, 39);
	rect3[9] = Rect(349, 159, 48, 43);
	rect3[10] = Rect(346, 91, 35, 28);
	rect3[11] = Rect(347, 50, 32, 17);
	rect3[12] = Rect(250, 380, 34, 15);
	rect3[13] = Rect(235, 334, 37, 22);
	rect3[14] = Rect(208, 260, 51, 39);
	rect3[15] = Rect(211, 167, 48, 36);
	rect3[16] = Rect(233, 95, 38, 24);
	rect3[17] = Rect(250, 50, 36, 18);
	for (int i = 0; i < 7; i++)//7个
	{
		rect[i] = rect0[i];
	}
	for (int i = 7; i < 25; i++)//18个
	{
		rect[i] = rect1[i - 7];
		rect[i].y += 480;
	}
	for (int i = 25; i < 32; i++)//7个
	{
		rect[i] = rect2[i - 25];
		rect[i].y += 480 * 2;
	}
	for (int i = 32; i < 50; i++)//18个
	{
		rect[i] = rect3[i - 32];
		rect[i].y += 480 * 3;
	}
}
//int转string,限二位数以下
string my_i2s(int i)
{
	string str = "00";
	str[0] = i / 10 + 0x30;
	str[1] = i % 10 + 0x30;
	return str;
}
//获取魔方状态信息
Cube GetCubeData(Mat src0, Mat src1)
{
	//标定量，投影方向
	double B1 = 46;
	double B2 = 136;
	////处理旋转矩阵
	double cB1 = cos(B1*3.141592654 / 180.0);
	double cB2 = cos(B2*3.141592654 / 180.0);
	double sB1 = sin(B1*3.141592654 / 180.0);
	double sB2 = sin(B2*3.141592654 / 180.0);
	double Rx[3][3] = {
		{ 1.0, 0.0, 0.0 },
		{ 0.0, cB1, -sB1 },
		{ 0.0, sB1, cB1 }
	};
	double Ry[3][3] = {
		{ cB2, 0.0, sB2 },
		{ 0.0, 1.0, 0.0 },
		{ -sB2, 0.0, cB2 }
	};
	//采集数据src0
	int data0[3][50];
	for (int i = 0; i < 50; i++)
	{
		Mat ROI = src0(rect[i]);////创建ROI对象 往ROI对象中存入图片选定区域的像素点颜色信心
		scalar[i] = mean(ROI);////通过mean函数获取ROI各通道的颜色颜色信息

		double X = scalar[i].val[0] / LightScale[i];////各通道信息加成光亮信息
		double Y = scalar[i].val[1] / LightScale[i];////各通道信息加成光亮信息
		double Z = scalar[i].val[2] / LightScale[i];////各通道信息加成光亮信息
		//cout << X << endl;
		//======================================================================坐标变换
		//x轴旋转
		double temp1 = Rx[0][0] * X + Rx[0][1] * Y + Rx[0][2] * Z;
		double temp2 = Rx[1][0] * X + Rx[1][1] * Y + Rx[1][2] * Z;
		double temp3 = Rx[2][0] * X + Rx[2][1] * Y + Rx[2][2] * Z;
		//y轴旋转
		data0[0][i] = (int)(Ry[0][0] * temp1 + Ry[0][1] * temp2 + Ry[0][2] * temp3);
		data0[1][i] = (int)(Ry[1][0] * temp1 + Ry[1][1] * temp2 + Ry[1][2] * temp3);
		data0[2][i] = (int)(Ry[2][0] * temp1 + Ry[2][1] * temp2 + Ry[2][2] * temp3);
		//z轴压缩
		data0[2][i] /= 2;
	}
	//采集数据src1
	int data1[3][50];
	for (int i = 0; i < 50; i++)
	{
		Mat ROI = src1(rect[i]);
		scalar[i] = mean(ROI);

		double X = scalar[i].val[0] / LightScale[i];
		double Y = scalar[i].val[1] / LightScale[i];
		double Z = scalar[i].val[2] / LightScale[i];
		//======================================================================坐标变换
		//x轴旋转
		double temp1 = Rx[0][0] * X + Rx[0][1] * Y + Rx[0][2] * Z;
		double temp2 = Rx[1][0] * X + Rx[1][1] * Y + Rx[1][2] * Z;
		double temp3 = Rx[2][0] * X + Rx[2][1] * Y + Rx[2][2] * Z;
		//y轴旋转
		data1[0][i] = (int)(Ry[0][0] * temp1 + Ry[0][1] * temp2 + Ry[0][2] * temp3);
		data1[1][i] = (int)(Ry[1][0] * temp1 + Ry[1][1] * temp2 + Ry[1][2] * temp3);
		data1[2][i] = (int)(Ry[2][0] * temp1 + Ry[2][1] * temp2 + Ry[2][2] * temp3);
		//z轴压缩
		data1[2][i] /= 2;
	}
	//建立映射
	int map0[50] = { 32,34,23,52,43,15,41,27,2, 24,25,16,38,4, 50,0, 1, 48,9, 30,6,33,35,18,40,47,11,37,21,49,28,26,36,10,45,46,20,29,8, 53,12,13,51,5, 39,14,42,44,22,31 };
	int map1[50] = { 27,24,4, 50,30,6, 33,47,11,37,38,9, 40,21,49,17,16,48,18,28,3,26,25,1, 35,42,14,39,12,53,45,36,41,15,43,44,13,46,19,52,23,22,51,20,34,7, 32,31,5, 29 };
	//int map0[50] = {
	//	34,41,7,52,32,23,43,
	//	25,16,38,36,10,45,1,48,9,8,53,12,35,18,40,39,14,42,
	//	28,21,47,3,49,26,37,
	//	46,20,29,27,2,24,13,51,5,4,50,0,44,22,31,30,6,33
	//};	
	//int map1[50] = { 
	//	24,33,2,50,27,4,30,
	//	38,9,40,41,15,43,16,48,18,19,52,23,25,1,35,34,7,32,
	//	45,12,42,10,53,36,39,
	//	44,13,40,47,11,37,22,51,20,21,49,17,31,5,29,28,3,26
	//};


	//cube 初始化
	Cube cube;
	for (int i = 0; i < 54; i++)
	{
		cube.GetValueFlag[i] = 0;
		cube.Idx[i] = -1;
		cube.CubeData[0][i] = 0;
		cube.CubeData[1][i] = 0;
		cube.CubeData[2][i] = 0;
	}
	//cube赋值
	for (int i = 0; i < 50; i++)
	{
		cube.GetValueFlag[map0[i]]++;
		for (int j = 0; j < 3; j++)
		{
			cube.CubeData[j][map0[i]] = (cube.CubeData[j][map0[i]] * (cube.GetValueFlag[map0[i]] - 1) + data0[j][i]) / cube.GetValueFlag[map0[i]];
		}
	}
	for (int i = 0; i < 50; i++)
	{
		cube.GetValueFlag[map1[i]]++;
		for (int j = 0; j < 3; j++)
		{
			cube.CubeData[j][map1[i]] = (cube.CubeData[j][map1[i]] * (cube.GetValueFlag[map1[i]] - 1) + data1[j][i]) / cube.GetValueFlag[map1[i]];
		}
	}
	return cube;
}

//@breaf：判断一个魔方是否可解
/*
#define F  0
#define R  1
#define U  2
#define B  3
#define L  4
#define D  5
*/
//输入格式：UF UR UB UL DF DR DB DL FR FL BR BL UFR URB UBL ULF DRF DFL DLB DBR F R U B L D
bool CubeCanSolve(Cube cube)
{
	//1,判断中心块是否错误、重复
	if ((cube.Idx[48 + F] != F) || (cube.Idx[48 + R] != R) || (cube.Idx[48 + U] != U) || (cube.Idx[48 + B] != B) || (cube.Idx[48 + L] != L) || (cube.Idx[48 + D] != D))
	{
		return false;
	}
	//2,判断每个边块的两个颜色是否相同
	for (int i = 0; i < 12; i++)
	{
		int start = i * 2;
		if (cube.Idx[start] % 3 == cube.Idx[start + 1] % 3) return false;
	}
	//3,判断每个角块的三个颜色是否相同
	for (int i = 0; i < 8; i++)
	{
		int start = 24 + i * 3;
		if ((cube.Idx[start] % 3 == cube.Idx[start + 1] % 3) || (cube.Idx[start] % 3 == cube.Idx[start + 2] % 3) || (cube.Idx[start + 1] % 3 == cube.Idx[start + 2] % 3))return false;
	}
	//4,判断边块色向是否正确
	int ColorSum = 0;
	for (int i = 0; i < 12; i++)
	{
		int start = i * 2;
		int map[3] = { 1, 0, 2 };

		if (map[cube.Idx[start] % 3] < map[cube.Idx[start + 1] % 3])ColorSum++;
	}
	if (ColorSum % 2 != 0)return false;
	//5,判断角块色向是否正确
	ColorSum = 0;
	for (int i = 0; i < 8; i++)
	{
		int start = 24 + i * 3;
		if
			(cube.Idx[start] % 3 == 2)ColorSum += 0;
		else if
			(cube.Idx[start + 1] % 3 == 2)ColorSum += 1;
		else if
			(cube.Idx[start + 2] % 3 == 2)ColorSum += 2;
	}
	if (ColorSum % 3 != 0)return false;
	//6，判断色片位置
	//抱歉写不出来。。。
	return true;
}
//判断魔方的Idx是否为6个9
bool CubeIdxIs6_9(Cube cube)
{
	int count[6] = { 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < 54; i++)
	{
		count[cube.Idx[i]]++;
	}
	for (int i = 0; i < 6; i++)
	{
		if (count[i] != 9)
		{
			return false;
		}
	}
	return true;
}
//输出魔方状态
void PrinfCube(Cube cube)
{
	ofstream dout("../bridge/file/CubeIdxData.txt");/////////////////////////////////////////////
	for (int i = 0; i < 48; i++)
	{
		dout << str[cube.Idx[i]];
	}
	dout.close();
	ofstream eout("../bridge/file/CubeIdxDataFlag.txt");///////////////////////////////////////////
	eout.close();

}
//kmean聚类
Cube KmeanCluster(Cube cube)
{
	//初始化索引
	for (int i = 0; i < 54; i++)
	{
		cube.Idx[i] = -1;
	}
	for (int i = 0; i < 6; i++)
	{
		cube.Idx[48 + i] = i;
	}
	//初始化聚类中心
	int Center[3][6];
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Center[j][i] = cube.CubeData[j][48 + i];
		}
	}
	while (true)
	{
		//保留当前索引
		int preIdx[54];
		for (int i = 0; i < 54; i++)
		{
			preIdx[i] = cube.Idx[i];
		}
		//对48个色块分类
		for (int i = 0; i < 48; i++)
		{
			double minDis = 1e8;
			int idx = -1;
			for (int c = 0; c < 6; c++)
			{
				double Dis =
					(cube.CubeData[0][i] - Center[0][c])*(cube.CubeData[0][i] - Center[0][c]) +
					(cube.CubeData[1][i] - Center[1][c])*(cube.CubeData[1][i] - Center[1][c]) +
					(cube.CubeData[2][i] - Center[2][c])*(cube.CubeData[2][i] - Center[2][c]);
				if (Dis < minDis)
				{
					minDis = Dis;
					idx = c;
				}
			}
			cube.Idx[i] = idx;
		}
		//写数据
		ofstream dout("result.txt");

		for (int j = 0; j < 54; j++)
		{
			dout << cube.Idx[j] << ";";
		}
		dout << "];" << endl;
		dout.close();
		//检验Idx是否改变
		bool IdxIsChanged = false;
		for (int i = 0; i < 54; i++)
		{
			if (preIdx[i] != cube.Idx[i])IdxIsChanged = true;  //一旦有任何一个索引与之前不同，则索引被改变
		}
		if (IdxIsChanged == false)   //分类结果已经稳定
		{
			return cube;
		}
		//重新分配聚类中心
		int count[6] = { 0, 0, 0, 0, 0, 0 };
		for (int i = 0; i < 54; i++)
		{
			int Class = cube.Idx[i];   //获取这个色片为哪一类
			if (Class != -1)
			{
				count[Class]++;
				for (int j = 0; j < 3; j++)
				{
					Center[j][Class] = (Center[j][Class] * (count[Class] - 1) + cube.CubeData[j][i]) / count[Class];
				}
			}
		}
	}
	return cube;
}
//数字平推
Cube CutOverNum(Cube cube)
{
	//初始化聚类中心
	int Center[3][6];
	int count1[6] = { 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < 54; i++)
	{
		int Class = cube.Idx[i];   //获取这个色片为哪一类
		count1[Class]++;
		for (int j = 0; j < 3; j++)
		{
			Center[j][Class] = (Center[j][Class] * (count1[Class] - 1) + cube.CubeData[j][i]) / count1[Class];
		}
	}
	//计算每个点与聚类中心的距离
	int Dis[54];
	for (int i = 0; i < 54; i++)
	{
		Dis[i] = 0;
		for (int j = 0; j < 3; j++)
		{
			Dis[i] += (cube.CubeData[j][i] - Center[j][cube.Idx[i]])*(cube.CubeData[j][i] - Center[j][cube.Idx[i]]);
		}
	}
	//标记每个点，在其所在类距聚类中心的距离排名
	int Level[54];
	for (int i = 0; i < 54; i++)
	{
		Level[i] = 0;
		for (int j = 0; j < 54; j++)//对任意两个点进行比较
		{
			if (cube.Idx[i] == cube.Idx[j])//若两个点为同一类
			{
				if (Dis[i] < Dis[j])
				{
					Level[i]++;  //记录比自己大的数有几个,有0个，则说明自己最大
				}
			}
		}
	}
	//取消排名大于等于9的点数的分类
	for (int i = 0; i < 54; i++)
	{
		if (Level[i] >= 9)
		{
			cube.Idx[i] = -1;
		}
	}
	//重新确定聚类中心
	int count2[6] = { 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < 54; i++)
	{
		int Class = cube.Idx[i];   //获取这个色片为哪一类
		if (Class != -1)
		{
			count2[Class]++;
			for (int j = 0; j < 3; j++)
			{
				Center[j][Class] = (Center[j][Class] * (count2[Class] - 1) + cube.CubeData[j][i]) / count2[Class];
			}
		}
	}
	//对被取消分类的点进行重新分类
	for (int i = 0; i < 54; i++)
	{
		if (cube.Idx[i] == -1)  //取消了分类的点
		{
			int minDis = 1000000;
			int c = -1;
			for (int j = 0; j < 6; j++)
			{
				if (count2[j] < 9)
				{
					int dis = 0;
					for (int k = 0; k < 3; k++)
					{
						dis += (cube.CubeData[k][i] - Center[k][j])*(cube.CubeData[k][i] - Center[k][j]);
					}
					if (dis < minDis)
					{
						minDis = dis;
						c = j;
					}
				}
			}
			cube.Idx[i] = c;
			count2[c]++;
		}
	}
	return cube;
}
//中心点抢占
Cube CenterGrab(Cube cube, bool CenterMove)
{
	//清空Idx
	for (int i = 0; i < 54; i++)
	{
		cube.Idx[i] = -1;
	}
	//初始化聚类中心
	int Center[3][6];
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Center[j][i] = cube.CubeData[j][48 + i];
		}
	}
	//逐次抢占9个点
	for (int i = 0; i < 9; i++)
	{
		//每一类抢占一个点
		for (int c = 0; c < 6; c++)
		{
			int minDis = 1000000;
			int p = -1;
			for (int j = 0; j < 54; j++)
			{
				if (cube.Idx[j] == -1)  //考虑尚未分类的点
				{
					int dis = 0;
					for (int k = 0; k < 3; k++)
					{
						dis += (cube.CubeData[k][j] - Center[k][c])*(cube.CubeData[k][j] - Center[k][c]);
					}
					if (dis < minDis)
					{
						minDis = dis;
						p = j;
					}
				}
			}
			cube.Idx[p] = c;
		}
		//重新调整聚类中心
		if (CenterMove)
		{
			int count[6] = { 0, 0, 0, 0, 0, 0 };
			for (int i = 0; i < 54; i++)
			{
				int Class = cube.Idx[i];   //获取这个色片为哪一类
				if (Class != -1)
				{
					count[Class]++;
					for (int j = 0; j < 3; j++)
					{
						Center[j][Class] = (Center[j][Class] * (count[Class] - 1) + cube.CubeData[j][i]) / count[Class];
					}
				}
			}
		}
	}
	return cube;
}
Cube Cluster(Cube cube)
{
	/*
	聚类思路：
	1，首先做一次kmean，判断各类元素数目是否为6个9。若是则退出。
	2，若不是，然后采用数字推平算法，推平为9个。                   //注意，数字抹去时，重新调整聚类中心。
	3，判断是否可解，可解则退出。
	4，采用中心点抢占算法。判断是否可解，可解则退出。              //注意，每次抢占都需要更新中心点。
	5，采用不更新中心点的中心抢占算法，可解则退出。                //注意，每次抢占不更新中心点。
	6，若不可解，返回复原魔方数据。
	*/
	//初始化Idx
	for (int i = 0; i < 54; i++)
	{
		cube.Idx[i] = -1;
	}
	//使用动态调整聚类中心的中心点抢占算法
	cout << "使用动态调整聚类中心的中心点抢占算法" << endl;
	cube = CenterGrab(cube, CENTER_MOVE);
	if (CubeCanSolve(cube))
	{
		cout << "动态调整聚类中心的中心点抢占算法可解" << endl;
		return cube;
	}
	else
	{
		cout << "使用动态调整聚类中心的中心点抢占算法不可解" << endl;
	}
	//使用中心点抢占算法
	cout << "使用中心点抢占算法" << endl;
	cube = CenterGrab(cube, CENTER_NO_MOVE);
	if (CubeCanSolve(cube))
	{
		cout << "中心点抢占算法可解" << endl;
		return cube;
	}
	else
	{
		cout << "使用中心点抢占算法不可解" << endl;
	}
	//kmean聚类
	cout << "使用kmean聚类" << endl;
	cube = KmeanCluster(cube);
	//判断各元素数是否为6个9
	if (CubeIdxIs6_9(cube))
	{
		if (CubeCanSolve(cube))
		{
			cout << "kmean聚类结果可解" << endl;
			return cube;
		}
		else
		{
			cout << "kmean结果不可解" << endl;
		}
	}
	//使用数字平推算法
	else
	{
		cout << "kmean未正确分类，使用数字平推" << endl;
		cube = CutOverNum(cube);
		if (CubeCanSolve(cube))
		{
			cout << "数字平推聚类结果可解" << endl;
			return cube;
		}
		else
		{
			cout << "数字平推结果不可解" << endl;
		}
	}
	return cube;
}
bool IsPictureCaped(void)
{
	char FileName[] = "../bridge/file/CluserStartFlag.txt";

	std::fstream _file;
	_file.open(FileName, ios::in);
	if (!_file)
	{
		return false;  //不存在
	}
	else
	{
		_file.close();
		Sleep(20);
		remove(FileName);
		return true;
	}
}