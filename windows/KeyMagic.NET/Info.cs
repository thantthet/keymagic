using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Drawing;
using System.IO;

namespace KeyMagic
{
    class Info
    {
        public Info(int _id, byte[] _data)
        {
            id = _id;
            data = _data;
        }

        public int id;
        public byte[] data;
    }

    class InfoCollection : List<Info>
    {
        public InfoCollection(string fileName) : base()
        {
            FileStream fs = new FileStream(fileName, FileMode.Open, FileAccess.Read);
            BinaryReader br = new BinaryReader(fs);
            FileHeader fh = new FileHeader(br);

            for (int i = 0; i < fh.stringCount; i++)
            {
                int length = br.ReadInt16();
                br.ReadBytes(length * sizeof(short));
            }

            for (int i = 0; i < fh.infoCount; i++)
            {
                int id = br.ReadInt32();
                int length = br.ReadInt16();
                byte[] b = br.ReadBytes(length);

                Info info = new Info(id, b);
                Add(info);
            }

            br.Close();
            fs.Close();
        }

        public Info FindById(string strId)
        {
            if (strId.Length != 4)
            {
                return null;
            }

            int intId = strId[3] | strId[2] << 8 | strId[1] << 16 | strId[0] << 24;
            
            List<Info>.Enumerator e = GetEnumerator();
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
            Info icon = FindById("icon");
            if (icon != null)
            {
                MemoryStream ms = new MemoryStream(icon.data);
                Bitmap bm = new Bitmap(ms);
                ms.Dispose();
                return bm;
            }
            return null;
        }

        public string GetName()
        {
            Info name = FindById("name");
            if (name != null)
            {
                return Encoding.UTF8.GetString(name.data);
            }
            return null;
        }

        public Hotkey GetHotkey()
        {
            Info hotkey = FindById("htky");
            if (hotkey != null)
            {
                return new Hotkey(Encoding.UTF8.GetString(hotkey.data));
            }
            return null;
        }

        public string GetDescription()
        {
            Info name = FindById("desc");
            if (name != null)
            {
                return Encoding.UTF8.GetString(name.data);
            }
            return null;
        }
    }
}
