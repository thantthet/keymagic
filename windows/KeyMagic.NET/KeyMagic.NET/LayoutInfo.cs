using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Drawing;
using System.IO;

namespace KeyMagic
{
    class LayoutInfo
    {
        public LayoutInfo(int _id, byte[] _data)
        {
            id = _id;
            data = _data;
        }

        public int id;
        public byte[] data;
    }

    class InfoCollection : List<LayoutInfo>
    {
        public InfoCollection(string fileName) : base()
        {
            KeyMagicDotNet.InfoList infoList = KeyMagicDotNet.KeyMagicKeyboard.getInfosFromKeyboardFile(fileName);
            //FileStream fs = new FileStream(fileName, FileMode.Open, FileAccess.Read);
            //BinaryReader br = new BinaryReader(fs);

            //try
            //{
            //    FileHeader fh = new FileHeader(br);

            //    for (int i = 0; i < fh.stringCount; i++)
            //    {
            //        int length = br.ReadInt16();
            //        br.ReadBytes(length * sizeof(short));
            //    }

            //    for (int i = 0; i < fh.infoCount; i++)
            //    {
            //        int id = br.ReadInt32();
            //        int length = br.ReadInt16();
            //        byte[] b = br.ReadBytes(length);

            //        LayoutInfo info = new LayoutInfo(id, b);
            //        Add(info);
            //    }
            //}
            //catch (Exception ex)
            //{
            //    throw ex;
            //}
            //finally
            //{
            //    br.Close();
            //    fs.Close();
            //}
        }

        public LayoutInfo FindById(string strId)
        {
            if (strId.Length != 4)
            {
                return null;
            }

            int intId = strId[3] | strId[2] << 8 | strId[1] << 16 | strId[0] << 24;
            
            List<LayoutInfo>.Enumerator e = GetEnumerator();
            while (e.MoveNext())
            {
                if (e.Current.id == intId)
                {
                    return e.Current;
                }
            }
            return null;
        }

        public Bitmap GetIcon()
        {
            LayoutInfo icon = FindById("icon");
            if (icon != null)
            {
                Bitmap bm;
                using (MemoryStream ms = new MemoryStream(icon.data))
                {
                    bm = new Bitmap(ms);
                }
                return bm;
            }
            return null;
        }

        public string GetName()
        {
            LayoutInfo name = FindById("name");
            if (name != null)
            {
                return Encoding.UTF8.GetString(name.data);
            }
            return null;
        }

        public Hotkey GetHotkey()
        {
            LayoutInfo hotkey = FindById("htky");
            if (hotkey != null)
            {
                return new Hotkey(Encoding.UTF8.GetString(hotkey.data));
            }
            return null;
        }

        public string GetDescription()
        {
            LayoutInfo name = FindById("desc");
            if (name != null)
            {
                return Encoding.UTF8.GetString(name.data);
            }
            return null;
        }

        public string GetFontFamily()
        {
            LayoutInfo font = FindById("font");
            if (font != null)
            {
                return Encoding.UTF8.GetString(font.data);
            }
            return null;
        }
    }
}
