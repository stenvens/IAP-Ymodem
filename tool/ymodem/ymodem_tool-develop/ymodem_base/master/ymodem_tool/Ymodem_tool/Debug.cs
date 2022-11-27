using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ymodem_tool
{
    class Debugging
    {
        //调试用
        public static void output(string log)
        {
            Console.WriteLine(GetTimeStampMs() + " - " + log);
        }

        //调试输出数组（16进制/byte形数组）
        public static void output_array(byte[] array, int length)
        {
            Console.Write("output_array({0}): ", length.ToString());
            for (int i = 0; i < length; i++)
            {
                Console.Write(Convert.ToString(array[i], 16).PadLeft(2, '0') + " ");
            }
            Console.WriteLine("");
        }

        //获取时间戳
        public static long GetTimeStamp()
        {
            TimeSpan ts = DateTime.UtcNow - new DateTime(1970, 1, 1, 0, 0, 0, 0);
            return Convert.ToInt64(ts.TotalSeconds);
        }

        //获取时间戳-毫秒
        public static long GetTimeStampMs()
        {
            TimeSpan ts = DateTime.UtcNow - new DateTime(1970, 1, 1, 0, 0, 0, 0);
            return Convert.ToInt64(ts.TotalMilliseconds);
        }
    }
}
