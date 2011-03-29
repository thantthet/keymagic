using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;

namespace KeyMagic
{
    public class Hotkey
    {
        public bool ctrl;
        public bool alt;
        public bool shift;
        public string keyString;
        public char keyChar;

        public Hotkey()
        {
            ctrl = false;
            alt = false;
            shift = false;
            keyString = "";
            keyChar = '\0';
        }

        public Hotkey(bool _ctrl, bool _alt, bool _shift, string _keyString)
        {
            ctrl = _ctrl;
            alt = _alt;
            shift = _shift;
            ParseKey(_keyString);
        }

        public Hotkey(string hotkey)
        {
            hotkey = hotkey.ToUpper();

            if (hotkey.Contains("CTRL"))
            {
                ctrl = true;
            }

            if (hotkey.Contains("ALT"))
            {
                alt = true;
            }

            if (hotkey.Contains("SHIFT"))
            {
                shift = true;
            }

            ParseKey(hotkey);
        }

        private void ParseKey(string s)
        {
            s = s.ToUpper();
            Regex normalKey = new Regex(@"\b([A-Z0-9]|[`\-=\[\]\\;',./])\s*$", RegexOptions.Compiled);
            Match mNormal = normalKey.Match(s);
            if (mNormal.Success)
            {
                keyString = mNormal.Value;
                keyChar = mNormal.Value[0];
            }

            Regex functionKey = new Regex(@"\bF[1-9][1-2]*\s*$");
            Match mfunction = functionKey.Match(s);
            if (mfunction.Success)
            {
                keyString = mfunction.Value;
                keyChar = (char)int.Parse(mfunction.Value.Substring(1));
            }
        }

        public override string ToString()
        {
            StringBuilder normalizeHK = new StringBuilder();
            if (ctrl)
            {
                normalizeHK.Append("Ctrl+");
            }

            if (alt)
            {
                normalizeHK.Append("Alt+");
            }

            if (shift)
            {
                normalizeHK.Append("Shift+");
            }

            normalizeHK.Append(keyString);

            return normalizeHK.ToString();
        }

        public int ToInt()
        {
            int hotkey = keyChar;

            if (ctrl)
            {
                hotkey |= 4;
            }

            if (alt)
            {
                hotkey |= 8;
            }

            if (shift)
            {
                hotkey |= 16;
            }

            return hotkey;
        }
    }
}
