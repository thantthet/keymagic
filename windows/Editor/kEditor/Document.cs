using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ScintillaNet;
using System.IO;
using System.Drawing;

namespace kEditor
{
    class Document
    {
        private Scintilla editor = new Scintilla();

        public Scintilla Editor
        {
            get { return editor; }
            set { editor = value; }
        }
        private String filePath = null;

        Styler lex;

        Styler Lex
        {
            get { return lex; }
            set { lex = value; }
        }

        public Document()
        {
            editor.Lexing.Lexer = (ScintillaNet.Lexer)100;

            ScintillaNet.Style S = editor.Styles[0x21];
            S.ForeColor = Color.DimGray;
            S.Italic = true;

            editor.Encoding = Encoding.UTF8;
        }

        public String FileName
        {
            get {
                if (String.IsNullOrEmpty(filePath)) return null;
                return Path.GetFileName(filePath);
            }
        }

        public bool Modified
        {
            get { return editor.Modified; }
            set { editor.Modified = value; }
        }

        public String FilePath
        {
            get { return filePath; }
            set { filePath = value; }
        }

        public bool SaveDoc()
        {
            return false;
        }

        public bool OpenDoc(String fileName)
        {
            return false;
        }

        public void SetText(String text)
        {
            editor.Text = text;
        }
    }
}
