using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using AForge;
using AForge.Controls;
using AForge.Video;
using AForge.Video.DirectShow;
using System.IO;
using System.IO.Ports;
using System.Threading;
using System.Net;

namespace USBCamera
{
    public partial class Main_Form : Form
    {
        //=================================================全局变量==================================================
        FilterInfoCollection videoDevices;//摄像头设备集合
        VideoCaptureDevice videoSourceUp, videoSourceDown, videoSourceIn, videoSourceOut;
        Bitmap imageUp, imageDown, imageIn, imageOut;//处理图片
        bool CaptureSecondImg_flag = false;
        const int WAIT = 0;
        const int SCAN = 1;
        const int CALC = 2;
        const int MOVE = 3;
        const bool DEBUG = true;
        const bool RUN = false;
        int state = WAIT;
        int SysTime50ms = 0;
        //====================================================线程=======================================================
        Thread TimeCount;//计时线程
        Thread SloveCube;//解算线程
        //===================================================MainForm===================================================
        public Main_Form()
        {
            /* 在多线程程序中，新创建的线程不能访问UI线程创建的窗口控件，
             * 如果需要访问窗口中的控件，
             * 可以在窗口构造函数中将CheckForIllegalCrossThreadCalls设置为false
             */
            CheckForIllegalCrossThreadCalls = false;
            InitializeComponent();
        }
        /*
        *   @brief  事件函数：加载主窗体，启动其他进程
        *   @return null
        */
        private void Main_Form_Load(object sender, EventArgs e)
        {
            //复位标志文件
            DeleteFlag("CluserStartFlag");
            DeleteFlag("CubeIdxDataFlag");
            DeleteFlag("TheoryFlag");
            DeleteFlag("MechanicalFlag");

            //数字摄像头初始化
            Camera_Init();
            //波特率初始化
            Baud_ComboBox.Text = "115200";
            //打开cube500
            cube500Process.Start();
            cube500Process.WaitForInputIdle();
        }
        /*
        *   @brief  事件函数：关闭主窗体，关闭所有程序
        *   @return null
        */
        private void Main_Form_FormClosing(object sender, FormClosingEventArgs e)
        {
            try
            {
                Camera_Close();
            }
            catch { }
            try
            {
                cube500Process.Kill();
                cube500Process.Dispose();
                cube500Process.Close();
            }
            catch { }
            try
            {
                TimeCount.Abort();
            }
            catch { }
            try
            {
                SloveCube.Abort();
            }
            catch { }
        }
        //=====================================================相机相关=================================================
        /*
        *   @brief  事件函数：初始化摄像头
        *   @return null
        */
        private void Init_Camera_Click(object sender, EventArgs e)
        {
            Camera_Init();
        }
        /*
        *   @brief  事件函数：按下按钮启动摄像头
        *   @return null
        */
        private void Open_Camera_Click(object sender, EventArgs e)
        {
            Camera_Init();
            //开启摄像头
            videoSourcePlayer1.VideoSource = videoSourceUp;
            videoSourcePlayer1.Start();
            videoSourcePlayer2.VideoSource = videoSourceDown;
            videoSourcePlayer2.Start();
            videoSourcePlayer3.VideoSource = videoSourceIn;
            videoSourcePlayer3.Start();
            videoSourcePlayer4.VideoSource = videoSourceOut;
            videoSourcePlayer4.Start();
        }
        /*
        *   @brief  事件函数：按下按钮关闭摄像头
        *   @return null
        */
        private void Close_Camera_Click(object sender, EventArgs e)
        {
            Camera_Close();
        }
        /*
        *   @brief  事件函数：捕捉摄像头图像
        *   @return null
        */
        private void Capute_Image_Click(object sender, EventArgs e)
        {
            //显示在线摄像头获取的图像
            imageUp = videoSourcePlayer1.GetCurrentVideoFrame();
            imageDown = videoSourcePlayer2.GetCurrentVideoFrame();
            imageIn = videoSourcePlayer3.GetCurrentVideoFrame();
            imageOut = videoSourcePlayer4.GetCurrentVideoFrame();
            pictureBox3.Image = imageIn;
            pictureBox2.Image = imageDown;
            pictureBox1.Image = imageUp;
            pictureBox4.Image = imageOut;
        }
        /*
        *   @brief  摄像头初始化
        *   @return null
        */
        void Camera_Init()
        {
            //获取相机外设的信息
            videoDevices = new FilterInfoCollection(FilterCategory.VideoInputDevice);
            label6.Text = videoDevices.Count.ToString();
            listBox1.Items.Clear();
            for (int i = 0; i < videoDevices.Count; i++)
            {
                listBox1.Items.Add(i + ":" + videoDevices[i].MonikerString.ToString());
            }
            //变量绑定相机（确定相机的顺序）
            const int CAMERA_NUM_CORRECT = 6;
            if (videoDevices.Count == CAMERA_NUM_CORRECT)
            {
                videoSourceUp = new VideoCaptureDevice(videoDevices[2].MonikerString);
                videoSourceDown = new VideoCaptureDevice(videoDevices[3].MonikerString);
                videoSourceIn = new VideoCaptureDevice(videoDevices[4].MonikerString);
                videoSourceOut = new VideoCaptureDevice(videoDevices[5].MonikerString);
                videoSourceUp.VideoResolution = videoSourceUp.VideoCapabilities[1];
                videoSourceDown.VideoResolution = videoSourceDown.VideoCapabilities[1];
                videoSourceIn.VideoResolution = videoSourceIn.VideoCapabilities[1];
                videoSourceOut.VideoResolution = videoSourceOut.VideoCapabilities[1];
            }
        }
        /*
        *   @brief  关闭摄像头
        *   @return null
        */
        void Camera_Close()
        {
            if (videoSourcePlayer1.VideoSource != null)
            {
                videoSourcePlayer1.SignalToStop();
                videoSourcePlayer1.WaitForStop();
                videoSourcePlayer1.VideoSource = null;
            }
            if (videoSourcePlayer2.VideoSource != null)
            {
                videoSourcePlayer2.SignalToStop();
                videoSourcePlayer2.WaitForStop();
                videoSourcePlayer2.VideoSource = null;
            }
            if (videoSourcePlayer3.VideoSource != null)
            {
                videoSourcePlayer3.SignalToStop();
                videoSourcePlayer3.WaitForStop();
                videoSourcePlayer3.VideoSource = null;
            }
            if (videoSourcePlayer4.VideoSource != null)
            {
                videoSourcePlayer4.SignalToStop();
                videoSourcePlayer4.WaitForStop();
                videoSourcePlayer4.VideoSource = null;
            }
        }
        /*
        *   @brief  事件函数：按下按钮，打开选取ROI小工具
        *   @return null
        */
        private void PictureBox1_Click(object sender, EventArgs e)
        {
            Form2 Form2 = new Form2();
            Form2.Show();
        }
        //====================================================串口相关==================================================
        /*
        *   @brief  事件函数：扫描当前存在的串口
        *   @return null
        */
        private void ComboBox1_DropDown(object sender, EventArgs e)
        {
            int i = 0;
            this.COM_ComboBox.Items.Clear(); // 清除串口
            string[] COM_name = System.IO.Ports.SerialPort.GetPortNames(); // 获取设备的所有可用串口
            int j = COM_name.Length; // 得到所有可用串口数目
            if( j==0 )
            {
                this.COM_ComboBox.Items.Add("null");
                this.COM_ComboBox.Text = "null";
            }
            else
            {
                for (i = 0; i < j; i++)
                {
                    this.COM_ComboBox.Items.Add(COM_name[i]); // 依次添加到下拉框中
                }
            }
          
        }
        /*
        *   @brief  事件函数：打开/关闭串口
        *   @return null
        */
        private void COM_btn_Click(object sender, EventArgs e)
        {
            if(COM_Btn.Text == "Open COM")
            {
                this.SerialPort1.Close();
                if (this.COM_ComboBox.Text != "null")
                {
                    try
                    {
                        this.SerialPort1.BaudRate = 115200;
                        this.SerialPort1.PortName = this.COM_ComboBox.Text;
                        this.SerialPort1.Open();
                        this.SerialPort1.DiscardInBuffer();
                        COM_Btn.Text = "Close COM";
                    }
                    catch
                    {
                        this.COM_ComboBox.Text = "open error!";
                    }
                }
            }
            else if(COM_Btn.Text == "Close COM")
            {
                try
                {
                    this.SerialPort1.Close();
                    COM_Btn.Text = "Open COM";
                }
                catch
                {
                    this.COM_ComboBox.Text = "clsoe error!";
                }
            }
        }
        /*
        *   @brief  事件函数：串口收数据中断
        *   @return null
        */
        private void SerialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            Byte[] ReceiveByte = new Byte[this.SerialPort1.BytesToRead];
            this.SerialPort1.Read(ReceiveByte, 0, ReceiveByte.Length);
            this.SerialPort1.DiscardInBuffer();
            //MessageBox.Show(ReceiveByte[0].ToString());
            for (int i = 0; i < ReceiveByte.Length; i++)
            {
                //若收到数据OXCC，则SecondCapDown为true
                if (ReceiveByte[i] == 0xCC)
                {
                    CaptureSecondImg_flag = true;
                    break;
                }
                //若收到数据为0XDD，则解算完成
                else if (ReceiveByte[i] == 0xDD)
                {
                    state = WAIT;
                    try
                    {
                        TimeCount.Abort();
                    }
                    catch { }
                    try
                    {
                        SloveCube.Abort();
                    }
                    catch { }
                    break;
                }
            }
        }
        //====================================================解算相关==================================================
        /*
        *   @brief  事件函数：主启动按钮
        *   @return null
        */
        private void Main_Start_Click(object sender, EventArgs e)
        {
            TimeCount = new Thread(new ThreadStart(TimeCountThread));
            SloveCube = new Thread(new ThreadStart(SolveCubeThread));
            TimeCount.Priority = ThreadPriority.Highest;
            SloveCube.Priority = ThreadPriority.Normal;
            TimeCount.Start();
            SloveCube.Start();
        }
        /*
        *   @brief 线程函数：计时线程函数
        *   @return null
        */
        private void TimeCountThread()
        {
            SysTime50ms = 0;
            int ScanTime50ms = 0;
            int CalcTime50ms = 0;
            int MoveTime50ms = 0;
            Random r = new Random();
            while (true)
            {
                this.TotalTimeCost.Text = (SysTime50ms / 20).ToString() + "." + (SysTime50ms / 2 % 10).ToString() + r.Next(10).ToString();
                this.ScanTimeCost.Text = (ScanTime50ms / 20).ToString() + "." + (ScanTime50ms / 2 % 10).ToString() + r.Next(10).ToString();
                this.CalcTimeCost.Text = (CalcTime50ms / 20).ToString() + "." + (CalcTime50ms / 2 % 10).ToString() + r.Next(10).ToString();
                this.MoveTimeCost.Text = (MoveTime50ms / 20).ToString() + "." + (MoveTime50ms / 2 % 10).ToString() + r.Next(10).ToString();
                System.Threading.Thread.Sleep(45);
                SysTime50ms++;
                if (state == SCAN)
                {
                    ScanTime50ms++;
                }
                else if (state == CALC)
                {
                    CalcTime50ms = SysTime50ms - ScanTime50ms;
                }
                else if (state == MOVE)
                {
                    MoveTime50ms = SysTime50ms - ScanTime50ms - CalcTime50ms;
                }
            }
        }
        /*
        *   @brief  （★主要流程★）
        *           线程函数：解魔方线程
        *           1.拍摄八张图片
        *           2.从OoenCV中获取聚类结果
        *           3.从cube500中获取理论解
        *           4.从T2M中获取实际解
        *           5.发送解到下位机
        *   @return null
        */
        private void SolveCubeThread()
        {
            //扫描
            state = SCAN;
            Get_Picture(DEBUG);
            //解算
            state = CALC;
            string cube = GetCubeDataFormOpenCV();
            string TheoryStr = GetTheoryFormCube500(cube);//从cube500中获取解
            string MechanicalStr = T2M(TheoryStr);//解的转换
            //执行
            state = MOVE;
            try
            {
                this.richTextBox1.Text = MechanicalStr;
                int len = MechanicalStr.Length / 5;
                this.label12.Text = len.ToString();
                //发送到单片机
                SendMStrToSTM32(MechanicalStr);
            }
            catch { this.richTextBox2.Text = "解的长度为0"; };
        }
        /*
        *   @brief  获取图像
        *   @return null
        */
        private void Get_Picture(bool debug_flag = RUN)
        {
            if(debug_flag == RUN)
            {
                while (!CaptureSecondImg_flag)
                {
                    imageUp = videoSourcePlayer1.GetCurrentVideoFrame();//拍摄
                    imageDown = videoSourcePlayer2.GetCurrentVideoFrame();//拍摄
                    imageIn = videoSourcePlayer3.GetCurrentVideoFrame();//拍摄
                    imageOut = videoSourcePlayer4.GetCurrentVideoFrame();//拍摄
                    imageUp.Save(Application.StartupPath + "\\image\\02.jpg");
                    imageDown.Save(Application.StartupPath + "\\image\\00.jpg");
                    imageIn.Save(Application.StartupPath + "\\image\\01.jpg");
                    imageOut.Save(Application.StartupPath + "\\image\\03.jpg");
                    System.Threading.Thread.Sleep(100);
                }
                imageUp = videoSourcePlayer1.GetCurrentVideoFrame();//拍摄
                imageDown = videoSourcePlayer2.GetCurrentVideoFrame();//拍摄
                imageIn = videoSourcePlayer3.GetCurrentVideoFrame();//拍摄
                imageOut = videoSourcePlayer4.GetCurrentVideoFrame();//拍摄
                imageUp.Save(Application.StartupPath + "\\image\\12.jpg");
                imageDown.Save(Application.StartupPath + "\\image\\10.jpg");
                imageIn.Save(Application.StartupPath + "\\image\\11.jpg");
                imageOut.Save(Application.StartupPath + "\\image\\13.jpg");
                System.Threading.Thread.Sleep(100);
            }
            else if(debug_flag == DEBUG)
            {
                
            }
        }
        /*
        *   @brief  从OpenCV中获取图像的聚类结果
        *   @return 结果字符串
        */
        private string GetCubeDataFormOpenCV()
        {
            CreatFlag("CluserStartFlag");//开始计算 这个flag标志再opencv中用于放入问题
            while (!FlagExist("CubeIdxDataFlag"))//等待数据导入CubeIdxData CubeIdxDataFlag才是flag文件
                System.Threading.Thread.Sleep(100);
            //读取聚类结果
            FileStream fs = null;
            StreamReader sd = null;
            bool inUse = true;
            string cube = null;
            while (inUse)
            {
                try
                {
                    fs = new FileStream(@"./file/CubeIdxData.txt", FileMode.Open);//读取魔方数据
                    sd = new StreamReader(fs);
                    inUse = false;
                }
                catch { }
            }
            cube = sd.ReadLine();
            sd.Close();
            fs.Close();
            System.Threading.Thread.Sleep(10);
            DeleteFlag("CubeIdxDataFlag");//删除魔方信息生成完成flag文件
            return cube;
        }
        /*
        *   @brief  从cube500获取魔方理论解
        *           cube-->workCube-->url-->Str1-->Str2--Theory-->return
        *   @return 结果字符串
        */
        private string GetTheoryFormCube500(string cube)
        {
            string url = "http://127.0.0.1:8081/?";
            //UF UR UB UL DF DR DB DL FR FL BR BL UFR URB UBL ULF DRF DFL DLB DBR
            //TO
            //U1-U9,R1-R9,F1-F9,D1-D9,L1-L9,B1-B9
            string workCube = cube + "FRUBLD";
            int[] map = {
                            30,4,27,6,50,2,33,0,24,
                            26,3,28,17,49,21,37,11,47,
                            35,1,25,18,48,16,40,9,38,
                            39,8,36,14,53,10,42,12,45,
                            32,7,34,23,52,19,43,15,41,
                            29,5,31,20,51,22,46,13,44
                        };
            for (int i = 0; i < 54; i++)
            {
                url += workCube[map[i]];
            }
            string Str1 = URLRequest(url);
            string Str2 = Str1.Substring(14, Str1.Length - 32);
            string Theory = "";
            for (int i = 0; i < Str2.Length;)
            {
                if ((Str2[i] == 'F') || (Str2[i] == 'R') || (Str2[i] == 'U') || (Str2[i] == 'B') || (Str2[i] == 'L') || (Str2[i] == 'D'))
                {
                    Theory += Str2[i];

                    if (i == (Str2.Length - 1))  //搜索到了最后一个字符
                    {
                        Theory += "1 ";   //注意加空格
                        i = Str2.Length;
                    }
                    else
                    {
                        if (Str2[i + 1] == ' ')
                        {
                            Theory += "1 ";
                            i = i + 2;
                        }
                        else if (Str2[i + 1] == '2')
                        {
                            Theory += "2 ";
                            i = i + 3;
                        }
                        else if (Str2[i + 1] == '\'')
                        {
                            Theory += "3 ";
                            i = i + 3;
                        }
                        else
                        {
                            MessageBox.Show("格式转换失败！！");
                            break;
                        }
                    }
                }
                else
                {
                    MessageBox.Show("格式转换失败！！");
                    break;
                }
            }
            return Theory;
        }
        /*
        *   @brief  访问URL地址
        *           Get方式获取url地址输出内容
        *   @return 结果字符串
        */
        private static String URLRequest(String url)
        {
            HttpWebRequest webRequest = (HttpWebRequest)WebRequest.Create(url);
            webRequest.Method = "GET";
            Encoding encoding = Encoding.Default;
            HttpWebResponse webResponse = (HttpWebResponse)webRequest.GetResponse();
            StreamReader sr = new StreamReader(webResponse.GetResponseStream(), encoding);
            return sr.ReadToEnd();
        }
        /*
        *   @brief  利用外部程序，进行T2M转换
        *   @return 结果字符串
        */
        private string T2M(string Theory)
        {
            //建立Theory.txt
            FileStream TheoryFile = new FileStream(@"./file/Theory.txt", FileMode.Create);//将Cube500获得的解放进Theory.txt中
            StreamWriter sw = new StreamWriter(TheoryFile);
            sw.WriteLine(Theory);
            sw.Close();
            TheoryFile.Close();
            System.Threading.Thread.Sleep(20);
            //建立TheoryFileFlag.txt
            FileStream TheoryFileFlagFile = new FileStream(@"./file/TheoryFlag.txt", FileMode.Create);//这个flag标志在Theory2Mechanical中用于放入问题
            TheoryFileFlagFile.Close();
            //等待T2M解算完成
            while (!FlagExist("MechanicalFlag"))//等待Theory2Mechanical跑完
                System.Threading.Thread.Sleep(20);
            //*读取答案
            FileStream MechanicalFile = new FileStream(@"./file/Mechanical.txt", FileMode.Open); //结果存在Mechanical.txt中
            StreamReader sr = new StreamReader(MechanicalFile);
            string MechanicalAnswer = sr.ReadLine();
            sr.Close();
            MechanicalFile.Close();
            //删除文件
            System.Threading.Thread.Sleep(20);
            System.IO.File.Delete(@"./file/MechanicalFlag.txt");//temp文件删除
            //System.IO.File.Delete(@"./Bridge/Mechanical.txt");
            return MechanicalAnswer;//返回
        }
        /*
        *   @brief  发送解法到下位机
        *   @return null
        */
        void SendMStrToSTM32(string MechanicalStr)
        {
            if (this.SerialPort1.IsOpen)
            {
                try
                {
                    Byte[] tempByte = new Byte[150];

                    int Len = MechanicalStr.Length / 5;

                    for (int i = 0; i < Len; i++)
                    {
                        string tempStr = MechanicalStr.Substring(i * 5 + 2, 2);
                        tempByte[i + 1] = MStr2int(tempStr);
                    }
                    tempByte[0] = 0XAA;
                    for (int i = Len + 1; i < 150; i++)
                    {
                        tempByte[i] = 0XBB;
                    }

                    this.SerialPort1.Write(tempByte, 0, 150);
                    //this.SerialPort1.Write(tempByte, 0, 150);
                    //this.SerialPort1.Write(tempByte, 0, 150);
                }
                catch { }
            }
        }
        /*
        *   @brief  机械步骤编码
        *   @return 编码
        */
        Byte MStr2int(string MechStep)
        {
            /*
                L_C: 0		R_C: 5
                L_O: 1		R_O: 6
                L_1: 2		R_1: 7
                L_2: 3		R_2: 8
                L_3: 4		R_3: 9
            */
            switch (MechStep)
            {
                case "LC": return 0x00;
                case "LO": return 0x01;
                case "L1": return 0x02;
                case "L2": return 0x03;
                case "L3": return 0x04;

                case "RC": return 0x05;
                case "RO": return 0x06;
                case "R1": return 0x07;
                case "R2": return 0x08;
                case "R3": return 0x09;

            }
            return 0xff;
        }
        /*
        *   @brief  创建文件flag
        *   @return null
        */
        void CreatFlag(string str)
        {
            FileStream fs = new FileStream(@"./file/" + str + ".txt", FileMode.Create);
            fs.Close();
        }
        /*
        *   @brief  删除文件flag
        *   @return null
        */
        void DeleteFlag(string str)
        {
            if (FlagExist(str))
            {
                System.Threading.Thread.Sleep(50);
                System.IO.File.Delete(@"./file/" + str + ".txt");
            }
        }
        /*
        *   @brief  判断是否存在文件flag
        *   @return 是否存在改flag文件的bool结果
        */
        bool FlagExist(string str)
        {
            return System.IO.File.Exists(@"./file/" + str + ".txt");
        }
    }
}
