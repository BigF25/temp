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


namespace USBCamera
{
    public partial class Main_Form : Form
    {
        /*************************全局变量*******************************/
        FilterInfoCollection videoDevices;//摄像头设备集合
        VideoCaptureDevice videoSourceUp, videoSourceDown, videoSourceIn, videoSourceOut;
        Bitmap imageUp, imageDown, imageIn, imageOut;//处理图片
        bool capture_second_img = false;
        const int WAIT = 0;
        const int SCAN = 1;
        const int CALC = 2;
        const int MOVE = 3;
        const bool DEBUG = true;
        const bool RUN = false;
        int state = WAIT;
        int SysTime50ms = 0;
        /*************************线程*******************************/
        Thread TimeCount;//计时线程
        Thread SloveCube;//解算线程
        /*
        *   @brief 加载窗体的事件函数
        *   @return null
        */
        private void Main_Form_Load(object sender, EventArgs e)
        {
            Camera_Init();
            Baud_ComboBox.Text = "115200";
            /*在多线程程序中，新创建的线程不能访问UI线程创建的窗口控件，如果需要访问窗口中的控件，
             * 可以在窗口构造函数中将CheckForIllegalCrossThreadCalls设置为 false*/
            CheckForIllegalCrossThreadCalls = false;
        }
        /*
        *   @brief 拍摄图片的按钮事件函数（调试用）
        *   @return null
        */
        public Main_Form()
        {
            InitializeComponent();
        }
        /*
        *   @brief 初始化摄像头的按钮事件函数
        *   @return null
        */
        private void Init_Camera_Click(object sender, EventArgs e)
        {
            Camera_Init();
        }
        /*
        *   @brief 启动摄像头的按钮事件函数
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
        *   @brief 关闭摄像头的按钮事件函数
        *   @return null
        */
        private void Close_Camera_Click(object sender, EventArgs e)
        {
            Camera_Close();
        }
        /*
        *   @brief 捕捉摄像头图像（调试用）
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
        *   @brief 刷新串口的事件函数
        *   @return null
        */
        private void comboBox1_DropDown(object sender, EventArgs e)
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
        *   @brief 打开/关闭串口的按钮事件函数
        *   @return null
        */
        private void COM_btn_Click(object sender, EventArgs e)
        {
            if(COM_Btn.Text == "Open COM")
            {
                this.serialPort1.Close();
                if (this.COM_ComboBox.Text != "null")
                {
                    try
                    {
                        this.serialPort1.BaudRate = 115200;
                        this.serialPort1.PortName = this.COM_ComboBox.Text;
                        this.serialPort1.Open();
                        this.serialPort1.DiscardInBuffer();
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
                    this.serialPort1.Close();
                    COM_Btn.Text = "Open COM";
                }
                catch
                {
                    this.COM_ComboBox.Text = "clsoe error!";
                }
            }
        }
        /*
        *   @brief 串口收数据中断
        *   @return null
        */
        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            Byte[] ReceiveByte = new Byte[this.serialPort1.BytesToRead];
            this.serialPort1.Read(ReceiveByte, 0, ReceiveByte.Length);
            this.serialPort1.DiscardInBuffer();
            //MessageBox.Show(ReceiveByte[0].ToString());
            for (int i = 0; i < ReceiveByte.Length; i++)
            {
                //若收到数据OXCC，则SecondCapDown为true
                if (ReceiveByte[i] == 0xCC)
                {
                    capture_second_img = true;
                    break;
                }
                //若收到数据为0XDD，则解算完成
                else if (ReceiveByte[i] == 0xDD)
                {
                    state = WAIT;

                    //try
                    //{
                    //    TimeCount.Abort();
                    //}
                    //catch { }
                    //try
                    //{
                    //    SloveCube.Abort();
                    //}
                    //catch { }
                    break;
                }
            }
        }
        /*
        *   @brief 主启动按钮
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

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            Form2 Form2 = new Form2();
            Form2.Show();
        }

        /*
*   @brief 关闭窗体的事件函数
*   @return null
*/
        private void Main_Form_FormClosing(object sender, FormClosingEventArgs e)
        {
            Camera_Close();
        }
        /*
        *   @brief 摄像头初始化
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
        *   @brief 关闭摄像头
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
        *   @brief 计时线程函数
        *   @return null
        */
        private void TimeCountThread()//时间线程
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
        *   @brief 解魔方线程
        *   @return null
        */
        private void SolveCubeThread()
        {
            state = SCAN;//标记状态为扫描状态
            Get_Picture(DEBUG);
            state = CALC;//标记状态为扫描状态
        }
        /*
        *   @brief 解魔方线程
        *   @return null
        */
        private void Get_Picture(bool debug_flag = false)
        {
            if(!debug_flag)
            {
                while (!capture_second_img)
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
            }
            else//（调试用）
            {
                
            }
        }
    }
}
