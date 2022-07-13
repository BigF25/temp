namespace USBCamera
{
    partial class Main_Form
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.videoSourcePlayer1 = new AForge.Controls.VideoSourcePlayer();
            this.videoSourcePlayer2 = new AForge.Controls.VideoSourcePlayer();
            this.videoSourcePlayer3 = new AForge.Controls.VideoSourcePlayer();
            this.videoSourcePlayer4 = new AForge.Controls.VideoSourcePlayer();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.pictureBox1 = new AForge.Controls.PictureBox();
            this.pictureBox2 = new AForge.Controls.PictureBox();
            this.pictureBox3 = new AForge.Controls.PictureBox();
            this.pictureBox4 = new AForge.Controls.PictureBox();
            this.Init_Camera = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.Capute_Image = new System.Windows.Forms.Button();
            this.Close_Camera = new System.Windows.Forms.Button();
            this.Open_Camera = new System.Windows.Forms.Button();
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.COM_Btn = new System.Windows.Forms.Button();
            this.Baud_ComboBox = new System.Windows.Forms.ComboBox();
            this.COM_ComboBox = new System.Windows.Forms.ComboBox();
            this.label19 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.label10 = new System.Windows.Forms.Label();
            this.Main_Start = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.MoveTimeCost = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.CalcTimeCost = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.ScanTimeCost = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.TotalTimeCost = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.cube500Process = new System.Diagnostics.Process();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // videoSourcePlayer1
            // 
            this.videoSourcePlayer1.Location = new System.Drawing.Point(18, 46);
            this.videoSourcePlayer1.Margin = new System.Windows.Forms.Padding(2);
            this.videoSourcePlayer1.Name = "videoSourcePlayer1";
            this.videoSourcePlayer1.Size = new System.Drawing.Size(240, 144);
            this.videoSourcePlayer1.TabIndex = 0;
            this.videoSourcePlayer1.Text = "videoSourcePlayer1";
            this.videoSourcePlayer1.VideoSource = null;
            // 
            // videoSourcePlayer2
            // 
            this.videoSourcePlayer2.Location = new System.Drawing.Point(271, 46);
            this.videoSourcePlayer2.Margin = new System.Windows.Forms.Padding(2);
            this.videoSourcePlayer2.Name = "videoSourcePlayer2";
            this.videoSourcePlayer2.Size = new System.Drawing.Size(240, 144);
            this.videoSourcePlayer2.TabIndex = 1;
            this.videoSourcePlayer2.Text = "videoSourcePlayer2";
            this.videoSourcePlayer2.VideoSource = null;
            // 
            // videoSourcePlayer3
            // 
            this.videoSourcePlayer3.Location = new System.Drawing.Point(18, 216);
            this.videoSourcePlayer3.Margin = new System.Windows.Forms.Padding(2);
            this.videoSourcePlayer3.Name = "videoSourcePlayer3";
            this.videoSourcePlayer3.Size = new System.Drawing.Size(240, 144);
            this.videoSourcePlayer3.TabIndex = 2;
            this.videoSourcePlayer3.Text = "videoSourcePlayer3";
            this.videoSourcePlayer3.VideoSource = null;
            // 
            // videoSourcePlayer4
            // 
            this.videoSourcePlayer4.Location = new System.Drawing.Point(271, 216);
            this.videoSourcePlayer4.Margin = new System.Windows.Forms.Padding(2);
            this.videoSourcePlayer4.Name = "videoSourcePlayer4";
            this.videoSourcePlayer4.Size = new System.Drawing.Size(240, 144);
            this.videoSourcePlayer4.TabIndex = 3;
            this.videoSourcePlayer4.Text = "videoSourcePlayer4";
            this.videoSourcePlayer4.VideoSource = null;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(83, 192);
            this.label1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(104, 22);
            this.label1.TabIndex = 4;
            this.label1.Text = "Camera Up";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(329, 192);
            this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(129, 22);
            this.label2.TabIndex = 5;
            this.label2.Text = "Camera Down";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(83, 362);
            this.label3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(98, 22);
            this.label3.TabIndex = 6;
            this.label3.Text = "Camera In";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(329, 362);
            this.label4.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(112, 22);
            this.label4.TabIndex = 7;
            this.label4.Text = "Camera Out";
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.pictureBox1.Image = null;
            this.pictureBox1.Location = new System.Drawing.Point(679, 195);
            this.pictureBox1.Margin = new System.Windows.Forms.Padding(2);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(120, 90);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 8;
            this.pictureBox1.TabStop = false;
            this.pictureBox1.Click += new System.EventHandler(this.pictureBox1_Click);
            // 
            // pictureBox2
            // 
            this.pictureBox2.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.pictureBox2.Image = null;
            this.pictureBox2.Location = new System.Drawing.Point(803, 195);
            this.pictureBox2.Margin = new System.Windows.Forms.Padding(2);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(120, 90);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox2.TabIndex = 9;
            this.pictureBox2.TabStop = false;
            // 
            // pictureBox3
            // 
            this.pictureBox3.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.pictureBox3.Image = null;
            this.pictureBox3.Location = new System.Drawing.Point(679, 289);
            this.pictureBox3.Margin = new System.Windows.Forms.Padding(2);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(120, 90);
            this.pictureBox3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox3.TabIndex = 10;
            this.pictureBox3.TabStop = false;
            // 
            // pictureBox4
            // 
            this.pictureBox4.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.pictureBox4.Image = null;
            this.pictureBox4.Location = new System.Drawing.Point(803, 289);
            this.pictureBox4.Margin = new System.Windows.Forms.Padding(2);
            this.pictureBox4.Name = "pictureBox4";
            this.pictureBox4.Size = new System.Drawing.Size(120, 90);
            this.pictureBox4.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox4.TabIndex = 11;
            this.pictureBox4.TabStop = false;
            // 
            // Init_Camera
            // 
            this.Init_Camera.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Init_Camera.Location = new System.Drawing.Point(520, 195);
            this.Init_Camera.Margin = new System.Windows.Forms.Padding(2);
            this.Init_Camera.Name = "Init_Camera";
            this.Init_Camera.Size = new System.Drawing.Size(155, 38);
            this.Init_Camera.TabIndex = 12;
            this.Init_Camera.Text = "Init Camera";
            this.Init_Camera.UseVisualStyleBackColor = true;
            this.Init_Camera.Click += new System.EventHandler(this.Init_Camera_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(516, 55);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(81, 22);
            this.label5.TabIndex = 13;
            this.label5.Text = "Number:";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.Capute_Image);
            this.groupBox1.Controls.Add(this.pictureBox4);
            this.groupBox1.Controls.Add(this.Close_Camera);
            this.groupBox1.Controls.Add(this.pictureBox2);
            this.groupBox1.Controls.Add(this.pictureBox3);
            this.groupBox1.Controls.Add(this.Open_Camera);
            this.groupBox1.Controls.Add(this.listBox1);
            this.groupBox1.Controls.Add(this.pictureBox1);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.Init_Camera);
            this.groupBox1.Controls.Add(this.videoSourcePlayer1);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.videoSourcePlayer2);
            this.groupBox1.Controls.Add(this.videoSourcePlayer4);
            this.groupBox1.Controls.Add(this.videoSourcePlayer3);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Font = new System.Drawing.Font("Times New Roman", 26.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox1.Location = new System.Drawing.Point(12, 41);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(949, 393);
            this.groupBox1.TabIndex = 14;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Camera-Related";
            // 
            // Capute_Image
            // 
            this.Capute_Image.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Capute_Image.Location = new System.Drawing.Point(520, 321);
            this.Capute_Image.Margin = new System.Windows.Forms.Padding(2);
            this.Capute_Image.Name = "Capute_Image";
            this.Capute_Image.Size = new System.Drawing.Size(155, 38);
            this.Capute_Image.TabIndex = 20;
            this.Capute_Image.Text = "Capture Image";
            this.Capute_Image.UseVisualStyleBackColor = true;
            this.Capute_Image.Click += new System.EventHandler(this.Capute_Image_Click);
            // 
            // Close_Camera
            // 
            this.Close_Camera.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Close_Camera.Location = new System.Drawing.Point(520, 279);
            this.Close_Camera.Margin = new System.Windows.Forms.Padding(2);
            this.Close_Camera.Name = "Close_Camera";
            this.Close_Camera.Size = new System.Drawing.Size(155, 38);
            this.Close_Camera.TabIndex = 19;
            this.Close_Camera.Text = "Close Cmera";
            this.Close_Camera.UseVisualStyleBackColor = true;
            this.Close_Camera.Click += new System.EventHandler(this.Close_Camera_Click);
            // 
            // Open_Camera
            // 
            this.Open_Camera.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Open_Camera.Location = new System.Drawing.Point(520, 237);
            this.Open_Camera.Margin = new System.Windows.Forms.Padding(2);
            this.Open_Camera.Name = "Open_Camera";
            this.Open_Camera.Size = new System.Drawing.Size(155, 38);
            this.Open_Camera.TabIndex = 18;
            this.Open_Camera.Text = "Open Cmera";
            this.Open_Camera.UseVisualStyleBackColor = true;
            this.Open_Camera.Click += new System.EventHandler(this.Open_Camera_Click);
            // 
            // listBox1
            // 
            this.listBox1.Font = new System.Drawing.Font("Times New Roman", 7.5F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.listBox1.FormattingEnabled = true;
            this.listBox1.ItemHeight = 12;
            this.listBox1.Location = new System.Drawing.Point(520, 102);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(403, 88);
            this.listBox1.TabIndex = 17;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(516, 77);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(87, 22);
            this.label7.TabIndex = 15;
            this.label7.Text = "Message:";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(601, 55);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(61, 22);
            this.label6.TabIndex = 14;
            this.label6.Text = "NULL";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.COM_Btn);
            this.groupBox2.Controls.Add(this.Baud_ComboBox);
            this.groupBox2.Controls.Add(this.COM_ComboBox);
            this.groupBox2.Controls.Add(this.label19);
            this.groupBox2.Controls.Add(this.label9);
            this.groupBox2.Font = new System.Drawing.Font("Times New Roman", 26.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox2.Location = new System.Drawing.Point(13, 440);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(602, 92);
            this.groupBox2.TabIndex = 15;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Com-Related";
            // 
            // COM_Btn
            // 
            this.COM_Btn.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.COM_Btn.Location = new System.Drawing.Point(452, 33);
            this.COM_Btn.Margin = new System.Windows.Forms.Padding(2);
            this.COM_Btn.Name = "COM_Btn";
            this.COM_Btn.Size = new System.Drawing.Size(124, 38);
            this.COM_Btn.TabIndex = 21;
            this.COM_Btn.Text = "Open COM";
            this.COM_Btn.UseVisualStyleBackColor = true;
            this.COM_Btn.Click += new System.EventHandler(this.COM_btn_Click);
            // 
            // Baud_ComboBox
            // 
            this.Baud_ComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.Baud_ComboBox.Font = new System.Drawing.Font("Times New Roman", 15.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Baud_ComboBox.FormattingEnabled = true;
            this.Baud_ComboBox.Items.AddRange(new object[] {
            "4800",
            "9600",
            "115200"});
            this.Baud_ComboBox.Location = new System.Drawing.Point(321, 36);
            this.Baud_ComboBox.Name = "Baud_ComboBox";
            this.Baud_ComboBox.Size = new System.Drawing.Size(110, 32);
            this.Baud_ComboBox.TabIndex = 24;
            // 
            // COM_ComboBox
            // 
            this.COM_ComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.COM_ComboBox.Font = new System.Drawing.Font("Times New Roman", 15.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.COM_ComboBox.FormattingEnabled = true;
            this.COM_ComboBox.Location = new System.Drawing.Point(86, 36);
            this.COM_ComboBox.Name = "COM_ComboBox";
            this.COM_ComboBox.Size = new System.Drawing.Size(110, 32);
            this.COM_ComboBox.TabIndex = 23;
            this.COM_ComboBox.DropDown += new System.EventHandler(this.comboBox1_DropDown);
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Font = new System.Drawing.Font("Times New Roman", 15.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label19.Location = new System.Drawing.Point(202, 44);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(113, 24);
            this.label19.TabIndex = 18;
            this.label19.Text = "Baud Rate:";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("Times New Roman", 15.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label9.Location = new System.Drawing.Point(13, 44);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(67, 24);
            this.label9.TabIndex = 2;
            this.label9.Text = "COM:";
            // 
            // serialPort1
            // 
            this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort1_DataReceived);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Font = new System.Drawing.Font("Times New Roman", 42F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label10.Location = new System.Drawing.Point(680, 9);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(283, 64);
            this.label10.TabIndex = 16;
            this.label10.Text = "CubeRobot";
            // 
            // Main_Start
            // 
            this.Main_Start.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Main_Start.Location = new System.Drawing.Point(620, 440);
            this.Main_Start.Margin = new System.Windows.Forms.Padding(2);
            this.Main_Start.Name = "Main_Start";
            this.Main_Start.Size = new System.Drawing.Size(100, 100);
            this.Main_Start.TabIndex = 25;
            this.Main_Start.Text = "START";
            this.Main_Start.UseVisualStyleBackColor = true;
            this.Main_Start.Click += new System.EventHandler(this.Main_Start_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.MoveTimeCost);
            this.groupBox3.Controls.Add(this.label14);
            this.groupBox3.Controls.Add(this.CalcTimeCost);
            this.groupBox3.Controls.Add(this.label11);
            this.groupBox3.Controls.Add(this.ScanTimeCost);
            this.groupBox3.Controls.Add(this.label15);
            this.groupBox3.Controls.Add(this.TotalTimeCost);
            this.groupBox3.Controls.Add(this.label17);
            this.groupBox3.Font = new System.Drawing.Font("Times New Roman", 26.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox3.Location = new System.Drawing.Point(13, 545);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(698, 122);
            this.groupBox3.TabIndex = 25;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Resolve-Related";
            // 
            // MoveTimeCost
            // 
            this.MoveTimeCost.AutoSize = true;
            this.MoveTimeCost.Font = new System.Drawing.Font("Times New Roman", 23.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.MoveTimeCost.Location = new System.Drawing.Point(545, 79);
            this.MoveTimeCost.Name = "MoveTimeCost";
            this.MoveTimeCost.Size = new System.Drawing.Size(96, 35);
            this.MoveTimeCost.TabIndex = 34;
            this.MoveTimeCost.Text = "0.00 S";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Font = new System.Drawing.Font("Times New Roman", 23.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label14.Location = new System.Drawing.Point(343, 79);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(69, 35);
            this.label14.TabIndex = 33;
            this.label14.Text = "Act:";
            // 
            // CalcTimeCost
            // 
            this.CalcTimeCost.AutoSize = true;
            this.CalcTimeCost.Font = new System.Drawing.Font("Times New Roman", 23.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CalcTimeCost.Location = new System.Drawing.Point(545, 44);
            this.CalcTimeCost.Name = "CalcTimeCost";
            this.CalcTimeCost.Size = new System.Drawing.Size(96, 35);
            this.CalcTimeCost.TabIndex = 31;
            this.CalcTimeCost.Text = "0.00 S";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Font = new System.Drawing.Font("Times New Roman", 23.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label11.Location = new System.Drawing.Point(343, 44);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(157, 35);
            this.label11.TabIndex = 30;
            this.label11.Text = "Calcaluate:";
            // 
            // ScanTimeCost
            // 
            this.ScanTimeCost.AutoSize = true;
            this.ScanTimeCost.Font = new System.Drawing.Font("Times New Roman", 23.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ScanTimeCost.Location = new System.Drawing.Point(208, 80);
            this.ScanTimeCost.Name = "ScanTimeCost";
            this.ScanTimeCost.Size = new System.Drawing.Size(96, 35);
            this.ScanTimeCost.TabIndex = 28;
            this.ScanTimeCost.Text = "0.00 S";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Font = new System.Drawing.Font("Times New Roman", 23.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label15.Location = new System.Drawing.Point(6, 79);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(84, 35);
            this.label15.TabIndex = 27;
            this.label15.Text = "Scan:";
            // 
            // TotalTimeCost
            // 
            this.TotalTimeCost.AutoSize = true;
            this.TotalTimeCost.Font = new System.Drawing.Font("Times New Roman", 23.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TotalTimeCost.Location = new System.Drawing.Point(208, 45);
            this.TotalTimeCost.Name = "TotalTimeCost";
            this.TotalTimeCost.Size = new System.Drawing.Size(96, 35);
            this.TotalTimeCost.TabIndex = 25;
            this.TotalTimeCost.Text = "0.00 S";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Font = new System.Drawing.Font("Times New Roman", 23.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label17.Location = new System.Drawing.Point(6, 44);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(88, 35);
            this.label17.TabIndex = 24;
            this.label17.Text = "Total:";
            // 
            // cube500Process
            // 
            this.cube500Process.StartInfo.Domain = "";
            this.cube500Process.StartInfo.FileName = ".\\cube500\\cube500sCN.exe";
            this.cube500Process.StartInfo.LoadUserProfile = false;
            this.cube500Process.StartInfo.Password = null;
            this.cube500Process.StartInfo.StandardErrorEncoding = null;
            this.cube500Process.StartInfo.StandardOutputEncoding = null;
            this.cube500Process.StartInfo.UserName = "";
            this.cube500Process.StartInfo.UseShellExecute = false;
            this.cube500Process.SynchronizingObject = this;
            // 
            // Main_Form
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Info;
            this.ClientSize = new System.Drawing.Size(973, 680);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.Main_Start);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Margin = new System.Windows.Forms.Padding(2);
            this.Name = "Main_Form";
            this.Text = "Main Form";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Main_Form_FormClosing);
            this.Load += new System.EventHandler(this.Main_Form_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private AForge.Controls.VideoSourcePlayer videoSourcePlayer1;
        private AForge.Controls.VideoSourcePlayer videoSourcePlayer2;
        private AForge.Controls.VideoSourcePlayer videoSourcePlayer3;
        private AForge.Controls.VideoSourcePlayer videoSourcePlayer4;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private AForge.Controls.PictureBox pictureBox1;
        private AForge.Controls.PictureBox pictureBox2;
        private AForge.Controls.PictureBox pictureBox3;
        private AForge.Controls.PictureBox pictureBox4;
        private System.Windows.Forms.Button Init_Camera;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.ListBox listBox1;
        private System.Windows.Forms.Button Close_Camera;
        private System.Windows.Forms.Button Open_Camera;
        private System.Windows.Forms.Button Capute_Image;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Label label9;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.ComboBox COM_ComboBox;
        private System.Windows.Forms.ComboBox Baud_ComboBox;
        private System.Windows.Forms.Button COM_Btn;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Button Main_Start;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label MoveTimeCost;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label CalcTimeCost;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label ScanTimeCost;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label TotalTimeCost;
        private System.Windows.Forms.Label label17;
        private System.Diagnostics.Process cube500Process;
    }
}

