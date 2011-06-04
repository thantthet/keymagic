using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using WeifenLuo.WinFormsUI.Docking;
using System.Drawing;
using System.Windows.Forms;

namespace kEditor
{
    class DockableDocument : Document
    {
        private DockContent dockContent = null;

        public DockContent DockContent
        {
            get { return dockContent; }
            set { dockContent = value; }
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
            try
            {
                dockContent = new DockContent();
                dockContent.ShowHint = DockState.Document;
                dockContent.BackColor = Color.Black;
                dockContent.DockAreas = DockAreas.Document | DockAreas.Float;
                dockContent.Controls.Add(this.Editor);
                this.Editor.Dock = DockStyle.Fill;
                dockContent.Show(dockPanel);
                dockContent.Tag = this;

                if (String.IsNullOrEmpty(filePath))
                {
                    this.Editor.Text = newDocumentTemplate;
                    this.Editor.UndoRedo.EmptyUndoBuffer();
                    this.Editor.Modified = false;

                    dockContent.TabText = "Untitled";
                    return;
                }
                System.IO.StreamReader sr = new System.IO.StreamReader(filePath);
                string str = sr.ReadToEnd();
                sr.Close();

                this.Editor.Text = str;
                FilePath = filePath;

                dockContent.Name = filePath;
                dockContent.TabText = FileName;
            }
            catch
            {
            }
        }
    }
}
