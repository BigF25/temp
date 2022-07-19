#include "Global.h"

using namespace std;

//=========================================全局常量
const string moveStr[10] = { "M_L1", "M_L2", "M_L3", "M_LC", "M_LO", "M_R1", "M_R2", "M_R3", "M_RC", "M_RO" };

//=========================================理论步骤
struct TheoryStep
{
	Point3 face;
	int distance;
};
//=====================================================dfs全局变量
int g_time[2];
int g_StepNum[2];
int g_TheoryStrStep[2];
TheoryStep g_TheorySteps[25];
TheoryStep g_TheorySteps2[2][25];
Rot g_CubeRot;
HandState g_HandState;
int g_MovBuff[2][120];
//=====================================================dfs存储变量
int s_time[2];
int s_StepNum[2];
int s_MovBuff[2][120];
HandState s_HandState[2];
Rot s_Rot[2];
//=====================================================函数声明
void dfs(int step,int state);
int Char2Int(char inChar);
int Point3ToNum(Point3 inPoint);
int RotMtplPoint3ToNum(Rot rot, Point3 face);
void BookInit(void);
bool IsTheorySended(void);
string GetTheory(void);
void SaveMechanical(void);
//=====================================================标志每一层搜索后的魔方状态 
//   //step //state //L_0_R_0  //x//y//z 
int book[25][2][3][3][2][3][2][3][2];
void main(void)
{
	cout << "初始化..." << endl;
	MechanicalStepsInit();
	RotInit();
	PointInit();
	LibInit();
	LibTimeInit();

	DWORD t_start, t_end;
	while (1)
	{
		cout << "等待Theory... " << endl;
		while (!IsTheorySended())  //等待问题被放入
		{
			Sleep(10);
		}
		t_start = GetTickCount();
	   // string TheoryStr = "R2 B1 F1 R2 D2 F1 D2 R3 B1 B1 D1 L1 U3 F2 L1 R3 B1 B1 D1 L1 ";
		string TheoryStr = GetTheory();
		cout << "成功得到Theory:" << endl;
		cout << TheoryStr << endl;
		//初始化g_TheoryStrStep
		int TheoryStrLength = TheoryStr.length() / 3;
		//分段搜索
		g_TheoryStrStep[0] = TheoryStrLength;
		g_TheoryStrStep[1] = 0;   //段长为0

		//初始化g_TheorySteps,g_TheorySteps2
		for (int i = 0; i < TheoryStrLength; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				g_TheorySteps[i].face.a[j][0] = P_FRUBLD[Char2Int(TheoryStr[i * 3])].a[j][0];
			}
			g_TheorySteps[i].distance = TheoryStr[i * 3 + 1] - 0x30 - 1;
		}
		for (int i = 0; i < g_TheoryStrStep[0]; i++)
		{
			g_TheorySteps2[0][i] = g_TheorySteps[i];
		}
		for (int i = 0; i < g_TheoryStrStep[1]; i++)
		{
			g_TheorySteps2[1][i] = g_TheorySteps[i + g_TheoryStrStep[0]];
		}
		//初始化搜索用的变量
		BookInit();
		g_HandState.Set(0, 0, 0, 0); 
		g_CubeRot.Set(0, 1, 1, 1, 2, 1);
		g_time[0] = 0;
		g_time[1] = 0;
		s_time[0] = 1000000;
		s_time[1] = 1000000;
		g_StepNum[0] = 0;
		g_StepNum[1] = 0;
		s_StepNum[0] = 1000;
		s_StepNum[1] = 1000;
		for (int i = 0; i < 120; i++)
		{
			g_MovBuff[0][i] = -1;
			g_MovBuff[1][i] = -1;
			s_MovBuff[0][i] = -1;
			s_MovBuff[1][i] = -1;
		}
		//深搜
		//cout << "开始搜索..." << endl;
		dfs(0, 0);  //第一阶段
		g_CubeRot = s_Rot[0];
		g_HandState = s_HandState[0];
		dfs(0, 1);  //第二阶段
		//cout << "搜索结束!" << endl;
		cout << "步骤数量:" << s_StepNum[0] + s_StepNum[1] << endl << endl;
		cout <<"执行步骤所需时间(ms): "<<endl << s_time[0] + s_time[1] << endl;
		// 打印输出
		SaveMechanical();
		//cout << "保存成功!" <<endl;

		//cout <<endl<< TheoryStr << endl;
		//cout << s_StepNum[0] + s_StepNum[1] << " " << s_time[0] + s_time[1] << endl;
		//cout << "Answer:" << endl;
		//for (int i = 0; i < s_StepNum[0]; i++)
		//{
		//	cout << moveStr[s_MovBuff[0][i]] << " ";
		//}
		//for (int i = 0; i < s_StepNum[1]; i++)
		//{
		//	cout << moveStr[s_MovBuff[1][i]] << " ";
		//}
		//cout << endl;
		t_end = GetTickCount();
		std::cout << "搜索用时"<<t_end - t_start << "ms" << endl;
		//break;
	}
	system("pause");
}
void SaveMechanical(void)
{
	 ofstream in;
	 //写入
	in.open(".\\Bridge\\Mechanical.txt", ios::trunc); //ios::trunc表示在打开文件前将文件清空,由于是写入,文件不存在则创建
	for (int i = 0; i < s_StepNum[0]; i++)
	{
		in << moveStr[s_MovBuff[0][i]] << " ";
	}
	in.close();

	//创建标志位
	in.open(".\\Bridge\\MechanicalFlag.txt", ios::trunc); //ios::trunc表示在打开文件前将文件清空,由于是写入,文件不存在则创建
	in.close();
}
string GetTheory(void)
{
	fstream infile;
	infile.open(".\\Bridge\\Theory.txt");
	if (!infile) cout << "read Theory.txt error" << endl;

	string str;
	getline(infile, str);   //按行读取,遇到换行符结束
	infile.close();
	remove(".\\Bridge\\Theory.txt");
	return str;
}
bool IsTheorySended(void)
{
	char FileName[] = ".\\Bridge\\TheoryFlag.txt";

	std::fstream _file;
	_file.open(FileName, ios::in);
	if (!_file)
	{
		return false;  //不存在
	}
	else
	{
		_file.close();
		remove(FileName);
		return true;
	}
}
void dfs(int step,int state)
{
	//到达最深处，判断此时g_time是否比s_stime小
	if (step == g_TheoryStrStep[state])
	{
		if (g_time[state]<s_time[state])
		{
			s_time[state] = g_time[state];
			s_StepNum[state] = g_StepNum[state];

			for (int i = 0; i<120; i++)
			{
				s_MovBuff[state][i] = g_MovBuff[state][i];
				if (g_MovBuff[state][i] == -1)break;
			}
			s_Rot[state] = g_CubeRot;
			s_HandState[state] = g_HandState;
			//cout << "state:" << state << "Step: " << g_StepNum[state] << "  Min Time:" << s_time[state];
			//cout << "Handstate: " << g_HandState.LeftNotNice<<g_HandState.RightNotNice << endl;
		}
		return;
	}
	//========================获取face
	int face;
	int __i;
	for (__i = 0; __i < 3; ++__i)
	{
		if (g_TheorySteps2[state][step].face.a[__i][0] != 0)break;
	}
	//face.a[i][0] != 0
	int __j;
	for (__j = 0; __j < 3; ++__j)
	{
		if (g_CubeRot.a[__j][__i] != 0)break;
	}
	if (__j == 0)
	{
		if (g_CubeRot.a[__j][__i] == g_TheorySteps2[state][step].face.a[__i][0])face = F;
		else face = B;
	}
	else if (__j == 1)
	{
		if (g_CubeRot.a[__j][__i] == g_TheorySteps2[state][step].face.a[__i][0])face = R;
		else face = L;
	}
	else if (__j == 2)
	{
		if (g_CubeRot.a[__j][__i] == g_TheorySteps2[state][step].face.a[__i][0])face = U;
		else face = D;
	}
	//========================获取face结束

	int j = g_TheorySteps2[state][step].distance;
	int k = g_HandState.LeftNotNice *2 + g_HandState.RightNotNice;  //注意此处二进制关系
	for (int l = 0; l<16; l++)
	{
		//保存当前状态
		Rot _tempRot = g_CubeRot;
		HandState _tempHandState = g_HandState;
		int tempStepNum = g_StepNum[state];
		int tempTime = g_time[state];
		int tempMoveBuff[120];
		for (int _i = 0; _i<120; _i++)
		{
			tempMoveBuff[_i] = g_MovBuff[state][_i];
			if (g_MovBuff[state][_i] == -1)break;  //注意此处已经赋值了一个-1
		}
		//加入本次节点
		g_time[state] += MechanicalGroupLib[face][j][k][l].time; 
		g_CubeRot = RotMtplRot(MechanicalGroupLib[face][j][k][l].rot, g_CubeRot);
		g_HandState = MechanicalGroupLib[face][j][k][l].endHandState;
		for (int _i = 0; _i < MechanicalGroupLib[face][j][k][l].StepNum; _i++)
		{
			g_MovBuff[state][g_StepNum[state] + _i] = MechanicalGroupLib[face][j][k][l].Steps[_i].num;
		}
		g_StepNum[state] += MechanicalGroupLib[face][j][k][l].StepNum;
		//=================查看此结果在此深度下有没有发生过
		int row[3], num[3];
		for (int _i = 0; _i < 3; _i++)  //3列
		{
			for (row[_i] = 0; row[_i] < 3; row[_i]++)
			{
				if (g_CubeRot.a[row[_i]][_i] != 0)
				{
					if (g_CubeRot.a[row[_i]][_i] == -1)
						num[_i] = 0;
					else if (g_CubeRot.a[row[_i]][_i] == 1)
						num[_i] = 1;
					else num[_i] = -1;
					break;
				}
			}
		}
		int hand = g_HandState.LeftNotNice * 2 + g_HandState.RightNotNice;  //注意此处二进制关系
		if (g_time[state] < book[step][state][hand][row[0]][num[0]][row[1]][num[1]][row[2]][num[2]])  //当前时间比历史最短时间短
		{
			book[step][state][hand][row[0]][num[0]][row[1]][num[1]][row[2]][num[2]] = g_time[state];
			//深搜
			dfs(step + 1, state);
			//复原之前保存的状态
			g_CubeRot = _tempRot;
			g_HandState = _tempHandState;
			g_time[state] = tempTime;
			g_StepNum[state] = tempStepNum;
			for (int _i = 0; _i<120; _i++)
			{
				g_MovBuff[state][_i] = tempMoveBuff[_i];
				if (tempMoveBuff[_i] == -1)break;
			}
		}
		else
		{
			//复原之前保存的状态
			g_CubeRot = _tempRot;
			g_HandState = _tempHandState;
			g_time[state] = tempTime;
			g_StepNum[state] = tempStepNum;
			for (int _i = 0; _i<120; _i++)
			{
				g_MovBuff[state][_i] = tempMoveBuff[_i];
				if (tempMoveBuff[_i] == -1)break;
			}
		}
		//=================查看此结果在此深度下有没有发生过结束
	}
}
//字符转换
int Char2Int(char inChar)
{
	switch (inChar)
	{
	case 'F':return F; break;
	case 'R':return R; break;
	case 'U':return U; break;
	case 'B':return B; break;
	case 'L':return L; break;
	case 'D':return D; break;
	default:return -1;
	}
	return -1;
}
//查找
int Point3ToNum(Point3 inPoint)
{
	if (inPoint.a[0][0] == 1)return F;
	if (inPoint.a[0][0] == -1)return B;
	if (inPoint.a[1][0] == 1)return R;
	if (inPoint.a[1][0] == -1)return L;
	if (inPoint.a[2][0] == 1)return U;
	if (inPoint.a[2][0] == -1)return D;
	return -1;
}
int RotMtplPoint3ToNum(Rot rot, Point3 face)
{
	int i;  
	for (i = 0; i < 3; ++i)
	{
		if (face.a[i][0] != 0)break;
	}
	//face.a[i][0] != 0
	int j;
	for (j = 0; j < 3; ++j)
	{
		if (rot.a[j][i] != 0)break;
	}
	if (j == 0)
	{
		if (rot.a[j][i] == face.a[i][0])return F;
		else return B;
	}
	else if (j == 1)
	{
		if (rot.a[j][i] == face.a[i][0])return R;
		else return L;
	}
	else if (j == 2)
	{
		if (rot.a[j][i] == face.a[i][0])return U;
		else return D;
	}
	return -1;
}
void BookInit(void)
{
	//book[25][3][3][2][3][2][3][2];
	for (int i = 0; i < 25;i++)
	for (int state = 0; state < 2;state++)
	for (int j = 0; j < 3;j++)
	for (int k = 0; k < 3;k++)
	for (int l = 0; l < 2;l++)
	for (int m = 0; m < 3;m++)
	for (int n = 0; n < 2;n++)
	for (int o = 0; o < 3;o++)
	for (int p = 0; p < 2; p++)
	{
		book[i][state][j][k][l][m][n][o][p] = 1000000;
	}
}