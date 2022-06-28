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
            SerialPort usb = null;
            for (int i = 0; i < 10; i++)
            {
                usb = new SerialPort("COM" + i, 9600);
                try
                {
                    usb.Open();
                    usb.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(delegate (Object o, SerialDataReceivedEventArgs Args)
                    {
                        String line = usb.ReadLine();
                        string cleaned = line.Replace("\n", "").Replace("\r", "");
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
