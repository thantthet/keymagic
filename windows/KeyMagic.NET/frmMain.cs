using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Resources;
using System.Globalization;
using System.Runtime.InteropServices;
using System.IO;
using System.Diagnostics;
using System.Drawing.Drawing2D;

namespace KeyMagic
{
    public partial class frmMain : Form
    {
        private bool AddKeyboardLayoutToListWithFile(string fileName)
        {
            InfoCollection infoCollection = new InfoCollection(fileName);
            string fileTitle = Path.GetFileNameWithoutExtension(fileName);
            ListViewItem lvItem = new ListViewItem(fileTitle);
            lvItem.Group = lvLayouts.Groups["Enabled"];

            Bitmap icon = infoCollection.GetIcon();
            if (icon != null)
            {
                imageList.Images.Add(fileTitle, icon);
                lvItem.ImageKey = fileTitle;
            }
            else
            {
                lvItem.ImageKey = "DefaultIcon";
            }

            String name = infoCollection.GetName();
            if (name != null)
            {
                lvItem.SubItems.Add(name);
            }
            else
            {
                lvItem.SubItems.Add(fileTitle);
            }

            Hotkey hotkey = infoCollection.GetHotkey();
            if (hotkey != null)
            {
                lvItem.SubItems.Add(hotkey.ToString());
            }
            else
            {
                lvItem.SubItems.Add("");
            }

            String desc = infoCollection.GetDescription();
            if (desc != null)
            {
                lvItem.SubItems.Add(desc);
            }
            else
            {
                lvItem.SubItems.Add("");
            }

            KToolStripMenuItem menuItem = new KToolStripMenuItem(lvItem.Text);
            menuItem.ImageKey = lvItem.ImageKey;
            menuItem.Click += new EventHandler(cmsLeftMenuItem_Click);

            try
            {
                File.Copy(fileName, GetSaveKeyboardPath(fileTitle));
                lvLayouts.Items.Add(lvItem);
                cmsLeft.Items.Add(menuItem);
            }
            catch (UnauthorizedAccessException ex)
            {
                AskToRunAsAdministrator("Access is denied and failed to copy keyboard layout file. Do you want to run KeyMagic as administrator?");
                return false;
            }
            catch (Exception ex)
            {
                MessageBox.Show(this, ex.Message, "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            return true;
        }

        private void AskToRunAsAdministrator(string msg)
        {
            string failedMessage = "Failed to run KeyMagic.";
            if (MessageBox.Show(this, msg, "Error!", MessageBoxButtons.YesNo, MessageBoxIcon.Error) == DialogResult.Yes)
            {
                try
                {
                    ProcessStartInfo psi = new ProcessStartInfo(Environment.GetCommandLineArgs()[0]);
                    psi.Verb = "runas";
                    Process p = Process.Start(psi);
                    if (p == null)
                    {
                        MessageBox.Show(this, failedMessage, "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return;
                    }
                    Dispose();
                    Application.Exit();
                }
                catch (Exception ex)
                {
                    MessageBox.Show(this, failedMessage, "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private string GetSaveKeyboardPath(string fileTitle)
        {
            return Path.Combine(keyboardDir, fileTitle + ".km2");
        }

        Icon mainIcon;
        bool W64bit;
        string keyboardDir = "KeyboardLayouts";

        #region Main Form Events

        public frmMain()
        {
            if (IntPtr.Size == 4)
            {
                W64bit = false;
            }
            else if (IntPtr.Size == 8)
            {
                W64bit = true;
            }

            InitializeComponent();

            try
            {
                //Create Keyboard Dir If needed
                Directory.CreateDirectory(keyboardDir);
            }
            catch (Exception ex)
            {
            }
            //Copy Image List
            cmsLeft.ImageList = imageList;

            //Load default Icon
            mainIcon = new Icon(typeof(frmMain), "KeyMagic.ico");
            imageList.Images.Add("DefaultIcon", mainIcon);
            nIcon.Icon = mainIcon;

            //Create Turn Off Menu Item
            KToolStripMenuItem menuItem = new KToolStripMenuItem("Turn Off");
            menuItem.Checked = true;
            menuItem.Click += new EventHandler(cmsLeftMenuItem_Click);
            cmsLeft.Items.Add(menuItem);

            //Load Keyboard Layouts
            LoadKeyboardLayoutList();

            ResetDwmBlurBehind();
            m_glassMargins = new DwmApi.MARGINS(5, 28, 5, 5);

            if (DwmApi.DwmIsCompositionEnabled()) DwmApi.DwmExtendFrameIntoClientArea(this.Handle, m_glassMargins);
        }

        DwmApi.MARGINS m_glassMargins;

        private void ResetDwmBlurBehind()
        {
            if (DwmApi.DwmIsCompositionEnabled())
            {
                DwmApi.DWM_BLURBEHIND bbhOff = new DwmApi.DWM_BLURBEHIND();
                bbhOff.dwFlags = DwmApi.DWM_BLURBEHIND.DWM_BB_ENABLE | DwmApi.DWM_BLURBEHIND.DWM_BB_BLURREGION;
                bbhOff.fEnable = false;
                bbhOff.hRegionBlur = IntPtr.Zero;
                DwmApi.DwmEnableBlurBehindWindow(this.Handle, bbhOff);
            }
        }

        private void frmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            e.Cancel = true;
            this.Hide();
        }

        #endregion

        void cmsLeftMenuItem_Click(object sender, EventArgs e)
        {
            foreach (ToolStripMenuItem item in cmsLeft.Items)
            {
                item.Checked = false;
            }
            KToolStripMenuItem thisItem = sender as KToolStripMenuItem;
            thisItem.Checked = true;
        }

        #region Manipulating ListItem Buttons

        private void btnAdd_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.CheckFileExists = true;
            ofd.Filter = "KeyMagic Keyboard Layout (*.km2)|*.km2|All files (*.*)|*.*";
            ofd.Multiselect = true;
            if (ofd.ShowDialog(this) == DialogResult.OK)
            {
                foreach (string FileName in ofd.FileNames)
                {
                    if (AddKeyboardLayoutToListWithFile(FileName) == false)
                    {
                        return;
                    }
                }
                SaveKeyboardLayoutList();
            }
        }

        private void btnEdit_Click(object sender, EventArgs e)
        {
            EditSelections();
        }

        private void btnRemove_Click(object sender, EventArgs e)
        {
            if (lvLayouts.SelectedItems.Count > 0)
            {
                if (MessageBox.Show(this, "Keyboard layout file(s) will also deleted and cannot be undone. Do you want to continue?", "Warning!",
                    MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation) == DialogResult.Yes)
                {
                    foreach (ListViewItem item in lvLayouts.SelectedItems)
                    {
                        try
                        {
                            File.Delete(GetSaveKeyboardPath(item.Text));
                            lvLayouts.Items.Remove(item);
                        }
                        catch (Exception ex)
                        {
                            Console.WriteLine(ex.Message);
                        }
                    }
                    SaveKeyboardLayoutList();
                }
            }
        }

        #endregion

        #region Loading/Saving Keyboard Layouts

        private void LoadKeyboardLayoutList()
        {
            LoadKeyboardLayoutList(true);
        }

        private void LoadKeyboardLayoutList(bool removeOld)
        {
            if (removeOld)
            {
                lvLayouts.Items.Clear();

                ToolStripItem defItem = cmsLeft.Items[0];
                cmsLeft.Items.Clear();
                cmsLeft.Items.Add(defItem);
            }

            KeyboardLayoutList keyboardList = new KeyboardLayoutList();

            try
            {
                FileStream fs = new FileStream("Layouts.xml", FileMode.Open, FileAccess.Read);
                System.Xml.XmlReader reader = System.Xml.XmlReader.Create(fs);

                reader.MoveToContent();
                if (reader.LocalName == "Layouts")
                {
                    reader.ReadStartElement("Layouts");
                    keyboardList.ReadXml(reader);
                    reader.ReadEndElement();
                }

                reader.Close();
                fs.Close();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

            foreach (KeyboardLayout layout in keyboardList)
            {
                InfoCollection infos = new InfoCollection(GetSaveKeyboardPath(layout.file));
                ListViewItem lvItem = new ListViewItem(layout.file);
                lvItem.SubItems.Add(layout.display);
                lvItem.SubItems.Add(layout.hotkey);
                lvItem.SubItems.Add(infos.GetDescription());
                Bitmap icon = infos.GetIcon();
                if (icon != null)
                {
                    imageList.Images.Add(layout.file, icon);
                    lvItem.ImageKey = layout.file;
                }
                if (layout.enable)
                {
                    lvItem.Group = lvLayouts.Groups["Enabled"];
                    KToolStripMenuItem menuItem = new KToolStripMenuItem(layout.display);
                    menuItem.ImageKey = lvItem.ImageKey;
                    menuItem.Click += new EventHandler(cmsLeftMenuItem_Click);
                    cmsLeft.Items.Add(menuItem);
                }
                else
                {
                    lvItem.Group = lvLayouts.Groups["Disabled"];
                }
                lvLayouts.Items.Add(lvItem);
            }
        }

        private void SaveKeyboardLayoutList()
        {
            int idxFilename = lvLayouts.Columns.IndexOf(colFileName);
            int idxDisplay = lvLayouts.Columns.IndexOf(colDisplayText);
            int idxHotkey = lvLayouts.Columns.IndexOf(colHotkey);

            KeyboardLayoutList keyboardList = new KeyboardLayoutList();
            foreach (ListViewItem lvi in lvLayouts.Items)
            {
                keyboardList.Add(new KeyboardLayout(
                    lvi.SubItems[idxFilename].Text,
                    lvi.SubItems[idxDisplay].Text,
                    lvi.SubItems[idxHotkey].Text,
                    lvi.Group.Name == "Enabled"));
            }

            try
            {
                FileStream fs = new FileStream("Layouts.xml", FileMode.Create);

                System.Xml.XmlWriterSettings settings = new System.Xml.XmlWriterSettings();
                settings.Indent = true;
                settings.IndentChars = ("    ");
                System.Xml.XmlWriter writer = System.Xml.XmlWriter.Create(fs, settings);

                writer.WriteStartElement("Layouts");
                keyboardList.WriteXml(writer);
                writer.WriteEndElement();
                writer.Close();

                fs.Close();
            }
            catch (UnauthorizedAccessException ex)
            {
                AskToRunAsAdministrator("Access is denied and failed to save keyboard layouts list. Do you want to run KeyMagic as administrator?");
            }
            catch (Exception ex)
            {
            }
        }

        #endregion

        #region DialogButtons

        private void btnOK_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        private void btnExit_Click(object sender, EventArgs e)
        {
            this.Dispose();
        }

        #endregion

        #region Right-click notify menu

        private void showToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Show();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Dispose();
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AboutKeyMagic about = new AboutKeyMagic();
            about.ShowDialog(this);
        }

        #endregion

        private void EditSelections()
        {
            int idxDisplay = lvLayouts.Columns.IndexOf(colDisplayText);
            int idxHotkey = lvLayouts.Columns.IndexOf(colHotkey);

            if (lvLayouts.SelectedItems.Count == 0)
            {
                return;
            }

            ListViewItem lvItem = lvLayouts.SelectedItems[0];
            string name = lvItem.Text;
            string display = lvItem.SubItems[idxDisplay].Text;
            string hotkey = lvItem.SubItems[idxHotkey].Text;

            EditorDialog edlg = new EditorDialog(display, hotkey, lvItem.Group.Name == "Enabled");
            edlg.Text = "Editing: " + name;
            if (edlg.ShowDialog(this) == DialogResult.OK)
            {
                lvItem.SubItems[idxDisplay].Text = edlg.txtDisplay.Text;
                lvItem.SubItems[idxHotkey].Text = edlg.hkHotkey.Hotkey.ToString();
                if (edlg.chkEnable.Checked)
                {
                    lvItem.Group = lvLayouts.Groups["Enabled"];
                }
                else
                {
                    lvItem.Group = lvLayouts.Groups["Disabled"];
                }

                SaveKeyboardLayoutList();
                LoadKeyboardLayoutList();
            }
        }

        private void lvLayouts_DoubleClick(object sender, EventArgs e)
        {
            EditSelections();
        }

        #region NotifyIcon Events

        private void nIcon_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                nIcon.ContextMenuStrip = cmsLeft;

                System.Reflection.MethodInfo mi = typeof(NotifyIcon).GetMethod("ShowContextMenu", System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.NonPublic);
                mi.Invoke(nIcon, null);

                nIcon.ContextMenuStrip = cmsRight;
            }
        }

        private void nIcon_DoubleClick(object sender, EventArgs e)
        {
            this.TopMost = true;
            this.Show();
            this.Focus();
            this.TopMost = false;
        }

        #endregion

        private void frmMain_Paint(object sender, PaintEventArgs e)
        {
            if (DwmApi.DwmIsCompositionEnabled())
            {
                //e.Graphics.FillRectangle(Brushes.Black, Rectangle.FromLTRB(0, 0, this.ClientRectangle.Width, m_glassMargins.cyTopHeight));
                e.Graphics.FillRectangle(Brushes.Black, this.ClientRectangle);
            }
        }

        private void tabControl_DrawItem(object sender, DrawItemEventArgs e)
        {
            Graphics g = e.Graphics;

            g.FillRectangle(Brushes.Green, e.Bounds);

            g.DrawString(tabControl.TabPages[e.Index].Text, e.Font, Brushes.Black, e.Bounds);
        }

        // This is a trick to get Windows to treat glass as part of the caption
        // area I learned from Daniel Moth.
        protected override void WndProc(ref Message msg)
        {
            base.WndProc(ref msg); // let the normal winproc process it

            const int WM_DWMCOMPOSITIONCHANGED = 0x031E;
            const int WM_NCHITTEST = 0x84;
            const int HTCLIENT = 0x01;

            switch (msg.Msg)
            {
                case WM_NCHITTEST:
                    if (HTCLIENT == msg.Result.ToInt32())
                    {
                        // it's inside the client area

                        // Parse the WM_NCHITTEST message parameters
                        // get the mouse pointer coordinates (in screen coordinates)
                        Point p = new Point();
                        p.X = (msg.LParam.ToInt32() & 0xFFFF);// low order word
                        p.Y = (msg.LParam.ToInt32() >> 16); // hi order word

                        // convert screen coordinates to client area coordinates
                        p = PointToClient(p);

                        // if it's on glass, then convert it from an HTCLIENT
                        // message to an HTCAPTION message and let Windows handle it from then on
                        if (PointIsOnGlass(p))
                            msg.Result = new IntPtr(2);
                    }
                    break;

                case WM_DWMCOMPOSITIONCHANGED:
                    m_glassMargins = null;
                    break;
            }
        }

        private bool PointIsOnGlass(Point p)
        {
            // test for region or entire client area
            // or if upper window glass and inside it.
            // not perfect, but you get the idea
            return m_glassMargins != null &&
                (m_glassMargins.cyTopHeight <= 0 ||
                 m_glassMargins.cyTopHeight > p.Y);
        }
    }
}
