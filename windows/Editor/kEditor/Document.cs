using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ScintillaNet;
using System.IO;
using System.Drawing;

namespace kEditor
{
    public class Document
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
            set {
                if (filePath != value)
                {
                    

                    filePath = value;
                }
            }
        }

        public bool SaveAsDoc(String file)
        {
            try
            {
                StreamWriter sw = new StreamWriter(file);
                sw.Write(Editor.Text);
                sw.Close();

                Editor.Modified = false;
                filePath = file;
            }
            catch
            {
                return false;
            }
            return true;
        }

        public bool SaveDoc()
        {
            return SaveAsDoc(filePath);
        }

        public bool OpenDoc(String file)
        {
            try
            {
                System.IO.StreamReader sr = new System.IO.StreamReader(file);
                Editor.Text = sr.ReadToEnd();
                sr.Close();

                Editor.UndoRedo.EmptyUndoBuffer();
                Editor.Modified = false;
            }
            catch
            {
                return false;
            }
            return true;
        }

        public void SetText(String text)
        {
            editor.Text = text;
        }
    }
}
