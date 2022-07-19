#include "structs.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////变量
//=========================================================================运动时间常量
const int HandCloseTime = 300; //气缸合并时间
const int HandOpenTime = 300; //气缸打开时间
const int HandMove90 = 300;  //手臂拧动90度所需时间
const int HandMove180 = 300; //手臂拧动180度所需时间
const int DelayBetwen2Step = 200;//两个连续步骤之间的延迟时间
//=========================================================================机械步骤全局变量
struct MechanicalStep M_L1, M_L2, M_L3, M_LC, M_LO;
struct MechanicalStep M_R1, M_R2, M_R3, M_RC, M_RO;
struct MechanicalStep M_END;
//=========================================================================旋转矩阵&全局立方体方位
Rot R_x1, R_x2, R_x3, R_y1, R_y2, R_y3, R_z1, R_z2, R_z3;
//=========================================================================六个面中心在空间点坐标
Point3 P_F, P_R, P_U, P_B, P_L, P_D;
Point3 P_FRUBLD[6];
//=========================================================================全局手臂状态

//==============================================================================操作库变量
MechanicalGroup MechanicalGroupLib[6][3][3][16];     //FRUBLD  //F1、F2、F3  //L_0_R_0  //每种情况下最多保留16个可行解
//////////////////////////////////////////////////////////////////////////////////////////////////////////=========================函数
//=========================================================================机械步骤初始化
void MechanicalStepsInit(void)
{
	//name
	M_L1.name = "L1";
	M_L2.name = "L2";
	M_L3.name = "L3";
	M_LC.name = "LC";
	M_LO.name = "LO";

	M_R1.name = "R1";
	M_R2.name = "R2";
	M_R3.name = "R3";
	M_RC.name = "RC";
	M_RO.name = "RO";
	M_END.name = "M_END";

	//time
	M_L1.time = HandMove90;
	M_L2.time = HandMove180;
	M_L3.time = HandMove90;
	M_LC.time = HandCloseTime;
	M_LO.time = HandOpenTime;

	M_R1.time = HandMove90;
	M_R2.time = HandMove180;
	M_R3.time = HandMove90;
	M_RC.time = HandCloseTime;
	M_RO.time = HandOpenTime;
	M_END.time = 0;

	//num
	M_L1.num = L1;
	M_L2.num = L2;
	M_L3.num = L3;
	M_LC.num = LC;
	M_LO.num = LO;

	M_R1.num = R1;
	M_R2.num = R2;
	M_R3.num = R3;
	M_RC.num = RC;
	M_RO.num = RO;
	M_END.num = -1;
}
//=========================================================================旋转矩阵初始化
void RotInit(void)
{
	R_x1.a[0][0] = 1;
	R_x1.a[2][1] = -1;
	R_x1.a[1][2] = 1;
	R_x2 = RotMtplRot(R_x1, R_x1);
	R_x3 = RotMtplRot(R_x1, R_x2);

	R_y1.a[2][0] = 1;
	R_y1.a[1][1] = 1;
	R_y1.a[0][2] = -1;
	R_y2 = RotMtplRot(R_y1, R_y1);
	R_y3 = RotMtplRot(R_y1, R_y2);

	R_z1.a[1][0] = -1;
	R_z1.a[0][1] = 1;
	R_z1.a[2][2] = 1;
	R_z2 = RotMtplRot(R_z1, R_z1);
	R_z2 = RotMtplRot(R_z1, R_z2);

}
//=========================================================================六个面中心在空间点坐标初始化
void PointInit(void)
{
	P_F.a[0][0] = 1;
	P_F.a[1][0] = 0;
	P_F.a[2][0] = 0;
	P_F.name = "F";
	P_FRUBLD[F] = P_F;

	P_R.a[0][0] = 0;
	P_R.a[1][0] = 1;
	P_R.a[2][0] = 0;
	P_R.name = "R";
	P_FRUBLD[R] = P_R;

	P_U.a[0][0] = 0;
	P_U.a[1][0] = 0;
	P_U.a[2][0] = 1;
	P_U.name = "U";
	P_FRUBLD[U] = P_U;

	P_B.a[0][0] = -1;
	P_B.a[1][0] = 0;
	P_B.a[2][0] = 0;
	P_B.name = "B";
	P_FRUBLD[B] = P_B;

	P_L.a[0][0] = 0;
	P_L.a[1][0] = -1;
	P_L.a[2][0] = 0;
	P_L.name = "L";
	P_FRUBLD[L] = P_L;

	P_D.a[0][0] = 0;
	P_D.a[1][0] = 0;
	P_D.a[2][0] = -1;
	P_D.name = "D";
	P_FRUBLD[D] = P_D;
}
//=========================================================================矩阵运算
Rot RotMtplRot(Rot l, Rot r)
{
	Rot temp;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			temp.a[i][j] = 0;
		}
	}
	for (int k = 0; k < 3; k++)
	{
		int j;
		for (j = 0; j < 3; j++)
		{
			if (l.a[k][j] != 0)break;
		}
		int i;
		for (i = 0; i < 3; i++)
		{
			if (r.a[j][i] != 0)break;
		}
		if (l.a[k][j] == r.a[j][i])temp.a[k][i] = 1;
		else temp.a[k][i] = -1;
	}
	return temp;
}
Point3 RotMtplPoint3(Rot l, Point3 r)
{
	Point3 temp;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			temp.a[i][j] = l.a[i][0] * r.a[0][j] + l.a[i][1] * r.a[1][j] + l.a[i][2] * r.a[2][j];
		}
	}
	return temp;
}
//=========================================================================初始化操作库
Rot tempRot;
HandState tempHandState;
void F1_L0R0Init(void)
{
	//F1_L0R0_0
	MechanicalStep F1_L0R0_0[] = { M_L1, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_1][L_0_R_0][0].Set(1, F1_L0R0_0, tempRot, tempHandState);
	//F1_L0R0_1
	MechanicalStep F1_L0R0_1[] = { M_LO, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_0_R_0][1].Set(4, F1_L0R0_1, tempRot, tempHandState);
	//F1_L0R0_2
	MechanicalStep F1_L0R0_2[] = { M_RO, M_L3, M_RC, M_L1, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_0_R_0][2].Set(4, F1_L0R0_2, tempRot, tempHandState);
	//F1_L0R0_3
	MechanicalStep F1_L0R0_3[] = { M_RO, M_L1, M_RC, M_L1, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_0_R_0][3].Set(4, F1_L0R0_3, tempRot, tempHandState);
	//F1_L0R0_4
	MechanicalStep F1_L0R0_4[] = { M_RO, M_L2, M_RC, M_L1, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_1][L_0_R_0][4].Set(4, F1_L0R0_4, tempRot, tempHandState);
	//F1_L0R0_5
	MechanicalStep F1_L0R0_5[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_1][L_0_R_0][5].Set(11, F1_L0R0_5, tempRot, tempHandState);
	//F1_L0R0_6
	MechanicalStep F1_L0R0_6[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_1][L_0_R_0][6].Set(11, F1_L0R0_6, tempRot, tempHandState);
	//F1_L0R0_7
	MechanicalStep F1_L0R0_7[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_0_R_0][7].Set(12, F1_L0R0_7, tempRot, tempHandState);
	//F1_L0R0_8
	MechanicalStep F1_L0R0_8[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_L3, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_0_R_0][8].Set(12, F1_L0R0_8, tempRot, tempHandState);
	//F1_L0R0_9
	MechanicalStep F1_L0R0_9[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_0_R_0][9].Set(12, F1_L0R0_9, tempRot, tempHandState);
	//F1_L0R0_10
	MechanicalStep F1_L0R0_10[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_0_R_0][10].Set(12, F1_L0R0_10, tempRot, tempHandState);
	//F1_L0R0_11
	MechanicalStep F1_L0R0_11[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_0_R_0][11].Set(13, F1_L0R0_11, tempRot, tempHandState);
	//F1_L0R0_12
	MechanicalStep F1_L0R0_12[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_1][L_0_R_0][12].Set(15, F1_L0R0_12, tempRot, tempHandState);
	//F1_L0R0_13
	MechanicalStep F1_L0R0_13[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_1][L_0_R_0][13].Set(15, F1_L0R0_13, tempRot, tempHandState);
	//F1_L0R0_14
	MechanicalStep F1_L0R0_14[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_1][L_0_R_0][14].Set(16, F1_L0R0_14, tempRot, tempHandState);
	//F1_L0R0_15
	MechanicalStep F1_L0R0_15[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_1][L_0_R_0][15].Set(16, F1_L0R0_15, tempRot, tempHandState);
}
void F2_L0R0Init(void)
{
	//F2_L0R0_0
	MechanicalStep F2_L0R0_0[] = { M_L2, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_0_R_0][0].Set(1, F2_L0R0_0, tempRot, tempHandState);
	//F2_L0R0_1
	MechanicalStep F2_L0R0_1[] = { M_RO, M_L2, M_RC, M_L2, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_0_R_0][1].Set(4, F2_L0R0_1, tempRot, tempHandState);
	//F2_L0R0_2
	MechanicalStep F2_L0R0_2[] = { M_LO, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_2][L_0_R_0][2].Set(4, F2_L0R0_2, tempRot, tempHandState);
	//F2_L0R0_3
	MechanicalStep F2_L0R0_3[] = { M_RO, M_L3, M_RC, M_L2, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_2][L_0_R_0][3].Set(4, F2_L0R0_3, tempRot, tempHandState);
	//F2_L0R0_4
	MechanicalStep F2_L0R0_4[] = { M_RO, M_L1, M_RC, M_L2, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_2][L_0_R_0][4].Set(4, F2_L0R0_4, tempRot, tempHandState);
	//F2_L0R0_5
	MechanicalStep F2_L0R0_5[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_0_R_0][5].Set(11, F2_L0R0_5, tempRot, tempHandState);
	//F2_L0R0_6
	MechanicalStep F2_L0R0_6[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_0_R_0][6].Set(11, F2_L0R0_6, tempRot, tempHandState);
	//F2_L0R0_7
	MechanicalStep F2_L0R0_7[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_L3, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_2][L_0_R_0][7].Set(12, F2_L0R0_7, tempRot, tempHandState);
	//F2_L0R0_8
	MechanicalStep F2_L0R0_8[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_2][L_0_R_0][8].Set(12, F2_L0R0_8, tempRot, tempHandState);
	//F2_L0R0_9
	MechanicalStep F2_L0R0_9[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_2][L_0_R_0][9].Set(12, F2_L0R0_9, tempRot, tempHandState);
	//F2_L0R0_10
	MechanicalStep F2_L0R0_10[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_2][L_0_R_0][10].Set(12, F2_L0R0_10, tempRot, tempHandState);
	//F2_L0R0_11
	MechanicalStep F2_L0R0_11[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_2][L_0_R_0][11].Set(13, F2_L0R0_11, tempRot, tempHandState);
	//F2_L0R0_12
	MechanicalStep F2_L0R0_12[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_0_R_0][12].Set(15, F2_L0R0_12, tempRot, tempHandState);
	//F2_L0R0_13
	MechanicalStep F2_L0R0_13[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_0_R_0][13].Set(15, F2_L0R0_13, tempRot, tempHandState);
	//F2_L0R0_14
	MechanicalStep F2_L0R0_14[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_0_R_0][14].Set(16, F2_L0R0_14, tempRot, tempHandState);
	//F2_L0R0_15
	MechanicalStep F2_L0R0_15[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_0_R_0][15].Set(16, F2_L0R0_15, tempRot, tempHandState);
}
void F3_L0R0Init(void)
{
	//F3_L0R0_0
	MechanicalStep F3_L0R0_0[] = { M_L3, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_3][L_0_R_0][0].Set(1, F3_L0R0_0, tempRot, tempHandState);
	//F3_L0R0_1
	MechanicalStep F3_L0R0_1[] = { M_LO, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_0_R_0][1].Set(4, F3_L0R0_1, tempRot, tempHandState);
	//F3_L0R0_2
	MechanicalStep F3_L0R0_2[] = { M_RO, M_L3, M_RC, M_L3, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_0_R_0][2].Set(4, F3_L0R0_2, tempRot, tempHandState);
	//F3_L0R0_3
	MechanicalStep F3_L0R0_3[] = { M_RO, M_L1, M_RC, M_L3, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_0_R_0][3].Set(4, F3_L0R0_3, tempRot, tempHandState);
	//F3_L0R0_4
	MechanicalStep F3_L0R0_4[] = { M_RO, M_L2, M_RC, M_L3, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_3][L_0_R_0][4].Set(4, F3_L0R0_4, tempRot, tempHandState);
	//F3_L0R0_5
	MechanicalStep F3_L0R0_5[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_3][L_0_R_0][5].Set(11, F3_L0R0_5, tempRot, tempHandState);
	//F3_L0R0_6
	MechanicalStep F3_L0R0_6[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_3][L_0_R_0][6].Set(11, F3_L0R0_6, tempRot, tempHandState);
	//F3_L0R0_7
	MechanicalStep F3_L0R0_7[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_L3, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_0_R_0][7].Set(12, F3_L0R0_7, tempRot, tempHandState);
	//F3_L0R0_8
	MechanicalStep F3_L0R0_8[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_0_R_0][8].Set(12, F3_L0R0_8, tempRot, tempHandState);
	//F3_L0R0_9
	MechanicalStep F3_L0R0_9[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_0_R_0][9].Set(12, F3_L0R0_9, tempRot, tempHandState);
	//F3_L0R0_10
	MechanicalStep F3_L0R0_10[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_0_R_0][10].Set(12, F3_L0R0_10, tempRot, tempHandState);
	//F3_L0R0_11
	MechanicalStep F3_L0R0_11[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_0_R_0][11].Set(13, F3_L0R0_11, tempRot, tempHandState);
	//F3_L0R0_12
	MechanicalStep F3_L0R0_12[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_3][L_0_R_0][12].Set(15, F3_L0R0_12, tempRot, tempHandState);
	//F3_L0R0_13
	MechanicalStep F3_L0R0_13[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_3][L_0_R_0][13].Set(15, F3_L0R0_13, tempRot, tempHandState);
	//F3_L0R0_14
	MechanicalStep F3_L0R0_14[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_3][L_0_R_0][14].Set(16, F3_L0R0_14, tempRot, tempHandState);
	//F3_L0R0_15
	MechanicalStep F3_L0R0_15[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_3][L_0_R_0][15].Set(16, F3_L0R0_15, tempRot, tempHandState);
}
void R1_L0R0Init(void)
{
	//R1_L0R0_0
	MechanicalStep R1_L0R0_0[] = { M_R1, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_1][L_0_R_0][0].Set(1, R1_L0R0_0, tempRot, tempHandState);
	//R1_L0R0_1
	MechanicalStep R1_L0R0_1[] = { M_LO, M_R3, M_LC, M_R1, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_0_R_0][1].Set(4, R1_L0R0_1, tempRot, tempHandState);
	//R1_L0R0_2
	MechanicalStep R1_L0R0_2[] = { M_LO, M_R1, M_LC, M_R1, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_0_R_0][2].Set(4, R1_L0R0_2, tempRot, tempHandState);
	//R1_L0R0_3
	MechanicalStep R1_L0R0_3[] = { M_RO, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_0_R_0][3].Set(4, R1_L0R0_3, tempRot, tempHandState);
	//R1_L0R0_4
	MechanicalStep R1_L0R0_4[] = { M_LO, M_R2, M_LC, M_R1, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_1][L_0_R_0][4].Set(4, R1_L0R0_4, tempRot, tempHandState);
	//R1_L0R0_5
	MechanicalStep R1_L0R0_5[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_LC, M_L1, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_1][L_0_R_0][5].Set(11, R1_L0R0_5, tempRot, tempHandState);
	//R1_L0R0_6
	MechanicalStep R1_L0R0_6[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_LC, M_L1, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_1][L_0_R_0][6].Set(11, R1_L0R0_6, tempRot, tempHandState);
	//R1_L0R0_7
	MechanicalStep R1_L0R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_0_R_0][7].Set(12, R1_L0R0_7, tempRot, tempHandState);
	//R1_L0R0_8
	MechanicalStep R1_L0R0_8[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_0_R_0][8].Set(12, R1_L0R0_8, tempRot, tempHandState);
	//R1_L0R0_9
	MechanicalStep R1_L0R0_9[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_0_R_0][9].Set(12, R1_L0R0_9, tempRot, tempHandState);
	//R1_L0R0_10
	MechanicalStep R1_L0R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_0_R_0][10].Set(12, R1_L0R0_10, tempRot, tempHandState);
	//R1_L0R0_11
	MechanicalStep R1_L0R0_11[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_0_R_0][11].Set(13, R1_L0R0_11, tempRot, tempHandState);
	//R1_L0R0_12
	MechanicalStep R1_L0R0_12[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_1][L_0_R_0][12].Set(15, R1_L0R0_12, tempRot, tempHandState);
	//R1_L0R0_13
	MechanicalStep R1_L0R0_13[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_1][L_0_R_0][13].Set(15, R1_L0R0_13, tempRot, tempHandState);
	//R1_L0R0_14
	MechanicalStep R1_L0R0_14[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_1][L_0_R_0][14].Set(16, R1_L0R0_14, tempRot, tempHandState);
	//R1_L0R0_15
	MechanicalStep R1_L0R0_15[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_1][L_0_R_0][15].Set(16, R1_L0R0_15, tempRot, tempHandState);
}
void R2_L0R0Init(void)
{
	//R2_L0R0_0
	MechanicalStep R2_L0R0_0[] = { M_R2, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_0_R_0][0].Set(1, R2_L0R0_0, tempRot, tempHandState);
	//R2_L0R0_1
	MechanicalStep R2_L0R0_1[] = { M_LO, M_R2, M_LC, M_R2, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_0_R_0][1].Set(4, R2_L0R0_1, tempRot, tempHandState);
	//R2_L0R0_2
	MechanicalStep R2_L0R0_2[] = { M_LO, M_R3, M_LC, M_R2, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_2][L_0_R_0][2].Set(4, R2_L0R0_2, tempRot, tempHandState);
	//R2_L0R0_3
	MechanicalStep R2_L0R0_3[] = { M_LO, M_R1, M_LC, M_R2, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_2][L_0_R_0][3].Set(4, R2_L0R0_3, tempRot, tempHandState);
	//R2_L0R0_4
	MechanicalStep R2_L0R0_4[] = { M_RO, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_2][L_0_R_0][4].Set(4, R2_L0R0_4, tempRot, tempHandState);
	//R2_L0R0_5
	MechanicalStep R2_L0R0_5[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_LC, M_L2, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_0_R_0][5].Set(11, R2_L0R0_5, tempRot, tempHandState);
	//R2_L0R0_6
	MechanicalStep R2_L0R0_6[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_LC, M_L2, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_0_R_0][6].Set(11, R2_L0R0_6, tempRot, tempHandState);
	//R2_L0R0_7
	MechanicalStep R2_L0R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_2][L_0_R_0][7].Set(12, R2_L0R0_7, tempRot, tempHandState);
	//R2_L0R0_8
	MechanicalStep R2_L0R0_8[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_2][L_0_R_0][8].Set(12, R2_L0R0_8, tempRot, tempHandState);
	//R2_L0R0_9
	MechanicalStep R2_L0R0_9[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_2][L_0_R_0][9].Set(12, R2_L0R0_9, tempRot, tempHandState);
	//R2_L0R0_10
	MechanicalStep R2_L0R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_2][L_0_R_0][10].Set(12, R2_L0R0_10, tempRot, tempHandState);
	//R2_L0R0_11
	MechanicalStep R2_L0R0_11[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_2][L_0_R_0][11].Set(13, R2_L0R0_11, tempRot, tempHandState);
	//R2_L0R0_12
	MechanicalStep R2_L0R0_12[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_0_R_0][12].Set(15, R2_L0R0_12, tempRot, tempHandState);
	//R2_L0R0_13
	MechanicalStep R2_L0R0_13[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_0_R_0][13].Set(15, R2_L0R0_13, tempRot, tempHandState);
	//R2_L0R0_14
	MechanicalStep R2_L0R0_14[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_0_R_0][14].Set(16, R2_L0R0_14, tempRot, tempHandState);
	//R2_L0R0_15
	MechanicalStep R2_L0R0_15[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_0_R_0][15].Set(16, R2_L0R0_15, tempRot, tempHandState);
}
void R3_L0R0Init(void)
{
	//R3_L0R0_0
	MechanicalStep R3_L0R0_0[] = { M_R3, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_3][L_0_R_0][0].Set(1, R3_L0R0_0, tempRot, tempHandState);
	//R3_L0R0_1
	MechanicalStep R3_L0R0_1[] = { M_LO, M_R3, M_LC, M_R3, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_0_R_0][1].Set(4, R3_L0R0_1, tempRot, tempHandState);
	//R3_L0R0_2
	MechanicalStep R3_L0R0_2[] = { M_LO, M_R1, M_LC, M_R3, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_0_R_0][2].Set(4, R3_L0R0_2, tempRot, tempHandState);
	//R3_L0R0_3
	MechanicalStep R3_L0R0_3[] = { M_RO, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_0_R_0][3].Set(4, R3_L0R0_3, tempRot, tempHandState);
	//R3_L0R0_4
	MechanicalStep R3_L0R0_4[] = { M_LO, M_R2, M_LC, M_R3, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_3][L_0_R_0][4].Set(4, R3_L0R0_4, tempRot, tempHandState);
	//R3_L0R0_5
	MechanicalStep R3_L0R0_5[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_LC, M_L3, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_3][L_0_R_0][5].Set(11, R3_L0R0_5, tempRot, tempHandState);
	//R3_L0R0_6
	MechanicalStep R3_L0R0_6[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_LC, M_L3, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_3][L_0_R_0][6].Set(11, R3_L0R0_6, tempRot, tempHandState);
	//R3_L0R0_7
	MechanicalStep R3_L0R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_0_R_0][7].Set(12, R3_L0R0_7, tempRot, tempHandState);
	//R3_L0R0_8
	MechanicalStep R3_L0R0_8[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_0_R_0][8].Set(12, R3_L0R0_8, tempRot, tempHandState);
	//R3_L0R0_9
	MechanicalStep R3_L0R0_9[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_0_R_0][9].Set(12, R3_L0R0_9, tempRot, tempHandState);
	//R3_L0R0_10
	MechanicalStep R3_L0R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_0_R_0][10].Set(12, R3_L0R0_10, tempRot, tempHandState);
	//R3_L0R0_11
	MechanicalStep R3_L0R0_11[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_0_R_0][11].Set(13, R3_L0R0_11, tempRot, tempHandState);
	//R3_L0R0_12
	MechanicalStep R3_L0R0_12[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_3][L_0_R_0][12].Set(15, R3_L0R0_12, tempRot, tempHandState);
	//R3_L0R0_13
	MechanicalStep R3_L0R0_13[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_3][L_0_R_0][13].Set(15, R3_L0R0_13, tempRot, tempHandState);
	//R3_L0R0_14
	MechanicalStep R3_L0R0_14[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_3][L_0_R_0][14].Set(16, R3_L0R0_14, tempRot, tempHandState);
	//R3_L0R0_15
	MechanicalStep R3_L0R0_15[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_3][L_0_R_0][15].Set(16, R3_L0R0_15, tempRot, tempHandState);
}
void U1_L0R0Init(void)
{
	//U1_L0R0_0
	MechanicalStep U1_L0R0_0[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_RC, M_R1, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_1][L_0_R_0][0].Set(7, U1_L0R0_0, tempRot, tempHandState);
	//U1_L0R0_1
	MechanicalStep U1_L0R0_1[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_1][L_0_R_0][1].Set(7, U1_L0R0_1, tempRot, tempHandState);
	//U1_L0R0_2
	MechanicalStep U1_L0R0_2[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_0_R_0][2].Set(8, U1_L0R0_2, tempRot, tempHandState);
	//U1_L0R0_3
	MechanicalStep U1_L0R0_3[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_0_R_0][3].Set(8, U1_L0R0_3, tempRot, tempHandState);
	//U1_L0R0_4
	MechanicalStep U1_L0R0_4[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_0_R_0][4].Set(8, U1_L0R0_4, tempRot, tempHandState);
	//U1_L0R0_5
	MechanicalStep U1_L0R0_5[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_0_R_0][5].Set(8, U1_L0R0_5, tempRot, tempHandState);
	//U1_L0R0_6
	MechanicalStep U1_L0R0_6[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_0_R_0][6].Set(8, U1_L0R0_6, tempRot, tempHandState);
	//U1_L0R0_7
	MechanicalStep U1_L0R0_7[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_0_R_0][7].Set(8, U1_L0R0_7, tempRot, tempHandState);
	//U1_L0R0_8
	MechanicalStep U1_L0R0_8[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_RC, M_R1, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_1][L_0_R_0][8].Set(8, U1_L0R0_8, tempRot, tempHandState);
	//U1_L0R0_9
	MechanicalStep U1_L0R0_9[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_L1, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_1][L_0_R_0][9].Set(8, U1_L0R0_9, tempRot, tempHandState);
	//U1_L0R0_10
	MechanicalStep U1_L0R0_10[] = { M_RO, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_0_R_0][10].Set(9, U1_L0R0_10, tempRot, tempHandState);
	//U1_L0R0_11
	MechanicalStep U1_L0R0_11[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_0_R_0][11].Set(9, U1_L0R0_11, tempRot, tempHandState);
	//U1_L0R0_12
	MechanicalStep U1_L0R0_12[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_1][L_0_R_0][12].Set(12, U1_L0R0_12, tempRot, tempHandState);
	//U1_L0R0_13
	MechanicalStep U1_L0R0_13[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_1][L_0_R_0][13].Set(12, U1_L0R0_13, tempRot, tempHandState);
	//U1_L0R0_14
	MechanicalStep U1_L0R0_14[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_1][L_0_R_0][14].Set(12, U1_L0R0_14, tempRot, tempHandState);
	//U1_L0R0_15
	MechanicalStep U1_L0R0_15[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_1][L_0_R_0][15].Set(12, U1_L0R0_15, tempRot, tempHandState);
}
void U2_L0R0Init(void)
{
	//U2_L0R0_0
	MechanicalStep U2_L0R0_0[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_RC, M_R2, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_0_R_0][0].Set(7, U2_L0R0_0, tempRot, tempHandState);
	//U2_L0R0_1
	MechanicalStep U2_L0R0_1[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_0_R_0][1].Set(7, U2_L0R0_1, tempRot, tempHandState);
	//U2_L0R0_2
	MechanicalStep U2_L0R0_2[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_L2, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_0_R_0][2].Set(8, U2_L0R0_2, tempRot, tempHandState);
	//U2_L0R0_3
	MechanicalStep U2_L0R0_3[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_RC, M_R2, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_0_R_0][3].Set(8, U2_L0R0_3, tempRot, tempHandState);
	//U2_L0R0_4
	MechanicalStep U2_L0R0_4[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_2][L_0_R_0][4].Set(8, U2_L0R0_4, tempRot, tempHandState);
	//U2_L0R0_5
	MechanicalStep U2_L0R0_5[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_2][L_0_R_0][5].Set(8, U2_L0R0_5, tempRot, tempHandState);
	//U2_L0R0_6
	MechanicalStep U2_L0R0_6[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_2][L_0_R_0][6].Set(8, U2_L0R0_6, tempRot, tempHandState);
	//U2_L0R0_7
	MechanicalStep U2_L0R0_7[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_2][L_0_R_0][7].Set(8, U2_L0R0_7, tempRot, tempHandState);
	//U2_L0R0_8
	MechanicalStep U2_L0R0_8[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_2][L_0_R_0][8].Set(8, U2_L0R0_8, tempRot, tempHandState);
	//U2_L0R0_9
	MechanicalStep U2_L0R0_9[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_2][L_0_R_0][9].Set(8, U2_L0R0_9, tempRot, tempHandState);
	//U2_L0R0_10
	MechanicalStep U2_L0R0_10[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_2][L_0_R_0][10].Set(9, U2_L0R0_10, tempRot, tempHandState);
	//U2_L0R0_11
	MechanicalStep U2_L0R0_11[] = { M_RO, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_2][L_0_R_0][11].Set(9, U2_L0R0_11, tempRot, tempHandState);
	//U2_L0R0_12
	MechanicalStep U2_L0R0_12[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_0_R_0][12].Set(12, U2_L0R0_12, tempRot, tempHandState);
	//U2_L0R0_13
	MechanicalStep U2_L0R0_13[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_0_R_0][13].Set(12, U2_L0R0_13, tempRot, tempHandState);
	//U2_L0R0_14
	MechanicalStep U2_L0R0_14[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_0_R_0][14].Set(12, U2_L0R0_14, tempRot, tempHandState);
	//U2_L0R0_15
	MechanicalStep U2_L0R0_15[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_0_R_0][15].Set(12, U2_L0R0_15, tempRot, tempHandState);
}
void U3_L0R0Init(void)
{
	//U3_L0R0_0
	MechanicalStep U3_L0R0_0[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_RC, M_R3, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_3][L_0_R_0][0].Set(7, U3_L0R0_0, tempRot, tempHandState);
	//U3_L0R0_1
	MechanicalStep U3_L0R0_1[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_3][L_0_R_0][1].Set(7, U3_L0R0_1, tempRot, tempHandState);
	//U3_L0R0_2
	MechanicalStep U3_L0R0_2[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_0_R_0][2].Set(8, U3_L0R0_2, tempRot, tempHandState);
	//U3_L0R0_3
	MechanicalStep U3_L0R0_3[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_0_R_0][3].Set(8, U3_L0R0_3, tempRot, tempHandState);
	//U3_L0R0_4
	MechanicalStep U3_L0R0_4[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_0_R_0][4].Set(8, U3_L0R0_4, tempRot, tempHandState);
	//U3_L0R0_5
	MechanicalStep U3_L0R0_5[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_0_R_0][5].Set(8, U3_L0R0_5, tempRot, tempHandState);
	//U3_L0R0_6
	MechanicalStep U3_L0R0_6[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_0_R_0][6].Set(8, U3_L0R0_6, tempRot, tempHandState);
	//U3_L0R0_7
	MechanicalStep U3_L0R0_7[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_0_R_0][7].Set(8, U3_L0R0_7, tempRot, tempHandState);
	//U3_L0R0_8
	MechanicalStep U3_L0R0_8[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_L3, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_3][L_0_R_0][8].Set(8, U3_L0R0_8, tempRot, tempHandState);
	//U3_L0R0_9
	MechanicalStep U3_L0R0_9[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_RC, M_R3, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_3][L_0_R_0][9].Set(8, U3_L0R0_9, tempRot, tempHandState);
	//U3_L0R0_10
	MechanicalStep U3_L0R0_10[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_0_R_0][10].Set(9, U3_L0R0_10, tempRot, tempHandState);
	//U3_L0R0_11
	MechanicalStep U3_L0R0_11[] = { M_RO, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_0_R_0][11].Set(9, U3_L0R0_11, tempRot, tempHandState);
	//U3_L0R0_12
	MechanicalStep U3_L0R0_12[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_3][L_0_R_0][12].Set(12, U3_L0R0_12, tempRot, tempHandState);
	//U3_L0R0_13
	MechanicalStep U3_L0R0_13[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_3][L_0_R_0][13].Set(12, U3_L0R0_13, tempRot, tempHandState);
	//U3_L0R0_14
	MechanicalStep U3_L0R0_14[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_3][L_0_R_0][14].Set(12, U3_L0R0_14, tempRot, tempHandState);
	//U3_L0R0_15
	MechanicalStep U3_L0R0_15[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_3][L_0_R_0][15].Set(12, U3_L0R0_15, tempRot, tempHandState);
}
void B1_L0R0Init(void)
{
	//B1_L0R0_0
	MechanicalStep B1_L0R0_0[] = { M_LO, M_R2, M_LC, M_L1, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_1][L_0_R_0][0].Set(4, B1_L0R0_0, tempRot, tempHandState);
	//B1_L0R0_1
	MechanicalStep B1_L0R0_1[] = { M_LO, M_R2, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_0_R_0][1].Set(5, B1_L0R0_1, tempRot, tempHandState);
	//B1_L0R0_2
	MechanicalStep B1_L0R0_2[] = { M_RO, M_L2, M_RC, M_LO, M_R2, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_0_R_0][2].Set(8, B1_L0R0_2, tempRot, tempHandState);
	//B1_L0R0_3
	MechanicalStep B1_L0R0_3[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_1][L_0_R_0][3].Set(8, B1_L0R0_3, tempRot, tempHandState);
	//B1_L0R0_4
	MechanicalStep B1_L0R0_4[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_1][L_0_R_0][4].Set(8, B1_L0R0_4, tempRot, tempHandState);
	//B1_L0R0_5
	MechanicalStep B1_L0R0_5[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_0_R_0][5].Set(9, B1_L0R0_5, tempRot, tempHandState);
	//B1_L0R0_6
	MechanicalStep B1_L0R0_6[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_0_R_0][6].Set(9, B1_L0R0_6, tempRot, tempHandState);
	//B1_L0R0_7
	MechanicalStep B1_L0R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_1][L_0_R_0][7].Set(11, B1_L0R0_7, tempRot, tempHandState);
	//B1_L0R0_8
	MechanicalStep B1_L0R0_8[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_1][L_0_R_0][8].Set(11, B1_L0R0_8, tempRot, tempHandState);
	//B1_L0R0_9
	MechanicalStep B1_L0R0_9[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_1][L_0_R_0][9].Set(11, B1_L0R0_9, tempRot, tempHandState);
	//B1_L0R0_10
	MechanicalStep B1_L0R0_10[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_0_R_0][10].Set(12, B1_L0R0_10, tempRot, tempHandState);
	//B1_L0R0_11
	MechanicalStep B1_L0R0_11[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_0_R_0][11].Set(12, B1_L0R0_11, tempRot, tempHandState);
	//B1_L0R0_12
	MechanicalStep B1_L0R0_12[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_0_R_0][12].Set(12, B1_L0R0_12, tempRot, tempHandState);
	//B1_L0R0_13
	MechanicalStep B1_L0R0_13[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_0_R_0][13].Set(12, B1_L0R0_13, tempRot, tempHandState);
	//B1_L0R0_14
	MechanicalStep B1_L0R0_14[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_RC, M_R1, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_1][L_0_R_0][14].Set(15, B1_L0R0_14, tempRot, tempHandState);
	//B1_L0R0_15
	MechanicalStep B1_L0R0_15[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_RC, M_R1, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_1][L_0_R_0][15].Set(15, B1_L0R0_15, tempRot, tempHandState);
}
void B2_L0R0Init(void)
{
	//B2_L0R0_0
	MechanicalStep B2_L0R0_0[] = { M_LO, M_R2, M_LC, M_L2, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_0_R_0][0].Set(4, B2_L0R0_0, tempRot, tempHandState);
	//B2_L0R0_1
	MechanicalStep B2_L0R0_1[] = { M_LO, M_R2, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_2][L_0_R_0][1].Set(5, B2_L0R0_1, tempRot, tempHandState);
	//B2_L0R0_2
	MechanicalStep B2_L0R0_2[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_0_R_0][2].Set(8, B2_L0R0_2, tempRot, tempHandState);
	//B2_L0R0_3
	MechanicalStep B2_L0R0_3[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_0_R_0][3].Set(8, B2_L0R0_3, tempRot, tempHandState);
	//B2_L0R0_4
	MechanicalStep B2_L0R0_4[] = { M_RO, M_L2, M_RC, M_LO, M_R2, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_2][L_0_R_0][4].Set(8, B2_L0R0_4, tempRot, tempHandState);
	//B2_L0R0_5
	MechanicalStep B2_L0R0_5[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_2][L_0_R_0][5].Set(9, B2_L0R0_5, tempRot, tempHandState);
	//B2_L0R0_6
	MechanicalStep B2_L0R0_6[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_2][L_0_R_0][6].Set(9, B2_L0R0_6, tempRot, tempHandState);
	//B2_L0R0_7
	MechanicalStep B2_L0R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_0_R_0][7].Set(11, B2_L0R0_7, tempRot, tempHandState);
	//B2_L0R0_8
	MechanicalStep B2_L0R0_8[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_0_R_0][8].Set(11, B2_L0R0_8, tempRot, tempHandState);
	//B2_L0R0_9
	MechanicalStep B2_L0R0_9[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_0_R_0][9].Set(11, B2_L0R0_9, tempRot, tempHandState);
	//B2_L0R0_10
	MechanicalStep B2_L0R0_10[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_2][L_0_R_0][10].Set(12, B2_L0R0_10, tempRot, tempHandState);
	//B2_L0R0_11
	MechanicalStep B2_L0R0_11[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_2][L_0_R_0][11].Set(12, B2_L0R0_11, tempRot, tempHandState);
	//B2_L0R0_12
	MechanicalStep B2_L0R0_12[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_2][L_0_R_0][12].Set(12, B2_L0R0_12, tempRot, tempHandState);
	//B2_L0R0_13
	MechanicalStep B2_L0R0_13[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_2][L_0_R_0][13].Set(12, B2_L0R0_13, tempRot, tempHandState);
	//B2_L0R0_14
	MechanicalStep B2_L0R0_14[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_RC, M_R2, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_0_R_0][14].Set(15, B2_L0R0_14, tempRot, tempHandState);
	//B2_L0R0_15
	MechanicalStep B2_L0R0_15[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_RC, M_R2, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_0_R_0][15].Set(15, B2_L0R0_15, tempRot, tempHandState);
}
void B3_L0R0Init(void)
{
	//B3_L0R0_0
	MechanicalStep B3_L0R0_0[] = { M_LO, M_R2, M_LC, M_L3, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_3][L_0_R_0][0].Set(4, B3_L0R0_0, tempRot, tempHandState);
	//B3_L0R0_1
	MechanicalStep B3_L0R0_1[] = { M_LO, M_R2, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_0_R_0][1].Set(5, B3_L0R0_1, tempRot, tempHandState);
	//B3_L0R0_2
	MechanicalStep B3_L0R0_2[] = { M_RO, M_L2, M_RC, M_LO, M_R2, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_0_R_0][2].Set(8, B3_L0R0_2, tempRot, tempHandState);
	//B3_L0R0_3
	MechanicalStep B3_L0R0_3[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_3][L_0_R_0][3].Set(8, B3_L0R0_3, tempRot, tempHandState);
	//B3_L0R0_4
	MechanicalStep B3_L0R0_4[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_3][L_0_R_0][4].Set(8, B3_L0R0_4, tempRot, tempHandState);
	//B3_L0R0_5
	MechanicalStep B3_L0R0_5[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_0_R_0][5].Set(9, B3_L0R0_5, tempRot, tempHandState);
	//B3_L0R0_6
	MechanicalStep B3_L0R0_6[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_0_R_0][6].Set(9, B3_L0R0_6, tempRot, tempHandState);
	//B3_L0R0_7
	MechanicalStep B3_L0R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_3][L_0_R_0][7].Set(11, B3_L0R0_7, tempRot, tempHandState);
	//B3_L0R0_8
	MechanicalStep B3_L0R0_8[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_3][L_0_R_0][8].Set(11, B3_L0R0_8, tempRot, tempHandState);
	//B3_L0R0_9
	MechanicalStep B3_L0R0_9[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_3][L_0_R_0][9].Set(11, B3_L0R0_9, tempRot, tempHandState);
	//B3_L0R0_10
	MechanicalStep B3_L0R0_10[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_0_R_0][10].Set(12, B3_L0R0_10, tempRot, tempHandState);
	//B3_L0R0_11
	MechanicalStep B3_L0R0_11[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_0_R_0][11].Set(12, B3_L0R0_11, tempRot, tempHandState);
	//B3_L0R0_12
	MechanicalStep B3_L0R0_12[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_0_R_0][12].Set(12, B3_L0R0_12, tempRot, tempHandState);
	//B3_L0R0_13
	MechanicalStep B3_L0R0_13[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_0_R_0][13].Set(12, B3_L0R0_13, tempRot, tempHandState);
	//B3_L0R0_14
	MechanicalStep B3_L0R0_14[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_RC, M_R3, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_3][L_0_R_0][14].Set(15, B3_L0R0_14, tempRot, tempHandState);
	//B3_L0R0_15
	MechanicalStep B3_L0R0_15[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_RC, M_R3, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_3][L_0_R_0][15].Set(15, B3_L0R0_15, tempRot, tempHandState);
}
void L1_L0R0Init(void)
{
	//L1_L0R0_0
	MechanicalStep L1_L0R0_0[] = { M_RO, M_L2, M_RC, M_R1, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_1][L_0_R_0][0].Set(4, L1_L0R0_0, tempRot, tempHandState);
	//L1_L0R0_1
	MechanicalStep L1_L0R0_1[] = { M_RO, M_L2, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_0_R_0][1].Set(5, L1_L0R0_1, tempRot, tempHandState);
	//L1_L0R0_2
	MechanicalStep L1_L0R0_2[] = { M_LO, M_R2, M_LC, M_RO, M_L2, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_0_R_0][2].Set(8, L1_L0R0_2, tempRot, tempHandState);
	//L1_L0R0_3
	MechanicalStep L1_L0R0_3[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_1][L_0_R_0][3].Set(8, L1_L0R0_3, tempRot, tempHandState);
	//L1_L0R0_4
	MechanicalStep L1_L0R0_4[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_1][L_0_R_0][4].Set(8, L1_L0R0_4, tempRot, tempHandState);
	//L1_L0R0_5
	MechanicalStep L1_L0R0_5[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_0_R_0][5].Set(9, L1_L0R0_5, tempRot, tempHandState);
	//L1_L0R0_6
	MechanicalStep L1_L0R0_6[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_0_R_0][6].Set(9, L1_L0R0_6, tempRot, tempHandState);
	//L1_L0R0_7
	MechanicalStep L1_L0R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_LC, M_L1, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_1][L_0_R_0][7].Set(11, L1_L0R0_7, tempRot, tempHandState);
	//L1_L0R0_8
	MechanicalStep L1_L0R0_8[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_R1, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_1][L_0_R_0][8].Set(11, L1_L0R0_8, tempRot, tempHandState);
	//L1_L0R0_9
	MechanicalStep L1_L0R0_9[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_LC, M_L1, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_1][L_0_R_0][9].Set(11, L1_L0R0_9, tempRot, tempHandState);
	//L1_L0R0_10
	MechanicalStep L1_L0R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_0_R_0][10].Set(12, L1_L0R0_10, tempRot, tempHandState);
	//L1_L0R0_11
	MechanicalStep L1_L0R0_11[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_0_R_0][11].Set(12, L1_L0R0_11, tempRot, tempHandState);
	//L1_L0R0_12
	MechanicalStep L1_L0R0_12[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_0_R_0][12].Set(12, L1_L0R0_12, tempRot, tempHandState);
	//L1_L0R0_13
	MechanicalStep L1_L0R0_13[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_0_R_0][13].Set(12, L1_L0R0_13, tempRot, tempHandState);
	//L1_L0R0_14
	MechanicalStep L1_L0R0_14[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_1][L_0_R_0][14].Set(15, L1_L0R0_14, tempRot, tempHandState);
	//L1_L0R0_15
	MechanicalStep L1_L0R0_15[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_1][L_0_R_0][15].Set(15, L1_L0R0_15, tempRot, tempHandState);
}
void L2_L0R0Init(void)
{
	//L2_L0R0_0
	MechanicalStep L2_L0R0_0[] = { M_RO, M_L2, M_RC, M_R2, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_0_R_0][0].Set(4, L2_L0R0_0, tempRot, tempHandState);
	//L2_L0R0_1
	MechanicalStep L2_L0R0_1[] = { M_RO, M_L2, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_2][L_0_R_0][1].Set(5, L2_L0R0_1, tempRot, tempHandState);
	//L2_L0R0_2
	MechanicalStep L2_L0R0_2[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_0_R_0][2].Set(8, L2_L0R0_2, tempRot, tempHandState);
	//L2_L0R0_3
	MechanicalStep L2_L0R0_3[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_0_R_0][3].Set(8, L2_L0R0_3, tempRot, tempHandState);
	//L2_L0R0_4
	MechanicalStep L2_L0R0_4[] = { M_LO, M_R2, M_LC, M_RO, M_L2, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_2][L_0_R_0][4].Set(8, L2_L0R0_4, tempRot, tempHandState);
	//L2_L0R0_5
	MechanicalStep L2_L0R0_5[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_2][L_0_R_0][5].Set(9, L2_L0R0_5, tempRot, tempHandState);
	//L2_L0R0_6
	MechanicalStep L2_L0R0_6[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_2][L_0_R_0][6].Set(9, L2_L0R0_6, tempRot, tempHandState);
	//L2_L0R0_7
	MechanicalStep L2_L0R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_LC, M_L2, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_0_R_0][7].Set(11, L2_L0R0_7, tempRot, tempHandState);
	//L2_L0R0_8
	MechanicalStep L2_L0R0_8[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_LC, M_L2, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_0_R_0][8].Set(11, L2_L0R0_8, tempRot, tempHandState);
	//L2_L0R0_9
	MechanicalStep L2_L0R0_9[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_R2, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_0_R_0][9].Set(11, L2_L0R0_9, tempRot, tempHandState);
	//L2_L0R0_10
	MechanicalStep L2_L0R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_2][L_0_R_0][10].Set(12, L2_L0R0_10, tempRot, tempHandState);
	//L2_L0R0_11
	MechanicalStep L2_L0R0_11[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_2][L_0_R_0][11].Set(12, L2_L0R0_11, tempRot, tempHandState);
	//L2_L0R0_12
	MechanicalStep L2_L0R0_12[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_2][L_0_R_0][12].Set(12, L2_L0R0_12, tempRot, tempHandState);
	//L2_L0R0_13
	MechanicalStep L2_L0R0_13[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_2][L_0_R_0][13].Set(12, L2_L0R0_13, tempRot, tempHandState);
	//L2_L0R0_14
	MechanicalStep L2_L0R0_14[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_0_R_0][14].Set(15, L2_L0R0_14, tempRot, tempHandState);
	//L2_L0R0_15
	MechanicalStep L2_L0R0_15[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_0_R_0][15].Set(15, L2_L0R0_15, tempRot, tempHandState);
}
void L3_L0R0Init(void)
{
	//L3_L0R0_0
	MechanicalStep L3_L0R0_0[] = { M_RO, M_L2, M_RC, M_R3, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_3][L_0_R_0][0].Set(4, L3_L0R0_0, tempRot, tempHandState);
	//L3_L0R0_1
	MechanicalStep L3_L0R0_1[] = { M_RO, M_L2, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_0_R_0][1].Set(5, L3_L0R0_1, tempRot, tempHandState);
	//L3_L0R0_2
	MechanicalStep L3_L0R0_2[] = { M_LO, M_R2, M_LC, M_RO, M_L2, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_0_R_0][2].Set(8, L3_L0R0_2, tempRot, tempHandState);
	//L3_L0R0_3
	MechanicalStep L3_L0R0_3[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_3][L_0_R_0][3].Set(8, L3_L0R0_3, tempRot, tempHandState);
	//L3_L0R0_4
	MechanicalStep L3_L0R0_4[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_3][L_0_R_0][4].Set(8, L3_L0R0_4, tempRot, tempHandState);
	//L3_L0R0_5
	MechanicalStep L3_L0R0_5[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_0_R_0][5].Set(9, L3_L0R0_5, tempRot, tempHandState);
	//L3_L0R0_6
	MechanicalStep L3_L0R0_6[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_0_R_0][6].Set(9, L3_L0R0_6, tempRot, tempHandState);
	//L3_L0R0_7
	MechanicalStep L3_L0R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_LC, M_L3, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_3][L_0_R_0][7].Set(11, L3_L0R0_7, tempRot, tempHandState);
	//L3_L0R0_8
	MechanicalStep L3_L0R0_8[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_LC, M_L3, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_3][L_0_R_0][8].Set(11, L3_L0R0_8, tempRot, tempHandState);
	//L3_L0R0_9
	MechanicalStep L3_L0R0_9[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_R3, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_3][L_0_R_0][9].Set(11, L3_L0R0_9, tempRot, tempHandState);
	//L3_L0R0_10
	MechanicalStep L3_L0R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_0_R_0][10].Set(12, L3_L0R0_10, tempRot, tempHandState);
	//L3_L0R0_11
	MechanicalStep L3_L0R0_11[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_0_R_0][11].Set(12, L3_L0R0_11, tempRot, tempHandState);
	//L3_L0R0_12
	MechanicalStep L3_L0R0_12[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_0_R_0][12].Set(12, L3_L0R0_12, tempRot, tempHandState);
	//L3_L0R0_13
	MechanicalStep L3_L0R0_13[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_0_R_0][13].Set(12, L3_L0R0_13, tempRot, tempHandState);
	//L3_L0R0_14
	MechanicalStep L3_L0R0_14[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_3][L_0_R_0][14].Set(15, L3_L0R0_14, tempRot, tempHandState);
	//L3_L0R0_15
	MechanicalStep L3_L0R0_15[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_3][L_0_R_0][15].Set(15, L3_L0R0_15, tempRot, tempHandState);
}
void D1_L0R0Init(void)
{
	//D1_L0R0_0
	MechanicalStep D1_L0R0_0[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_RC, M_R1, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_1][L_0_R_0][0].Set(7, D1_L0R0_0, tempRot, tempHandState);
	//D1_L0R0_1
	MechanicalStep D1_L0R0_1[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_1][L_0_R_0][1].Set(7, D1_L0R0_1, tempRot, tempHandState);
	//D1_L0R0_2
	MechanicalStep D1_L0R0_2[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_0_R_0][2].Set(8, D1_L0R0_2, tempRot, tempHandState);
	//D1_L0R0_3
	MechanicalStep D1_L0R0_3[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_0_R_0][3].Set(8, D1_L0R0_3, tempRot, tempHandState);
	//D1_L0R0_4
	MechanicalStep D1_L0R0_4[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_0_R_0][4].Set(8, D1_L0R0_4, tempRot, tempHandState);
	//D1_L0R0_5
	MechanicalStep D1_L0R0_5[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_0_R_0][5].Set(8, D1_L0R0_5, tempRot, tempHandState);
	//D1_L0R0_6
	MechanicalStep D1_L0R0_6[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_0_R_0][6].Set(8, D1_L0R0_6, tempRot, tempHandState);
	//D1_L0R0_7
	MechanicalStep D1_L0R0_7[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_0_R_0][7].Set(8, D1_L0R0_7, tempRot, tempHandState);
	//D1_L0R0_8
	MechanicalStep D1_L0R0_8[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_RC, M_R1, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_1][L_0_R_0][8].Set(8, D1_L0R0_8, tempRot, tempHandState);
	//D1_L0R0_9
	MechanicalStep D1_L0R0_9[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_L1, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_1][L_0_R_0][9].Set(8, D1_L0R0_9, tempRot, tempHandState);
	//D1_L0R0_10
	MechanicalStep D1_L0R0_10[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_0_R_0][10].Set(9, D1_L0R0_10, tempRot, tempHandState);
	//D1_L0R0_11
	MechanicalStep D1_L0R0_11[] = { M_RO, M_L2, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_0_R_0][11].Set(9, D1_L0R0_11, tempRot, tempHandState);
	//D1_L0R0_12
	MechanicalStep D1_L0R0_12[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_1][L_0_R_0][12].Set(12, D1_L0R0_12, tempRot, tempHandState);
	//D1_L0R0_13
	MechanicalStep D1_L0R0_13[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_1][L_0_R_0][13].Set(12, D1_L0R0_13, tempRot, tempHandState);
	//D1_L0R0_14
	MechanicalStep D1_L0R0_14[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_1][L_0_R_0][14].Set(12, D1_L0R0_14, tempRot, tempHandState);
	//D1_L0R0_15
	MechanicalStep D1_L0R0_15[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_1][L_0_R_0][15].Set(12, D1_L0R0_15, tempRot, tempHandState);
}
void D2_L0R0Init(void)
{
	//D2_L0R0_0
	MechanicalStep D2_L0R0_0[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_RC, M_R2, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_0_R_0][0].Set(7, D2_L0R0_0, tempRot, tempHandState);
	//D2_L0R0_1
	MechanicalStep D2_L0R0_1[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_0_R_0][1].Set(7, D2_L0R0_1, tempRot, tempHandState);
	//D2_L0R0_2
	MechanicalStep D2_L0R0_2[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_RC, M_R2, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_0_R_0][2].Set(8, D2_L0R0_2, tempRot, tempHandState);
	//D2_L0R0_3
	MechanicalStep D2_L0R0_3[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_L2, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_0_R_0][3].Set(8, D2_L0R0_3, tempRot, tempHandState);
	//D2_L0R0_4
	MechanicalStep D2_L0R0_4[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_2][L_0_R_0][4].Set(8, D2_L0R0_4, tempRot, tempHandState);
	//D2_L0R0_5
	MechanicalStep D2_L0R0_5[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_2][L_0_R_0][5].Set(8, D2_L0R0_5, tempRot, tempHandState);
	//D2_L0R0_6
	MechanicalStep D2_L0R0_6[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_2][L_0_R_0][6].Set(8, D2_L0R0_6, tempRot, tempHandState);
	//D2_L0R0_7
	MechanicalStep D2_L0R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_2][L_0_R_0][7].Set(8, D2_L0R0_7, tempRot, tempHandState);
	//D2_L0R0_8
	MechanicalStep D2_L0R0_8[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_2][L_0_R_0][8].Set(8, D2_L0R0_8, tempRot, tempHandState);
	//D2_L0R0_9
	MechanicalStep D2_L0R0_9[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_2][L_0_R_0][9].Set(8, D2_L0R0_9, tempRot, tempHandState);
	//D2_L0R0_10
	MechanicalStep D2_L0R0_10[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_2][L_0_R_0][10].Set(9, D2_L0R0_10, tempRot, tempHandState);
	//D2_L0R0_11
	MechanicalStep D2_L0R0_11[] = { M_RO, M_L2, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_2][L_0_R_0][11].Set(9, D2_L0R0_11, tempRot, tempHandState);
	//D2_L0R0_12
	MechanicalStep D2_L0R0_12[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_0_R_0][12].Set(12, D2_L0R0_12, tempRot, tempHandState);
	//D2_L0R0_13
	MechanicalStep D2_L0R0_13[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_0_R_0][13].Set(12, D2_L0R0_13, tempRot, tempHandState);
	//D2_L0R0_14
	MechanicalStep D2_L0R0_14[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_0_R_0][14].Set(12, D2_L0R0_14, tempRot, tempHandState);
	//D2_L0R0_15
	MechanicalStep D2_L0R0_15[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_0_R_0][15].Set(12, D2_L0R0_15, tempRot, tempHandState);
}
void D3_L0R0Init(void)
{
	//D3_L0R0_0
	MechanicalStep D3_L0R0_0[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_RC, M_R3, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_3][L_0_R_0][0].Set(7, D3_L0R0_0, tempRot, tempHandState);
	//D3_L0R0_1
	MechanicalStep D3_L0R0_1[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_3][L_0_R_0][1].Set(7, D3_L0R0_1, tempRot, tempHandState);
	//D3_L0R0_2
	MechanicalStep D3_L0R0_2[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_0_R_0][2].Set(8, D3_L0R0_2, tempRot, tempHandState);
	//D3_L0R0_3
	MechanicalStep D3_L0R0_3[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_0_R_0][3].Set(8, D3_L0R0_3, tempRot, tempHandState);
	//D3_L0R0_4
	MechanicalStep D3_L0R0_4[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_0_R_0][4].Set(8, D3_L0R0_4, tempRot, tempHandState);
	//D3_L0R0_5
	MechanicalStep D3_L0R0_5[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_0_R_0][5].Set(8, D3_L0R0_5, tempRot, tempHandState);
	//D3_L0R0_6
	MechanicalStep D3_L0R0_6[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_0_R_0][6].Set(8, D3_L0R0_6, tempRot, tempHandState);
	//D3_L0R0_7
	MechanicalStep D3_L0R0_7[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_0_R_0][7].Set(8, D3_L0R0_7, tempRot, tempHandState);
	//D3_L0R0_8
	MechanicalStep D3_L0R0_8[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_RC, M_R3, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_3][L_0_R_0][8].Set(8, D3_L0R0_8, tempRot, tempHandState);
	//D3_L0R0_9
	MechanicalStep D3_L0R0_9[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_L3, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_3][L_0_R_0][9].Set(8, D3_L0R0_9, tempRot, tempHandState);
	//D3_L0R0_10
	MechanicalStep D3_L0R0_10[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_0_R_0][10].Set(9, D3_L0R0_10, tempRot, tempHandState);
	//D3_L0R0_11
	MechanicalStep D3_L0R0_11[] = { M_RO, M_L2, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_0_R_0][11].Set(9, D3_L0R0_11, tempRot, tempHandState);
	//D3_L0R0_12
	MechanicalStep D3_L0R0_12[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_3][L_0_R_0][12].Set(12, D3_L0R0_12, tempRot, tempHandState);
	//D3_L0R0_13
	MechanicalStep D3_L0R0_13[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_3][L_0_R_0][13].Set(12, D3_L0R0_13, tempRot, tempHandState);
	//D3_L0R0_14
	MechanicalStep D3_L0R0_14[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_3][L_0_R_0][14].Set(12, D3_L0R0_14, tempRot, tempHandState);
	//D3_L0R0_15
	MechanicalStep D3_L0R0_15[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_3][L_0_R_0][15].Set(12, D3_L0R0_15, tempRot, tempHandState);
}
void F1_L0R1Init(void)
{
	//F1_L0R1_0
	MechanicalStep F1_L0R1_0[] = { M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_1][L_0_R_1][0].Set(4, F1_L0R1_0, tempRot, tempHandState);
	//F1_L0R1_1
	MechanicalStep F1_L0R1_1[] = { M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_0_R_1][1].Set(5, F1_L0R1_1, tempRot, tempHandState);
	//F1_L0R1_2
	MechanicalStep F1_L0R1_2[] = { M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_0_R_1][2].Set(5, F1_L0R1_2, tempRot, tempHandState);
	//F1_L0R1_3
	MechanicalStep F1_L0R1_3[] = { M_RO, M_R1, M_L2, M_RC, M_L1, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_1][L_0_R_1][3].Set(5, F1_L0R1_3, tempRot, tempHandState);
	//F1_L0R1_4
	MechanicalStep F1_L0R1_4[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_RC, M_R1, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_1][L_0_R_1][4].Set(8, F1_L0R1_4, tempRot, tempHandState);
	//F1_L0R1_5
	MechanicalStep F1_L0R1_5[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_RC, M_R1, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_1][L_0_R_1][5].Set(8, F1_L0R1_5, tempRot, tempHandState);
	//F1_L0R1_6
	MechanicalStep F1_L0R1_6[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_0_R_1][6].Set(9, F1_L0R1_6, tempRot, tempHandState);
	//F1_L0R1_7
	MechanicalStep F1_L0R1_7[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_0_R_1][7].Set(9, F1_L0R1_7, tempRot, tempHandState);
	//F1_L0R1_8
	MechanicalStep F1_L0R1_8[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_0_R_1][8].Set(11, F1_L0R1_8, tempRot, tempHandState);
	//F1_L0R1_9
	MechanicalStep F1_L0R1_9[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_0_R_1][9].Set(11, F1_L0R1_9, tempRot, tempHandState);
	//F1_L0R1_10
	MechanicalStep F1_L0R1_10[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_0_R_1][10].Set(11, F1_L0R1_10, tempRot, tempHandState);
	//F1_L0R1_11
	MechanicalStep F1_L0R1_11[] = { M_LO, M_R1, M_LC, M_RO, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_0_R_1][11].Set(12, F1_L0R1_11, tempRot, tempHandState);
	//F1_L0R1_12
	MechanicalStep F1_L0R1_12[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_1][L_0_R_1][12].Set(12, F1_L0R1_12, tempRot, tempHandState);
	//F1_L0R1_13
	MechanicalStep F1_L0R1_13[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_1][L_0_R_1][13].Set(12, F1_L0R1_13, tempRot, tempHandState);
	//F1_L0R1_14
	MechanicalStep F1_L0R1_14[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_1][L_0_R_1][14].Set(19, F1_L0R1_14, tempRot, tempHandState);
	//F1_L0R1_15
	MechanicalStep F1_L0R1_15[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_1][L_0_R_1][15].Set(19, F1_L0R1_15, tempRot, tempHandState);
}
void F2_L0R1Init(void)
{
	//F2_L0R1_0
	MechanicalStep F2_L0R1_0[] = { M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_0_R_1][0].Set(4, F2_L0R1_0, tempRot, tempHandState);
	//F2_L0R1_1
	MechanicalStep F2_L0R1_1[] = { M_RO, M_R1, M_L2, M_RC, M_L2, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_0_R_1][1].Set(5, F2_L0R1_1, tempRot, tempHandState);
	//F2_L0R1_2
	MechanicalStep F2_L0R1_2[] = { M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_2][L_0_R_1][2].Set(5, F2_L0R1_2, tempRot, tempHandState);
	//F2_L0R1_3
	MechanicalStep F2_L0R1_3[] = { M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_2][L_0_R_1][3].Set(5, F2_L0R1_3, tempRot, tempHandState);
	//F2_L0R1_4
	MechanicalStep F2_L0R1_4[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_RC, M_R2, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_0_R_1][4].Set(8, F2_L0R1_4, tempRot, tempHandState);
	//F2_L0R1_5
	MechanicalStep F2_L0R1_5[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_RC, M_R2, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_0_R_1][5].Set(8, F2_L0R1_5, tempRot, tempHandState);
	//F2_L0R1_6
	MechanicalStep F2_L0R1_6[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_2][L_0_R_1][6].Set(9, F2_L0R1_6, tempRot, tempHandState);
	//F2_L0R1_7
	MechanicalStep F2_L0R1_7[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_2][L_0_R_1][7].Set(9, F2_L0R1_7, tempRot, tempHandState);
	//F2_L0R1_8
	MechanicalStep F2_L0R1_8[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_2][L_0_R_1][8].Set(11, F2_L0R1_8, tempRot, tempHandState);
	//F2_L0R1_9
	MechanicalStep F2_L0R1_9[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_2][L_0_R_1][9].Set(11, F2_L0R1_9, tempRot, tempHandState);
	//F2_L0R1_10
	MechanicalStep F2_L0R1_10[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_2][L_0_R_1][10].Set(11, F2_L0R1_10, tempRot, tempHandState);
	//F2_L0R1_11
	MechanicalStep F2_L0R1_11[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_0_R_1][11].Set(12, F2_L0R1_11, tempRot, tempHandState);
	//F2_L0R1_12
	MechanicalStep F2_L0R1_12[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_0_R_1][12].Set(12, F2_L0R1_12, tempRot, tempHandState);
	//F2_L0R1_13
	MechanicalStep F2_L0R1_13[] = { M_LO, M_R1, M_LC, M_RO, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_2][L_0_R_1][13].Set(12, F2_L0R1_13, tempRot, tempHandState);
	//F2_L0R1_14
	MechanicalStep F2_L0R1_14[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_0_R_1][14].Set(19, F2_L0R1_14, tempRot, tempHandState);
	//F2_L0R1_15
	MechanicalStep F2_L0R1_15[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_0_R_1][15].Set(19, F2_L0R1_15, tempRot, tempHandState);
}
void F3_L0R1Init(void)
{
	//F3_L0R1_0
	MechanicalStep F3_L0R1_0[] = { M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_3][L_0_R_1][0].Set(4, F3_L0R1_0, tempRot, tempHandState);
	//F3_L0R1_1
	MechanicalStep F3_L0R1_1[] = { M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_0_R_1][1].Set(5, F3_L0R1_1, tempRot, tempHandState);
	//F3_L0R1_2
	MechanicalStep F3_L0R1_2[] = { M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_0_R_1][2].Set(5, F3_L0R1_2, tempRot, tempHandState);
	//F3_L0R1_3
	MechanicalStep F3_L0R1_3[] = { M_RO, M_R1, M_L2, M_RC, M_L3, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_3][L_0_R_1][3].Set(5, F3_L0R1_3, tempRot, tempHandState);
	//F3_L0R1_4
	MechanicalStep F3_L0R1_4[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_RC, M_R3, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_3][L_0_R_1][4].Set(8, F3_L0R1_4, tempRot, tempHandState);
	//F3_L0R1_5
	MechanicalStep F3_L0R1_5[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_RC, M_R3, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_3][L_0_R_1][5].Set(8, F3_L0R1_5, tempRot, tempHandState);
	//F3_L0R1_6
	MechanicalStep F3_L0R1_6[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_0_R_1][6].Set(9, F3_L0R1_6, tempRot, tempHandState);
	//F3_L0R1_7
	MechanicalStep F3_L0R1_7[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_0_R_1][7].Set(9, F3_L0R1_7, tempRot, tempHandState);
	//F3_L0R1_8
	MechanicalStep F3_L0R1_8[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_0_R_1][8].Set(11, F3_L0R1_8, tempRot, tempHandState);
	//F3_L0R1_9
	MechanicalStep F3_L0R1_9[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_0_R_1][9].Set(11, F3_L0R1_9, tempRot, tempHandState);
	//F3_L0R1_10
	MechanicalStep F3_L0R1_10[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_0_R_1][10].Set(11, F3_L0R1_10, tempRot, tempHandState);
	//F3_L0R1_11
	MechanicalStep F3_L0R1_11[] = { M_LO, M_R1, M_LC, M_RO, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_0_R_1][11].Set(12, F3_L0R1_11, tempRot, tempHandState);
	//F3_L0R1_12
	MechanicalStep F3_L0R1_12[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_3][L_0_R_1][12].Set(12, F3_L0R1_12, tempRot, tempHandState);
	//F3_L0R1_13
	MechanicalStep F3_L0R1_13[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_3][L_0_R_1][13].Set(12, F3_L0R1_13, tempRot, tempHandState);
	//F3_L0R1_14
	MechanicalStep F3_L0R1_14[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_3][L_0_R_1][14].Set(19, F3_L0R1_14, tempRot, tempHandState);
	//F3_L0R1_15
	MechanicalStep F3_L0R1_15[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_3][L_0_R_1][15].Set(19, F3_L0R1_15, tempRot, tempHandState);
}
void R1_L0R1Init(void)
{
	//R1_L0R1_0
	MechanicalStep R1_L0R1_0[] = { M_R1, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_0_R_1][0].Set(1, R1_L0R1_0, tempRot, tempHandState);
	//R1_L0R1_1
	MechanicalStep R1_L0R1_1[] = { M_LO, M_R2, M_LC, M_R1, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_0_R_1][1].Set(4, R1_L0R1_1, tempRot, tempHandState);
	//R1_L0R1_2
	MechanicalStep R1_L0R1_2[] = { M_RO, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_1][L_0_R_1][2].Set(4, R1_L0R1_2, tempRot, tempHandState);
	//R1_L0R1_3
	MechanicalStep R1_L0R1_3[] = { M_LO, M_R3, M_LC, M_R1, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_1][L_0_R_1][3].Set(4, R1_L0R1_3, tempRot, tempHandState);
	//R1_L0R1_4
	MechanicalStep R1_L0R1_4[] = { M_LO, M_R1, M_LC, M_R1, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_1][L_0_R_1][4].Set(4, R1_L0R1_4, tempRot, tempHandState);
	//R1_L0R1_5
	MechanicalStep R1_L0R1_5[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_LC, M_L1, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_1][L_0_R_1][5].Set(12, R1_L0R1_5, tempRot, tempHandState);
	//R1_L0R1_6
	MechanicalStep R1_L0R1_6[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_LC, M_L1, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_1][L_0_R_1][6].Set(12, R1_L0R1_6, tempRot, tempHandState);
	//R1_L0R1_7
	MechanicalStep R1_L0R1_7[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_1][L_0_R_1][7].Set(12, R1_L0R1_7, tempRot, tempHandState);
	//R1_L0R1_8
	MechanicalStep R1_L0R1_8[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_1][L_0_R_1][8].Set(12, R1_L0R1_8, tempRot, tempHandState);
	//R1_L0R1_9
	MechanicalStep R1_L0R1_9[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_0_R_1][9].Set(13, R1_L0R1_9, tempRot, tempHandState);
	//R1_L0R1_10
	MechanicalStep R1_L0R1_10[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_0_R_1][10].Set(13, R1_L0R1_10, tempRot, tempHandState);
	//R1_L0R1_11
	MechanicalStep R1_L0R1_11[] = { M_RO, M_R1, M_L2, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_0_R_1][11].Set(13, R1_L0R1_11, tempRot, tempHandState);
	//R1_L0R1_12
	MechanicalStep R1_L0R1_12[] = { M_RO, M_R1, M_L2, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_0_R_1][12].Set(13, R1_L0R1_12, tempRot, tempHandState);
	//R1_L0R1_13
	MechanicalStep R1_L0R1_13[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_RC, M_R1, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_1][L_0_R_1][13].Set(13, R1_L0R1_13, tempRot, tempHandState);
	//R1_L0R1_14
	MechanicalStep R1_L0R1_14[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_0_R_1][14].Set(15, R1_L0R1_14, tempRot, tempHandState);
	//R1_L0R1_15
	MechanicalStep R1_L0R1_15[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_0_R_1][15].Set(15, R1_L0R1_15, tempRot, tempHandState);
}
void R2_L0R1Init(void)
{
	//R2_L0R1_0
	MechanicalStep R2_L0R1_0[] = { M_R2, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_2][L_0_R_1][0].Set(1, R2_L0R1_0, tempRot, tempHandState);
	//R2_L0R1_1
	MechanicalStep R2_L0R1_1[] = { M_LO, M_R3, M_LC, M_R2, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_0_R_1][1].Set(4, R2_L0R1_1, tempRot, tempHandState);
	//R2_L0R1_2
	MechanicalStep R2_L0R1_2[] = { M_RO, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_0_R_1][2].Set(4, R2_L0R1_2, tempRot, tempHandState);
	//R2_L0R1_3
	MechanicalStep R2_L0R1_3[] = { M_LO, M_R1, M_LC, M_R2, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_0_R_1][3].Set(4, R2_L0R1_3, tempRot, tempHandState);
	//R2_L0R1_4
	MechanicalStep R2_L0R1_4[] = { M_LO, M_R2, M_LC, M_R2, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_2][L_0_R_1][4].Set(4, R2_L0R1_4, tempRot, tempHandState);
	//R2_L0R1_5
	MechanicalStep R2_L0R1_5[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_LC, M_L2, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_0_R_1][5].Set(12, R2_L0R1_5, tempRot, tempHandState);
	//R2_L0R1_6
	MechanicalStep R2_L0R1_6[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_LC, M_L2, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_0_R_1][6].Set(12, R2_L0R1_6, tempRot, tempHandState);
	//R2_L0R1_7
	MechanicalStep R2_L0R1_7[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_0_R_1][7].Set(12, R2_L0R1_7, tempRot, tempHandState);
	//R2_L0R1_8
	MechanicalStep R2_L0R1_8[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_0_R_1][8].Set(12, R2_L0R1_8, tempRot, tempHandState);
	//R2_L0R1_9
	MechanicalStep R2_L0R1_9[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_RC, M_R2, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_0_R_1][9].Set(13, R2_L0R1_9, tempRot, tempHandState);
	//R2_L0R1_10
	MechanicalStep R2_L0R1_10[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_2][L_0_R_1][10].Set(13, R2_L0R1_10, tempRot, tempHandState);
	//R2_L0R1_11
	MechanicalStep R2_L0R1_11[] = { M_RO, M_R1, M_L2, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_2][L_0_R_1][11].Set(13, R2_L0R1_11, tempRot, tempHandState);
	//R2_L0R1_12
	MechanicalStep R2_L0R1_12[] = { M_RO, M_R1, M_L2, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_2][L_0_R_1][12].Set(13, R2_L0R1_12, tempRot, tempHandState);
	//R2_L0R1_13
	MechanicalStep R2_L0R1_13[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_2][L_0_R_1][13].Set(13, R2_L0R1_13, tempRot, tempHandState);
	//R2_L0R1_14
	MechanicalStep R2_L0R1_14[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_2][L_0_R_1][14].Set(15, R2_L0R1_14, tempRot, tempHandState);
	//R2_L0R1_15
	MechanicalStep R2_L0R1_15[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_2][L_0_R_1][15].Set(15, R2_L0R1_15, tempRot, tempHandState);
}
void R3_L0R1Init(void)
{
	//R3_L0R1_0
	MechanicalStep R3_L0R1_0[] = { M_R3, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_0_R_1][0].Set(1, R3_L0R1_0, tempRot, tempHandState);
	//R3_L0R1_1
	MechanicalStep R3_L0R1_1[] = { M_LO, M_R2, M_LC, M_R3, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_0_R_1][1].Set(4, R3_L0R1_1, tempRot, tempHandState);
	//R3_L0R1_2
	MechanicalStep R3_L0R1_2[] = { M_RO, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_3][L_0_R_1][2].Set(4, R3_L0R1_2, tempRot, tempHandState);
	//R3_L0R1_3
	MechanicalStep R3_L0R1_3[] = { M_LO, M_R3, M_LC, M_R3, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_3][L_0_R_1][3].Set(4, R3_L0R1_3, tempRot, tempHandState);
	//R3_L0R1_4
	MechanicalStep R3_L0R1_4[] = { M_LO, M_R1, M_LC, M_R3, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_3][L_0_R_1][4].Set(4, R3_L0R1_4, tempRot, tempHandState);
	//R3_L0R1_5
	MechanicalStep R3_L0R1_5[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_LC, M_L3, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_3][L_0_R_1][5].Set(12, R3_L0R1_5, tempRot, tempHandState);
	//R3_L0R1_6
	MechanicalStep R3_L0R1_6[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_LC, M_L3, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_3][L_0_R_1][6].Set(12, R3_L0R1_6, tempRot, tempHandState);
	//R3_L0R1_7
	MechanicalStep R3_L0R1_7[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_3][L_0_R_1][7].Set(12, R3_L0R1_7, tempRot, tempHandState);
	//R3_L0R1_8
	MechanicalStep R3_L0R1_8[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_3][L_0_R_1][8].Set(12, R3_L0R1_8, tempRot, tempHandState);
	//R3_L0R1_9
	MechanicalStep R3_L0R1_9[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_0_R_1][9].Set(13, R3_L0R1_9, tempRot, tempHandState);
	//R3_L0R1_10
	MechanicalStep R3_L0R1_10[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_0_R_1][10].Set(13, R3_L0R1_10, tempRot, tempHandState);
	//R3_L0R1_11
	MechanicalStep R3_L0R1_11[] = { M_RO, M_R1, M_L2, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_0_R_1][11].Set(13, R3_L0R1_11, tempRot, tempHandState);
	//R3_L0R1_12
	MechanicalStep R3_L0R1_12[] = { M_RO, M_R1, M_L2, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_0_R_1][12].Set(13, R3_L0R1_12, tempRot, tempHandState);
	//R3_L0R1_13
	MechanicalStep R3_L0R1_13[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_RC, M_R3, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_3][L_0_R_1][13].Set(13, R3_L0R1_13, tempRot, tempHandState);
	//R3_L0R1_14
	MechanicalStep R3_L0R1_14[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_0_R_1][14].Set(15, R3_L0R1_14, tempRot, tempHandState);
	//R3_L0R1_15
	MechanicalStep R3_L0R1_15[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_0_R_1][15].Set(15, R3_L0R1_15, tempRot, tempHandState);
}
void U1_L0R1Init(void)
{
	//U1_L0R1_0
	MechanicalStep U1_L0R1_0[] = { M_LO, M_R3, M_LC, M_L1, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_1][L_0_R_1][0].Set(4, U1_L0R1_0, tempRot, tempHandState);
	//U1_L0R1_1
	MechanicalStep U1_L0R1_1[] = { M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_0_R_1][1].Set(5, U1_L0R1_1, tempRot, tempHandState);
	//U1_L0R1_2
	MechanicalStep U1_L0R1_2[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_1][L_0_R_1][2].Set(8, U1_L0R1_2, tempRot, tempHandState);
	//U1_L0R1_3
	MechanicalStep U1_L0R1_3[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_0_R_1][3].Set(9, U1_L0R1_3, tempRot, tempHandState);
	//U1_L0R1_4
	MechanicalStep U1_L0R1_4[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_0_R_1][4].Set(9, U1_L0R1_4, tempRot, tempHandState);
	//U1_L0R1_5
	MechanicalStep U1_L0R1_5[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_R1, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_1][L_0_R_1][5].Set(9, U1_L0R1_5, tempRot, tempHandState);
	//U1_L0R1_6
	MechanicalStep U1_L0R1_6[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_LC, M_L1, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_1][L_0_R_1][6].Set(9, U1_L0R1_6, tempRot, tempHandState);
	//U1_L0R1_7
	MechanicalStep U1_L0R1_7[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_0_R_1][7].Set(10, U1_L0R1_7, tempRot, tempHandState);
	//U1_L0R1_8
	MechanicalStep U1_L0R1_8[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_0_R_1][8].Set(11, U1_L0R1_8, tempRot, tempHandState);
	//U1_L0R1_9
	MechanicalStep U1_L0R1_9[] = { M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_0_R_1][9].Set(11, U1_L0R1_9, tempRot, tempHandState);
	//U1_L0R1_10
	MechanicalStep U1_L0R1_10[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_0_R_1][10].Set(11, U1_L0R1_10, tempRot, tempHandState);
	//U1_L0R1_11
	MechanicalStep U1_L0R1_11[] = { M_RO, M_R1, M_RC, M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_0_R_1][11].Set(12, U1_L0R1_11, tempRot, tempHandState);
	//U1_L0R1_12
	MechanicalStep U1_L0R1_12[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_1][L_0_R_1][12].Set(13, U1_L0R1_12, tempRot, tempHandState);
	//U1_L0R1_13
	MechanicalStep U1_L0R1_13[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_1][L_0_R_1][13].Set(13, U1_L0R1_13, tempRot, tempHandState);
	//U1_L0R1_14
	MechanicalStep U1_L0R1_14[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_1][L_0_R_1][14].Set(15, U1_L0R1_14, tempRot, tempHandState);
	//U1_L0R1_15
	MechanicalStep U1_L0R1_15[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_1][L_0_R_1][15].Set(15, U1_L0R1_15, tempRot, tempHandState);
}
void U2_L0R1Init(void)
{
	//U2_L0R1_0
	MechanicalStep U2_L0R1_0[] = { M_LO, M_R3, M_LC, M_L2, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_0_R_1][0].Set(4, U2_L0R1_0, tempRot, tempHandState);
	//U2_L0R1_1
	MechanicalStep U2_L0R1_1[] = { M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_2][L_0_R_1][1].Set(5, U2_L0R1_1, tempRot, tempHandState);
	//U2_L0R1_2
	MechanicalStep U2_L0R1_2[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_0_R_1][2].Set(8, U2_L0R1_2, tempRot, tempHandState);
	//U2_L0R1_3
	MechanicalStep U2_L0R1_3[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_LC, M_L2, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_0_R_1][3].Set(9, U2_L0R1_3, tempRot, tempHandState);
	//U2_L0R1_4
	MechanicalStep U2_L0R1_4[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_R2, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_0_R_1][4].Set(9, U2_L0R1_4, tempRot, tempHandState);
	//U2_L0R1_5
	MechanicalStep U2_L0R1_5[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_2][L_0_R_1][5].Set(9, U2_L0R1_5, tempRot, tempHandState);
	//U2_L0R1_6
	MechanicalStep U2_L0R1_6[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_2][L_0_R_1][6].Set(9, U2_L0R1_6, tempRot, tempHandState);
	//U2_L0R1_7
	MechanicalStep U2_L0R1_7[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_2][L_0_R_1][7].Set(10, U2_L0R1_7, tempRot, tempHandState);
	//U2_L0R1_8
	MechanicalStep U2_L0R1_8[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_2][L_0_R_1][8].Set(11, U2_L0R1_8, tempRot, tempHandState);
	//U2_L0R1_9
	MechanicalStep U2_L0R1_9[] = { M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_2][L_0_R_1][9].Set(11, U2_L0R1_9, tempRot, tempHandState);
	//U2_L0R1_10
	MechanicalStep U2_L0R1_10[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_2][L_0_R_1][10].Set(11, U2_L0R1_10, tempRot, tempHandState);
	//U2_L0R1_11
	MechanicalStep U2_L0R1_11[] = { M_RO, M_R1, M_RC, M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_2][L_0_R_1][11].Set(12, U2_L0R1_11, tempRot, tempHandState);
	//U2_L0R1_12
	MechanicalStep U2_L0R1_12[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_0_R_1][12].Set(13, U2_L0R1_12, tempRot, tempHandState);
	//U2_L0R1_13
	MechanicalStep U2_L0R1_13[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_0_R_1][13].Set(13, U2_L0R1_13, tempRot, tempHandState);
	//U2_L0R1_14
	MechanicalStep U2_L0R1_14[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_0_R_1][14].Set(15, U2_L0R1_14, tempRot, tempHandState);
	//U2_L0R1_15
	MechanicalStep U2_L0R1_15[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_0_R_1][15].Set(15, U2_L0R1_15, tempRot, tempHandState);
}
void U3_L0R1Init(void)
{
	//U3_L0R1_0
	MechanicalStep U3_L0R1_0[] = { M_LO, M_R3, M_LC, M_L3, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_3][L_0_R_1][0].Set(4, U3_L0R1_0, tempRot, tempHandState);
	//U3_L0R1_1
	MechanicalStep U3_L0R1_1[] = { M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_0_R_1][1].Set(5, U3_L0R1_1, tempRot, tempHandState);
	//U3_L0R1_2
	MechanicalStep U3_L0R1_2[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_3][L_0_R_1][2].Set(8, U3_L0R1_2, tempRot, tempHandState);
	//U3_L0R1_3
	MechanicalStep U3_L0R1_3[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_0_R_1][3].Set(9, U3_L0R1_3, tempRot, tempHandState);
	//U3_L0R1_4
	MechanicalStep U3_L0R1_4[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_0_R_1][4].Set(9, U3_L0R1_4, tempRot, tempHandState);
	//U3_L0R1_5
	MechanicalStep U3_L0R1_5[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_R3, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_3][L_0_R_1][5].Set(9, U3_L0R1_5, tempRot, tempHandState);
	//U3_L0R1_6
	MechanicalStep U3_L0R1_6[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_LC, M_L3, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_3][L_0_R_1][6].Set(9, U3_L0R1_6, tempRot, tempHandState);
	//U3_L0R1_7
	MechanicalStep U3_L0R1_7[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_0_R_1][7].Set(10, U3_L0R1_7, tempRot, tempHandState);
	//U3_L0R1_8
	MechanicalStep U3_L0R1_8[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_0_R_1][8].Set(11, U3_L0R1_8, tempRot, tempHandState);
	//U3_L0R1_9
	MechanicalStep U3_L0R1_9[] = { M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_0_R_1][9].Set(11, U3_L0R1_9, tempRot, tempHandState);
	//U3_L0R1_10
	MechanicalStep U3_L0R1_10[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_0_R_1][10].Set(11, U3_L0R1_10, tempRot, tempHandState);
	//U3_L0R1_11
	MechanicalStep U3_L0R1_11[] = { M_RO, M_R1, M_RC, M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_0_R_1][11].Set(12, U3_L0R1_11, tempRot, tempHandState);
	//U3_L0R1_12
	MechanicalStep U3_L0R1_12[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_3][L_0_R_1][12].Set(13, U3_L0R1_12, tempRot, tempHandState);
	//U3_L0R1_13
	MechanicalStep U3_L0R1_13[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_3][L_0_R_1][13].Set(13, U3_L0R1_13, tempRot, tempHandState);
	//U3_L0R1_14
	MechanicalStep U3_L0R1_14[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_3][L_0_R_1][14].Set(15, U3_L0R1_14, tempRot, tempHandState);
	//U3_L0R1_15
	MechanicalStep U3_L0R1_15[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_3][L_0_R_1][15].Set(15, U3_L0R1_15, tempRot, tempHandState);
}
void B1_L0R1Init(void)
{
	//B1_L0R1_0
	MechanicalStep B1_L0R1_0[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_1][L_0_R_1][0].Set(7, B1_L0R1_0, tempRot, tempHandState);
	//B1_L0R1_1
	MechanicalStep B1_L0R1_1[] = { M_RO, M_R1, M_RC, M_LO, M_R2, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_0_R_1][1].Set(8, B1_L0R1_1, tempRot, tempHandState);
	//B1_L0R1_2
	MechanicalStep B1_L0R1_2[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_0_R_1][2].Set(8, B1_L0R1_2, tempRot, tempHandState);
	//B1_L0R1_3
	MechanicalStep B1_L0R1_3[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_0_R_1][3].Set(8, B1_L0R1_3, tempRot, tempHandState);
	//B1_L0R1_4
	MechanicalStep B1_L0R1_4[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_RC, M_R1, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_1][L_0_R_1][4].Set(8, B1_L0R1_4, tempRot, tempHandState);
	//B1_L0R1_5
	MechanicalStep B1_L0R1_5[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_RC, M_R1, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_1][L_0_R_1][5].Set(8, B1_L0R1_5, tempRot, tempHandState);
	//B1_L0R1_6
	MechanicalStep B1_L0R1_6[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L2, M_RC, M_L1, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_1][L_0_R_1][6].Set(8, B1_L0R1_6, tempRot, tempHandState);
	//B1_L0R1_7
	MechanicalStep B1_L0R1_7[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_0_R_1][7].Set(9, B1_L0R1_7, tempRot, tempHandState);
	//B1_L0R1_8
	MechanicalStep B1_L0R1_8[] = { M_RO, M_R1, M_L2, M_RC, M_LO, M_R2, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_0_R_1][8].Set(9, B1_L0R1_8, tempRot, tempHandState);
	//B1_L0R1_9
	MechanicalStep B1_L0R1_9[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_0_R_1][9].Set(9, B1_L0R1_9, tempRot, tempHandState);
	//B1_L0R1_10
	MechanicalStep B1_L0R1_10[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_1][L_0_R_1][10].Set(9, B1_L0R1_10, tempRot, tempHandState);
	//B1_L0R1_11
	MechanicalStep B1_L0R1_11[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_1][L_0_R_1][11].Set(9, B1_L0R1_11, tempRot, tempHandState);
	//B1_L0R1_12
	MechanicalStep B1_L0R1_12[] = { M_LO, M_R1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_0_R_1][12].Set(11, B1_L0R1_12, tempRot, tempHandState);
	//B1_L0R1_13
	MechanicalStep B1_L0R1_13[] = { M_LO, M_R1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_0_R_1][13].Set(11, B1_L0R1_13, tempRot, tempHandState);
	//B1_L0R1_14
	MechanicalStep B1_L0R1_14[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_1][L_0_R_1][14].Set(16, B1_L0R1_14, tempRot, tempHandState);
	//B1_L0R1_15
	MechanicalStep B1_L0R1_15[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_1][L_0_R_1][15].Set(16, B1_L0R1_15, tempRot, tempHandState);
}
void B2_L0R1Init(void)
{
	//B2_L0R1_0
	MechanicalStep B2_L0R1_0[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_0_R_1][0].Set(7, B2_L0R1_0, tempRot, tempHandState);
	//B2_L0R1_1
	MechanicalStep B2_L0R1_1[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_RC, M_R2, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_0_R_1][1].Set(8, B2_L0R1_1, tempRot, tempHandState);
	//B2_L0R1_2
	MechanicalStep B2_L0R1_2[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_RC, M_R2, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_0_R_1][2].Set(8, B2_L0R1_2, tempRot, tempHandState);
	//B2_L0R1_3
	MechanicalStep B2_L0R1_3[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L2, M_RC, M_L2, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_0_R_1][3].Set(8, B2_L0R1_3, tempRot, tempHandState);
	//B2_L0R1_4
	MechanicalStep B2_L0R1_4[] = { M_RO, M_R1, M_RC, M_LO, M_R2, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_2][L_0_R_1][4].Set(8, B2_L0R1_4, tempRot, tempHandState);
	//B2_L0R1_5
	MechanicalStep B2_L0R1_5[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_2][L_0_R_1][5].Set(8, B2_L0R1_5, tempRot, tempHandState);
	//B2_L0R1_6
	MechanicalStep B2_L0R1_6[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_2][L_0_R_1][6].Set(8, B2_L0R1_6, tempRot, tempHandState);
	//B2_L0R1_7
	MechanicalStep B2_L0R1_7[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_0_R_1][7].Set(9, B2_L0R1_7, tempRot, tempHandState);
	//B2_L0R1_8
	MechanicalStep B2_L0R1_8[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_0_R_1][8].Set(9, B2_L0R1_8, tempRot, tempHandState);
	//B2_L0R1_9
	MechanicalStep B2_L0R1_9[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_2][L_0_R_1][9].Set(9, B2_L0R1_9, tempRot, tempHandState);
	//B2_L0R1_10
	MechanicalStep B2_L0R1_10[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_2][L_0_R_1][10].Set(9, B2_L0R1_10, tempRot, tempHandState);
	//B2_L0R1_11
	MechanicalStep B2_L0R1_11[] = { M_RO, M_R1, M_L2, M_RC, M_LO, M_R2, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_2][L_0_R_1][11].Set(9, B2_L0R1_11, tempRot, tempHandState);
	//B2_L0R1_12
	MechanicalStep B2_L0R1_12[] = { M_LO, M_R1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_2][L_0_R_1][12].Set(11, B2_L0R1_12, tempRot, tempHandState);
	//B2_L0R1_13
	MechanicalStep B2_L0R1_13[] = { M_LO, M_R1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_2][L_0_R_1][13].Set(11, B2_L0R1_13, tempRot, tempHandState);
	//B2_L0R1_14
	MechanicalStep B2_L0R1_14[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_0_R_1][14].Set(16, B2_L0R1_14, tempRot, tempHandState);
	//B2_L0R1_15
	MechanicalStep B2_L0R1_15[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_0_R_1][15].Set(16, B2_L0R1_15, tempRot, tempHandState);
}
void B3_L0R1Init(void)
{
	//B3_L0R1_0
	MechanicalStep B3_L0R1_0[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_3][L_0_R_1][0].Set(7, B3_L0R1_0, tempRot, tempHandState);
	//B3_L0R1_1
	MechanicalStep B3_L0R1_1[] = { M_RO, M_R1, M_RC, M_LO, M_R2, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_0_R_1][1].Set(8, B3_L0R1_1, tempRot, tempHandState);
	//B3_L0R1_2
	MechanicalStep B3_L0R1_2[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_0_R_1][2].Set(8, B3_L0R1_2, tempRot, tempHandState);
	//B3_L0R1_3
	MechanicalStep B3_L0R1_3[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_0_R_1][3].Set(8, B3_L0R1_3, tempRot, tempHandState);
	//B3_L0R1_4
	MechanicalStep B3_L0R1_4[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_RC, M_R3, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_3][L_0_R_1][4].Set(8, B3_L0R1_4, tempRot, tempHandState);
	//B3_L0R1_5
	MechanicalStep B3_L0R1_5[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_RC, M_R3, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_3][L_0_R_1][5].Set(8, B3_L0R1_5, tempRot, tempHandState);
	//B3_L0R1_6
	MechanicalStep B3_L0R1_6[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L2, M_RC, M_L3, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_3][L_0_R_1][6].Set(8, B3_L0R1_6, tempRot, tempHandState);
	//B3_L0R1_7
	MechanicalStep B3_L0R1_7[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_0_R_1][7].Set(9, B3_L0R1_7, tempRot, tempHandState);
	//B3_L0R1_8
	MechanicalStep B3_L0R1_8[] = { M_RO, M_R1, M_L2, M_RC, M_LO, M_R2, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_0_R_1][8].Set(9, B3_L0R1_8, tempRot, tempHandState);
	//B3_L0R1_9
	MechanicalStep B3_L0R1_9[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_0_R_1][9].Set(9, B3_L0R1_9, tempRot, tempHandState);
	//B3_L0R1_10
	MechanicalStep B3_L0R1_10[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_3][L_0_R_1][10].Set(9, B3_L0R1_10, tempRot, tempHandState);
	//B3_L0R1_11
	MechanicalStep B3_L0R1_11[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_3][L_0_R_1][11].Set(9, B3_L0R1_11, tempRot, tempHandState);
	//B3_L0R1_12
	MechanicalStep B3_L0R1_12[] = { M_LO, M_R1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_0_R_1][12].Set(11, B3_L0R1_12, tempRot, tempHandState);
	//B3_L0R1_13
	MechanicalStep B3_L0R1_13[] = { M_LO, M_R1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_0_R_1][13].Set(11, B3_L0R1_13, tempRot, tempHandState);
	//B3_L0R1_14
	MechanicalStep B3_L0R1_14[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_3][L_0_R_1][14].Set(16, B3_L0R1_14, tempRot, tempHandState);
	//B3_L0R1_15
	MechanicalStep B3_L0R1_15[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_3][L_0_R_1][15].Set(16, B3_L0R1_15, tempRot, tempHandState);
}
void L1_L0R1Init(void)
{
	//L1_L0R1_0
	MechanicalStep L1_L0R1_0[] = { M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_1][L_0_R_1][0].Set(5, L1_L0R1_0, tempRot, tempHandState);
	//L1_L0R1_1
	MechanicalStep L1_L0R1_1[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_0_R_1][1].Set(6, L1_L0R1_1, tempRot, tempHandState);
	//L1_L0R1_2
	MechanicalStep L1_L0R1_2[] = { M_LO, M_R1, M_LC, M_RO, M_L2, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_0_R_1][2].Set(8, L1_L0R1_2, tempRot, tempHandState);
	//L1_L0R1_3
	MechanicalStep L1_L0R1_3[] = { M_LO, M_R3, M_LC, M_RO, M_L2, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_0_R_1][3].Set(8, L1_L0R1_3, tempRot, tempHandState);
	//L1_L0R1_4
	MechanicalStep L1_L0R1_4[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_1][L_0_R_1][4].Set(8, L1_L0R1_4, tempRot, tempHandState);
	//L1_L0R1_5
	MechanicalStep L1_L0R1_5[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_0_R_1][5].Set(9, L1_L0R1_5, tempRot, tempHandState);
	//L1_L0R1_6
	MechanicalStep L1_L0R1_6[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_1][L_0_R_1][6].Set(11, L1_L0R1_6, tempRot, tempHandState);
	//L1_L0R1_7
	MechanicalStep L1_L0R1_7[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_1][L_0_R_1][7].Set(11, L1_L0R1_7, tempRot, tempHandState);
	//L1_L0R1_8
	MechanicalStep L1_L0R1_8[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_LC, M_L1, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_1][L_0_R_1][8].Set(12, L1_L0R1_8, tempRot, tempHandState);
	//L1_L0R1_9
	MechanicalStep L1_L0R1_9[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_LC, M_L1, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_1][L_0_R_1][9].Set(12, L1_L0R1_9, tempRot, tempHandState);
	//L1_L0R1_10
	MechanicalStep L1_L0R1_10[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_1][L_0_R_1][10].Set(12, L1_L0R1_10, tempRot, tempHandState);
	//L1_L0R1_11
	MechanicalStep L1_L0R1_11[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_1][L_0_R_1][11].Set(12, L1_L0R1_11, tempRot, tempHandState);
	//L1_L0R1_12
	MechanicalStep L1_L0R1_12[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_0_R_1][12].Set(13, L1_L0R1_12, tempRot, tempHandState);
	//L1_L0R1_13
	MechanicalStep L1_L0R1_13[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_0_R_1][13].Set(13, L1_L0R1_13, tempRot, tempHandState);
	//L1_L0R1_14
	MechanicalStep L1_L0R1_14[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_0_R_1][14].Set(15, L1_L0R1_14, tempRot, tempHandState);
	//L1_L0R1_15
	MechanicalStep L1_L0R1_15[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_0_R_1][15].Set(15, L1_L0R1_15, tempRot, tempHandState);
}
void L2_L0R1Init(void)
{
	//L2_L0R1_0
	MechanicalStep L2_L0R1_0[] = { M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_0_R_1][0].Set(5, L2_L0R1_0, tempRot, tempHandState);
	//L2_L0R1_1
	MechanicalStep L2_L0R1_1[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_2][L_0_R_1][1].Set(6, L2_L0R1_1, tempRot, tempHandState);
	//L2_L0R1_2
	MechanicalStep L2_L0R1_2[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_0_R_1][2].Set(8, L2_L0R1_2, tempRot, tempHandState);
	//L2_L0R1_3
	MechanicalStep L2_L0R1_3[] = { M_LO, M_R1, M_LC, M_RO, M_L2, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_2][L_0_R_1][3].Set(8, L2_L0R1_3, tempRot, tempHandState);
	//L2_L0R1_4
	MechanicalStep L2_L0R1_4[] = { M_LO, M_R3, M_LC, M_RO, M_L2, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_2][L_0_R_1][4].Set(8, L2_L0R1_4, tempRot, tempHandState);
	//L2_L0R1_5
	MechanicalStep L2_L0R1_5[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_2][L_0_R_1][5].Set(9, L2_L0R1_5, tempRot, tempHandState);
	//L2_L0R1_6
	MechanicalStep L2_L0R1_6[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_0_R_1][6].Set(11, L2_L0R1_6, tempRot, tempHandState);
	//L2_L0R1_7
	MechanicalStep L2_L0R1_7[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_0_R_1][7].Set(11, L2_L0R1_7, tempRot, tempHandState);
	//L2_L0R1_8
	MechanicalStep L2_L0R1_8[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_LC, M_L2, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_0_R_1][8].Set(12, L2_L0R1_8, tempRot, tempHandState);
	//L2_L0R1_9
	MechanicalStep L2_L0R1_9[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_LC, M_L2, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_0_R_1][9].Set(12, L2_L0R1_9, tempRot, tempHandState);
	//L2_L0R1_10
	MechanicalStep L2_L0R1_10[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_0_R_1][10].Set(12, L2_L0R1_10, tempRot, tempHandState);
	//L2_L0R1_11
	MechanicalStep L2_L0R1_11[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_0_R_1][11].Set(12, L2_L0R1_11, tempRot, tempHandState);
	//L2_L0R1_12
	MechanicalStep L2_L0R1_12[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_2][L_0_R_1][12].Set(13, L2_L0R1_12, tempRot, tempHandState);
	//L2_L0R1_13
	MechanicalStep L2_L0R1_13[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_2][L_0_R_1][13].Set(13, L2_L0R1_13, tempRot, tempHandState);
	//L2_L0R1_14
	MechanicalStep L2_L0R1_14[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_2][L_0_R_1][14].Set(15, L2_L0R1_14, tempRot, tempHandState);
	//L2_L0R1_15
	MechanicalStep L2_L0R1_15[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_2][L_0_R_1][15].Set(15, L2_L0R1_15, tempRot, tempHandState);
}
void L3_L0R1Init(void)
{
	//L3_L0R1_0
	MechanicalStep L3_L0R1_0[] = { M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_3][L_0_R_1][0].Set(5, L3_L0R1_0, tempRot, tempHandState);
	//L3_L0R1_1
	MechanicalStep L3_L0R1_1[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_0_R_1][1].Set(6, L3_L0R1_1, tempRot, tempHandState);
	//L3_L0R1_2
	MechanicalStep L3_L0R1_2[] = { M_LO, M_R1, M_LC, M_RO, M_L2, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_0_R_1][2].Set(8, L3_L0R1_2, tempRot, tempHandState);
	//L3_L0R1_3
	MechanicalStep L3_L0R1_3[] = { M_LO, M_R3, M_LC, M_RO, M_L2, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_0_R_1][3].Set(8, L3_L0R1_3, tempRot, tempHandState);
	//L3_L0R1_4
	MechanicalStep L3_L0R1_4[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_3][L_0_R_1][4].Set(8, L3_L0R1_4, tempRot, tempHandState);
	//L3_L0R1_5
	MechanicalStep L3_L0R1_5[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_0_R_1][5].Set(9, L3_L0R1_5, tempRot, tempHandState);
	//L3_L0R1_6
	MechanicalStep L3_L0R1_6[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_3][L_0_R_1][6].Set(11, L3_L0R1_6, tempRot, tempHandState);
	//L3_L0R1_7
	MechanicalStep L3_L0R1_7[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_3][L_0_R_1][7].Set(11, L3_L0R1_7, tempRot, tempHandState);
	//L3_L0R1_8
	MechanicalStep L3_L0R1_8[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_LC, M_L3, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_3][L_0_R_1][8].Set(12, L3_L0R1_8, tempRot, tempHandState);
	//L3_L0R1_9
	MechanicalStep L3_L0R1_9[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_LC, M_L3, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_3][L_0_R_1][9].Set(12, L3_L0R1_9, tempRot, tempHandState);
	//L3_L0R1_10
	MechanicalStep L3_L0R1_10[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_3][L_0_R_1][10].Set(12, L3_L0R1_10, tempRot, tempHandState);
	//L3_L0R1_11
	MechanicalStep L3_L0R1_11[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_3][L_0_R_1][11].Set(12, L3_L0R1_11, tempRot, tempHandState);
	//L3_L0R1_12
	MechanicalStep L3_L0R1_12[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_0_R_1][12].Set(13, L3_L0R1_12, tempRot, tempHandState);
	//L3_L0R1_13
	MechanicalStep L3_L0R1_13[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_0_R_1][13].Set(13, L3_L0R1_13, tempRot, tempHandState);
	//L3_L0R1_14
	MechanicalStep L3_L0R1_14[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_0_R_1][14].Set(15, L3_L0R1_14, tempRot, tempHandState);
	//L3_L0R1_15
	MechanicalStep L3_L0R1_15[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_0_R_1][15].Set(15, L3_L0R1_15, tempRot, tempHandState);
}
void D1_L0R1Init(void)
{
	//D1_L0R1_0
	MechanicalStep D1_L0R1_0[] = { M_LO, M_R1, M_LC, M_L1, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_1][L_0_R_1][0].Set(4, D1_L0R1_0, tempRot, tempHandState);
	//D1_L0R1_1
	MechanicalStep D1_L0R1_1[] = { M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_0_R_1][1].Set(5, D1_L0R1_1, tempRot, tempHandState);
	//D1_L0R1_2
	MechanicalStep D1_L0R1_2[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_1][L_0_R_1][2].Set(8, D1_L0R1_2, tempRot, tempHandState);
	//D1_L0R1_3
	MechanicalStep D1_L0R1_3[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_0_R_1][3].Set(9, D1_L0R1_3, tempRot, tempHandState);
	//D1_L0R1_4
	MechanicalStep D1_L0R1_4[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_0_R_1][4].Set(9, D1_L0R1_4, tempRot, tempHandState);
	//D1_L0R1_5
	MechanicalStep D1_L0R1_5[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_R1, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_1][L_0_R_1][5].Set(9, D1_L0R1_5, tempRot, tempHandState);
	//D1_L0R1_6
	MechanicalStep D1_L0R1_6[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R3, M_LC, M_L1, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_1][L_0_R_1][6].Set(9, D1_L0R1_6, tempRot, tempHandState);
	//D1_L0R1_7
	MechanicalStep D1_L0R1_7[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_0_R_1][7].Set(10, D1_L0R1_7, tempRot, tempHandState);
	//D1_L0R1_8
	MechanicalStep D1_L0R1_8[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_0_R_1][8].Set(11, D1_L0R1_8, tempRot, tempHandState);
	//D1_L0R1_9
	MechanicalStep D1_L0R1_9[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_0_R_1][9].Set(11, D1_L0R1_9, tempRot, tempHandState);
	//D1_L0R1_10
	MechanicalStep D1_L0R1_10[] = { M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_0_R_1][10].Set(11, D1_L0R1_10, tempRot, tempHandState);
	//D1_L0R1_11
	MechanicalStep D1_L0R1_11[] = { M_RO, M_R1, M_RC, M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_0_R_1][11].Set(12, D1_L0R1_11, tempRot, tempHandState);
	//D1_L0R1_12
	MechanicalStep D1_L0R1_12[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_1][L_0_R_1][12].Set(13, D1_L0R1_12, tempRot, tempHandState);
	//D1_L0R1_13
	MechanicalStep D1_L0R1_13[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_1][L_0_R_1][13].Set(13, D1_L0R1_13, tempRot, tempHandState);
	//D1_L0R1_14
	MechanicalStep D1_L0R1_14[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_1][L_0_R_1][14].Set(15, D1_L0R1_14, tempRot, tempHandState);
	//D1_L0R1_15
	MechanicalStep D1_L0R1_15[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_1][L_0_R_1][15].Set(15, D1_L0R1_15, tempRot, tempHandState);
}
void D2_L0R1Init(void)
{
	//D2_L0R1_0
	MechanicalStep D2_L0R1_0[] = { M_LO, M_R1, M_LC, M_L2, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_0_R_1][0].Set(4, D2_L0R1_0, tempRot, tempHandState);
	//D2_L0R1_1
	MechanicalStep D2_L0R1_1[] = { M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_2][L_0_R_1][1].Set(5, D2_L0R1_1, tempRot, tempHandState);
	//D2_L0R1_2
	MechanicalStep D2_L0R1_2[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_0_R_1][2].Set(8, D2_L0R1_2, tempRot, tempHandState);
	//D2_L0R1_3
	MechanicalStep D2_L0R1_3[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_R2, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_0_R_1][3].Set(9, D2_L0R1_3, tempRot, tempHandState);
	//D2_L0R1_4
	MechanicalStep D2_L0R1_4[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R3, M_LC, M_L2, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_0_R_1][4].Set(9, D2_L0R1_4, tempRot, tempHandState);
	//D2_L0R1_5
	MechanicalStep D2_L0R1_5[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_2][L_0_R_1][5].Set(9, D2_L0R1_5, tempRot, tempHandState);
	//D2_L0R1_6
	MechanicalStep D2_L0R1_6[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_2][L_0_R_1][6].Set(9, D2_L0R1_6, tempRot, tempHandState);
	//D2_L0R1_7
	MechanicalStep D2_L0R1_7[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_2][L_0_R_1][7].Set(10, D2_L0R1_7, tempRot, tempHandState);
	//D2_L0R1_8
	MechanicalStep D2_L0R1_8[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_2][L_0_R_1][8].Set(11, D2_L0R1_8, tempRot, tempHandState);
	//D2_L0R1_9
	MechanicalStep D2_L0R1_9[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_2][L_0_R_1][9].Set(11, D2_L0R1_9, tempRot, tempHandState);
	//D2_L0R1_10
	MechanicalStep D2_L0R1_10[] = { M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_2][L_0_R_1][10].Set(11, D2_L0R1_10, tempRot, tempHandState);
	//D2_L0R1_11
	MechanicalStep D2_L0R1_11[] = { M_RO, M_R1, M_RC, M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_2][L_0_R_1][11].Set(12, D2_L0R1_11, tempRot, tempHandState);
	//D2_L0R1_12
	MechanicalStep D2_L0R1_12[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_0_R_1][12].Set(13, D2_L0R1_12, tempRot, tempHandState);
	//D2_L0R1_13
	MechanicalStep D2_L0R1_13[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_0_R_1][13].Set(13, D2_L0R1_13, tempRot, tempHandState);
	//D2_L0R1_14
	MechanicalStep D2_L0R1_14[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_0_R_1][14].Set(15, D2_L0R1_14, tempRot, tempHandState);
	//D2_L0R1_15
	MechanicalStep D2_L0R1_15[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_0_R_1][15].Set(15, D2_L0R1_15, tempRot, tempHandState);
}
void D3_L0R1Init(void)
{
	//D3_L0R1_0
	MechanicalStep D3_L0R1_0[] = { M_LO, M_R1, M_LC, M_L3, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_3][L_0_R_1][0].Set(4, D3_L0R1_0, tempRot, tempHandState);
	//D3_L0R1_1
	MechanicalStep D3_L0R1_1[] = { M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_0_R_1][1].Set(5, D3_L0R1_1, tempRot, tempHandState);
	//D3_L0R1_2
	MechanicalStep D3_L0R1_2[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_3][L_0_R_1][2].Set(8, D3_L0R1_2, tempRot, tempHandState);
	//D3_L0R1_3
	MechanicalStep D3_L0R1_3[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_0_R_1][3].Set(9, D3_L0R1_3, tempRot, tempHandState);
	//D3_L0R1_4
	MechanicalStep D3_L0R1_4[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_0_R_1][4].Set(9, D3_L0R1_4, tempRot, tempHandState);
	//D3_L0R1_5
	MechanicalStep D3_L0R1_5[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_R3, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_3][L_0_R_1][5].Set(9, D3_L0R1_5, tempRot, tempHandState);
	//D3_L0R1_6
	MechanicalStep D3_L0R1_6[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R3, M_LC, M_L3, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_3][L_0_R_1][6].Set(9, D3_L0R1_6, tempRot, tempHandState);
	//D3_L0R1_7
	MechanicalStep D3_L0R1_7[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_0_R_1][7].Set(10, D3_L0R1_7, tempRot, tempHandState);
	//D3_L0R1_8
	MechanicalStep D3_L0R1_8[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_0_R_1][8].Set(11, D3_L0R1_8, tempRot, tempHandState);
	//D3_L0R1_9
	MechanicalStep D3_L0R1_9[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_0_R_1][9].Set(11, D3_L0R1_9, tempRot, tempHandState);
	//D3_L0R1_10
	MechanicalStep D3_L0R1_10[] = { M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_0_R_1][10].Set(11, D3_L0R1_10, tempRot, tempHandState);
	//D3_L0R1_11
	MechanicalStep D3_L0R1_11[] = { M_RO, M_R1, M_RC, M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_0_R_1][11].Set(12, D3_L0R1_11, tempRot, tempHandState);
	//D3_L0R1_12
	MechanicalStep D3_L0R1_12[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_3][L_0_R_1][12].Set(13, D3_L0R1_12, tempRot, tempHandState);
	//D3_L0R1_13
	MechanicalStep D3_L0R1_13[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_3][L_0_R_1][13].Set(13, D3_L0R1_13, tempRot, tempHandState);
	//D3_L0R1_14
	MechanicalStep D3_L0R1_14[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_3][L_0_R_1][14].Set(15, D3_L0R1_14, tempRot, tempHandState);
	//D3_L0R1_15
	MechanicalStep D3_L0R1_15[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_3][L_0_R_1][15].Set(15, D3_L0R1_15, tempRot, tempHandState);
}
void F1_L1R0Init(void)
{
	//F1_L1R0_0
	MechanicalStep F1_L1R0_0[] = { M_L1, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_1_R_0][0].Set(1, F1_L1R0_0, tempRot, tempHandState);
	//F1_L1R0_1
	MechanicalStep F1_L1R0_1[] = { M_RO, M_L2, M_RC, M_L1, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_1_R_0][1].Set(4, F1_L1R0_1, tempRot, tempHandState);
	//F1_L1R0_2
	MechanicalStep F1_L1R0_2[] = { M_LO, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_1][L_1_R_0][2].Set(4, F1_L1R0_2, tempRot, tempHandState);
	//F1_L1R0_3
	MechanicalStep F1_L1R0_3[] = { M_RO, M_L1, M_RC, M_L1, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_1][L_1_R_0][3].Set(4, F1_L1R0_3, tempRot, tempHandState);
	//F1_L1R0_4
	MechanicalStep F1_L1R0_4[] = { M_RO, M_L3, M_RC, M_L1, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_1][L_1_R_0][4].Set(4, F1_L1R0_4, tempRot, tempHandState);
	//F1_L1R0_5
	MechanicalStep F1_L1R0_5[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_RC, M_R1, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_1][L_1_R_0][5].Set(12, F1_L1R0_5, tempRot, tempHandState);
	//F1_L1R0_6
	MechanicalStep F1_L1R0_6[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_RC, M_R1, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_1][L_1_R_0][6].Set(12, F1_L1R0_6, tempRot, tempHandState);
	//F1_L1R0_7
	MechanicalStep F1_L1R0_7[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_1][L_1_R_0][7].Set(12, F1_L1R0_7, tempRot, tempHandState);
	//F1_L1R0_8
	MechanicalStep F1_L1R0_8[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_1][L_1_R_0][8].Set(12, F1_L1R0_8, tempRot, tempHandState);
	//F1_L1R0_9
	MechanicalStep F1_L1R0_9[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_1_R_0][9].Set(13, F1_L1R0_9, tempRot, tempHandState);
	//F1_L1R0_10
	MechanicalStep F1_L1R0_10[] = { M_LO, M_L1, M_R2, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_1_R_0][10].Set(13, F1_L1R0_10, tempRot, tempHandState);
	//F1_L1R0_11
	MechanicalStep F1_L1R0_11[] = { M_LO, M_L1, M_R2, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_1_R_0][11].Set(13, F1_L1R0_11, tempRot, tempHandState);
	//F1_L1R0_12
	MechanicalStep F1_L1R0_12[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_1_R_0][12].Set(13, F1_L1R0_12, tempRot, tempHandState);
	//F1_L1R0_13
	MechanicalStep F1_L1R0_13[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_LC, M_L1, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_1][L_1_R_0][13].Set(13, F1_L1R0_13, tempRot, tempHandState);
	//F1_L1R0_14
	MechanicalStep F1_L1R0_14[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_1_R_0][14].Set(15, F1_L1R0_14, tempRot, tempHandState);
	//F1_L1R0_15
	MechanicalStep F1_L1R0_15[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_1][L_1_R_0][15].Set(15, F1_L1R0_15, tempRot, tempHandState);
}
void F2_L1R0Init(void)
{
	//F2_L1R0_0
	MechanicalStep F2_L1R0_0[] = { M_L2, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_2][L_1_R_0][0].Set(1, F2_L1R0_0, tempRot, tempHandState);
	//F2_L1R0_1
	MechanicalStep F2_L1R0_1[] = { M_LO, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_1_R_0][1].Set(4, F2_L1R0_1, tempRot, tempHandState);
	//F2_L1R0_2
	MechanicalStep F2_L1R0_2[] = { M_RO, M_L1, M_RC, M_L2, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_1_R_0][2].Set(4, F2_L1R0_2, tempRot, tempHandState);
	//F2_L1R0_3
	MechanicalStep F2_L1R0_3[] = { M_RO, M_L3, M_RC, M_L2, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_1_R_0][3].Set(4, F2_L1R0_3, tempRot, tempHandState);
	//F2_L1R0_4
	MechanicalStep F2_L1R0_4[] = { M_RO, M_L2, M_RC, M_L2, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_2][L_1_R_0][4].Set(4, F2_L1R0_4, tempRot, tempHandState);
	//F2_L1R0_5
	MechanicalStep F2_L1R0_5[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_RC, M_R2, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_1_R_0][5].Set(12, F2_L1R0_5, tempRot, tempHandState);
	//F2_L1R0_6
	MechanicalStep F2_L1R0_6[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_RC, M_R2, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_1_R_0][6].Set(12, F2_L1R0_6, tempRot, tempHandState);
	//F2_L1R0_7
	MechanicalStep F2_L1R0_7[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_1_R_0][7].Set(12, F2_L1R0_7, tempRot, tempHandState);
	//F2_L1R0_8
	MechanicalStep F2_L1R0_8[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_1_R_0][8].Set(12, F2_L1R0_8, tempRot, tempHandState);
	//F2_L1R0_9
	MechanicalStep F2_L1R0_9[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_LC, M_L2, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_2][L_1_R_0][9].Set(13, F2_L1R0_9, tempRot, tempHandState);
	//F2_L1R0_10
	MechanicalStep F2_L1R0_10[] = { M_LO, M_L1, M_R2, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_2][L_1_R_0][10].Set(13, F2_L1R0_10, tempRot, tempHandState);
	//F2_L1R0_11
	MechanicalStep F2_L1R0_11[] = { M_LO, M_L1, M_R2, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_2][L_1_R_0][11].Set(13, F2_L1R0_11, tempRot, tempHandState);
	//F2_L1R0_12
	MechanicalStep F2_L1R0_12[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_2][L_1_R_0][12].Set(13, F2_L1R0_12, tempRot, tempHandState);
	//F2_L1R0_13
	MechanicalStep F2_L1R0_13[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_2][L_1_R_0][13].Set(13, F2_L1R0_13, tempRot, tempHandState);
	//F2_L1R0_14
	MechanicalStep F2_L1R0_14[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_2][L_1_R_0][14].Set(15, F2_L1R0_14, tempRot, tempHandState);
	//F2_L1R0_15
	MechanicalStep F2_L1R0_15[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_2][L_1_R_0][15].Set(15, F2_L1R0_15, tempRot, tempHandState);
}
void F3_L1R0Init(void)
{
	//F3_L1R0_0
	MechanicalStep F3_L1R0_0[] = { M_L3, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_1_R_0][0].Set(1, F3_L1R0_0, tempRot, tempHandState);
	//F3_L1R0_1
	MechanicalStep F3_L1R0_1[] = { M_RO, M_L2, M_RC, M_L3, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_1_R_0][1].Set(4, F3_L1R0_1, tempRot, tempHandState);
	//F3_L1R0_2
	MechanicalStep F3_L1R0_2[] = { M_LO, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_3][L_1_R_0][2].Set(4, F3_L1R0_2, tempRot, tempHandState);
	//F3_L1R0_3
	MechanicalStep F3_L1R0_3[] = { M_RO, M_L1, M_RC, M_L3, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_3][L_1_R_0][3].Set(4, F3_L1R0_3, tempRot, tempHandState);
	//F3_L1R0_4
	MechanicalStep F3_L1R0_4[] = { M_RO, M_L3, M_RC, M_L3, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_3][L_1_R_0][4].Set(4, F3_L1R0_4, tempRot, tempHandState);
	//F3_L1R0_5
	MechanicalStep F3_L1R0_5[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_RC, M_R3, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_3][L_1_R_0][5].Set(12, F3_L1R0_5, tempRot, tempHandState);
	//F3_L1R0_6
	MechanicalStep F3_L1R0_6[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_RC, M_R3, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_3][L_1_R_0][6].Set(12, F3_L1R0_6, tempRot, tempHandState);
	//F3_L1R0_7
	MechanicalStep F3_L1R0_7[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_3][L_1_R_0][7].Set(12, F3_L1R0_7, tempRot, tempHandState);
	//F3_L1R0_8
	MechanicalStep F3_L1R0_8[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[F][_3][L_1_R_0][8].Set(12, F3_L1R0_8, tempRot, tempHandState);
	//F3_L1R0_9
	MechanicalStep F3_L1R0_9[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_1_R_0][9].Set(13, F3_L1R0_9, tempRot, tempHandState);
	//F3_L1R0_10
	MechanicalStep F3_L1R0_10[] = { M_LO, M_L1, M_R2, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_1_R_0][10].Set(13, F3_L1R0_10, tempRot, tempHandState);
	//F3_L1R0_11
	MechanicalStep F3_L1R0_11[] = { M_LO, M_L1, M_R2, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_1_R_0][11].Set(13, F3_L1R0_11, tempRot, tempHandState);
	//F3_L1R0_12
	MechanicalStep F3_L1R0_12[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_1_R_0][12].Set(13, F3_L1R0_12, tempRot, tempHandState);
	//F3_L1R0_13
	MechanicalStep F3_L1R0_13[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_LC, M_L3, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[F][_3][L_1_R_0][13].Set(13, F3_L1R0_13, tempRot, tempHandState);
	//F3_L1R0_14
	MechanicalStep F3_L1R0_14[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_1_R_0][14].Set(15, F3_L1R0_14, tempRot, tempHandState);
	//F3_L1R0_15
	MechanicalStep F3_L1R0_15[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[F][_3][L_1_R_0][15].Set(15, F3_L1R0_15, tempRot, tempHandState);
}
void R1_L1R0Init(void)
{
	//R1_L1R0_0
	MechanicalStep R1_L1R0_0[] = { M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_1][L_1_R_0][0].Set(4, R1_L1R0_0, tempRot, tempHandState);
	//R1_L1R0_1
	MechanicalStep R1_L1R0_1[] = { M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_1_R_0][1].Set(5, R1_L1R0_1, tempRot, tempHandState);
	//R1_L1R0_2
	MechanicalStep R1_L1R0_2[] = { M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_1_R_0][2].Set(5, R1_L1R0_2, tempRot, tempHandState);
	//R1_L1R0_3
	MechanicalStep R1_L1R0_3[] = { M_LO, M_L1, M_R2, M_LC, M_R1, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_1][L_1_R_0][3].Set(5, R1_L1R0_3, tempRot, tempHandState);
	//R1_L1R0_4
	MechanicalStep R1_L1R0_4[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_LC, M_L1, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_1][L_1_R_0][4].Set(8, R1_L1R0_4, tempRot, tempHandState);
	//R1_L1R0_5
	MechanicalStep R1_L1R0_5[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_LC, M_L1, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_1][L_1_R_0][5].Set(8, R1_L1R0_5, tempRot, tempHandState);
	//R1_L1R0_6
	MechanicalStep R1_L1R0_6[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_1_R_0][6].Set(9, R1_L1R0_6, tempRot, tempHandState);
	//R1_L1R0_7
	MechanicalStep R1_L1R0_7[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_1_R_0][7].Set(9, R1_L1R0_7, tempRot, tempHandState);
	//R1_L1R0_8
	MechanicalStep R1_L1R0_8[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_1_R_0][8].Set(11, R1_L1R0_8, tempRot, tempHandState);
	//R1_L1R0_9
	MechanicalStep R1_L1R0_9[] = { M_RO, M_L1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_1_R_0][9].Set(11, R1_L1R0_9, tempRot, tempHandState);
	//R1_L1R0_10
	MechanicalStep R1_L1R0_10[] = { M_RO, M_L1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_1_R_0][10].Set(11, R1_L1R0_10, tempRot, tempHandState);
	//R1_L1R0_11
	MechanicalStep R1_L1R0_11[] = { M_RO, M_L1, M_RC, M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_1][L_1_R_0][11].Set(12, R1_L1R0_11, tempRot, tempHandState);
	//R1_L1R0_12
	MechanicalStep R1_L1R0_12[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_1][L_1_R_0][12].Set(12, R1_L1R0_12, tempRot, tempHandState);
	//R1_L1R0_13
	MechanicalStep R1_L1R0_13[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_1][L_1_R_0][13].Set(12, R1_L1R0_13, tempRot, tempHandState);
	//R1_L1R0_14
	MechanicalStep R1_L1R0_14[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_1][L_1_R_0][14].Set(19, R1_L1R0_14, tempRot, tempHandState);
	//R1_L1R0_15
	MechanicalStep R1_L1R0_15[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_1][L_1_R_0][15].Set(19, R1_L1R0_15, tempRot, tempHandState);
}
void R2_L1R0Init(void)
{
	//R2_L1R0_0
	MechanicalStep R2_L1R0_0[] = { M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_1_R_0][0].Set(4, R2_L1R0_0, tempRot, tempHandState);
	//R2_L1R0_1
	MechanicalStep R2_L1R0_1[] = { M_LO, M_L1, M_R2, M_LC, M_R2, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_1_R_0][1].Set(5, R2_L1R0_1, tempRot, tempHandState);
	//R2_L1R0_2
	MechanicalStep R2_L1R0_2[] = { M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_2][L_1_R_0][2].Set(5, R2_L1R0_2, tempRot, tempHandState);
	//R2_L1R0_3
	MechanicalStep R2_L1R0_3[] = { M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_2][L_1_R_0][3].Set(5, R2_L1R0_3, tempRot, tempHandState);
	//R2_L1R0_4
	MechanicalStep R2_L1R0_4[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_LC, M_L2, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_1_R_0][4].Set(8, R2_L1R0_4, tempRot, tempHandState);
	//R2_L1R0_5
	MechanicalStep R2_L1R0_5[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_LC, M_L2, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_1_R_0][5].Set(8, R2_L1R0_5, tempRot, tempHandState);
	//R2_L1R0_6
	MechanicalStep R2_L1R0_6[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_2][L_1_R_0][6].Set(9, R2_L1R0_6, tempRot, tempHandState);
	//R2_L1R0_7
	MechanicalStep R2_L1R0_7[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_2][L_1_R_0][7].Set(9, R2_L1R0_7, tempRot, tempHandState);
	//R2_L1R0_8
	MechanicalStep R2_L1R0_8[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_2][L_1_R_0][8].Set(11, R2_L1R0_8, tempRot, tempHandState);
	//R2_L1R0_9
	MechanicalStep R2_L1R0_9[] = { M_RO, M_L1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_2][L_1_R_0][9].Set(11, R2_L1R0_9, tempRot, tempHandState);
	//R2_L1R0_10
	MechanicalStep R2_L1R0_10[] = { M_RO, M_L1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_2][L_1_R_0][10].Set(11, R2_L1R0_10, tempRot, tempHandState);
	//R2_L1R0_11
	MechanicalStep R2_L1R0_11[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_1_R_0][11].Set(12, R2_L1R0_11, tempRot, tempHandState);
	//R2_L1R0_12
	MechanicalStep R2_L1R0_12[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_1_R_0][12].Set(12, R2_L1R0_12, tempRot, tempHandState);
	//R2_L1R0_13
	MechanicalStep R2_L1R0_13[] = { M_RO, M_L1, M_RC, M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_2][L_1_R_0][13].Set(12, R2_L1R0_13, tempRot, tempHandState);
	//R2_L1R0_14
	MechanicalStep R2_L1R0_14[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_1_R_0][14].Set(19, R2_L1R0_14, tempRot, tempHandState);
	//R2_L1R0_15
	MechanicalStep R2_L1R0_15[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_2][L_1_R_0][15].Set(19, R2_L1R0_15, tempRot, tempHandState);
}
void R3_L1R0Init(void)
{
	//R3_L1R0_0
	MechanicalStep R3_L1R0_0[] = { M_LO, M_L1, M_LC, M_R3, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_3][L_1_R_0][0].Set(4, R3_L1R0_0, tempRot, tempHandState);
	//R3_L1R0_1
	MechanicalStep R3_L1R0_1[] = { M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_1_R_0][1].Set(5, R3_L1R0_1, tempRot, tempHandState);
	//R3_L1R0_2
	MechanicalStep R3_L1R0_2[] = { M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_1_R_0][2].Set(5, R3_L1R0_2, tempRot, tempHandState);
	//R3_L1R0_3
	MechanicalStep R3_L1R0_3[] = { M_LO, M_L1, M_R2, M_LC, M_R3, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_3][L_1_R_0][3].Set(5, R3_L1R0_3, tempRot, tempHandState);
	//R3_L1R0_4
	MechanicalStep R3_L1R0_4[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_LC, M_L3, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_3][L_1_R_0][4].Set(8, R3_L1R0_4, tempRot, tempHandState);
	//R3_L1R0_5
	MechanicalStep R3_L1R0_5[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_LC, M_L3, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_3][L_1_R_0][5].Set(8, R3_L1R0_5, tempRot, tempHandState);
	//R3_L1R0_6
	MechanicalStep R3_L1R0_6[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_1_R_0][6].Set(9, R3_L1R0_6, tempRot, tempHandState);
	//R3_L1R0_7
	MechanicalStep R3_L1R0_7[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_1_R_0][7].Set(9, R3_L1R0_7, tempRot, tempHandState);
	//R3_L1R0_8
	MechanicalStep R3_L1R0_8[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, 1, 1, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_1_R_0][8].Set(11, R3_L1R0_8, tempRot, tempHandState);
	//R3_L1R0_9
	MechanicalStep R3_L1R0_9[] = { M_RO, M_L1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_1_R_0][9].Set(11, R3_L1R0_9, tempRot, tempHandState);
	//R3_L1R0_10
	MechanicalStep R3_L1R0_10[] = { M_RO, M_L1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_1_R_0][10].Set(11, R3_L1R0_10, tempRot, tempHandState);
	//R3_L1R0_11
	MechanicalStep R3_L1R0_11[] = { M_RO, M_L1, M_RC, M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[R][_3][L_1_R_0][11].Set(12, R3_L1R0_11, tempRot, tempHandState);
	//R3_L1R0_12
	MechanicalStep R3_L1R0_12[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_3][L_1_R_0][12].Set(12, R3_L1R0_12, tempRot, tempHandState);
	//R3_L1R0_13
	MechanicalStep R3_L1R0_13[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[R][_3][L_1_R_0][13].Set(12, R3_L1R0_13, tempRot, tempHandState);
	//R3_L1R0_14
	MechanicalStep R3_L1R0_14[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_3][L_1_R_0][14].Set(19, R3_L1R0_14, tempRot, tempHandState);
	//R3_L1R0_15
	MechanicalStep R3_L1R0_15[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(1, 1, 0, 1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[R][_3][L_1_R_0][15].Set(19, R3_L1R0_15, tempRot, tempHandState);
}
void U1_L1R0Init(void)
{
	//U1_L1R0_0
	MechanicalStep U1_L1R0_0[] = { M_RO, M_L1, M_RC, M_R1, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_1][L_1_R_0][0].Set(4, U1_L1R0_0, tempRot, tempHandState);
	//U1_L1R0_1
	MechanicalStep U1_L1R0_1[] = { M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_1_R_0][1].Set(5, U1_L1R0_1, tempRot, tempHandState);
	//U1_L1R0_2
	MechanicalStep U1_L1R0_2[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_1][L_1_R_0][2].Set(8, U1_L1R0_2, tempRot, tempHandState);
	//U1_L1R0_3
	MechanicalStep U1_L1R0_3[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_1_R_0][3].Set(9, U1_L1R0_3, tempRot, tempHandState);
	//U1_L1R0_4
	MechanicalStep U1_L1R0_4[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_1_R_0][4].Set(9, U1_L1R0_4, tempRot, tempHandState);
	//U1_L1R0_5
	MechanicalStep U1_L1R0_5[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L3, M_RC, M_R1, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_1][L_1_R_0][5].Set(9, U1_L1R0_5, tempRot, tempHandState);
	//U1_L1R0_6
	MechanicalStep U1_L1R0_6[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_L1, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_1][L_1_R_0][6].Set(9, U1_L1R0_6, tempRot, tempHandState);
	//U1_L1R0_7
	MechanicalStep U1_L1R0_7[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_1_R_0][7].Set(10, U1_L1R0_7, tempRot, tempHandState);
	//U1_L1R0_8
	MechanicalStep U1_L1R0_8[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_1_R_0][8].Set(11, U1_L1R0_8, tempRot, tempHandState);
	//U1_L1R0_9
	MechanicalStep U1_L1R0_9[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_1_R_0][9].Set(11, U1_L1R0_9, tempRot, tempHandState);
	//U1_L1R0_10
	MechanicalStep U1_L1R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_1_R_0][10].Set(11, U1_L1R0_10, tempRot, tempHandState);
	//U1_L1R0_11
	MechanicalStep U1_L1R0_11[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_1][L_1_R_0][11].Set(12, U1_L1R0_11, tempRot, tempHandState);
	//U1_L1R0_12
	MechanicalStep U1_L1R0_12[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_1][L_1_R_0][12].Set(13, U1_L1R0_12, tempRot, tempHandState);
	//U1_L1R0_13
	MechanicalStep U1_L1R0_13[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_1][L_1_R_0][13].Set(13, U1_L1R0_13, tempRot, tempHandState);
	//U1_L1R0_14
	MechanicalStep U1_L1R0_14[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_1][L_1_R_0][14].Set(15, U1_L1R0_14, tempRot, tempHandState);
	//U1_L1R0_15
	MechanicalStep U1_L1R0_15[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_1][L_1_R_0][15].Set(15, U1_L1R0_15, tempRot, tempHandState);
}
void U2_L1R0Init(void)
{
	//U2_L1R0_0
	MechanicalStep U2_L1R0_0[] = { M_RO, M_L1, M_RC, M_R2, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_1_R_0][0].Set(4, U2_L1R0_0, tempRot, tempHandState);
	//U2_L1R0_1
	MechanicalStep U2_L1R0_1[] = { M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_2][L_1_R_0][1].Set(5, U2_L1R0_1, tempRot, tempHandState);
	//U2_L1R0_2
	MechanicalStep U2_L1R0_2[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_1_R_0][2].Set(8, U2_L1R0_2, tempRot, tempHandState);
	//U2_L1R0_3
	MechanicalStep U2_L1R0_3[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L3, M_RC, M_R2, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_1_R_0][3].Set(9, U2_L1R0_3, tempRot, tempHandState);
	//U2_L1R0_4
	MechanicalStep U2_L1R0_4[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_L2, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_1_R_0][4].Set(9, U2_L1R0_4, tempRot, tempHandState);
	//U2_L1R0_5
	MechanicalStep U2_L1R0_5[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_2][L_1_R_0][5].Set(9, U2_L1R0_5, tempRot, tempHandState);
	//U2_L1R0_6
	MechanicalStep U2_L1R0_6[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_2][L_1_R_0][6].Set(9, U2_L1R0_6, tempRot, tempHandState);
	//U2_L1R0_7
	MechanicalStep U2_L1R0_7[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_2][L_1_R_0][7].Set(10, U2_L1R0_7, tempRot, tempHandState);
	//U2_L1R0_8
	MechanicalStep U2_L1R0_8[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_2][L_1_R_0][8].Set(11, U2_L1R0_8, tempRot, tempHandState);
	//U2_L1R0_9
	MechanicalStep U2_L1R0_9[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_2][L_1_R_0][9].Set(11, U2_L1R0_9, tempRot, tempHandState);
	//U2_L1R0_10
	MechanicalStep U2_L1R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_2][L_1_R_0][10].Set(11, U2_L1R0_10, tempRot, tempHandState);
	//U2_L1R0_11
	MechanicalStep U2_L1R0_11[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_2][L_1_R_0][11].Set(12, U2_L1R0_11, tempRot, tempHandState);
	//U2_L1R0_12
	MechanicalStep U2_L1R0_12[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_1_R_0][12].Set(13, U2_L1R0_12, tempRot, tempHandState);
	//U2_L1R0_13
	MechanicalStep U2_L1R0_13[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_1_R_0][13].Set(13, U2_L1R0_13, tempRot, tempHandState);
	//U2_L1R0_14
	MechanicalStep U2_L1R0_14[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_1_R_0][14].Set(15, U2_L1R0_14, tempRot, tempHandState);
	//U2_L1R0_15
	MechanicalStep U2_L1R0_15[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_2][L_1_R_0][15].Set(15, U2_L1R0_15, tempRot, tempHandState);
}
void U3_L1R0Init(void)
{
	//U3_L1R0_0
	MechanicalStep U3_L1R0_0[] = { M_RO, M_L1, M_RC, M_R3, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_3][L_1_R_0][0].Set(4, U3_L1R0_0, tempRot, tempHandState);
	//U3_L1R0_1
	MechanicalStep U3_L1R0_1[] = { M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, 1, 2, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_1_R_0][1].Set(5, U3_L1R0_1, tempRot, tempHandState);
	//U3_L1R0_2
	MechanicalStep U3_L1R0_2[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_3][L_1_R_0][2].Set(8, U3_L1R0_2, tempRot, tempHandState);
	//U3_L1R0_3
	MechanicalStep U3_L1R0_3[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_1_R_0][3].Set(9, U3_L1R0_3, tempRot, tempHandState);
	//U3_L1R0_4
	MechanicalStep U3_L1R0_4[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_1_R_0][4].Set(9, U3_L1R0_4, tempRot, tempHandState);
	//U3_L1R0_5
	MechanicalStep U3_L1R0_5[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L3, M_RC, M_R3, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_3][L_1_R_0][5].Set(9, U3_L1R0_5, tempRot, tempHandState);
	//U3_L1R0_6
	MechanicalStep U3_L1R0_6[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_L3, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_3][L_1_R_0][6].Set(9, U3_L1R0_6, tempRot, tempHandState);
	//U3_L1R0_7
	MechanicalStep U3_L1R0_7[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_1_R_0][7].Set(10, U3_L1R0_7, tempRot, tempHandState);
	//U3_L1R0_8
	MechanicalStep U3_L1R0_8[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_1_R_0][8].Set(11, U3_L1R0_8, tempRot, tempHandState);
	//U3_L1R0_9
	MechanicalStep U3_L1R0_9[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(2, -1, 1, 1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_1_R_0][9].Set(11, U3_L1R0_9, tempRot, tempHandState);
	//U3_L1R0_10
	MechanicalStep U3_L1R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_1_R_0][10].Set(11, U3_L1R0_10, tempRot, tempHandState);
	//U3_L1R0_11
	MechanicalStep U3_L1R0_11[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[U][_3][L_1_R_0][11].Set(12, U3_L1R0_11, tempRot, tempHandState);
	//U3_L1R0_12
	MechanicalStep U3_L1R0_12[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	tempRot.Set(1, 1, 2, 1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_3][L_1_R_0][12].Set(13, U3_L1R0_12, tempRot, tempHandState);
	//U3_L1R0_13
	MechanicalStep U3_L1R0_13[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[U][_3][L_1_R_0][13].Set(13, U3_L1R0_13, tempRot, tempHandState);
	//U3_L1R0_14
	MechanicalStep U3_L1R0_14[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_3][L_1_R_0][14].Set(15, U3_L1R0_14, tempRot, tempHandState);
	//U3_L1R0_15
	MechanicalStep U3_L1R0_15[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	tempRot.Set(2, 1, 0, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[U][_3][L_1_R_0][15].Set(15, U3_L1R0_15, tempRot, tempHandState);
}
void B1_L1R0Init(void)
{
	//B1_L1R0_0
	MechanicalStep B1_L1R0_0[] = { M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_1][L_1_R_0][0].Set(5, B1_L1R0_0, tempRot, tempHandState);
	//B1_L1R0_1
	MechanicalStep B1_L1R0_1[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_1_R_0][1].Set(6, B1_L1R0_1, tempRot, tempHandState);
	//B1_L1R0_2
	MechanicalStep B1_L1R0_2[] = { M_RO, M_L3, M_RC, M_LO, M_R2, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_1_R_0][2].Set(8, B1_L1R0_2, tempRot, tempHandState);
	//B1_L1R0_3
	MechanicalStep B1_L1R0_3[] = { M_RO, M_L1, M_RC, M_LO, M_R2, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_1_R_0][3].Set(8, B1_L1R0_3, tempRot, tempHandState);
	//B1_L1R0_4
	MechanicalStep B1_L1R0_4[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_1][L_1_R_0][4].Set(8, B1_L1R0_4, tempRot, tempHandState);
	//B1_L1R0_5
	MechanicalStep B1_L1R0_5[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_1_R_0][5].Set(9, B1_L1R0_5, tempRot, tempHandState);
	//B1_L1R0_6
	MechanicalStep B1_L1R0_6[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_1][L_1_R_0][6].Set(11, B1_L1R0_6, tempRot, tempHandState);
	//B1_L1R0_7
	MechanicalStep B1_L1R0_7[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_1][L_1_R_0][7].Set(11, B1_L1R0_7, tempRot, tempHandState);
	//B1_L1R0_8
	MechanicalStep B1_L1R0_8[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_RC, M_R1, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_1][L_1_R_0][8].Set(12, B1_L1R0_8, tempRot, tempHandState);
	//B1_L1R0_9
	MechanicalStep B1_L1R0_9[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_RC, M_R1, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_1][L_1_R_0][9].Set(12, B1_L1R0_9, tempRot, tempHandState);
	//B1_L1R0_10
	MechanicalStep B1_L1R0_10[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_1][L_1_R_0][10].Set(12, B1_L1R0_10, tempRot, tempHandState);
	//B1_L1R0_11
	MechanicalStep B1_L1R0_11[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_1][L_1_R_0][11].Set(12, B1_L1R0_11, tempRot, tempHandState);
	//B1_L1R0_12
	MechanicalStep B1_L1R0_12[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_1_R_0][12].Set(13, B1_L1R0_12, tempRot, tempHandState);
	//B1_L1R0_13
	MechanicalStep B1_L1R0_13[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_1_R_0][13].Set(13, B1_L1R0_13, tempRot, tempHandState);
	//B1_L1R0_14
	MechanicalStep B1_L1R0_14[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_1_R_0][14].Set(15, B1_L1R0_14, tempRot, tempHandState);
	//B1_L1R0_15
	MechanicalStep B1_L1R0_15[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_1][L_1_R_0][15].Set(15, B1_L1R0_15, tempRot, tempHandState);
}
void B2_L1R0Init(void)
{
	//B2_L1R0_0
	MechanicalStep B2_L1R0_0[] = { M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_1_R_0][0].Set(5, B2_L1R0_0, tempRot, tempHandState);
	//B2_L1R0_1
	MechanicalStep B2_L1R0_1[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_2][L_1_R_0][1].Set(6, B2_L1R0_1, tempRot, tempHandState);
	//B2_L1R0_2
	MechanicalStep B2_L1R0_2[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_1_R_0][2].Set(8, B2_L1R0_2, tempRot, tempHandState);
	//B2_L1R0_3
	MechanicalStep B2_L1R0_3[] = { M_RO, M_L3, M_RC, M_LO, M_R2, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_2][L_1_R_0][3].Set(8, B2_L1R0_3, tempRot, tempHandState);
	//B2_L1R0_4
	MechanicalStep B2_L1R0_4[] = { M_RO, M_L1, M_RC, M_LO, M_R2, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_2][L_1_R_0][4].Set(8, B2_L1R0_4, tempRot, tempHandState);
	//B2_L1R0_5
	MechanicalStep B2_L1R0_5[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_2][L_1_R_0][5].Set(9, B2_L1R0_5, tempRot, tempHandState);
	//B2_L1R0_6
	MechanicalStep B2_L1R0_6[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_1_R_0][6].Set(11, B2_L1R0_6, tempRot, tempHandState);
	//B2_L1R0_7
	MechanicalStep B2_L1R0_7[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_1_R_0][7].Set(11, B2_L1R0_7, tempRot, tempHandState);
	//B2_L1R0_8
	MechanicalStep B2_L1R0_8[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_RC, M_R2, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_1_R_0][8].Set(12, B2_L1R0_8, tempRot, tempHandState);
	//B2_L1R0_9
	MechanicalStep B2_L1R0_9[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_RC, M_R2, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_1_R_0][9].Set(12, B2_L1R0_9, tempRot, tempHandState);
	//B2_L1R0_10
	MechanicalStep B2_L1R0_10[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_1_R_0][10].Set(12, B2_L1R0_10, tempRot, tempHandState);
	//B2_L1R0_11
	MechanicalStep B2_L1R0_11[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_2][L_1_R_0][11].Set(12, B2_L1R0_11, tempRot, tempHandState);
	//B2_L1R0_12
	MechanicalStep B2_L1R0_12[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_2][L_1_R_0][12].Set(13, B2_L1R0_12, tempRot, tempHandState);
	//B2_L1R0_13
	MechanicalStep B2_L1R0_13[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_2][L_1_R_0][13].Set(13, B2_L1R0_13, tempRot, tempHandState);
	//B2_L1R0_14
	MechanicalStep B2_L1R0_14[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_2][L_1_R_0][14].Set(15, B2_L1R0_14, tempRot, tempHandState);
	//B2_L1R0_15
	MechanicalStep B2_L1R0_15[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_2][L_1_R_0][15].Set(15, B2_L1R0_15, tempRot, tempHandState);
}
void B3_L1R0Init(void)
{
	//B3_L1R0_0
	MechanicalStep B3_L1R0_0[] = { M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_3][L_1_R_0][0].Set(5, B3_L1R0_0, tempRot, tempHandState);
	//B3_L1R0_1
	MechanicalStep B3_L1R0_1[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(0, -1, 1, 1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_1_R_0][1].Set(6, B3_L1R0_1, tempRot, tempHandState);
	//B3_L1R0_2
	MechanicalStep B3_L1R0_2[] = { M_RO, M_L3, M_RC, M_LO, M_R2, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_1_R_0][2].Set(8, B3_L1R0_2, tempRot, tempHandState);
	//B3_L1R0_3
	MechanicalStep B3_L1R0_3[] = { M_RO, M_L1, M_RC, M_LO, M_R2, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_1_R_0][3].Set(8, B3_L1R0_3, tempRot, tempHandState);
	//B3_L1R0_4
	MechanicalStep B3_L1R0_4[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_3][L_1_R_0][4].Set(8, B3_L1R0_4, tempRot, tempHandState);
	//B3_L1R0_5
	MechanicalStep B3_L1R0_5[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_1_R_0][5].Set(9, B3_L1R0_5, tempRot, tempHandState);
	//B3_L1R0_6
	MechanicalStep B3_L1R0_6[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_3][L_1_R_0][6].Set(11, B3_L1R0_6, tempRot, tempHandState);
	//B3_L1R0_7
	MechanicalStep B3_L1R0_7[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(0, -1, 2, 1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[B][_3][L_1_R_0][7].Set(11, B3_L1R0_7, tempRot, tempHandState);
	//B3_L1R0_8
	MechanicalStep B3_L1R0_8[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_RC, M_R3, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_3][L_1_R_0][8].Set(12, B3_L1R0_8, tempRot, tempHandState);
	//B3_L1R0_9
	MechanicalStep B3_L1R0_9[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_RC, M_R3, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_3][L_1_R_0][9].Set(12, B3_L1R0_9, tempRot, tempHandState);
	//B3_L1R0_10
	MechanicalStep B3_L1R0_10[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_3][L_1_R_0][10].Set(12, B3_L1R0_10, tempRot, tempHandState);
	//B3_L1R0_11
	MechanicalStep B3_L1R0_11[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[B][_3][L_1_R_0][11].Set(12, B3_L1R0_11, tempRot, tempHandState);
	//B3_L1R0_12
	MechanicalStep B3_L1R0_12[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	tempRot.Set(1, -1, 0, 1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_1_R_0][12].Set(13, B3_L1R0_12, tempRot, tempHandState);
	//B3_L1R0_13
	MechanicalStep B3_L1R0_13[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_1_R_0][13].Set(13, B3_L1R0_13, tempRot, tempHandState);
	//B3_L1R0_14
	MechanicalStep B3_L1R0_14[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_1_R_0][14].Set(15, B3_L1R0_14, tempRot, tempHandState);
	//B3_L1R0_15
	MechanicalStep B3_L1R0_15[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(1, -1, 2, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[B][_3][L_1_R_0][15].Set(15, B3_L1R0_15, tempRot, tempHandState);
}
void L1_L1R0Init(void)
{
	//L1_L1R0_0
	MechanicalStep L1_L1R0_0[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_1][L_1_R_0][0].Set(7, L1_L1R0_0, tempRot, tempHandState);
	//L1_L1R0_1
	MechanicalStep L1_L1R0_1[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_1_R_0][1].Set(8, L1_L1R0_1, tempRot, tempHandState);
	//L1_L1R0_2
	MechanicalStep L1_L1R0_2[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_1_R_0][2].Set(8, L1_L1R0_2, tempRot, tempHandState);
	//L1_L1R0_3
	MechanicalStep L1_L1R0_3[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_1_R_0][3].Set(8, L1_L1R0_3, tempRot, tempHandState);
	//L1_L1R0_4
	MechanicalStep L1_L1R0_4[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_LC, M_L1, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_1][L_1_R_0][4].Set(8, L1_L1R0_4, tempRot, tempHandState);
	//L1_L1R0_5
	MechanicalStep L1_L1R0_5[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_LC, M_L1, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_1][L_1_R_0][5].Set(8, L1_L1R0_5, tempRot, tempHandState);
	//L1_L1R0_6
	MechanicalStep L1_L1R0_6[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_R1, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_1][L_1_R_0][6].Set(8, L1_L1R0_6, tempRot, tempHandState);
	//L1_L1R0_7
	MechanicalStep L1_L1R0_7[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_1_R_0][7].Set(9, L1_L1R0_7, tempRot, tempHandState);
	//L1_L1R0_8
	MechanicalStep L1_L1R0_8[] = { M_LO, M_L1, M_R2, M_LC, M_RO, M_L2, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_1_R_0][8].Set(9, L1_L1R0_8, tempRot, tempHandState);
	//L1_L1R0_9
	MechanicalStep L1_L1R0_9[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_1_R_0][9].Set(9, L1_L1R0_9, tempRot, tempHandState);
	//L1_L1R0_10
	MechanicalStep L1_L1R0_10[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_1][L_1_R_0][10].Set(9, L1_L1R0_10, tempRot, tempHandState);
	//L1_L1R0_11
	MechanicalStep L1_L1R0_11[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_1][L_1_R_0][11].Set(9, L1_L1R0_11, tempRot, tempHandState);
	//L1_L1R0_12
	MechanicalStep L1_L1R0_12[] = { M_RO, M_L1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_1_R_0][12].Set(11, L1_L1R0_12, tempRot, tempHandState);
	//L1_L1R0_13
	MechanicalStep L1_L1R0_13[] = { M_RO, M_L1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_1][L_1_R_0][13].Set(11, L1_L1R0_13, tempRot, tempHandState);
	//L1_L1R0_14
	MechanicalStep L1_L1R0_14[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_1][L_1_R_0][14].Set(16, L1_L1R0_14, tempRot, tempHandState);
	//L1_L1R0_15
	MechanicalStep L1_L1R0_15[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_1][L_1_R_0][15].Set(16, L1_L1R0_15, tempRot, tempHandState);
}
void L2_L1R0Init(void)
{
	//L2_L1R0_0
	MechanicalStep L2_L1R0_0[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_1_R_0][0].Set(7, L2_L1R0_0, tempRot, tempHandState);
	//L2_L1R0_1
	MechanicalStep L2_L1R0_1[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_LC, M_L2, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_1_R_0][1].Set(8, L2_L1R0_1, tempRot, tempHandState);
	//L2_L1R0_2
	MechanicalStep L2_L1R0_2[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_LC, M_L2, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_1_R_0][2].Set(8, L2_L1R0_2, tempRot, tempHandState);
	//L2_L1R0_3
	MechanicalStep L2_L1R0_3[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_R2, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_1_R_0][3].Set(8, L2_L1R0_3, tempRot, tempHandState);
	//L2_L1R0_4
	MechanicalStep L2_L1R0_4[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_2][L_1_R_0][4].Set(8, L2_L1R0_4, tempRot, tempHandState);
	//L2_L1R0_5
	MechanicalStep L2_L1R0_5[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_2][L_1_R_0][5].Set(8, L2_L1R0_5, tempRot, tempHandState);
	//L2_L1R0_6
	MechanicalStep L2_L1R0_6[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_2][L_1_R_0][6].Set(8, L2_L1R0_6, tempRot, tempHandState);
	//L2_L1R0_7
	MechanicalStep L2_L1R0_7[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_1_R_0][7].Set(9, L2_L1R0_7, tempRot, tempHandState);
	//L2_L1R0_8
	MechanicalStep L2_L1R0_8[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_1_R_0][8].Set(9, L2_L1R0_8, tempRot, tempHandState);
	//L2_L1R0_9
	MechanicalStep L2_L1R0_9[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_2][L_1_R_0][9].Set(9, L2_L1R0_9, tempRot, tempHandState);
	//L2_L1R0_10
	MechanicalStep L2_L1R0_10[] = { M_LO, M_L1, M_R2, M_LC, M_RO, M_L2, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_2][L_1_R_0][10].Set(9, L2_L1R0_10, tempRot, tempHandState);
	//L2_L1R0_11
	MechanicalStep L2_L1R0_11[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_2][L_1_R_0][11].Set(9, L2_L1R0_11, tempRot, tempHandState);
	//L2_L1R0_12
	MechanicalStep L2_L1R0_12[] = { M_RO, M_L1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_2][L_1_R_0][12].Set(11, L2_L1R0_12, tempRot, tempHandState);
	//L2_L1R0_13
	MechanicalStep L2_L1R0_13[] = { M_RO, M_L1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_2][L_1_R_0][13].Set(11, L2_L1R0_13, tempRot, tempHandState);
	//L2_L1R0_14
	MechanicalStep L2_L1R0_14[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_1_R_0][14].Set(16, L2_L1R0_14, tempRot, tempHandState);
	//L2_L1R0_15
	MechanicalStep L2_L1R0_15[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_2][L_1_R_0][15].Set(16, L2_L1R0_15, tempRot, tempHandState);
}
void L3_L1R0Init(void)
{
	//L3_L1R0_0
	MechanicalStep L3_L1R0_0[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_3][L_1_R_0][0].Set(7, L3_L1R0_0, tempRot, tempHandState);
	//L3_L1R0_1
	MechanicalStep L3_L1R0_1[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_1_R_0][1].Set(8, L3_L1R0_1, tempRot, tempHandState);
	//L3_L1R0_2
	MechanicalStep L3_L1R0_2[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_1_R_0][2].Set(8, L3_L1R0_2, tempRot, tempHandState);
	//L3_L1R0_3
	MechanicalStep L3_L1R0_3[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, 1, 1, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_1_R_0][3].Set(8, L3_L1R0_3, tempRot, tempHandState);
	//L3_L1R0_4
	MechanicalStep L3_L1R0_4[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_LC, M_L3, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_3][L_1_R_0][4].Set(8, L3_L1R0_4, tempRot, tempHandState);
	//L3_L1R0_5
	MechanicalStep L3_L1R0_5[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_LC, M_L3, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_3][L_1_R_0][5].Set(8, L3_L1R0_5, tempRot, tempHandState);
	//L3_L1R0_6
	MechanicalStep L3_L1R0_6[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_R3, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_3][L_1_R_0][6].Set(8, L3_L1R0_6, tempRot, tempHandState);
	//L3_L1R0_7
	MechanicalStep L3_L1R0_7[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(2, -1, 0, -1, 1, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_1_R_0][7].Set(9, L3_L1R0_7, tempRot, tempHandState);
	//L3_L1R0_8
	MechanicalStep L3_L1R0_8[] = { M_LO, M_L1, M_R2, M_LC, M_RO, M_L2, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, -1, 1, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_1_R_0][8].Set(9, L3_L1R0_8, tempRot, tempHandState);
	//L3_L1R0_9
	MechanicalStep L3_L1R0_9[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_1_R_0][9].Set(9, L3_L1R0_9, tempRot, tempHandState);
	//L3_L1R0_10
	MechanicalStep L3_L1R0_10[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	tempRot.Set(2, 1, 1, -1, 0, 1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_3][L_1_R_0][10].Set(9, L3_L1R0_10, tempRot, tempHandState);
	//L3_L1R0_11
	MechanicalStep L3_L1R0_11[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[L][_3][L_1_R_0][11].Set(9, L3_L1R0_11, tempRot, tempHandState);
	//L3_L1R0_12
	MechanicalStep L3_L1R0_12[] = { M_RO, M_L1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_1_R_0][12].Set(11, L3_L1R0_12, tempRot, tempHandState);
	//L3_L1R0_13
	MechanicalStep L3_L1R0_13[] = { M_RO, M_L1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[L][_3][L_1_R_0][13].Set(11, L3_L1R0_13, tempRot, tempHandState);
	//L3_L1R0_14
	MechanicalStep L3_L1R0_14[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(1, 1, 0, -1, 2, 1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_3][L_1_R_0][14].Set(16, L3_L1R0_14, tempRot, tempHandState);
	//L3_L1R0_15
	MechanicalStep L3_L1R0_15[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(1, -1, 0, -1, 2, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[L][_3][L_1_R_0][15].Set(16, L3_L1R0_15, tempRot, tempHandState);
}
void D1_L1R0Init(void)
{
	//D1_L1R0_0
	MechanicalStep D1_L1R0_0[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_1][L_1_R_0][0].Set(8, D1_L1R0_0, tempRot, tempHandState);
	//D1_L1R0_1
	MechanicalStep D1_L1R0_1[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_1_R_0][1].Set(9, D1_L1R0_1, tempRot, tempHandState);
	//D1_L1R0_2
	MechanicalStep D1_L1R0_2[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_1_R_0][2].Set(9, D1_L1R0_2, tempRot, tempHandState);
	//D1_L1R0_3
	MechanicalStep D1_L1R0_3[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_L1, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_1][L_1_R_0][3].Set(9, D1_L1R0_3, tempRot, tempHandState);
	//D1_L1R0_4
	MechanicalStep D1_L1R0_4[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_RC, M_R1, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_1][L_1_R_0][4].Set(9, D1_L1R0_4, tempRot, tempHandState);
	//D1_L1R0_5
	MechanicalStep D1_L1R0_5[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_1_R_0][5].Set(10, D1_L1R0_5, tempRot, tempHandState);
	//D1_L1R0_6
	MechanicalStep D1_L1R0_6[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_1][L_1_R_0][6].Set(10, D1_L1R0_6, tempRot, tempHandState);
	//D1_L1R0_7
	MechanicalStep D1_L1R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_1_R_0][7].Set(11, D1_L1R0_7, tempRot, tempHandState);
	//D1_L1R0_8
	MechanicalStep D1_L1R0_8[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_1_R_0][8].Set(11, D1_L1R0_8, tempRot, tempHandState);
	//D1_L1R0_9
	MechanicalStep D1_L1R0_9[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_1_R_0][9].Set(11, D1_L1R0_9, tempRot, tempHandState);
	//D1_L1R0_10
	MechanicalStep D1_L1R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_1_R_0][10].Set(12, D1_L1R0_10, tempRot, tempHandState);
	//D1_L1R0_11
	MechanicalStep D1_L1R0_11[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_1][L_1_R_0][11].Set(13, D1_L1R0_11, tempRot, tempHandState);
	//D1_L1R0_12
	MechanicalStep D1_L1R0_12[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_1][L_1_R_0][12].Set(13, D1_L1R0_12, tempRot, tempHandState);
	//D1_L1R0_13
	MechanicalStep D1_L1R0_13[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L2, M_R1, M_RC, M_R1, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_1][L_1_R_0][13].Set(14, D1_L1R0_13, tempRot, tempHandState);
	//D1_L1R0_14
	MechanicalStep D1_L1R0_14[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_1][L_1_R_0][14].Set(15, D1_L1R0_14, tempRot, tempHandState);
	//D1_L1R0_15
	MechanicalStep D1_L1R0_15[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_1][L_1_R_0][15].Set(15, D1_L1R0_15, tempRot, tempHandState);
}
void D2_L1R0Init(void)
{
	//D2_L1R0_0
	MechanicalStep D2_L1R0_0[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_1_R_0][0].Set(8, D2_L1R0_0, tempRot, tempHandState);
	//D2_L1R0_1
	MechanicalStep D2_L1R0_1[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_RC, M_R2, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_1_R_0][1].Set(9, D2_L1R0_1, tempRot, tempHandState);
	//D2_L1R0_2
	MechanicalStep D2_L1R0_2[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_L2, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_1_R_0][2].Set(9, D2_L1R0_2, tempRot, tempHandState);
	//D2_L1R0_3
	MechanicalStep D2_L1R0_3[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_2][L_1_R_0][3].Set(9, D2_L1R0_3, tempRot, tempHandState);
	//D2_L1R0_4
	MechanicalStep D2_L1R0_4[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_2][L_1_R_0][4].Set(9, D2_L1R0_4, tempRot, tempHandState);
	//D2_L1R0_5
	MechanicalStep D2_L1R0_5[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_1_R_0][5].Set(10, D2_L1R0_5, tempRot, tempHandState);
	//D2_L1R0_6
	MechanicalStep D2_L1R0_6[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_2][L_1_R_0][6].Set(10, D2_L1R0_6, tempRot, tempHandState);
	//D2_L1R0_7
	MechanicalStep D2_L1R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_2][L_1_R_0][7].Set(11, D2_L1R0_7, tempRot, tempHandState);
	//D2_L1R0_8
	MechanicalStep D2_L1R0_8[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_2][L_1_R_0][8].Set(11, D2_L1R0_8, tempRot, tempHandState);
	//D2_L1R0_9
	MechanicalStep D2_L1R0_9[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_2][L_1_R_0][9].Set(11, D2_L1R0_9, tempRot, tempHandState);
	//D2_L1R0_10
	MechanicalStep D2_L1R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_2][L_1_R_0][10].Set(12, D2_L1R0_10, tempRot, tempHandState);
	//D2_L1R0_11
	MechanicalStep D2_L1R0_11[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_1_R_0][11].Set(13, D2_L1R0_11, tempRot, tempHandState);
	//D2_L1R0_12
	MechanicalStep D2_L1R0_12[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_1_R_0][12].Set(13, D2_L1R0_12, tempRot, tempHandState);
	//D2_L1R0_13
	MechanicalStep D2_L1R0_13[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L2, M_R1, M_RC, M_R2, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_2][L_1_R_0][13].Set(14, D2_L1R0_13, tempRot, tempHandState);
	//D2_L1R0_14
	MechanicalStep D2_L1R0_14[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_1_R_0][14].Set(15, D2_L1R0_14, tempRot, tempHandState);
	//D2_L1R0_15
	MechanicalStep D2_L1R0_15[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_2][L_1_R_0][15].Set(15, D2_L1R0_15, tempRot, tempHandState);
}
void D3_L1R0Init(void)
{
	//D3_L1R0_0
	MechanicalStep D3_L1R0_0[] = { M_RO, M_L3, M_RC, M_R3, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_3][L_1_R_0][0].Set(4, D3_L1R0_0, tempRot, tempHandState);
	//D3_L1R0_1
	MechanicalStep D3_L1R0_1[] = { M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, 1, 2, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_1_R_0][1].Set(5, D3_L1R0_1, tempRot, tempHandState);
	//D3_L1R0_2
	MechanicalStep D3_L1R0_2[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_3][L_1_R_0][2].Set(8, D3_L1R0_2, tempRot, tempHandState);
	//D3_L1R0_3
	MechanicalStep D3_L1R0_3[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_1_R_0][3].Set(9, D3_L1R0_3, tempRot, tempHandState);
	//D3_L1R0_4
	MechanicalStep D3_L1R0_4[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_1_R_0][4].Set(9, D3_L1R0_4, tempRot, tempHandState);
	//D3_L1R0_5
	MechanicalStep D3_L1R0_5[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_L3, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_3][L_1_R_0][5].Set(9, D3_L1R0_5, tempRot, tempHandState);
	//D3_L1R0_6
	MechanicalStep D3_L1R0_6[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_RC, M_R3, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_3][L_1_R_0][6].Set(9, D3_L1R0_6, tempRot, tempHandState);
	//D3_L1R0_7
	MechanicalStep D3_L1R0_7[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	tempRot.Set(0, -1, 2, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_1_R_0][7].Set(10, D3_L1R0_7, tempRot, tempHandState);
	//D3_L1R0_8
	MechanicalStep D3_L1R0_8[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(2, 1, 1, 1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_1_R_0][8].Set(11, D3_L1R0_8, tempRot, tempHandState);
	//D3_L1R0_9
	MechanicalStep D3_L1R0_9[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_1_R_0][9].Set(11, D3_L1R0_9, tempRot, tempHandState);
	//D3_L1R0_10
	MechanicalStep D3_L1R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_1_R_0][10].Set(11, D3_L1R0_10, tempRot, tempHandState);
	//D3_L1R0_11
	MechanicalStep D3_L1R0_11[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	tempRot.Set(2, -1, 1, -1, 0, -1);
	tempHandState.Set(0, 0, 0, 0);
	MechanicalGroupLib[D][_3][L_1_R_0][11].Set(12, D3_L1R0_11, tempRot, tempHandState);
	//D3_L1R0_12
	MechanicalStep D3_L1R0_12[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	tempRot.Set(1, -1, 2, 1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_3][L_1_R_0][12].Set(13, D3_L1R0_12, tempRot, tempHandState);
	//D3_L1R0_13
	MechanicalStep D3_L1R0_13[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	tempRot.Set(1, 1, 2, -1, 0, -1);
	tempHandState.Set(0, 0, 1, 0);
	MechanicalGroupLib[D][_3][L_1_R_0][13].Set(13, D3_L1R0_13, tempRot, tempHandState);
	//D3_L1R0_14
	MechanicalStep D3_L1R0_14[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	tempRot.Set(2, -1, 0, 1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_3][L_1_R_0][14].Set(15, D3_L1R0_14, tempRot, tempHandState);
	//D3_L1R0_15
	MechanicalStep D3_L1R0_15[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	tempRot.Set(2, 1, 0, -1, 1, -1);
	tempHandState.Set(0, 0, 0, 1);
	MechanicalGroupLib[D][_3][L_1_R_0][15].Set(15, D3_L1R0_15, tempRot, tempHandState);
}
void LibInit(void)
{
	F1_L0R0Init();
	F1_L0R1Init();
	F1_L1R0Init();
	F2_L0R0Init();
	F2_L0R1Init();
	F2_L1R0Init();
	F3_L0R0Init();
	F3_L0R1Init();
	F3_L1R0Init();
	R1_L0R0Init();
	R1_L0R1Init();
	R1_L1R0Init();
	R2_L0R0Init();
	R2_L0R1Init();
	R2_L1R0Init();
	R3_L0R0Init();
	R3_L0R1Init();
	R3_L1R0Init();
	U1_L0R0Init();
	U1_L0R1Init();
	U1_L1R0Init();
	U2_L0R0Init();
	U2_L0R1Init();
	U2_L1R0Init();
	U3_L0R0Init();
	U3_L0R1Init();
	U3_L1R0Init();
	B1_L0R0Init();
	B1_L0R1Init();
	B1_L1R0Init();
	B2_L0R0Init();
	B2_L0R1Init();
	B2_L1R0Init();
	B3_L0R0Init();
	B3_L0R1Init();
	B3_L1R0Init();
	L1_L0R0Init();
	L1_L0R1Init();
	L1_L1R0Init();
	L2_L0R0Init();
	L2_L0R1Init();
	L2_L1R0Init();
	L3_L0R0Init();
	L3_L0R1Init();
	L3_L1R0Init();
	D1_L0R0Init();
	D1_L0R1Init();
	D1_L1R0Init();
	D2_L0R0Init();
	D2_L0R1Init();
	D2_L1R0Init();
	D3_L0R0Init();
	D3_L0R1Init();
	D3_L1R0Init();

}
//LibInit end
//=========================================================================初始化操作库中各个操作组合的总时间
int Time_Air = 120;
int Tim_KZ90 = 53;
int Tim_ND90 = 54;
int Tim_ND180 = 90;
int Tim_DD90 = 122;
int Tim_DD180 = 194;

#define CLOSE 0
#define OPEN  1
void LibTimeInit(void)
{
	//MechanicalGroupLib[F][_1][L_0_R_0][0]
	for (int i = F; i <= D; i++)
	{
		for (int j = _1; j <= _3; j++)
		{
			for (int k = L_0_R_0; k <= L_1_R_0; k++)
			{
				for (int l = 0; l<16; l++)
				{
					//MechanicalGroupLib[i][j][k][l]
					MechanicalGroupLib[i][j][k][l].time = 0;
					int LeftHand = CLOSE;
					int RightHand = CLOSE;
					for (int m = 0; MechanicalGroupLib[i][j][k][l].Steps[m].num != -1; m++)
					{
						//气缸动
						if (MechanicalGroupLib[i][j][k][l].Steps[m].num == LO)
						{
							LeftHand = OPEN;
							MechanicalGroupLib[i][j][k][l].time += Time_Air;

						}
						else if (MechanicalGroupLib[i][j][k][l].Steps[m].num == LC)
						{
							LeftHand = CLOSE;
						}
						else if (MechanicalGroupLib[i][j][k][l].Steps[m].num == RO)
						{
							RightHand = OPEN;
							MechanicalGroupLib[i][j][k][l].time += Time_Air;
						}
						else if (MechanicalGroupLib[i][j][k][l].Steps[m].num == RC)
						{
							RightHand = CLOSE;
						}
						else  //电机动
						{
							//拧动
							if ((RightHand == CLOSE) && (LeftHand == CLOSE))  
							{
								if ((MechanicalGroupLib[i][j][k][l].Steps[m].num == L2) ||
									(MechanicalGroupLib[i][j][k][l].Steps[m].num == R2))
								{
									MechanicalGroupLib[i][j][k][l].time += Tim_ND180;
								}
								else
								{
									MechanicalGroupLib[i][j][k][l].time += Tim_ND90;
								}
							}
							//空转
							else if ((MechanicalGroupLib[i][j][k][l].Steps[m].num == L1) ||
								(MechanicalGroupLib[i][j][k][l].Steps[m].num == L3) &&
								((RightHand == CLOSE) && (LeftHand == OPEN)))
							{
								MechanicalGroupLib[i][j][k][l].time += Tim_KZ90;
							}
							else if ((MechanicalGroupLib[i][j][k][l].Steps[m].num == R1) ||
								(MechanicalGroupLib[i][j][k][l].Steps[m].num == R3) &&
								((RightHand == OPEN) && (LeftHand == CLOSE)))
							{
								MechanicalGroupLib[i][j][k][l].time += Tim_KZ90;
							}
							//带动
							else if ((MechanicalGroupLib[i][j][k][l].Steps[m].num == L1) ||
								(MechanicalGroupLib[i][j][k][l].Steps[m].num == L3) &&
								((RightHand == OPEN) && (LeftHand == CLOSE)))
							{
								MechanicalGroupLib[i][j][k][l].time += Tim_DD90;
							}
							else if ((MechanicalGroupLib[i][j][k][l].Steps[m].num == R1) ||
								(MechanicalGroupLib[i][j][k][l].Steps[m].num == R3) &&
								((RightHand == CLOSE) && (LeftHand == OPEN)))
							{
								MechanicalGroupLib[i][j][k][l].time += Tim_DD90;
							}
							else if ((MechanicalGroupLib[i][j][k][l].Steps[m].num == L2) &&
								((RightHand == OPEN) && (LeftHand == CLOSE)))
							{
								MechanicalGroupLib[i][j][k][l].time += Tim_DD180;
							}
							else if ((MechanicalGroupLib[i][j][k][l].Steps[m].num == R2) &&
								((RightHand == CLOSE) && (LeftHand == OPEN)))
							{
								MechanicalGroupLib[i][j][k][l].time += Tim_DD180;
							}
						}
					}
				}
			}
		}
	}
}