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

namespace kEditor
{
    public partial class mainFrame : Form
    {
        private bool WorkingWithFile = false;
        private string FileName;
        private string FilePath;
        Styler lex;

        private List<string> recentFiles;
        private Font selectedFont;
        private string titleSuffix = " - KMS Editor";

        private ToolTip editorToolTip;
        private String toolTipString;
        private List<string> autoCompleteList = new List<string>();

        private ConfigStyles frmColor;
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

            thisExe = Environment.GetCommandLineArgs()[0];
            thisDir = System.IO.Path.GetDirectoryName(thisExe);

            SciEditor.Lexing.Lexer = (ScintillaNet.Lexer)100;

            bool defaultEditor = isDefaultEditor();

            if (Properties.Settings.Default.forceDefaultEditor)
            {
                if (defaultEditor == false)
                {
                    makeDefaultEditor();
                }
                defaultEditorToolStripMenuItem.Checked = true;
                forceAsDefaultEditorToolStripMenuItem.Checked = true;
            }
            else if (defaultEditor)
            {
                defaultEditorToolStripMenuItem.Checked = true;
            }

            string[] args = Environment.GetCommandLineArgs();
            if (args.Length == 2)
            {
                Properties.Settings.Default.LastFilePath = args[1];
            }
        }

        private bool isDefaultEditor()
        {
            using (RegistryKey userChoice = Registry.CurrentUser.OpenSubKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.kms\\UserChoice"))
            {
                if (userChoice != null)
                {
                    if (userChoice.GetValue("progid").Equals("KEYMAGIC.KMS") == false)
                    {
                        return false;
                    }
                }
            }
            using (RegistryKey kms = Registry.ClassesRoot.OpenSubKey(".kms"))
            {
                if (kms != null)
                {
                    if (kms.GetValue("").Equals("KEYMAGIC.KMS") == false)
                    {
                        return false;
                    }
                    using (RegistryKey keyKMS = Registry.ClassesRoot.OpenSubKey("KEYMAGIC.KMS"))
                    {
                        if (keyKMS != null)
                        {
                            using (RegistryKey openCommand = keyKMS.OpenSubKey("shell\\Open\\command"))
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

        private void makeDefaultEditor()
        {
            RegistryKey keyExt = Registry.ClassesRoot.CreateSubKey(".kms");
            //if (keyExt.GetValue("").Equals("KEYMAGIC.KMS") == false)
            //{
            keyExt.SetValue("", "KEYMAGIC.KMS");

            RegistryKey keyKMS = Registry.ClassesRoot.CreateSubKey("KEYMAGIC.KMS");
            keyKMS.SetValue("", "KeyMagic keyboard layout script file");

            RegistryKey openCommand = keyKMS.CreateSubKey("shell\\Open\\command");
            //if (openCommand.GetValue("").Equals("") == false)
            //{
            string command = string.Format("\"{0}\" \"%1\"", Environment.GetCommandLineArgs()[0]);
            openCommand.SetValue("", command, RegistryValueKind.ExpandString);

            string icon = string.Format("\"{0}\",0", Environment.GetCommandLineArgs()[0]);
            RegistryKey defaultIcon = keyKMS.CreateSubKey("DefaultIcon");
            defaultIcon.SetValue("", icon, RegistryValueKind.ExpandString);
            //}

            openCommand.Close();
            keyKMS.Close();

            SHChangeNotify(0x08000000, 0, UIntPtr.Zero, UIntPtr.Zero);
            //}
            keyExt.Close();
        }

        private void mainForm_Load(object sender, EventArgs e)
        {
            //SciEditor.ConfigurationManager.Language = "KeyMagic Layout Script";

            ScintillaNet.Style S = SciEditor.Styles[0x21];
            S.ForeColor = Color.DimGray;
            S.Italic = true;

            SciEditor.Encoding = Encoding.ASCII;

            lex = new Styler(SciEditor);
            frmColor = new ConfigStyles(SciEditor, lex.GetStyleNameIndex());

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
                
                "VK_RMENU","VK_RALT","VK_ALT_GR","VK_LMENU","VK_LALT",
                "VK_RCONTROL","VK_RCTRL","VK_LCTRL","VK_LCONTROL"
            }
            );

            Text = "Untitled" + titleSuffix;

            hexadecimalToolStripMenuItem.Checked = glyphTable.HexNotation;
            lineNumbersToolStripMenuItem.Checked = Properties.Settings.Default.LineNumber;

            UpdateRecentFiles();
            openFile(Properties.Settings.Default.LastFilePath);

            glyphTable.GlyphRange = new CharacterRange(Properties.Settings.Default.GlyphRangeFirst, Properties.Settings.Default.GlyphRangeLength);
        }

        private void mainFrame_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (askToSaveModifiedDocument() == System.Windows.Forms.DialogResult.Cancel)
            {
                e.Cancel = true;

            }
            if (recentFiles.Count > 0)
            {
                Properties.Settings.Default.LastFilePath = recentFiles[recentFiles.Count - 1];
            }
            Properties.Settings.Default.RecentFiles = string.Join(",", recentFiles.ToArray());
            Properties.Settings.Default.DefaultFontName = selectedFont.Name;
            Properties.Settings.Default.DefaultFontSize = selectedFont.Size;
            Properties.Settings.Default.GlyphRangeFirst = glyphTable.GlyphRange.First;
            Properties.Settings.Default.GlyphRangeLength = glyphTable.GlyphRange.Length;
            Properties.Settings.Default.LineNumber = lineNumbersToolStripMenuItem.Checked;
            lex.SaveStyles();
            Properties.Settings.Default.Save();
        }

        private void UpdateRecentFiles()
        {
            if (recentFiles == null)
            {
                recentFiles = new List<string>(Properties.Settings.Default.RecentFiles.Split(','));
                recentFiles.Remove("");
            }

            for (int i = recentFilesToolStripMenuItem.DropDownItems.Count - 2; i > 0; i--)
            {
                recentFilesToolStripMenuItem.DropDownItems[i - 1].Dispose();
            }

            ToolStripItem[] tsi = new ToolStripItem[recentFiles.Count];
            foreach (string s in recentFiles)
            {
                ToolStripMenuItem menuItem = new ToolStripMenuItem(s);
                menuItem.Click += new EventHandler(RecentFileMenuItem_Click);
                recentFilesToolStripMenuItem.DropDownItems.Insert(0, menuItem);
            }
        }

        private void SciEditor_TextChanged(object sender, EventArgs e)
        {
            //sm.HiliteSyntax();
            computeLineNumMargin();
        }

        private void computeLineNumMargin()
        {
            if (lineNumbersToolStripMenuItem.Checked)
            {
                SciEditor.Margins[0].Width = SciEditor.Lines.Count.ToString().Length * 10;
            }
        }

        private void SciEditor_SelectionChanged(object sender, EventArgs e)
        {
            string selText;

            if (editorToolTip != null && editorToolTip.Active)
            {
                editorToolTip.Hide(SciEditor);
                editorToolTip.Dispose();
            }

            if (SciEditor.NativeInterface.GetSelectionStart() == SciEditor.NativeInterface.GetSelectionEnd())
            {
                return;
            }

            SciEditor.NativeInterface.GetSelText(out selText);
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
                        editorToolTip.Show(toolTipString, SciEditor, caretPoint);
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

            e.Graphics.DrawString(e.ToolTipText, selectedFont, Brushes.Blue, e.Bounds, sFormat);

            sFormat.Dispose();
        }

        private void glyphTable_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            int cellIndex = glyphTable.GetCellAtPoint(e.Location);
            if (cellIndex == -1 || cellIndex >= glyphTable.NumbersOfGlyph)
            {
                return;
            }
            char charValue = glyphTable.characterAtCell(cellIndex);

            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                int state = GetAsyncKeyState(0x11);
                string format = "U{0:X4}" + ((state & 0xf000) != 0 ? " + " : "");
                SciEditor.InsertText(string.Format(format, (int)charValue));
            }
        }

        private DialogResult askToSaveModifiedDocument()
        {
            string wraningMessage = "Do you want to save changes?";
            string errorMessage = "File has not been saved successfully.";

            if (SciEditor.Modified)
            {
                DialogResult dr = MessageBox.Show(this, wraningMessage, "Warnings", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation);
                if (dr == System.Windows.Forms.DialogResult.Yes)
                {
                    if (saveFile() == false)
                    {
                        MessageBox.Show(this, errorMessage, "Warnings", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                        return System.Windows.Forms.DialogResult.Cancel;
                    }
                    return System.Windows.Forms.DialogResult.OK;
                }
                return dr;
            }
            return System.Windows.Forms.DialogResult.OK;
        }

        private bool openFile(string OpenFilePath)
        {
            if (OpenFilePath == null)
            {
                openFileDlg.CheckFileExists = true;
                openFileDlg.CheckPathExists = true;
                if (openFileDlg.ShowDialog(this) != System.Windows.Forms.DialogResult.OK)
                {
                    return false;
                }
                else
                {
                    FilePath = openFileDlg.FileName;
                }
            }
            else if (System.IO.File.Exists(OpenFilePath))
            {
                FilePath = OpenFilePath;
            }
            else { return false; }

            FileName = System.IO.Path.GetFileName(FilePath);

            System.IO.StreamReader sr = new System.IO.StreamReader(FilePath);
            string str = sr.ReadToEnd();
            sr.Close();
            SciEditor.Text = str;

            SciEditor.Modified = false;
            WorkingWithFile = true;

            Text = FileName;
            SciEditor.UndoRedo.EmptyUndoBuffer();

            setRecentFile();

            return true;
        }

        private bool saveFile()
        {
            if (WorkingWithFile == false)
            {
                saveFileDlg.FileName = FilePath;
                if (saveFileDlg.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                {
                    FileName = System.IO.Path.GetFileName(saveFileDlg.FileName);
                    FilePath = saveFileDlg.FileName;
                }
                else
                {
                    return false;
                }
            }

            if (FilePath != null && FilePath != "")
            {
                System.IO.StreamWriter sw = new System.IO.StreamWriter(FilePath, false, Encoding.ASCII);
                sw.Write(SciEditor.Text);
                sw.Flush();
                sw.Close();
                sw.Dispose();

                SciEditor.Modified = false;
                WorkingWithFile = true;

                Text = FileName;
                setRecentFile();
            }
            else
            {
                return false;
            }

            return true;
        }

        private void setRecentFile()
        {
            recentFiles.Remove(FilePath);
            recentFiles.Add(FilePath);

            if (recentFiles.Count > 5)
            {
                recentFiles.RemoveRange(0, recentFiles.Count - 5);
            }
            UpdateRecentFiles();
        }

        #region Tool Strip Events

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            saveFile();
        }

        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void undoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SciEditor.UndoRedo.Undo();

        }

        private void redoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SciEditor.UndoRedo.Redo();
        }

        private void selectAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SciEditor.Selection.SelectAll();
        }

        private void findToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SciEditor.FindReplace.ShowFind();
        }

        private void goToToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SciEditor.GoTo.ShowGoToDialog();
        }

        private void findAndReplaceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SciEditor.FindReplace.ShowReplace();
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
            if (askToSaveModifiedDocument() != System.Windows.Forms.DialogResult.Cancel)
            {
                SciEditor.Text = "";
                SciEditor.UndoRedo.EmptyUndoBuffer();
                FileName = "Untitled" + titleSuffix;
                FilePath = "";

                Text = FileName;

                SciEditor.Modified = false;
                WorkingWithFile = false;
            }
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
            WorkingWithFile = false;
            if (saveFile() == false)
            {
                WorkingWithFile = true;
            }
        }

        private void setGlyphRangeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            glyphRangeForm glyphRange = new glyphRangeForm();
            if (glyphRange.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                glyphTable.GlyphRange = glyphRange.characterRange;
            }
        }

        private void RecentFileMenuItem_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem recentItem = (ToolStripMenuItem)sender;
            if (askToSaveModifiedDocument() != System.Windows.Forms.DialogResult.Cancel)
            {
                openFile(recentItem.Text);
            }
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (askToSaveModifiedDocument() != System.Windows.Forms.DialogResult.Cancel)
            {
                openFile(null);
            }
        }

        private void copyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SciEditor.NativeInterface.Copy();
        }

        private void cutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SciEditor.NativeInterface.Cut();
        }

        private void pasteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SciEditor.NativeInterface.Paste();
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SciEditor.NativeInterface.DeleteBack();
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

        private void SciEditor_CharAdded(object sender, ScintillaNet.CharAddedEventArgs e)
        {
            curWord = SciEditor.GetWordFromPosition(SciEditor.Selection.Start);
            if (curWord.Length == 0)
            {
                return;
            }
            Predicate<string> startWord = compareWithCurrentWord;
            List<string> list = autoCompleteList.FindAll(startWord);

            if (list.Count > 0)
            {
                SciEditor.AutoComplete.Show(curWord.Length, string.Join(SciEditor.AutoComplete.ListSeparator.ToString(), list.ToArray()));
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
            Properties.Settings.Default.forceDefaultEditor = forceAsDefaultEditorToolStripMenuItem.Checked;
            if (forceAsDefaultEditorToolStripMenuItem.Checked)
            {
                makeDefaultEditor();
                defaultEditorToolStripMenuItem.Checked = true;
            }
        }

        private void changeStylesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (frmColor.Visible == false)
            {
                frmColor = new ConfigStyles(SciEditor, lex.GetStyleNameIndex());
                frmColor.Show();
            }
            else
            {
                frmColor.BringToFront();
            }
        }

        private void compileAndSaveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (SciEditor.Modified)
            {
                if (askToSaveModifiedDocument() != System.Windows.Forms.DialogResult.OK)
                {
                    return;
                }
            }
            if (WorkingWithFile == false)
            {
                return;
            }

            SaveFileDialog saveFileDlg = new SaveFileDialog();
            saveFileDlg.AddExtension = true;
            saveFileDlg.DefaultExt = "km2";
            saveFileDlg.Filter = "KeyMagic Layout File|*km2";

            if (saveFileDlg.ShowDialog() != System.Windows.Forms.DialogResult.OK) return;

            string saveFileName = saveFileDlg.FileName;
            CallParser(FilePath, saveFileName);
        }

        private bool CallParser(string FileIn, string FileOut)
        {
            bool ret = true;
            try
            {
                if (System.IO.File.Exists(thisDir + "\\parser.exe") == false)
                {
                    MessageBox.Show(this, "Parser program not found! You could download at http://keymagic.googlecode.com/", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
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
                    MessageBox.Show(this, errText, "Failed", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    ret = false;
                }
                else
                {
                    string[] splitted = outText.Split('\n');
                    string lastLine = splitted[splitted.Length - 2];
                    MessageBox.Show(this, lastLine + "~~\n" + errText, "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
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
            if (SciEditor.Modified)
            {
                if (askToSaveModifiedDocument() != System.Windows.Forms.DialogResult.OK)
                {
                    return;
                }
            }
            if (WorkingWithFile == false)
            {
                return;
            }

            CallParser(FilePath, "");
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            aboutForm aboutF = new aboutForm();
            aboutF.ShowDialog(this);
        }

        private void SciEditor_ModifiedChanged(object sender, EventArgs e)
        {
            if (WorkingWithFile)
            {
                if (SciEditor.Modified)
                {
                    this.Text = string.Format("*{0}", FileName);
                }
                else 
                {
                    this.Text = FileName;
                }
            }
        }
    }
}
