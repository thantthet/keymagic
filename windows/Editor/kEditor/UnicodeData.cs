using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Text.RegularExpressions;
using System.Windows.Forms;

namespace kEditor
{
    class UnicodeData
    {
        private string unicodeData = "";
        
        public UnicodeData(string fileName)
        {
            try
            {
                unicodeData = File.ReadAllText(Path.GetDirectoryName(Environment.GetCommandLineArgs()[0]) + @"\" + fileName);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        public KeyValuePair<String, int>[] getContains(string filter)
        {
            List<KeyValuePair<String, int>> list = new List<KeyValuePair<string, int>>();
            Regex regex;

            try
            {
                regex = new Regex("^([a-fA-F0-9]{4});([^;]*?" + filter + "[^;]*);", RegexOptions.Multiline | RegexOptions.IgnoreCase);
            } catch (ArgumentException ex)
            {
                Console.WriteLine(ex.Message);
                return null;
            }
            Match m = regex.Match(unicodeData);
            while (m.Success)
            {
                string name = m.Groups[2].Value;
                string code = m.Groups[1].Value;
                list.Add(new KeyValuePair<string,int>(name, int.Parse(code, System.Globalization.NumberStyles.HexNumber)));
                m = m.NextMatch();
            }

            return list.ToArray();
        }

        public string GetNameForChar(char c)
        {
            string code = ((int)c).ToString("X4");
            Regex regex = new Regex("^" + code + ";([^;]+);", RegexOptions.Multiline);
            Match m = regex.Match(unicodeData);
            if (m.Success)
            {
                return m.Groups[1].Value;
            }
            return "Not found in UnicodeData.txt";
        }
    }
}
