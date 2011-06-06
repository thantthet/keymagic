using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.Drawing;
using System.Diagnostics;
using ScintillaNet;
using System.Runtime.InteropServices;
using ScintillaNet.Configuration;
using System.Xml;

namespace kEditor
{
    class Styler
    {
        //struct Range
        //{
        //    public int start;
        //    public int end;

        //    public Range(int s, int l)
        //    {
        //        start = s;
        //        end = l;
        //    }
        //}

        //byte[] trailingBytesForUTF8 = new byte[] {
        //        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        //        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        //        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        //        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        //        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        //        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        //        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        //        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
        //    };

        //struct Syntax
        //{
        //    public int styleIndex;
        //    public Regex matcher;
        //    public int colorGroup;

        //    public Syntax(Regex Matcher, int StyleIndex, int ColorGroup)
        //    {
        //        matcher = Matcher;
        //        styleIndex = StyleIndex;
        //        colorGroup = ColorGroup;
        //    }
        //}

        //Dictionary<Regex, int> Syntaxs = new Dictionary<Regex, int>();
        //List<Syntax> SyntaxList = new List<Syntax>();
        List<string> variables = new List<string>();

        ScintillaNet.Scintilla STLControl;

        Dictionary<string, int> styleIndex = new Dictionary<string, int>();

        private void InitStyleIndex()
        {
            styleIndex.Add("variable", 1);
            //styleIndex.Add("Variable (declare)", 2);
            //styleIndex.Add("Operators", 3);
            styleIndex.Add("include keyword", 4);
            styleIndex.Add("hex notation", 5);
            styleIndex.Add("NULL/ANY value", 6);
            styleIndex.Add("modifier", 7);
            styleIndex.Add("reference", 8);
            //styleIndex.Add("Virtual-key Start End Operator", 9);
            //styleIndex.Add("Out Operator", 10);
            styleIndex.Add("switch", 11);
            styleIndex.Add("block comment", 12);
            styleIndex.Add("line comment", 13);
            //styleIndex.Add("Line Joiner", 14);
            styleIndex.Add("string", 15);
            //styleIndex.Add("In-string Hex Notation", 16);
        }

        public Dictionary<string, int> GetStyleNameIndex()
        {
            return styleIndex;
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
                Style thisStyle = STLControl.Styles[index];
                XmlElement styleElement = xmlDoc.CreateElement("Style");

                XmlAttribute name = xmlDoc.CreateAttribute("index");
                name.Value = index.ToString();
                styleElement.Attributes.Append(name);

                XmlAttribute fontName = xmlDoc.CreateAttribute("fontName");
                fontName.Value = thisStyle.FontName;
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

        public Styler(ScintillaNet.Scintilla ScintillaCtrl)
        {
            STLControl = ScintillaCtrl;

            InitStyleIndex();
            LoadStyle(STLControl);

            //ApplyStyle(@"(\$\w+)", styleIndex["Variable"], 1);
            //ApplyStyle(@"(\$\w+?)\s*=[^>]", styleIndex["Variable (declare)"], 1);
            //ApplyStyle(@"([=+&])", styleIndex["Operators"], 1);
            //ApplyStyle(@"(VK_\w+)", styleIndex["Virtual-key"], 1);
            //ApplyStyle(@"[Uu][0-9A-Fa-f]{4}", styleIndex["Hex Notation"], 0);
            //ApplyStyle(@"(?:NULL)|(?:null)", styleIndex["NULL Value"], 0);
            //ApplyStyle(@"(\[[*^]\])", styleIndex["Any Operator"], 1);
            //ApplyStyle(@"(\[\$\w+\])", styleIndex["Reference"], 1);
            //ApplyStyle(@"([><])", styleIndex["Virtual-key Start End Operator"], 1);
            //ApplyStyle(@"(=>)", styleIndex["Out Operator"], 1);
            //ApplyStyle(@"(\(\s*(?:['""])[^\1\n]+?\1\s*\))\B", styleIndex["Switch"], 1);
            //ApplyStyle(@"(\\)\s+?$", styleIndex["Line Joiner"], 1);
            //ApplyStyle(@"((['""])[^\1\n]+?\2\B)", styleIndex["String"], 1);
            //ApplyStyle(@"(['""])[^\1\n]*(\\[Uu][0-9a-fA-F]{4})[^\1\n]*?\1\B", styleIndex["In-string Hex Notation"], 2);
        }

        private void LoadStyle(Scintilla control)
        {
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.LoadXml(Properties.Settings.Default.StyleConfig);
            XmlNodeList nodeList = xmlDoc.SelectNodes("Styles/Style");
            foreach (XmlElement styleNode in nodeList)
            {
                Style thisStyle;
                int styleIndex;

                string index = styleNode.GetAttribute("index");
                string fontName = styleNode.GetAttribute("fontName");
                string fontSize = styleNode.GetAttribute("fontSize");
                string foreColor = styleNode.GetAttribute("foreColor");
                string backColor = styleNode.GetAttribute("backColor");
                string bold = styleNode.GetAttribute("bold");
                string italic = styleNode.GetAttribute("italic");
                string underline = styleNode.GetAttribute("underline");

                if (index == "") continue;
                else
                {
                    if (int.TryParse(index, out styleIndex) == false) continue;
                    thisStyle = control.Styles[styleIndex];
                }

                if (fontName != "") thisStyle.FontName = fontName;
                if (parseFloat(fontSize) > float.MinValue) thisStyle.Size = float.Parse(fontSize);
                if (parseHex(foreColor) > int.MinValue) thisStyle.ForeColor = Color.FromArgb(int.Parse(foreColor, System.Globalization.NumberStyles.HexNumber));
                if (parseHex(backColor) > int.MinValue) thisStyle.BackColor = Color.FromArgb(int.Parse(backColor, System.Globalization.NumberStyles.HexNumber));
                if (isBool(bold)) thisStyle.Bold = bool.Parse(bold);
                if (isBool(italic)) thisStyle.Italic = bool.Parse(italic);
                if (isBool(underline)) thisStyle.Underline = bool.Parse(underline);
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

        //private void ApplyStyle(string Pattern, int index, int groupNumToColor)
        //{
        //    Regex rx = new Regex(Pattern, RegexOptions.Multiline | RegexOptions.Singleline);
        //    //Syntaxs.Add(rx, index);
        //    Syntax syntax = new Syntax(rx, index, groupNumToColor);
        //    SyntaxList.Add(syntax);
        //}

        //private Range[] UTF8Ranges(byte[] UCS, int length)
        //{

        //    Range[] ranges = new Range[length];

        //    int rIndx = 0;
        //    for (int i = 0; rIndx < length; i++)
        //    {
        //        byte b = UCS[i];
        //        ushort extraBytesToRead = trailingBytesForUTF8[b];
        //        if (i + extraBytesToRead >= UCS.Length)
        //        {
        //            return ranges;
        //        }

        //        ranges[rIndx].start = i;
        //        ranges[rIndx].end = i + extraBytesToRead + 1;
        //        rIndx++;

        //        i += extraBytesToRead;
        //    }

        //    return ranges;
        //}

        //enum States {
        //    Normal = 0,
        //    StreamComment = 12,
        //    LineComment = 13,
        //    Variable = 1,
        //    Operator = 3,
        //    VirtualKey = 4,
        //    HexNote = 5,
        //}

        //public void HiliteSyntax(byte[] rawText, int actualLength, int rawTextPos, string Text)
        //{
        //    Debug.WriteLine(Text);
        //    STLControl.NativeInterface.StartStyling(rawTextPos, 0xff);

        //    List<Range> comments = new List<Range>();
        //    Range[] ranges = UTF8Ranges(rawText, actualLength);

        //    States lastState = States.Normal;
        //    int lastStateChangedPos = 0;

        //    byte[] wholeRawText = STLControl.RawText;

        //    if (rawTextPos >= 2 && STLControl.NativeInterface.GetStyleAt(ranges[0].start + rawTextPos - 1) == styleIndex["Block Comment"])
        //    {
        //        lastState = States.StreamComment;
        //        for (int i = actualLength >=2 ? 2 : 0 + rawTextPos; i > rawTextPos; i--)
        //        {
        //            if (char.IsWhiteSpace((char)wholeRawText[i])) continue;
        //            else if (i >= 2 && wholeRawText[i - 1] == '*' && wholeRawText[i] == '/') lastState = States.Normal;
        //        }
        //        if (lastState == States.StreamComment)
        //        {
        //            comments.Add(new Range(0, 0));
        //        }
        //    }
            
        //    for (int i = 0; i < Text.Length; i++)
        //    {
        //        char ch = Text[i];
        //        char chNext = (i + 1) < Text.Length ? Text[i + 1] : '\0';

        //        if (lastState == States.StreamComment)
        //        {
        //            if (ch == '*' && chNext == '/')
        //            {
        //                int last = comments.Count - 1;
        //                ++i;
        //                STLControl.NativeInterface.SetStyling(ranges[i].end - ranges[comments[last].start].start, (int)lastState);
        //                comments[last] = new Range(comments[last].start, i);
        //                lastState = States.Normal;
        //            }
        //        }
        //        else if (lastState == States.LineComment)
        //        {
        //            if (ch == '\n')
        //            {
        //                int last = comments.Count - 1;

        //                STLControl.NativeInterface.SetStyling(ranges[i].end - ranges[comments[last].start].start, (int)lastState);
        //                comments[last] = new Range(comments[last].start, i);
        //                lastState = States.Normal;
        //            }
        //        }
        //        //else if (lastState == States.Variable)
        //        //{
        //        //    if (char.IsWhiteSpace(ch))
        //        //    {
        //        //        STLControl.NativeInterface.SetStyling(ranges[i].end - ranges[lastStateChangedPos].start, (int)lastState);
        //        //        lastStateChangedPos = i;
        //        //        lastState = States.Normal;
        //        //    }
        //        //}
        //        //else if (lastState == States.VirtualKey)
        //        //{
        //        //    if (char.IsWhiteSpace(ch))
        //        //    {
        //        //        STLControl.NativeInterface.SetStyling(ranges[i].end - ranges[lastStateChangedPos].start, (int)lastState);
        //        //        lastStateChangedPos = i;
        //        //        lastState = States.Normal;
        //        //    }
        //        //}
        //        // NORMAL STATE
        //        else if (ch == '/' && chNext == '*')
        //        {
        //            lastState = States.StreamComment;
        //            comments.Add(new Range(i, 0));
        //            ++i;
        //        }
        //        else if (ch == '/' && chNext == '/')
        //        {
        //            lastState = States.LineComment;
        //            comments.Add(new Range(i, 0));
        //            ++i;
        //        }
        //        //else if (ch == '$')
        //        //{
        //        //    lastState = States.Variable;
        //        //    lastStateChangedPos = i;
        //        //}
        //        //else if (ch == '+' || ch == '=')
        //        //{
        //        //    STLControl.NativeInterface.SetStyling(ranges[i].end - ranges[i].start, (int)States.Operator);
        //        //    lastState = States.Normal;
        //        //    lastStateChangedPos = i;
        //        //}
        //        //else if (ch == 'V' && chNext == 'K')
        //        //{
        //        //    lastState = States.VirtualKey;
        //        //    lastStateChangedPos = i;
        //        //}
        //        //else if (ch == 'U' || ch == 'u')
        //        //{
        //        //    if (i + 4 < Text.Length)
        //        //    {
        //        //        if (isHex(chNext) && isHex(Text[i + 2]) && isHex(Text[i + 3]) && isHex(Text[i + 4]))
        //        //        {
        //        //            STLControl.NativeInterface.SetStyling(ranges[i + 4].end - ranges[i].start, (int)States.HexNote);
        //        //            i += 4;
        //        //            lastStateChangedPos = i;
        //        //            lastState = States.Normal;
        //        //        }
        //        //    }
        //        //}
        //        else
        //        {
        //            STLControl.NativeInterface.SetStyling(ranges[i].end - ranges[i].start, 0);
        //            lastStateChangedPos = i;
        //        }
        //    }

        //    if (lastState == States.StreamComment || lastState == States.LineComment)
        //    {
        //        int last = comments.Count - 1;
        //        comments[last] = new Range(comments[last].start, Text.Length);
        //        STLControl.NativeInterface.SetStyling(ranges[ranges.Length - 1].end + ranges[comments[last].start].start, (int)lastState);
        //    }
        //    Debug.Write(Environment.TickCount + ">>");
        //    if (Text.Length > 25)
        //    {
        //        Debug.WriteLine(Text.Substring(0, 15) + "..." + Text.Substring(Text.Length - 10));
        //    }

        //    //short[] stylebits = new short[actualLength];

        //    foreach (Syntax syntax in SyntaxList)
        //    {
        //        ScintillaNet.Style Style = STLControl.Styles[syntax.styleIndex];
        //        Match m = syntax.matcher.Match(Text);

        //        while (m.Success)
        //        {
        //            if (syntax.styleIndex == 2)
        //            {
        //                if (variables.Contains(m.Groups[1].Value) == false)
        //                {
        //                    variables.Add(m.Groups[1].Value);
        //                }
        //            }

        //            //Check if in comment block
        //            bool inComment = false;
        //            foreach (Range cRange in comments)
        //            {
        //                if (m.Index >= cRange.start && m.Index <= cRange.end)
        //                {
        //                    inComment = true;
        //                    break;
        //                }
        //            }
        //            Group g = m.Groups[syntax.colorGroup];
        //            if (inComment == false)
        //            {
        //                //FillByte(ref stylebits, g.Index, g.Length, (byte)Style.Index);
        //                Style.Apply(ranges[g.Index].start + rawTextPos,
        //                        ranges[g.Index + g.Length - 1].end - ranges[g.Index].start);
        //            }
        //            m = m.NextMatch();
        //        }
        //    }
        //    //STLControl.NativeInterface.StartStyling(rawTextPos, 0xff);
        //    //short lastbit = 0;
        //    //int lastpos = -1;
        //    //int pos = 0;
        //    //foreach (short b in stylebits)
        //    //{
        //    //    if (lastpos == -1)
        //    //    {
        //    //        lastbit = 0;
        //    //        lastpos = pos;
        //    //    }
        //    //    else if (lastbit != b)
        //    //    {
        //    //        STLControl.NativeInterface.SetStyling(ranges[pos - 1].end - ranges[lastpos].start, lastbit);
        //    //        lastbit = b;
        //    //        lastpos = pos;
        //    //    }
        //    //    pos++;
        //    //}
        //}

        //private bool isHex(char ch)
        //{
        //    return (ch >= 'a' && ch <= 'f' || ch >= 'A' && ch <= 'F' || char.IsDigit(ch));
        //}

        //private void FillByte(ref short[] stylebits, int start, int length, short b)
        //{
        //    for (int i = start; i < start + length; i++)
        //    {
        //        stylebits[i] = b;
        //    }
        //}

        //internal void HiliteSyntax(TextModifiedEventArgs e)
        //{
        //    Line startLine, endLine;
        //    startLine = endLine = STLControl.Lines.FromPosition(e.Position);
        //    for (int i = 0; i < e.LinesAddedCount; i++)
        //    {
        //        endLine = endLine.Next;
        //    }

        //    TextRange textRange = new TextRange();
        //    textRange.chrg.cpMin = startLine.StartPosition;
        //    textRange.chrg.cpMax = endLine.EndPosition;
        //    textRange.lpstrText = Marshal.AllocCoTaskMem(endLine.EndPosition - startLine.StartPosition + 2);

        //    STLControl.NativeInterface.GetTextRange(ref textRange);
        //    string rangeText;

        //    byte[] managedBytes = new byte[endLine.EndPosition - startLine.StartPosition + 2];
        //    Marshal.Copy(textRange.lpstrText, managedBytes, 0, endLine.EndPosition - startLine.StartPosition + 2);

        //    rangeText = UnicodeEncoding.UTF8.GetString(managedBytes);

        //    HiliteSyntax(managedBytes, rangeText.Length, startLine.StartPosition, rangeText);
        //}
        //private byte[] lastText;

        //internal void HiliteSyntax(StyleNeededEventArgs e)
        //{
        //    if (e.Range.StyledText.Equals(lastText))
        //    {
        //        return;
        //    }
        //    lastText = e.Range.StyledText;

        //    TextRange textRange = new TextRange();
        //    textRange.chrg.cpMin = e.Range.StartingLine.StartPosition;
        //    textRange.chrg.cpMax = e.Range.EndingLine.EndPosition;
        //    textRange.lpstrText = Marshal.AllocCoTaskMem(e.Range.EndingLine.EndPosition - e.Range.StartingLine.StartPosition + 2);

        //    STLControl.NativeInterface.GetTextRange(ref textRange);

        //    byte[] managedBytes = new byte[textRange.chrg.cpMax - textRange.chrg.cpMin + 2];
        //    Marshal.Copy(textRange.lpstrText, managedBytes, 0, textRange.chrg.cpMax - textRange.chrg.cpMin + 2);
        //    Marshal.FreeCoTaskMem(textRange.lpstrText);
        //    string rangeText = e.Range.Text;
        //    HiliteSyntax(managedBytes, rangeText.Length, e.Range.Start, rangeText);
        //}

        public string[] findVariable(string p)
        {
            curWord = p;
            List<string> matches = variables.FindAll(finder);
            return matches.ToArray();
        }

        private string curWord;

        private bool finder(string var)
        {   
            string Text = STLControl.Text;
            int foundAt = Text.IndexOf(var);
            if (foundAt != -1)
            {
                for (int i = foundAt + var.Length; i < Text.Length; i++)
                {
                    if (char.IsWhiteSpace(Text[i])) continue;
                    if (Text[i] == '=' && Text[i+1] == '>')
                    {
                        variables.Remove(var);
                        return false;
                    }
                    break;
                }
                return var.StartsWith(curWord, StringComparison.OrdinalIgnoreCase);
            }
            else
            {
                variables.Remove(var);
            }
            return false;
        }

        public int getStyleIndex(string p)
        {
            return styleIndex[p];
        }

        public void SetStyles(Scintilla editor)
        {
            LoadStyle(editor);
        }
    }
}