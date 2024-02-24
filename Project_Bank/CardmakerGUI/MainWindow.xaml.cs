using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO.Ports;
using System;

namespace CardmakerGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        SerialPort usb = null;



        private void write_to_card()
        {
            if (usb is not null)
            {
                String utf8string = in_name.Text;
                int balance = Int32.Parse(in_balance.Text);
                uint transaction = UInt32.Parse(in_count.Text);

                var balance_bytes = BitConverter.GetBytes(balance);
                var transaction_bytes = BitConverter.GetBytes(transaction);

                byte[] string_buff = System.Text.UTF8Encoding.UTF8.GetBytes(utf8string);
                byte[] bytes = new byte[32];
                Array.Copy(string_buff, bytes, Math.Min(string_buff.Length, 16));
                Array.Copy(balance_bytes, 0, bytes, 16, 4);
                Array.Copy(transaction_bytes, 0, bytes, 20, 4);
                try
                {
                    usb.Write(bytes, 0, 32);
                }catch(Exception ex)
                {
                    serial_output.Text = ex.Message;
                }

            }else
            {
                serial_output.Text = "Connect first";
            }
        }

        private void Connect()
        {
            for (int i = 0; i < 10; i++)
            {
                usb = new SerialPort("COM" + i, 9600);
                try
                {
                    usb.Open();
                    serial_output.Text = "";
                    COMLABEL.Content = "COM" + i;

                    usb.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(delegate (Object o, SerialDataReceivedEventArgs Args)
                    {
                        String line = usb.ReadLine();
                        string cleaned = line.Replace("\n", "").Replace("\r", "");
                        cleaned = DateTime.Now.ToString("MM-dd HH:mm:ss") + " " + cleaned;

                        this.Dispatcher.Invoke(() =>
                        {
                        serial_output.Text += line;
                        });

                        Console.WriteLine(cleaned);
                    });
                    break;
                }
                catch (System.IO.IOException e)
                { Console.WriteLine(e.Message); }

            }
        }



        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Connect();
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            write_to_card();
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            serial_output.Text = "";
        }
    }
}