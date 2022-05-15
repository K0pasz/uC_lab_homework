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
        //Delegate for the Invoke() in the serialPort1_DataReceived method
        private delegate void ReceiveData(object sender, SerialDataReceivedEventArgs e);
        public Form1()
        {
            InitializeComponent();
        }

        //Method to close the serial port after reading the data
        //We will start a thread to do it to prevent a deadlock
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

        //Loading the serial ports into the combobox when the application is starting
        //Error message shows when Nucleo isn't connected
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

        //Opening the serial port
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

        //Closing the serial port
        private void btnClose_Click(object sender, EventArgs e)
        {
            btnOpen.Enabled = true;
            btnClose.Enabled = false;
            
            try
            {
                System.Threading.Thread CloseDown = new System.Threading.Thread(new System.Threading.ThreadStart(CloseSerial));
                CloseDown.Start();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Message", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        //Closing the serial port when exiting the app
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                serialPort1.Close();
            }
        }

        //Putting the timestamps into the listview when we send it from the Nucleo
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

                        //Getting the buffer's data into a string   
                        string response = Encoding.UTF8.GetString(buffer);

                        //Splitting the string into lines
                        var lines = response.Split('\n');

                        //Reading each line and putting the timestamps into the listview
                        foreach(string line in lines)
                        {
                            ListViewItem eachRow = new ListViewItem(steps.ToString());
                            ListViewItem.ListViewSubItem time = new ListViewItem.ListViewSubItem(eachRow, line);
                            eachRow.SubItems.Add(time);
                            listView1.Items.Add(eachRow);

                            steps++;
                        }
                        //Removing the last line because somehow the app adds one additional line
                        if(listView1.Items.Count > 0)
                            listView1.Items.RemoveAt(listView1.Items.Count - 1);

                        //Locking the app to prevent the duplicated data on the screen (in the listview)
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
