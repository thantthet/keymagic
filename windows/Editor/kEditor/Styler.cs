using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using ScintillaNET;
using System.Xml;

namespace kEditor
{
    class Styler
    {
        public static Styler shared = new Styler();

        public event EventHandler StyleDidChange;

        public struct StyleCopy
        {
            public string Font;
            public Color ForeColor;
            public Color BackColor;
            public float Size;
            public bool Bold;
            public bool Italic;
            public bool Underline;
        }

        Dictionary<string, int> styleIndex = new Dictionary<string, int>();
        Dictionary<int, StyleCopy> styles = new Dictionary<int, StyleCopy>();

        StyleCopy defaultStyle
        {
            get
            {
                StyleCopy ss = new StyleCopy();
                ss.Font = "Courier New";
                ss.Size = 10;
                ss.ForeColor = Color.Black;
                ss.BackColor = Color.White;
                ss.Bold = false;
                ss.Italic = false;
                ss.Underline = false;
                return ss;
            }
        }
        private void InitStyleIndex()
        {
            styleIndex.Add("variable", KmsLexer.STYLE.Variable);
            //styleIndex.Add("Variable (declare)", 2);
            //styleIndex.Add("Operators", 3);
            styleIndex.Add("include keyword", KmsLexer.STYLE.Include);
            styleIndex.Add("hex notation", KmsLexer.STYLE.HexCode);
            styleIndex.Add("NULL/ANY value", KmsLexer.STYLE.Keyword);
            styleIndex.Add("modifier", KmsLexer.STYLE.KeyState);
            styleIndex.Add("reference", KmsLexer.STYLE.Reference);
            //styleIndex.Add("Virtual-key Start End Operator", 9);
            //styleIndex.Add("Out Operator", 10);
            styleIndex.Add("switch", KmsLexer.STYLE.Switch);
            styleIndex.Add("comment", KmsLexer.STYLE.Comment);
            //styleIndex.Add("line comment", KmsLexer.STYLE.Comment);
            //styleIndex.Add("Line Joiner", 14);
            styleIndex.Add("string", KmsLexer.STYLE.String);
            //styleIndex.Add("In-string Hex Notation", 16);
        }

        public Dictionary<string, int> GetStyleNameIndex()
        {
            return styleIndex;
        }

        public Dictionary<int, StyleCopy> GetStyles()
        {
            return styles;
        }

        public StyleCopy GetStyle(int index)
        {
            if (styles.ContainsKey(index))
            {
                return styles[index];
            }
            return defaultStyle;
        }

        public StyleCopy GetStyle(string name)
        {
            return GetStyle(styleIndex[name]);
        }

        public void SetStyle(int index, StyleCopy style)
        {
            styles[index] = style;

            StyleDidChange.Invoke(this, new EventArgs());
        }

        public void SaveStyles()
        {
            StringBuilder stringBuilder = new StringBuilder();
            XmlWriter xmlWriter = XmlWriter.Create(stringBuilder);
            XmlDocument xmlDoc = new XmlDocument();

            XmlElement stylesNode = xmlDoc.CreateElement("Styles");
            xmlDoc.AppendChild(stylesNode);

            foreach (int index in styleIndex.Values)
            {
                StyleCopy thisStyle = defaultStyle;
                if (styles.ContainsKey(index))
                {
                    thisStyle = styles[index];
                }
                XmlElement styleElement = xmlDoc.CreateElement("Style");

                XmlAttribute name = xmlDoc.CreateAttribute("index");
                name.Value = index.ToString();
                styleElement.Attributes.Append(name);

                XmlAttribute fontName = xmlDoc.CreateAttribute("fontName");
                fontName.Value = thisStyle.Font;
                styleElement.Attributes.Append(fontName);

                XmlAttribute fontSize = xmlDoc.CreateAttribute("fontSize");
                fontSize.Value = thisStyle.Size.ToString();
                styleElement.Attributes.Append(fontSize);

                XmlAttribute foreColor = xmlDoc.CreateAttribute("foreColor");
                foreColor.Value = thisStyle.ForeColor.ToArgb().ToString("X");
                styleElement.Attributes.Append(foreColor);

                XmlAttribute backColor = xmlDoc.CreateAttribute("backColor");
                backColor.Value = thisStyle.BackColor.ToArgb().ToString("X");
                styleElement.Attributes.Append(backColor);

                XmlAttribute bold = xmlDoc.CreateAttribute("bold");
                bold.Value = thisStyle.Bold.ToString();
                styleElement.Attributes.Append(bold);

                XmlAttribute italic = xmlDoc.CreateAttribute("italic");
                italic.Value = thisStyle.Italic.ToString();
                styleElement.Attributes.Append(italic);

                XmlAttribute underline = xmlDoc.CreateAttribute("underline");
                underline.Value = thisStyle.Underline.ToString();
                styleElement.Attributes.Append(italic);

                stylesNode.AppendChild(styleElement);
            }

            xmlDoc.WriteContentTo(xmlWriter);
            xmlWriter.Close();
            Properties.Settings.Default.StyleConfig = stringBuilder.ToString();
        }

        public Styler()
        {
            InitStyleIndex();
            LoadStyle();
        }

        private void LoadStyle()
        {
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.LoadXml(Properties.Settings.Default.StyleConfig);
            XmlNodeList nodeList = xmlDoc.SelectNodes("Styles/Style");
            foreach (XmlElement styleNode in nodeList)
            {
                StyleCopy thisStyle = defaultStyle;
                int styleIndex;

                string index = styleNode.GetAttribute("index");
                string fontName = styleNode.GetAttribute("fontName");
                string fontSize = styleNode.GetAttribute("fontSize");
                string foreColor = styleNode.GetAttribute("foreColor");
                string backColor = styleNode.GetAttribute("backColor");
                string bold = styleNode.GetAttribute("bold");
                string italic = styleNode.GetAttribute("italic");
                string underline = styleNode.GetAttribute("underline");

                if (index == "" || int.TryParse(index, out styleIndex) == false)
                {
                    continue;
                }

                if (fontName != "") thisStyle.Font = fontName;
                if (parseFloat(fontSize) > float.MinValue) thisStyle.Size = float.Parse(fontSize);
                if (parseHex(foreColor) > int.MinValue) thisStyle.ForeColor = Color.FromArgb(int.Parse(foreColor, System.Globalization.NumberStyles.HexNumber));
                if (parseHex(backColor) > int.MinValue) thisStyle.BackColor = Color.FromArgb(int.Parse(backColor, System.Globalization.NumberStyles.HexNumber));
                if (isBool(bold)) thisStyle.Bold = bool.Parse(bold);
                if (isBool(italic)) thisStyle.Italic = bool.Parse(italic);
                if (isBool(underline)) thisStyle.Underline = bool.Parse(underline);

                styles[styleIndex] = thisStyle;
            }
        }

        private int parseHex(string s)
        {
            int result;
            if (int.TryParse(s, System.Globalization.NumberStyles.HexNumber, null, out result))
            {
                return result;
            }
            return int.MinValue;
        }

        private bool isBool(string b)
        {
            bool result;
            if (bool.TryParse(b, out result)) {
                return true;
            }
            return false;
        }

        private int parseFloat(string s)
        {
            int result;
            if (int.TryParse(s, out result))
            {
                return result;
            }
            return int.MinValue;
        }

        public int GetStyleIndex(string p)
        {
            return styleIndex[p];
        }
    }
}