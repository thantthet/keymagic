using System;
using System.Text;
using ScintillaNET;
using System.IO;
using System.Drawing;

namespace kEditor
{
    public class Document
    {
        private Scintilla editor = new Scintilla();
        private KmsLexer lexer = new KmsLexer(String.Join(" ", Keywords.all));

        public Scintilla Editor
        {
            get { return editor; }
            set { editor = value; }
        }
        private String filePath = null;

        public Document()
        {
            ApplyStyle();

            editor.Lexer = Lexer.Container;
            editor.StyleNeeded += Editor_StyleNeeded;

            Styler.shared.StyleDidChange += Shared_StyleDidChange;
        }

        private void ApplyStyle()
        {
            editor.StyleResetDefault();
            editor.Styles[Style.Default].Font = "Consolas";
            editor.Styles[Style.Default].Size = 10;
            editor.StyleClearAll();

            foreach (var ss in Styler.shared.GetStyles())
            {
                editor.Styles[ss.Key].Font = ss.Value.Font;
                editor.Styles[ss.Key].ForeColor = ss.Value.ForeColor;
                editor.Styles[ss.Key].BackColor = ss.Value.BackColor;
                editor.Styles[ss.Key].SizeF = ss.Value.Size;
                editor.Styles[ss.Key].Bold = ss.Value.Bold;
                editor.Styles[ss.Key].Italic = ss.Value.Italic;
                editor.Styles[ss.Key].Underline = ss.Value.Underline;
            }
        }

        private void Shared_StyleDidChange(object sender, EventArgs e)
        {
            ApplyStyle();
        }

        private void Editor_StyleNeeded(object sender, StyleNeededEventArgs e)
        {
            var startPos = editor.GetEndStyled();
            var endPos = e.Position;

            lexer.Style(editor, startPos, endPos);
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

                Editor.SetSavePoint();
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

                Editor.EmptyUndoBuffer();
                Editor.SetSavePoint();
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
