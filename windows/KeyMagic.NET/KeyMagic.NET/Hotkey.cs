using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;

namespace KeyMagic
{
    public class Hotkey
    {
        private bool ctrl;

        public bool Ctrl
        {
            get { return ctrl; }
            set { ctrl = value; }
        }
        private bool alt;

        public bool Alt
        {
            get { return alt; }
            set { alt = value; }
        }
        private bool shift;

        public bool Shift
        {
            get { return shift; }
            set { shift = value; }
        }
        private string keyString;

        public string KeyString
        {
            get { return keyString; }
            set { keyString = value; }
        }
        private char keyChar;

        public char KeyChar
        {
            get { return keyChar; }
            set { keyChar = value; }
        }

        public uint WinModifiers
        {
            get
            {
                uint mod = 0;
                if (Alt)
                {
                    mod |= 0x0001;
                }
                if (Ctrl)
                {
                    mod |= 0x0002;
                }
                if (Shift)
                {
                    mod |= 0x004;
                }
                return mod;
            }
        }

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

            Regex functionKey = new Regex(@"\b(F[1-9][0-2]*)\s*$", RegexOptions.Compiled);
            Match mfunction = functionKey.Match(s);
            if (mfunction.Success)
            {
                keyString = mfunction.Groups[1].Value;
                keyChar = (char)(int.Parse(mfunction.Value.Substring(1)) + 0x6F);
            }

            Regex spaceKey = new Regex(@"\bSPACE\s*$", RegexOptions.Compiled);
            Match mSpace = spaceKey.Match(s);
            if (mSpace.Success)
            {
                keyString = "Space";
                keyChar = ' ';
            }
        }

        public override string ToString()
        {
            StringBuilder normalizeHK = new StringBuilder();
            if (ctrl)
            {
                normalizeHK.Append("Ctrl+");
            }

            if (shift)
            {
                normalizeHK.Append("Shift+");
            }

            if (alt)
            {
                normalizeHK.Append("Alt+");
            }

            normalizeHK.Append(KeyString);

            return normalizeHK.ToString().TrimEnd('+');
        }

        public int ToInt()
        {
            int hotkey = keyChar;

            if (ctrl)
            {
                hotkey |= 0x100;
            }

            if (alt)
            {
                hotkey |= 0x200;
            }

            if (shift)
            {
                hotkey |= 0x400;
            }

            return hotkey;
        }
    }
}
