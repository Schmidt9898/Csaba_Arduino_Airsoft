using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace usblog
{
    class Program
    {
        static void Main(string[] args)
        {
            StreamWriter file = new StreamWriter("arduinolog.txt", append: true);
            file.WriteLine("---------------------------------------------------\n");
            file.Flush();
            SerialPort usb = null;




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
                        Console.WriteLine(cleaned);
                        file.WriteLine(cleaned);
                        file.Flush();
                    });
                    break;
                }
                catch (System.IO.IOException e)
                { Console.WriteLine(e.Message); }

            }
            while (true)
                System.Threading.Thread.Sleep(1000);
            file.Close();
    }   }
}
