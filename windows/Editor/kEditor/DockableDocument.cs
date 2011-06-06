using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using WeifenLuo.WinFormsUI.Docking;
using System.Drawing;
using System.Windows.Forms;

namespace kEditor
{
    public class DockableDocument : Document
    {
        private DockContent dockContent = null;

        public DockContent DockContent
        {
            get { return dockContent; }
            set { dockContent = value; }
        }

        public string DocTitle
        {
            get { return (FileName != null ? FileName : "Untitled"); }
        }

        private string newDocumentTemplate =
            "/*\n" +
            "@NAME = 'Untitled Keyboard Layout'\n" +
            "@DESCRIPTION = 'Created using kEditor'\n" +
            "@ICON = ''\n" +
            "@FONTFAMILY = ''\n" +
            "@HOTKEY = ''\n" +
            "@EAT_ALL_UNUSED_KEYS = 'false'\n" +
            "@TRACK_CAPSLOCK = 'false'\n" +
            "@SMART_BACKSPACE = 'true'\n" +
            "@US_LAYOUT_BASED = 'true'\n" +
            "*/\n\n";

        public DockableDocument(String filePath, DockPanel dockPanel)
        {
            Editor.ModifiedChanged += new EventHandler(Editor_ModifiedChanged);

            try
            {
                dockContent = new DockContent();
                dockContent.ShowHint = DockState.Document;
                dockContent.BackColor = Color.Black;
                dockContent.DockAreas = DockAreas.Document | DockAreas.Float;
                dockContent.Controls.Add(this.Editor);
                Editor.Dock = DockStyle.Fill;
                dockContent.Show(dockPanel);
                dockContent.Tag = this;

                if (String.IsNullOrEmpty(filePath))
                {
                    Editor.Text = newDocumentTemplate;
                    Editor.UndoRedo.EmptyUndoBuffer();
                    Editor.Modified = false;

                    dockContent.TabText = "Untitled";
                    return;
                }

                if (OpenDoc(filePath))
                {
                    FilePath = filePath;
                    dockContent.Name = filePath;
                    dockContent.TabText = DocTitle;
                }
            }
            catch
            {
            }
        }

        void Editor_ModifiedChanged(object sender, EventArgs e)
        {
            if (Editor.Modified)
            {
                dockContent.TabText = DocTitle + "*";
            }
            else
            {
                dockContent.TabText = DocTitle;
            }
        }

        public DialogResult SaveAs()
        {
            SaveFileDialog saveFileDlg = new SaveFileDialog();
            saveFileDlg.AddExtension = true;
            saveFileDlg.DefaultExt = "kms";
            saveFileDlg.Filter = "KeyMagic Layout Script|*kms";

            DialogResult dr = saveFileDlg.ShowDialog();

            if (dr == System.Windows.Forms.DialogResult.OK)
            {
                FilePath = saveFileDlg.FileName;
            }
            else
            {
                return dr;
            }

            if (SaveAsDoc(FilePath))
            {
                dockContent.TabText = DocTitle;
                return DialogResult.OK;
            }

            return DialogResult.None;
        }

        public DialogResult Save()
        {
            if (string.IsNullOrEmpty(FilePath))
            {
                SaveFileDialog saveFileDlg = new SaveFileDialog();
                saveFileDlg.AddExtension = true;
                saveFileDlg.DefaultExt = "kms";
                saveFileDlg.Filter = "KeyMagic Layout Script|*kms";

                DialogResult dr = saveFileDlg.ShowDialog();

                if (dr == System.Windows.Forms.DialogResult.OK)
                {
                    FilePath = saveFileDlg.FileName;
                }
                else
                {
                    return dr;
                }
            }

            if (SaveDoc())
            {
                dockContent.TabText = DocTitle;
                return DialogResult.OK;
            }

            return DialogResult.None;
        }
    }
}
