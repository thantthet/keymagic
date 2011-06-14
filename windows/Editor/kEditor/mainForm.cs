using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.Runtime.InteropServices;
using System.Diagnostics;
using Microsoft.Win32;
using WeifenLuo.WinFormsUI.Docking;
using Utils.MessageBoxExLib;
using System.IO;
using System.Collections;
using System.Resources;

namespace kEditor
{
    public partial class mainFrame : Form
    {
        //private bool WorkingWithFile = false;
        //private string FileName;
        //private string ActiveFilePath;
        private DockableDocument activeDocument;

        public DockableDocument ActiveDocument
        {
            get { return activeDocument; }
            set { activeDocument = value; }
        }

        Styler lex;

        private List<string> recentFiles;
        private Font selectedFont;
        private string titleSuffix = " - KMS Editor";

        private ToolTip editorToolTip;
        private String toolTipString;
        private List<string> autoCompleteList = new List<string>();

        private ConfigStyles frmStyleConfig;
        private string thisExe;
        private string thisDir;

        #region Native Imports
        [DllImport("user32.dll", CharSet = CharSet.Auto, ExactSpelling = true)]
        private static extern bool GetCaretPos(out Point caretPoint);

        [DllImport("user32.dll", CharSet = CharSet.Auto, ExactSpelling = true)]
        private static extern short GetAsyncKeyState(int Key);

        [DllImport("shell32.dll", CharSet = CharSet.Auto, ExactSpelling = true)]
        private static extern void SHChangeNotify(int uEventID, uint uFlags, UIntPtr dwItem1, UIntPtr dwItem2);
        #endregion

        public mainFrame()
        {
            InitializeComponent();

            DoDocking();

            thisExe = Environment.GetCommandLineArgs()[0];
            thisDir = System.IO.Path.GetDirectoryName(thisExe);

            SciEditor.Lexing.Lexer = (ScintillaNet.Lexer)100;

            bool defaultEditor = isDefaultEditor();
            defaultEditorToolStripMenuItem.Checked = defaultEditor;

            if (Properties.Settings.Default.ForceDefaultEditor)
            {
                forceAsDefaultEditorToolStripMenuItem.Checked = true;
                if (defaultEditor == false && makeDefaultEditor())
                {
                    defaultEditorToolStripMenuItem.Checked = true;
                }
            }

            string[] args = Environment.GetCommandLineArgs();
        }
        DockPanel dockPanel;
        DockContent GlyphDock;
        DockContent OutputDock;

        private void DoDocking()
        {
            dockPanel = new DockPanel();
            dockPanel.Dock = DockStyle.Fill;
            dockPanel.BackColor = Color.Beige;
            Controls.Add(dockPanel);
            dockPanel.BringToFront();

            //string[] all = System.Reflection.Assembly.GetEntryAssembly().GetManifestResourceNames();

            //foreach (string one in all)
            //{
            //    MessageBox.Show(one);
            //}
            GlyphDock = new DockContent();
            using (Bitmap bm = Properties.Resources.GlyphMap)
            {
                GlyphDock.Icon = Icon.FromHandle(bm.GetHicon());
            }
            GlyphDock.Name = "GlyphDock";
            GlyphDock.Text = "Glyph Table";
            GlyphDock.ShowHint = DockState.DockLeft;
            GlyphDock.DockAreas = DockAreas.DockBottom | DockAreas.DockLeft | DockAreas.DockRight | DockAreas.DockTop | DockAreas.Float;
            GlyphDock.Controls.Add(GlyphMapTableLayout);
            GlyphDock.HideOnClose = true;
            GlyphDock.Show(dockPanel);

            GlyphMapTableLayout.Dock = DockStyle.Fill;

            OutputDock = new DockContent();
            using (Bitmap bm = Properties.Resources.Report)
            {
                OutputDock.Icon = Icon.FromHandle(bm.GetHicon());
            }
            OutputDock.Text = "Output";
            OutputDock.Name = "Output";
            OutputDock.TabText = "Output";
            OutputDock.DockAreas = DockAreas.DockBottom | DockAreas.DockLeft | DockAreas.DockRight | DockAreas.DockTop | DockAreas.Float;
            OutputDock.ShowHint = DockState.DockBottom;
            OutputDock.HideOnClose = true;
            OutputDock.Controls.Add(txtOutput);
            txtOutput.Dock = DockStyle.Fill;
            OutputDock.Show(dockPanel);
            OutputDock.DockTo(dockPanel, DockStyle.Bottom);
        }

        private bool isDefaultEditor()
        {
            using (RegistryKey userChoice = Registry.CurrentUser.OpenSubKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.kms\\UserChoice", false))
            {
                if (userChoice != null)
                {
                    if (userChoice.GetValue("progid").Equals("KEYMAGIC.KMS") == false)
                    {
                        return false;
                    }
                }
            }
            using (RegistryKey kms = Registry.ClassesRoot.OpenSubKey(".kms", false))
            {
                if (kms != null)
                {
                    if (kms.GetValue("").Equals("KEYMAGIC.KMS") == false)
                    {
                        return false;
                    }
                    using (RegistryKey keyKMS = Registry.ClassesRoot.OpenSubKey("KEYMAGIC.KMS", false))
                    {
                        if (keyKMS != null)
                        {
                            using (RegistryKey openCommand = keyKMS.OpenSubKey("shell\\Open\\command", false))
                            {
                                string command = string.Format("\"{0}\" \"%1\"", Environment.GetCommandLineArgs()[0]);
                                if (openCommand.GetValue("").Equals(command) == false)
                                {
                                    return false;
                                }
                            }
                        }
                        else
                        {
                            return false;
                        }
                    }
                }
                else
                {
                    return false;
                }
            }
            return true;
        }

        private bool makeDefaultEditor()
        {
            try
            {
                RegistryKey keyExt = Registry.ClassesRoot.CreateSubKey(".kms");

                keyExt.SetValue("", "KEYMAGIC.KMS");

                RegistryKey keyKMS = Registry.ClassesRoot.CreateSubKey("KEYMAGIC.KMS");
                keyKMS.SetValue("", "KeyMagic keyboard layout script file");

                RegistryKey openCommand = keyKMS.CreateSubKey("shell\\Open\\command");

                string command = string.Format("\"{0}\" \"%1\"", Environment.GetCommandLineArgs()[0]);
                openCommand.SetValue("", command, RegistryValueKind.ExpandString);

                string icon = string.Format("\"{0}\",0", Environment.GetCommandLineArgs()[0]);
                RegistryKey defaultIcon = keyKMS.CreateSubKey("DefaultIcon");
                defaultIcon.SetValue("", icon, RegistryValueKind.ExpandString);

                openCommand.Close();
                keyKMS.Close();

                SHChangeNotify(0x08000000, 0, UIntPtr.Zero, UIntPtr.Zero);

                keyExt.Close();
            }
            catch (UnauthorizedAccessException uax)
            {
                if (Properties.Settings.Default.DoNotAskForAdmin == true) return false;


                MessageBoxEx msgBox = MessageBoxExManager.GetMessageBox("access denied");
                if (msgBox == null)
                {
                    msgBox = MessageBoxExManager.CreateMessageBox("access denied");
                    msgBox.Caption = "Access denied";
                    msgBox.Text = string.Format("{0}\n{1}", uax.Message, "Do you want to run the KMS Editor as administrator.");
                    msgBox.Icon = MessageBoxExIcon.Exclamation;

                    msgBox.AddButton("OK", "OK");
                    msgBox.AddButton("Not Now", "NN");
                    msgBox.AddButton("Don't ask again", "DONT");
                }
                switch (msgBox.Show(this))
                {
                    case "DONT":
                        Properties.Settings.Default.ForceDefaultEditor = false;
                        break;
                    case "OK":
                        RunAsAdmin(string.Empty);
                        break;
                }
                return false;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                return false;
            }
            return true;
        }

        private bool RunAsAdmin(string args)
        {
            string failedMessage = "Failed to create process.";
            try
            {
                ProcessStartInfo psi = new ProcessStartInfo(Environment.GetCommandLineArgs()[0]);
                psi.Arguments = args;
                psi.Verb = "runas";
                Process p = Process.Start(psi);
                if (p == null)
                {
                    MessageBox.Show(this, failedMessage, "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
                this.Close();
            }
            catch (Exception)
            {
                MessageBox.Show(this, failedMessage, "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
            return true;
        }

        private void mainForm_Load(object sender, EventArgs e)
        {
            lex = new Styler(SciEditor);
            frmStyleConfig = new ConfigStyles(SciEditor, lex.GetStyleNameIndex());

            selectedFont = new Font(Properties.Settings.Default.DefaultFontName, Properties.Settings.Default.DefaultFontSize);
            glyphTable.Font = selectedFont;

            autoCompleteList.AddRange(new string[] 
            {
                "ANY",
                "VK_KEY_1","VK_KEY_2","VK_KEY_3","VK_KEY_4","VK_KEY_5","VK_KEY_6","VK_KEY_7","VK_KEY_8","VK_KEY_9","VK_KEY_0",

                "VK_KEY_A","VK_KEY_B","VK_KEY_C","VK_KEY_D","VK_KEY_E","VK_KEY_F","VK_KEY_G","VK_KEY_H","VK_KEY_I","VK_KEY_J",
                "VK_KEY_K","VK_KEY_L","VK_KEY_M","VK_KEY_N","VK_KEY_O","VK_KEY_P","VK_KEY_Q","VK_KEY_R","VK_KEY_S","VK_KEY_T",
                "VK_KEY_U","VK_KEY_V","VK_KEY_W","VK_KEY_X","VK_KEY_Y","VK_KEY_Z",

                "VK_NUMPAD0","VK_NUMPAD2","VK_NUMPAD3","VK_NUMPAD4","VK_NUMPAD5","VK_NUMPAD6","VK_NUMPAD7","VK_NUMPAD8","VK_NUMPAD9","VK_NUMPAD10",

                "NULL","VK_BACK","VK_TAB","VK_ENTER","VK_RETURN","VK_SHIFT","VK_CONTROL","VK_CTRL","VK_MENU","VK_ALT","VK_SPACE",
                "VK_PRIOR","VK_CAPITAL","VK_CAPSLOCK",
                
                "VK_OEM_1","VK_COLON","VK_OEM_2","VK_QUESTION","VK_OEM_3","VK_CFLEX","VK_OEM_4","VK_LBRACKET","VK_OEM_5","VK_BACKSLASH",
                "VK_OEM_6","VK_RBRACKET","VK_OEM_7","VK_QUOTE","VK_OEM_8","VK_EXCM","VK_OEM_AX","VK_OEM_102","VK_LESSTHEN","VK_ICO_HELP","VK_ICO_00",
                "VK_OEM_MINUS","VK_OEM_PLUS",

                "VK_RMENU","VK_RALT","VK_ALT_GR","VK_LMENU","VK_LALT",
                "VK_RCONTROL","VK_RCTRL","VK_LCTRL","VK_LCONTROL"
            }
            );

            Text = "Untitled" + titleSuffix;

            glyphTable.HexNotation = Properties.Settings.Default.HexNotation;
            hexadecimalToolStripMenuItem.Checked = glyphTable.HexNotation;
            lineNumbersToolStripMenuItem.Checked = Properties.Settings.Default.LineNumber;

            UpdateRecentFiles();
            string[] tabs = Properties.Settings.Default.LastTabs.Split(new char[] { '|' }, StringSplitOptions.RemoveEmptyEntries);
            foreach (string tab in tabs)
            {
                CreateNewDocument(tab);
            }

            if (dockPanel.DocumentsCount == 0)
            {
                CreateNewDocument(string.Empty);
            }

            glyphTable.Filter = Properties.Settings.Default.GlyphFilterText;
            txtFilter.Text = glyphTable.Filter;
        }

        private DockableDocument CreateNewDocument(string filePath)
        {
            DockableDocument doc = new DockableDocument(filePath, dockPanel);
            lex.SetStyles(doc.Editor);
            doc.DockContent.Activated += new EventHandler(DockContent_Activated);
            doc.DockContent.FormClosing += new FormClosingEventHandler(DockContent_FormClosing);
            doc.Editor.CharAdded += new EventHandler<ScintillaNet.CharAddedEventArgs>(Editor_CharAdded);
            doc.Editor.TextChanged += new EventHandler<EventArgs>(Editor_TextChanged);
            doc.Editor.SelectionChanged += new EventHandler(Editor_SelectionChanged);

            if (string.IsNullOrEmpty(doc.DocTitle))
            {
                doc.DockContent.Close();
                return null;
            }

            activeDocument = doc;
            Text = doc.DocTitle + titleSuffix;

            return doc;
        }

        void DockContent_FormClosing(object sender, FormClosingEventArgs e)
        {
            DockContent dc = sender as DockContent;
            DockableDocument dd = dc.Tag as DockableDocument;

            if (dd.Editor.Modified)
            {
                switch (MessageBox.Show(this, string.Format("Do you want to save '{0}'?", dc.TabText), "Saving?", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button1))
                {
                    case DialogResult.Cancel:
                        e.Cancel = true;
                        break;
                    case DialogResult.Yes:
                        if (dd.Save() == DialogResult.Cancel)
                        {
                            e.Cancel = true;
                        }
                        break;
                    case DialogResult.No:
                        break;
                }
                //MessageBoxEx msgBox = MessageBoxExManager.CreateMessageBox("ask for saving");
                //msgBox.Caption = "Saving?";
                //msgBox.Text = string.Format("Do you want to save '{0}'?", dd.FileName);
                //msgBox.Icon = MessageBoxExIcon.Exclamation;

                //msgBox.AddButton("Yes", "Y");
                //msgBox.AddButton("No", "N");
                //msgBox.AddButton("Cancel", "C");
                //switch (msgBox.Show(this))
                //{
                //    case "C":
                //        e.Cancel = true;
                //        break;
                //    case "Y":
                //        if (dd.Save() == DialogResult.Cancel)
                //        {
                //            e.Cancel = true;
                //        }
                //        break;
                //    case "N":
                //        break;
                //}
            }
        }

        private void mainFrame_FormClosing(object sender, FormClosingEventArgs e)
        {
            List<string> tabs = new List<string>();
            foreach (DockContent dc in dockPanel.Documents)
            {
                DockableDocument dd = dc.Tag as DockableDocument;
                if (string.IsNullOrEmpty(dd.FilePath) == false)
                {
                    tabs.Add(dd.FilePath);
                }
            }

            Properties.Settings.Default.LastTabs = string.Join("|", tabs.ToArray());
            Properties.Settings.Default.RecentFiles = string.Join("|", recentFiles.ToArray());
            Properties.Settings.Default.DefaultFontName = selectedFont.Name;
            Properties.Settings.Default.DefaultFontSize = selectedFont.Size;
            Properties.Settings.Default.GlyphFilterText = glyphTable.Filter;
            Properties.Settings.Default.LineNumber = lineNumbersToolStripMenuItem.Checked;
            Properties.Settings.Default.HexNotation = hexadecimalToolStripMenuItem.Checked;
            //lex.SaveStyles();
            Properties.Settings.Default.Save();
        }

        private void UpdateRecentFiles()
        {
            if (recentFiles == null)
            {
                recentFiles = new List<string>(Properties.Settings.Default.RecentFiles.Split('|'));
                recentFiles.Remove("");
            }

            for (int i = recentFilesToolStripMenuItem.DropDownItems.Count - 2; i > 0; i--)
            {
                recentFilesToolStripMenuItem.DropDownItems[i - 1].Dispose();
            }

            //ToolStripItem[] tsi = new ToolStripItem[recentFiles.Count];
            foreach (string s in recentFiles)
            {
                ToolStripMenuItem menuItem = new ToolStripMenuItem(s);
                menuItem.Click += new EventHandler(RecentFileMenuItem_Click);
                recentFilesToolStripMenuItem.DropDownItems.Insert(0, menuItem);
            }
        }

        private void Editor_TextChanged(object sender, EventArgs e)
        {
            ScintillaNet.Scintilla Editor = sender as ScintillaNet.Scintilla;
            //sm.HiliteSyntax();
            computeLineNumMargin(Editor);
        }

        private void computeLineNumMargin(ScintillaNet.Scintilla Editor)
        {
            if (lineNumbersToolStripMenuItem.Checked)
            {
                Editor.Margins[0].Width = Editor.Lines.Count.ToString().Length * 10;
            }
        }

        private void Editor_SelectionChanged(object sender, EventArgs e)
        {
            ScintillaNet.Scintilla Editor = sender as ScintillaNet.Scintilla;
            string selText;

            if (editorToolTip != null && editorToolTip.Active)
            {
                editorToolTip.Hide(Editor);
                editorToolTip.Dispose();
            }

            if (Editor.NativeInterface.GetSelectionStart() == Editor.NativeInterface.GetSelectionEnd())
            {
                return;
            }

            Editor.NativeInterface.GetSelText(out selText);
            selText = selText.Trim();

            if (selText.Length > 0)
            {
                if (selText.Length > 4 && selText.StartsWith("U", StringComparison.OrdinalIgnoreCase))
                {
                    editorToolTip = new ToolTip();
                    editorToolTip.Popup += new PopupEventHandler(editorToolTip_Popup);
                    editorToolTip.OwnerDraw = true;
                    editorToolTip.Draw += new DrawToolTipEventHandler(editorToolTip_Draw);

                    string[] splitted = selText.Split(' ');
                    StringBuilder concated = new StringBuilder();
                    foreach (string split in splitted)
                    {
                        if (split.StartsWith("U", StringComparison.OrdinalIgnoreCase) && split.Length > 4)
                        {
                            string hexString = split.Substring(1, 4);
                            int hexCode;
                            if (int.TryParse(hexString, System.Globalization.NumberStyles.AllowHexSpecifier, null, out hexCode))
                            {
                                concated.Append((char)hexCode);
                            }
                        }
                    }

                    Point caretPoint = new Point();
                    if (GetCaretPos(out caretPoint))
                    {
                        caretPoint.X += 5;
                        caretPoint.Y -= 30;
                        toolTipString = concated.ToString();
                        editorToolTip.Show(toolTipString, Editor, caretPoint);
                    }
                }
            }
        }

        void editorToolTip_Popup(object sender, PopupEventArgs e)
        {
            Graphics g = CreateGraphics();
            int multBy = 1;
            Size msSize = g.MeasureString("    " + toolTipString, selectedFont).ToSize();
            if (msSize.Width < 10) multBy = 4;
            else if (msSize.Width < 6) multBy = 8;
            e.ToolTipSize = new Size(msSize.Width * multBy, msSize.Height);
        }

        void editorToolTip_Draw(object sender, DrawToolTipEventArgs e)
        {
            e.DrawBackground();
            e.DrawBorder();

            StringFormat sFormat = new StringFormat();
            sFormat.Alignment = StringAlignment.Center;

            TextRenderer.DrawText(e.Graphics, e.ToolTipText, selectedFont, e.Bounds, Color.Blue, TextFormatFlags.Default);
            //e.Graphics.DrawString(e.ToolTipText, selectedFont, Brushes.Blue, e.Bounds, sFormat);

            sFormat.Dispose();
        }

        private void glyphTable_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            int cellIndex = glyphTable.GetCellAtPoint(e.Location);
            if (cellIndex == -1 || cellIndex >= glyphTable.NumbersOfGlyph)
            {
                return;
            }
            int charValue = glyphTable.characterAtCell(cellIndex);

            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                int state = GetAsyncKeyState(0x10);
                string format = "U{0:X4}" + ((state & 0xf000) != 0 ? " + " : "");
                ActiveDocument.Editor.InsertText(string.Format(format, (int)charValue));
            }
        }

        private DialogResult askToSaveModifiedDocument()
        {
            string wraningMessage = "Do you want to save changes?";
            string errorMessage = "File has not been saved successfully.";

            if (ActiveDocument.Modified == false)
            {
                return DialogResult.OK;
            }

            DialogResult dr = MessageBox.Show(this, wraningMessage, "Warnings", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation);
            if (dr == System.Windows.Forms.DialogResult.Yes)
            {
                if (ActiveDocument.Save() != DialogResult.OK)
                {
                    MessageBox.Show(this, errorMessage, "Warnings", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    return System.Windows.Forms.DialogResult.Cancel;
                }
                return DialogResult.OK;
            }
            return dr;
        }

        //private bool openFile(string OpenFilePath)
        //{
        //    String FileName;

        //    if (OpenFilePath == null)
        //    {
        //        openFileDlg.CheckFileExists = true;

        //        if (openFileDlg.ShowDialog(this) != System.Windows.Forms.DialogResult.OK)
        //        {
        //            return false;
        //        }
        //        else
        //        {
        //            OpenFilePath = openFileDlg.FileName;
        //        }
        //    }

        //    FileName = System.IO.Path.GetFileName(OpenFilePath);

        //    DockableDocument dockDoc = new DockableDocument(OpenFilePath, dockPanel);
        //    dockDoc.DockContent.Activated += new EventHandler(DockContent_Activated);
        //    bool success = dockDoc.DockContent != null;
        //    lex.SetStyles(dockDoc.Editor);

        //    if (success)
        //    {
        //        ActiveDocument = dockDoc;
        //        setRecentFile(OpenFilePath);
        //    }
        //    return success;

        //    //System.IO.StreamReader sr = new System.IO.StreamReader(FilePath);
        //    //string str = sr.ReadToEnd();
        //    //sr.Close();
        //    //SciEditor.Text = str;

        //    //SciEditor.Modified = false;
        //    //WorkingWithFile = true;

        //    //Text = FileName;
        //    //SciEditor.UndoRedo.EmptyUndoBuffer();

        //    //return true;
        //}

        void DockContent_Activated(object sender, EventArgs e)
        {
            DockContent docDock = sender as DockContent;
            ActiveDocument = docDock.Tag as DockableDocument;
            Text = ActiveDocument.DocTitle + titleSuffix;
        }

        //private bool saveActiveDocument()
        //{
        //    if (string.IsNullOrEmpty(activeDocument.FilePath))
        //    {
        //        //saveFileDlg.FileName = FilePath;
        //        if (saveFileDlg.ShowDialog() == System.Windows.Forms.DialogResult.OK)
        //        {
        //            ActiveDocument.FilePath = saveFileDlg.FileName;
        //        }
        //        else
        //        {
        //            return false;
        //        }
        //    }

        //    try
        //    {
        //        System.IO.StreamWriter sw = new System.IO.StreamWriter(ActiveDocument.FilePath, false, Encoding.UTF8);
        //        sw.Write(SciEditor.Text);
        //        sw.Flush();
        //        sw.Close();
        //        sw.Dispose();

        //        activeDocument.Editor.Modified = false;

        //        Text = ActiveDocument.FileName;
        //        setRecentFile(ActiveDocument.FilePath);
        //    }
        //    catch (Exception ex)
        //    {
        //        MessageBox.Show(ex.Message);
        //        return false;
        //    }

        //    return true;
        //}

        private void setRecentFile(String FilePath)
        {
            recentFiles.Remove(FilePath);
            recentFiles.Add(FilePath);

            int maxCount = 10;

            if (recentFiles.Count > maxCount)
            {
                recentFiles.RemoveRange(0, recentFiles.Count - maxCount);
            }
            UpdateRecentFiles();
        }

        #region Tool Strip Events

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ActiveDocument.Save();
            Text = ActiveDocument.DocTitle + titleSuffix;
            setRecentFile(ActiveDocument.FilePath);
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void defaultFontToolStripMenuItem_Click(object sender, EventArgs e)
        {
            fontDlg.Font = glyphTable.Font;
            fontDlg.ShowEffects = false;

            if (fontDlg.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                glyphTable.Font = fontDlg.Font;
                selectedFont = fontDlg.Font;
            }
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ActiveDocument = CreateNewDocument("");
            //if (askToSaveModifiedDocument() != System.Windows.Forms.DialogResult.Cancel)
            //{
            //    SciEditor.Text = newDocumentTemplate;
            //    SciEditor.UndoRedo.EmptyUndoBuffer();
            //    SciEditor.Selection.Start = newDocumentTemplate.Length;
            //    SciEditor.Selection.End = newDocumentTemplate.Length;
            //    FileName = "Untitled" + titleSuffix;
            //    FilePath = "";

            //    Text = FileName;

            //    SciEditor.Modified = false;
            //    WorkingWithFile = false;
            //}
        }

        private void hexadecimalToolStripMenuItem_Click(object sender, EventArgs e)
        {
            glyphTable.HexNotation = hexadecimalToolStripMenuItem.Checked;
        }

        private void clearRecentFileListToolStripMenuItem_Click(object sender, EventArgs e)
        {
            recentFiles.Clear();
            UpdateRecentFiles();
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ActiveDocument.SaveAs();
            Text = ActiveDocument.DocTitle + titleSuffix;
            setRecentFile(ActiveDocument.FilePath);
        }

        private void RecentFileMenuItem_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem recentItem = (ToolStripMenuItem)sender;
            if (SwitchIfOpened(recentItem.Text)) return;
            ActiveDocument = CreateNewDocument(recentItem.Text);
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openFileDlg.CheckFileExists = true;

            if (openFileDlg.ShowDialog(this) != System.Windows.Forms.DialogResult.OK)
            {
                return;
            }
            else
            {
                string OpenFilePath = openFileDlg.FileName;

                if (SwitchIfOpened(OpenFilePath)) return;

                ActiveDocument = CreateNewDocument(OpenFilePath);
                setRecentFile(OpenFilePath);
            }
        }

        private bool SwitchIfOpened(string OpenFilePath)
        {
            foreach (DockContent dc in dockPanel.Documents)
            {
                DockableDocument dd = dc.Tag as DockableDocument;
                if (string.IsNullOrEmpty(dd.FilePath) == false && dd.FilePath.Equals(OpenFilePath))
                {
                    dc.Show();
                    return true;
                }
            }
            return false;
        }

        private void lineNumbersToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (lineNumbersToolStripMenuItem.Checked)
            {
                SciEditor.Margins[0].Width = 30;
            }
            else
            {
                SciEditor.Margins[0].Width = 0;
            }
        }
        #endregion

        private string curWord;

        private void Editor_CharAdded(object sender, ScintillaNet.CharAddedEventArgs e)
        {
            ScintillaNet.Scintilla Editor = sender as ScintillaNet.Scintilla;

            curWord = Editor.GetWordFromPosition(Editor.Selection.Start);
            if (curWord.Length == 0)
            {
                return;
            }
            Predicate<string> startWord = compareWithCurrentWord;
            List<string> list = autoCompleteList.FindAll(startWord);

            if (list.Count > 0)
            {
                Editor.AutoComplete.Show(curWord.Length, string.Join(SciEditor.AutoComplete.ListSeparator.ToString(), list.ToArray()));
            }
        }

        private bool compareWithCurrentWord(string listStr)
        {
            if (listStr.StartsWith(curWord, StringComparison.OrdinalIgnoreCase))
            {
                return true;
            }
            return false;
        }

        private void forceAsDefaultEditorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Properties.Settings.Default.ForceDefaultEditor = forceAsDefaultEditorToolStripMenuItem.Checked;
            if (forceAsDefaultEditorToolStripMenuItem.Checked)
            {
                if (isDefaultEditor() == false && makeDefaultEditor())
                {
                    defaultEditorToolStripMenuItem.Checked = true;
                }
            }
        }

        private void changeStylesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (frmStyleConfig.Visible == false)
            {
                frmStyleConfig = new ConfigStyles(SciEditor, lex.GetStyleNameIndex());
                frmStyleConfig.Show();
            }
            else
            {
                frmStyleConfig.BringToFront();
            }
        }

        private void compileAndSaveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Compile(null);
        }

        private bool Compile(string saveFileName)
        {
            if (SciEditor.Modified)
            {
                if (askToSaveModifiedDocument() != System.Windows.Forms.DialogResult.OK)
                {
                    return false;
                }
            }

            SaveFileDialog saveFileDlg = new SaveFileDialog();
            saveFileDlg.AddExtension = true;
            saveFileDlg.DefaultExt = "km2";
            saveFileDlg.Filter = "KeyMagic Layout File|*km2";

            if (string.IsNullOrEmpty(saveFileName))
            {
                if (saveFileDlg.ShowDialog() != System.Windows.Forms.DialogResult.OK) return false;
                saveFileName = saveFileDlg.FileName;
            }
            return CallParser(ActiveDocument.FilePath, saveFileName);
        }

        private bool CallParser(string FileIn, string FileOut)
        {
            string nl = Environment.NewLine;
            bool ret = true;
            if (string.IsNullOrEmpty(FileIn)) return false;

            try
            {
                if (System.IO.File.Exists(thisDir + "\\parser.exe") == false)
                {
                    txtOutput.Text = "Parser program not found! You could download at http://keymagic.googlecode.com/" + nl;
                    //MessageBox.Show(this, "Parser program not found! You could download at http://keymagic.googlecode.com/", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
                string commandLine;
                if (FileOut != null && FileOut != "")
                {
                    commandLine = string.Format("\"{0}\" \"{1}\"", FileIn, FileOut);
                }
                else
                {
                    commandLine = string.Format("\"{0}\"", FileIn);
                }
                txtOutput.Text = "Executing parser : parser.exe " + commandLine + nl;
                ProcessStartInfo psi = new ProcessStartInfo(thisDir + "\\parser.exe", commandLine);
                psi.WindowStyle = ProcessWindowStyle.Hidden;
                psi.RedirectStandardOutput = true;
                psi.RedirectStandardError = true;
                psi.UseShellExecute = false;
                psi.CreateNoWindow = true;
                Process parserProcess = Process.Start(psi);
                System.IO.StreamReader stdout = parserProcess.StandardOutput;
                System.IO.StreamReader stderr = parserProcess.StandardError;
                parserProcess.WaitForExit(5000);

                string errText = stderr.ReadToEnd();
                string outText = stdout.ReadToEnd();

                if (parserProcess.ExitCode == 1)
                {
                    txtOutput.Text += errText + nl;
                    //MessageBox.Show(this, errText, "Failed", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    ret = false;
                }
                else
                {
                    string[] splitted = outText.Split('\n');
                    string lastLine = splitted[splitted.Length - 2];
                    txtOutput.Text += lastLine + nl + errText;
                    //MessageBox.Show(this, lastLine + "~~\n" + errText, "Success", MessageBoxButtons.OK, errText != "" ? MessageBoxIcon.Warning : MessageBoxIcon.Information);
                }
                stdout.Close();
                stderr.Close();
            }
            catch (System.IO.FileNotFoundException notFoundEx)
            {
                MessageBox.Show(notFoundEx.Message);
            }
            return ret;
        }

        private void toolsToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void checkSyntaxToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CheckSyntax(ActiveDocument.FilePath);
        }

        private bool CheckSyntax(string filePath)
        {
            if (askToSaveModifiedDocument() != System.Windows.Forms.DialogResult.OK)
            {
                return false;
            }

            return CallParser(ActiveDocument.FilePath, "");
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            aboutForm aboutF = new aboutForm();
            aboutF.ShowDialog(this);
        }

        private void txtFilter_TextChanged(object sender, EventArgs e)
        {
            glyphTable.Filter = txtFilter.GetText();
        }

        private void glyphTable_SelectionChanged(object sender, EventArgs e)
        {
            if (glyphTable.SelectedCell == -1)
            {
                return;
            }
            int c = glyphTable.Characters[glyphTable.SelectedCell];
            lblGlyphName.Text = glyphTable.GetNameForChar(c);
        }

        private void glyphTable_MouseMove(object sender, MouseEventArgs e)
        {
            int index = glyphTable.GetCellAtPoint(e.Location);
            if (index == -1)
            {
                return;
            }
            lblGlyphName.Text = glyphTable.GetNameForChar(glyphTable.Characters[index]);
        }

        private void glyphTable_MouseLeave(object sender, EventArgs e)
        {
            if (glyphTable.SelectedCell == -1)
            {
                return;
            }
            int c = glyphTable.Characters[glyphTable.SelectedCell];
            lblGlyphName.Text = glyphTable.GetNameForChar(c);
        }

        private void testToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string tempFileName = Path.GetTempFileName();
            if (Compile(tempFileName))
            {
                KeyMagicDotNet.KeyMagicEngine engine = new KeyMagicDotNet.KeyMagicEngine();
                if (engine.loadKeyboardFile(tempFileName) == false)
                {
                    MessageBox.Show("Cannot load keyboard file to test");
                    return;
                }

                KeyMagicDotNet.KeyMagicKeyboard keyboard = engine.getKeyboard();
                Byte[] font = keyboard.getInfo("font");
                Font f = selectedFont;
                if (font != null)
                {
                    f = new Font(Encoding.UTF8.GetString(font), selectedFont.Size);
                }

                TesterForm tester = new TesterForm(engine, f);
                tester.Show();
                tester.FormClosed += new FormClosedEventHandler(
                    delegate(object xsender, FormClosedEventArgs xe)
                    {
                        File.Delete(tempFileName);
                    }
                    );
            }
            else
            {
                File.Delete(tempFileName);
            }
        }

        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            activeDocument.DockContent.Close();
        }

        private void defaultEditorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            defaultEditorToolStripMenuItem.Checked = makeDefaultEditor();
        }

        #region Edit Menu

        private void undoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            activeDocument.Editor.UndoRedo.Undo();
        }

        private void redoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            activeDocument.Editor.UndoRedo.Redo();
        }

        private void cutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            activeDocument.Editor.Clipboard.Cut();
        }

        private void copyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            activeDocument.Editor.Clipboard.Copy();
        }

        private void pasteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            activeDocument.Editor.Clipboard.Paste();
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            activeDocument.Editor.NativeInterface.DeleteBack();
        }

        private void selectAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            activeDocument.Editor.Selection.SelectAll();
        }

        private void findToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void findAndReplaceToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void goToToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        #endregion

        private void glyphTableToolStripMenuItem_Click(object sender, EventArgs e)
        {
            GlyphDock.Show();
        }

        private void outputToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OutputDock.Show();
        }
    }
}
