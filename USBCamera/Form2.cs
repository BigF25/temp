using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;


namespace USBCamera
{
    public partial class Form2 : Form
    {
        Point start; //画框的起始点
        bool blnDraw = false;
        Point location;
        Size size;
        int count=0;
        public Form2()
        {
            InitializeComponent();
        }

        private void Form2_Load(object sender, EventArgs e)
        {
            FileStream fileStream1 = new FileStream("image/ChoseROI/00.jpg", FileMode.Open, FileAccess.Read);
            FileStream fileStream2 = new FileStream("image/ChoseROI/01.jpg", FileMode.Open, FileAccess.Read);
            FileStream fileStream3 = new FileStream("image/ChoseROI/02.jpg", FileMode.Open, FileAccess.Read);
            FileStream fileStream4 = new FileStream("image/ChoseROI/03.jpg", FileMode.Open, FileAccess.Read);
            pictureBox1.Image = Image.FromStream(fileStream1);
            pictureBox2.Image = Image.FromStream(fileStream2);
            pictureBox3.Image = Image.FromStream(fileStream3);
            pictureBox4.Image = Image.FromStream(fileStream4);
        }

        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            start = e.Location;
            Invalidate();
            blnDraw = true;
        }

        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            if (blnDraw)
            {
                if (e.Button != MouseButtons.Left)//判断是否按下左键
                    return;
                Point tempEndPoint = e.Location; //记录框的位置和大小
                location = new Point(
                    Math.Min(start.X, tempEndPoint.X),
                    Math.Min(start.Y, tempEndPoint.Y));
                size = new Size(
                    Math.Abs(start.X - tempEndPoint.X),
                    Math.Abs(start.Y - tempEndPoint.Y));
                pictureBox1.Invalidate();
            }
        }

        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            blnDraw = false; //结束绘制
            richTextBox1.AppendText("rect0[" + count + "] = Rect(" + location.X + ", " + location.Y + ", " + size.Width + ", " + size.Height + ");\n");
            count++;
        }

        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {
            if (blnDraw)
            {
                if (pictureBox1.Image != null)
                {
                    if (location != null && size.Width > 0 && size.Height > 0)
                    {
                        e.Graphics.DrawRectangle(new Pen(Color.Red, 3), location.X, location.Y, size.Width, size.Height);//重新绘制颜色为红色
                    }
                }
            }
        }

        private void pictureBox2_MouseDown(object sender, MouseEventArgs e)
        {
            start = e.Location;
            Invalidate();
            blnDraw = true;
        }

        private void pictureBox2_MouseMove(object sender, MouseEventArgs e)
        {
            if (blnDraw)
            {
                if (e.Button != MouseButtons.Left)//判断是否按下左键
                    return;
                Point tempEndPoint = e.Location; //记录框的位置和大小
                location = new Point(
                    Math.Min(start.X, tempEndPoint.X),
                    Math.Min(start.Y, tempEndPoint.Y));
                size = new Size(
                    Math.Abs(start.X - tempEndPoint.X),
                    Math.Abs(start.Y - tempEndPoint.Y));
                pictureBox2.Invalidate();
            }
        }

        private void pictureBox2_MouseUp(object sender, MouseEventArgs e)
        {
            blnDraw = false; //结束绘制
            richTextBox1.AppendText("rect1[" + count + "] = Rect(" + location.X + ", " + location.Y + ", " + size.Width + ", " + size.Height + ");\n");
            count++;
        }

        private void pictureBox2_Paint(object sender, PaintEventArgs e)
        {
            if (blnDraw)
            {
                if (pictureBox2.Image != null)
                {
                    if (location != null && size.Width > 0 && size.Height > 0)
                    {
                        e.Graphics.DrawRectangle(new Pen(Color.Red, 3), location.X, location.Y, size.Width, size.Height);//重新绘制颜色为红色
                    }
                }
            }
        }

        private void pictureBox3_MouseDown(object sender, MouseEventArgs e)
        {
            start = e.Location;
            Invalidate();
            blnDraw = true;
        }

        private void pictureBox3_MouseMove(object sender, MouseEventArgs e)
        {
            if (blnDraw)
            {
                if (e.Button != MouseButtons.Left)//判断是否按下左键
                    return;
                Point tempEndPoint = e.Location; //记录框的位置和大小
                location = new Point(
                    Math.Min(start.X, tempEndPoint.X),
                    Math.Min(start.Y, tempEndPoint.Y));
                size = new Size(
                    Math.Abs(start.X - tempEndPoint.X),
                    Math.Abs(start.Y - tempEndPoint.Y));
                pictureBox3.Invalidate();
            }
        }

        private void pictureBox3_MouseUp(object sender, MouseEventArgs e)
        {
            blnDraw = false; //结束绘制
            richTextBox1.AppendText("rect2[" + count + "] = Rect(" + location.X + ", " + location.Y + ", " + size.Width + ", " + size.Height + ");\n");
            count++;
        }

        private void pictureBox3_Paint(object sender, PaintEventArgs e)
        {
            if (blnDraw)
            {
                if (pictureBox3.Image != null)
                {
                    if (location != null && size.Width > 0 && size.Height > 0)
                    {
                        e.Graphics.DrawRectangle(new Pen(Color.Red, 3), location.X, location.Y, size.Width, size.Height);//重新绘制颜色为红色
                    }
                }
            }
        }

        private void pictureBox4_MouseDown(object sender, MouseEventArgs e)
        {
            start = e.Location;
            Invalidate();
            blnDraw = true;
        }

        private void pictureBox4_MouseMove(object sender, MouseEventArgs e)
        {
            if (blnDraw)
            {
                if (e.Button != MouseButtons.Left)//判断是否按下左键
                    return;
                Point tempEndPoint = e.Location; //记录框的位置和大小
                location = new Point(
                    Math.Min(start.X, tempEndPoint.X),
                    Math.Min(start.Y, tempEndPoint.Y));
                size = new Size(
                    Math.Abs(start.X - tempEndPoint.X),
                    Math.Abs(start.Y - tempEndPoint.Y));
                pictureBox4.Invalidate();
            }
        }

        private void pictureBox4_MouseUp(object sender, MouseEventArgs e)
        {
            blnDraw = false; //结束绘制
            richTextBox1.AppendText("rect3[" + count + "] = Rect(" + location.X + ", " + location.Y + ", " + size.Width + ", " + size.Height + ");\n");
            count++;
        }

        private void pictureBox4_Paint(object sender, PaintEventArgs e)
        {
            if (blnDraw)
            {
                if (pictureBox4.Image != null)
                {
                    if (location != null && size.Width > 0 && size.Height > 0)
                    {
                        e.Graphics.DrawRectangle(new Pen(Color.Red, 3), location.X, location.Y, size.Width, size.Height);//重新绘制颜色为红色
                    }
                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            count = 0;
            richTextBox1.Clear();
        }
    }
}

