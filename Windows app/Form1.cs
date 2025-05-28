using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace SlicerGame
{
    public partial class Form1: Form
    {
        private SerialPort serialPort;
        private string nickname;
        private List<(string Nickname, int Score)> scoreList = new List<(string, int)>();
        private readonly string scoreFilePath = "scores.txt";

        public Form1()
        {
            InitializeComponent();
        }

        private void ConnectButton_Click(object sender, EventArgs e)
        {
            // Get selected COM port
            string portName = ComPortComboBox.SelectedItem as string;

            // Get speed from TextBox
            string speedText = SpeedTextBox.Text.Trim();

            // Validate speed
            if (string.IsNullOrEmpty(speedText) || !int.TryParse(speedText, out int baudRate) || baudRate == 0)
            {
                MessageBox.Show
                (
                    "Please enter a valid, non-zero integer value for the speed.",
                    "Invalid Speed",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Warning
                );
                SpeedTextBox.Focus();
                return; // Do not continue
            }

            // Validate COM port selection
            if (string.IsNullOrEmpty(portName))
            {
                MessageBox.Show
                (
                    "Please select a COM port.",
                    "No Port Selected",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Warning
                );
                ComPortComboBox.Focus();
                return; // Do not continue
            }

            try
            {
                // If you have an existing open port, close it first
                if (serialPort != null && serialPort.IsOpen)
                    serialPort.Close();

                serialPort = new SerialPort(portName, baudRate);
                serialPort.Open();

                serialPort.DataReceived += SerialPort_DataReceived;

                MessageBox.Show("Connection successful!", "Connected", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show
                (
                    $"Could not open port: {ex.Message}",
                    "Connection Error",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error
                );
            }
        }

        private void SaveNicknameButton_Click(object sender, EventArgs e)
        {
            string input = NicknameTextBox.Text.Trim();

            // If input is empty, set to "unknown user"
            if (string.IsNullOrEmpty(input))
            {
                nickname = "unknown user";
            }
            else if (input.Length > 15)
            {
                MessageBox.Show
                (
                    "Nickname must be 15 characters or fewer.",
                    "Invalid Nickname",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Warning
                );
                NicknameTextBox.Focus();
                return;
            }
            else
            {
                nickname = input;
            }

            MessageBox.Show
            (
                $"Nickname saved: {nickname}",
                "Success",
                MessageBoxButtons.OK,
                MessageBoxIcon.Information
            );
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // Get available COM port names
            string[] portNames = SerialPort.GetPortNames();

            // Optionally clear existing items
            ComPortComboBox.Items.Clear();

            // Add port names to ComboBox
            ComPortComboBox.Items.AddRange(portNames);

            // Try to select COM6 if it's in the list
            int index = ComPortComboBox.Items.IndexOf("COM6");
            if (index != -1)
            {
                ComPortComboBox.SelectedIndex = index;
            }
            else if (portNames.Length > 0)
            {
                // If COM6 isn't found, select the first port by default
                ComPortComboBox.SelectedIndex = 0;
            }

            SpeedTextBox.Text = "115200";
            nickname = "unknown user";
            this.FormClosing += Form1_FormClosing;

            if (!File.Exists(scoreFilePath))
            {
                File.Create(scoreFilePath).Close(); // Create and immediately close the file
            }
            else
            {
                LoadScoreList(scoreFilePath);
                // After loading, update the list box
                UpdateScoreListBox();
            }
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                string data = serialPort.ReadLine().Trim();
                if (int.TryParse(data, out int score))
                {
                    // Add to the score list
                    scoreList.Add((nickname, score));

                    // Sort descending by score
                    var sorted = scoreList.OrderByDescending(item => item.Score).ToList();

                    // Update the list on the UI thread
                    this.Invoke((MethodInvoker)delegate
                    {
                        ScoreListBox.Items.Clear();
                        int ordinal = 1;
                        foreach (var entry in sorted)
                        {
                            ScoreListBox.Items.Add($"{ordinal}. {entry.Nickname} - {entry.Score}");
                            ordinal++;
                        }
                    });
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }
        }

        private void SaveScoreList(string filePath)
        {
            try
            {
                using (StreamWriter writer = new StreamWriter(filePath))
                {
                    foreach (var entry in scoreList)
                    {
                        writer.WriteLine($"{entry.Nickname};{entry.Score}");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }
        }

        private void LoadScoreList(string filePath)
        {
            scoreList.Clear();
            if (File.Exists(filePath))
            {
                try
                {
                    foreach (var line in File.ReadAllLines(filePath))
                    {
                        var parts = line.Split(';');
                        if (parts.Length == 2 && int.TryParse(parts[1], out int score))
                        {
                            scoreList.Add((parts[0], score));
                        }
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.ToString());
                }
            }
        }

        private void UpdateScoreListBox()
        {
            var sorted = scoreList.OrderByDescending(item => item.Score).ToList();
            ScoreListBox.Items.Clear();
            int ordinal = 1;
            foreach (var entry in sorted)
            {
                ScoreListBox.Items.Add($"{ordinal}. {entry.Nickname} - {entry.Score}");
                ordinal++;
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            // Close the serial port if open
            if (serialPort != null && serialPort.IsOpen)
            {
                try
                {
                    serialPort.Close();
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.ToString());
                }
            }

            // Save the score list
            SaveScoreList(scoreFilePath);
        }
    }
}
