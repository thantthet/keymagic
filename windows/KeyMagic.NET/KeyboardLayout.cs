using System;
using System.Collections.Generic;
using System.Text;
using System.Xml.Serialization;

namespace KeyMagic
{
    class KeyboardLayout : IXmlSerializable
    {
        public string file;
        public string display;
        public string hotkey;
        public bool enable;

        public KeyboardLayout()
        {
        }

        public KeyboardLayout(string _file, string _display, string _hotkey, bool _enable)
        {
            file = _file;
            display = _display;
            hotkey = _hotkey;
            enable = _enable;
        }

        #region IXmlSerializable Members

        public System.Xml.Schema.XmlSchema GetSchema()
        {
            return null;
        }

        public void ReadXml(System.Xml.XmlReader reader)
        {
            reader.MoveToContent();
            enable = System.Xml.XmlConvert.ToBoolean(reader["Enabled"]);
            reader.ReadToDescendant("File");
            file = reader.ReadElementString();
            reader.ReadToDescendant("Display");
            display = reader.ReadElementString();
            reader.ReadToDescendant("Hotkey");
            hotkey = reader.ReadElementString();
        }

        public void WriteXml(System.Xml.XmlWriter writer)
        {
            writer.WriteAttributeString("Enabled", System.Xml.XmlConvert.ToString(enable));
            writer.WriteElementString("File", file);
            writer.WriteElementString("Display", display);
            writer.WriteElementString("Hotkey", hotkey);
        }

        #endregion
    }

    class KeyboardLayoutList : List<KeyboardLayout>, IXmlSerializable
    {

        #region IXmlSerializable Members

        public System.Xml.Schema.XmlSchema GetSchema()
        {
            return null;
        }

        public void ReadXml(System.Xml.XmlReader reader)
        {
            while (reader.ReadToNextSibling("Layout"))
            {
                KeyboardLayout layout = new KeyboardLayout();
                layout.ReadXml(reader);
                Add(layout);
                reader.Read();
            }
        }

        public void WriteXml(System.Xml.XmlWriter writer)
        {
            List<KeyboardLayout>.Enumerator e = GetEnumerator();
            while (e.MoveNext())
            {
                writer.WriteStartElement("Layout");
                e.Current.WriteXml(writer);
                writer.WriteEndElement();
            }
        }

        #endregion
    }
}
