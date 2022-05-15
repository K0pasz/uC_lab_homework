using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PedoMeterDesktop
{
    public partial class Form1 : Form
    {
        
        private delegate void ReceiveData(object sender, SerialDataReceivedEventArgs e);
        public Form1()
        {
            InitializeComponent();
        }

        private void CloseSerial()
        {

            try
            {
                serialPort1.DtrEnable = false;
                serialPort1.RtsEnable = false;
                serialPort1.DiscardInBuffer();
                serialPort1.DiscardOutBuffer();
                serialPort1.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);

            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            try
            {
                string[] ports = SerialPort.GetPortNames();
                comboBoxPort.Items.AddRange(ports);
                comboBoxPort.SelectedIndex = 0;
                btnClose.Enabled = false;
            }
            catch
            {
                MessageBox.Show("There are no available communication ports.\nPlease connect the pedometer to the PC via USB cable! :)", "Message", MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.Close();
            }

        }

        private void btnOpen_Click(object sender, EventArgs e)
        {
            btnOpen.Enabled = false;
            btnClose.Enabled = true;

            try
            {
                serialPort1.PortName = comboBoxPort.Text;
                serialPort1.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Message", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            btnOpen.Enabled = true;
            btnClose.Enabled = false;

            try
            {        
                serialPort1.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Message", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                serialPort1.Close();
            }
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if (InvokeRequired)
                Invoke(new ReceiveData(serialPort1_DataReceived), new object[] { sender, e });
            else if (!IsDisposed)
            {
                try
                {
                    if (serialPort1.IsOpen)
                    {
                        int steps = 1;
                        //Read buffer
                        byte[] buffer = new byte[9999];
                        int readByteCount = serialPort1.BaseStream.Read(buffer,0,serialPort1.BytesToRead);

                        
                        string response = Encoding.UTF8.GetString(buffer);

                        var lines = response.Split('\n');

                        foreach(string line in lines)
                        {
                            ListViewItem eachRow = new ListViewItem(steps.ToString());
                            ListViewItem.ListViewSubItem time = new ListViewItem.ListViewSubItem(eachRow, line);
                            eachRow.SubItems.Add(time);
                            listView1.Items.Add(eachRow);

                            steps++;
                        }
                        if(listView1.Items.Count > 0)
                            listView1.Items.RemoveAt(listView1.Items.Count - 1);

                        btnOpen.Enabled = false;
                        btnClose.Enabled = false;

                        System.Threading.Thread CloseDown = new System.Threading.Thread(new System.Threading.ThreadStart(CloseSerial));
                        CloseDown.Start();
                        return;
                        

                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "Message", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }
    }
}
