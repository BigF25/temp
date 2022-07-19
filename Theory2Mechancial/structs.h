#ifndef STRUCTS_H
#define STRUCTS_H

#include "Global.h"
using namespace std;
//=========================================================================��
#define L_0_R_0  0  //������е�۶�����
#define L_0_R_1  1  //������ţ��ұߺ���
#define L_1_R_0  2  //�ұ����ţ���ߺ���

#define F 0
#define R 1
#define U 2
#define B 3
#define L 4
#define D 5

#define _1 0
#define _2 1
#define _3 2

#define L1 0
#define L2 1
#define L3 2
#define LC 3
#define LO 4
#define R1 5
#define R2 6
#define R3 7
#define RC 8
#define RO 9
//=======================================================================�ֱ�״̬�ṹ��
//�����ʼ�ֱ�״̬
struct HandState
{
	int IsLeftOpen = 0;  //0Ϊ�պ�
	int IsRightOpen = 0;

	int LeftNotNice = 0; //0Ϊnice
	int RightNotNice = 0;

	void Set(int _IsLeftOpen, int _IsRightOpen, int _LeftNotNice, int _RightNotNice)
	{
		IsLeftOpen = _IsLeftOpen;
		IsRightOpen = _IsRightOpen;
		LeftNotNice = _LeftNotNice;
		RightNotNice = _RightNotNice;
	}

};
//=======================================================================��ת������ؽṹ��

struct Rot
{
	int a[3][3];
	void Set(int row0,int num0,int row1,int num1,int row2,int num2)
	{
		for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			a[i][j] = 0;

		a[row0][0] = num0;
		a[row1][1] = num1;
		a[row2][2] = num2;
	}
};
extern Rot R_x1, R_x2, R_x3, R_y1, R_y2, R_y3, R_z1, R_z2, R_z3;
//=======================================================================����ռ��
struct Point3
{
	int a[3][1];
	string name;
};
extern Point3 P_F, P_R, P_U, P_B, P_L, P_D;
extern Point3 P_FRUBLD[6];
//=========================================================================��е����ṹ��
struct MechanicalStep  //��е���赥��
{
	string name;
	int time;
	int num;
};
//��ʼ����е����
extern struct MechanicalStep M_L1, M_L2, M_L3, M_LC, M_LO;
extern struct MechanicalStep M_R1, M_R2, M_R3, M_RC, M_RO;
extern struct MechanicalStep M_END;
//=========================================================================��е������Ͻṹ��
struct MechanicalGroup  //��е����
{
	int time = 0; //��Group��������ʱ��
	int StepNum = 0;  //��������
	struct Rot rot;  //�˲������ת����
	struct HandState endHandState;  //ĩ��ʱ���ֱ۵�״̬
	struct MechanicalStep Steps[20];//ÿ�����۲����������Ļ�е���費����15�����������һ��M_ENDӦ<=16

	void Set(int _StepNum, struct MechanicalStep *_Steps,struct Rot _rot,struct HandState _state)
	{
		StepNum = _StepNum;
		rot = _rot;
		endHandState = _state;
		int i;
		for (i = 0; _Steps[i].name != "M_END"; i++)
		{
			if (i >= 20){ cout << "SetError!" << endl; break; }

			Steps[i] = _Steps[i];
		}
		Steps[i] = M_END;
	}
};                                       
extern MechanicalGroup MechanicalGroupLib[6][3][3][16];//FRUBLD  //F1��F2��F3  //L_0_R_0  //ÿ���������ౣ��8�����н�
//=======================================================================extern ����
extern void MechanicalStepsInit(void);
void RotInit(void);
extern void PointInit(void);
extern void LibInit(void);
extern void LibTimeInit(void);
extern Rot RotMtplRot(Rot l, Rot r);
extern Point3 RotMtplPoint3(Rot l, Point3 r);
#endif