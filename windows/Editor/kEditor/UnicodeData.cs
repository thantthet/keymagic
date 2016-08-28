using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using System.Reflection;
using System.Linq;
using System.Collections;

namespace kEditor
{
    class UnicodeData
    {
        private Dictionary<int, string> uniList = new Dictionary<int, string>();

        public UnicodeData()
        {
            uniList.Add(0x1000, "Ka Gyi");
        }
        
        public UnicodeData(string fileName)
        {
            try
            {
                String unicodeDataFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\" + fileName;
                StreamReader sr = new StreamReader(unicodeDataFile);
                
                char[] sep = new char[] { ';' };

                string line = sr.ReadLine();
                while (string.IsNullOrEmpty(line) == false)
                {
                    string[] ss = line.Split(sep);
                    uniList.Add(int.Parse(ss[0], System.Globalization.NumberStyles.HexNumber), ss[1]);
                    line = sr.ReadLine();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        public KeyValuePair<String, int>[] getContains(string filter = null)
        {
            if (filter == null)
            {
                filter = "";
            }

            filter = filter.ToUpper();
            
            var a = from r in uniList
                    where r.Value.Contains(filter)
                    select new KeyValuePair<string, int>(r.Value, r.Key);

            return a.ToArray();
        }

        public string GetNameForChar(int c)
        {
            var a = (from r in uniList
                     where r.Key == (int)c
                     select r.Value).FirstOrDefault();

            if (a != null)
            {
                return a;
            }

            return "Not found in UnicodeData.txt";
        }
    }
}
