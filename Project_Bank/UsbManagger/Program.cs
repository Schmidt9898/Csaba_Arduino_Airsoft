using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace UsbManagger
{
    internal class Program
    {
        static void Main(string[] args)
        {
            SerialPort usb = null;

            bool can_continue = false;

            for (int i = 0; i < 10; i++)
            {
                if (args.Length > 0)
                {
                    Console.WriteLine(args[0]);
                    usb = new SerialPort(args[0], 9600);
                }
                else
                {
                    usb = new SerialPort("COM" + i, 9600);

                }
                try
                {
                    usb.Open();
                    usb.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(delegate (Object o, SerialDataReceivedEventArgs Args)
                    {
                        String line = usb.ReadLine();
                        string cleaned = line.Replace("\n", "").Replace("\r", "");
                        cleaned = DateTime.Now.ToString("MM-dd HH:mm:ss") + " " + cleaned;

                        if (cleaned.Contains("Waiting for profile"))
                        {
                            String utf8string = "Laci";
                            uint balance = 10;
                            uint transaction = 11; // transaction count we dont realy need this

                            var balance_bytes = BitConverter.GetBytes(balance);
                            var transaction_bytes = BitConverter.GetBytes(transaction);

                            byte[] string_buff = System.Text.UTF8Encoding.UTF8.GetBytes(utf8string);
                            byte[] bytes = new byte[32];
                            Array.Copy(string_buff, bytes, Math.Min(string_buff.Length, 16));
                            Array.Copy(balance_bytes, 0, bytes, 16, 4);
                            Array.Copy(transaction_bytes, 0, bytes, 20, 4);
                            usb.Write(bytes, 0, 32);
                            
                        }
                        Console.WriteLine(cleaned);
                    });
                    break;
                }
                catch (System.IO.IOException e)
                { Console.WriteLine(e.Message); }

            }
            while (true)
            {
                System.Threading.Thread.Sleep(2000);
            }




        }
    }
}
