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
using System.Reflection;
using System.Security.Principal;
using Microsoft.Win32.TaskScheduler;
using System.Net;
using Microsoft.Win32;

namespace KeyMagic
{
    public partial class frmMain : Form
    {
        enum DLLMSG : int
        {
            KM_SETKEYBOARD = 0x5401,
            KM_LOSTFOCUS = 0x5402,
            KM_GOTFOCUS = 0x5403,
            KM_LISTCHANGED = 0x5404,
            KM_ERROR = 0x5405,
            KM_GETKBLNAME = 0x5406
        }

        //store additional icon for tray, this looks better for images with alpha when drawn
        Dictionary<String, Icon> iconList = new Dictionary<string, Icon>();
        DwmApi.MARGINS m_glassMargins;

        string keyboardDir = "KeyboardLayouts";

        Icon mainIcon;

        bool isAdmin = false;
        bool is64bit = false;
        bool isVistaOrLater = false;

        //IntPtr DllPtrFileToLoad = IntPtr.Zero;
        //IntPtr DllPtrHotkeys = IntPtr.Zero;
        IntPtr LastClientHandle = IntPtr.Zero;

        #region Main Form Events

        public frmMain()
        {
            InitializeComponent();
        }

        private void frmMain_Load(object sender, EventArgs e)
        {
            ProcessCommandLineArgs();
            InitializeTrayIcon();
            InitializeKeyboardLayoutMenu();
            InitializeMisc();
            InitializeAeroGlass();
            InitializeHook();
            InitializeLogonRun();

            CreateKeyboardDirectory();
            LoadKeyboardLayoutList();
        }

        private void ProcessCommandLineArgs()
        {
            string[] args = Environment.GetCommandLineArgs();
            foreach (string arg in args)
            {
                switch (arg)
                {
                    case "-RegRun":
                        KeyMagic.Properties.Settings.Default.RunAtStartup = true;
                        break;
                    case "-UnRegRun":
                        KeyMagic.Properties.Settings.Default.RunAtStartup = false;
                        break;
                }
            }
        }

        private void frmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            KeyMagic.Properties.Settings.Default.Save();
            if (e.CloseReason == CloseReason.UserClosing)
            {
                e.Cancel = true;
                this.Hide();
            }
        }

        private void frmMain_FormClosed(object sender, FormClosedEventArgs e)
        {
            //UnHookWindowsHooks();
            SaveColumnWidths();
            KeyMagic.Properties.Settings.Default.Save();

            processX64.Kill();
            processX32.Kill();
        }

        private void SaveColumnWidths()
        {
            KeyMagic.Properties.Settings.Default.colFileNameWidth = lvLayouts.Columns[0].Width;
            KeyMagic.Properties.Settings.Default.colDisplayWidth = lvLayouts.Columns[1].Width;
            KeyMagic.Properties.Settings.Default.colHotkeyWidth = lvLayouts.Columns[2].Width;
            KeyMagic.Properties.Settings.Default.colDescWidth = lvLayouts.Columns[3].Width;
        }

        private void frmMain_Paint(object sender, PaintEventArgs e)
        {
            if (DwmApi.DwmIsCompositionEnabled())
            {
                //e.Graphics.FillRectangle(Brushes.Black, Rectangle.FromLTRB(0, 0, this.ClientRectangle.Width, m_glassMargins.cyTopHeight));
                e.Graphics.FillRectangle(Brushes.Black, this.ClientRectangle);
            }
        }

        #endregion

        #region LogonRun
        private void InitializeLogonRun()
        {
            SetLogonRun(KeyMagic.Properties.Settings.Default.RunAtStartup);
            Properties.Settings.Default.RunAtStartup = GetOnLogonRun();
        }

        private void SetLogonRun(bool reg)
        {
            if (reg)
            {
                if (isVistaOrLater)
                {
                    RegisterTask();
                }
                else
                {
                    RegisterRegRun();
                }
            }
            else
            {
                if (isVistaOrLater)
                {
                    UnregisterTask();
                }
                else
                {
                    UnregisterRegRun();
                }
            }
        }

        private bool GetOnLogonRun()
        {
            if (isVistaOrLater)
            {
                if (isTaskRegistered())
                {
                    return true;
                }
            }
            else
            {
                if (isRunRegRegistered())
                {
                    return true;
                }
            }
            return false;
        }

        private void UnregisterRegRun()
        {
            if (isRunRegRegistered() == false)
            {
                return;
            }

            try
            {
                RegistryKey runKey = Registry.LocalMachine.OpenSubKey(@"SOFTWARE\Microsoft\Windows\CurrentVersion\Run", false);
                runKey.DeleteValue("KeyMagic");
            }
            catch (UnauthorizedAccessException)
            {
                AskToRunAsAdministrator("Access is denied when setting KeyMagic to be run on startup! Do you want to restart KeyMagic as Administrator?", "-UnRegRun");
            }
        }

        private void UnregisterTask()
        {
            if (isTaskRegistered() == false)
            {
                return;
            }

            using (TaskService ts = new TaskService())
            {
                try
                {
                    ts.RootFolder.DeleteTask("KeyMagic");
                }
                catch (UnauthorizedAccessException)
                {
                    AskToRunAsAdministrator("Access is denied when setting KeyMagic to be run on startup! Do you want to restart KeyMagic as Administrator?", "-UnRegRun");
                }
            }
        }

        private void RegisterRegRun()
        {
            if (isRunRegRegistered())
            {
                return;
            }

            try
            {
                Registry.SetValue(@"HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run",
                    "KeyMagic", string.Format("{0} {1}", Assembly.GetExecutingAssembly().Location, "--run"), RegistryValueKind.String);
            }
            catch (UnauthorizedAccessException)
            {
                AskToRunAsAdministrator("Access is denied when setting KeyMagic to be run on startup! Do you want to restart KeyMagic as Administrator?", "-RegRun");
            }
        }

        private bool isRunRegRegistered()
        {
            string path = (string)Registry.GetValue(@"HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run", "KeyMagic", "");
            if (string.IsNullOrEmpty(path))
            {
                return false;
            }
            return true;
        }

        private void RegisterTask()
        {
            if (isTaskRegistered())
            {
                return;
            }

            // Get the service on the local machine
            using (TaskService ts = new TaskService())
            {
                try
                {
                    // Create a new task definition and assign properties
                    TaskDefinition td = ts.NewTask();
                    td.Principal.RunLevel = TaskRunLevel.Highest;
                    td.RegistrationInfo.Description = "Run KeyMagic on startup";

                    // Create a trigger that will fire the task at this time every other day
                    td.Triggers.Add(new LogonTrigger());

                    // Create an action that will launch Notepad whenever the trigger fires
                    td.Actions.Add(new ExecAction(Assembly.GetExecutingAssembly().Location, "--task", null));

                    // Register the task in the root folder
                    ts.RootFolder.RegisterTaskDefinition(@"KeyMagic", td);

                    // Remove the task we just created
                    // ts.RootFolder.DeleteTask("KeyMagic");
                }
                catch (UnauthorizedAccessException)
                {
                    AskToRunAsAdministrator("Access is denied when setting KeyMagic to be run on startup! Do you want to restart KeyMagic as Administrator?", "-RegRun");
                }
            }
        }

        private bool isTaskRegistered()
        {
            try
            {
                using (TaskService ts = new TaskService())
                {
                    Task task = ts.RootFolder.Tasks["KeyMagic"];
                }
            }
            catch (DirectoryNotFoundException)
            {
                return false;
            }
            return true;
        }
        #endregion

        #region Initialization Functions

        private void InitializeMisc()
        {
            ActiveKeyboardList.Add(DefaultLayout);

            using (AboutKeyMagic aboutDlg = new AboutKeyMagic())
            {
                lblAboutTitle.Text = String.Format("About {0} {1}", aboutDlg.AssemblyTitle, aboutDlg.AssemblyVersion);
            }

            if (IntPtr.Size == 4)
            {
                is64bit = false;
            }
            else if (IntPtr.Size == 8)
            {
                is64bit = true;
            }

            if (Environment.OSVersion.Version.Major >= 6)
            {
                isVistaOrLater = true;
            }

            isAdmin = IsUserAdministrator();
        }

        private void InitializeTrayIcon()
        {
            //Load default Icon
            mainIcon = new Icon(typeof(frmMain), "KeyMagic.ico");
            imageList.Images.Add("DefaultIcon", mainIcon);
            nIcon.Icon = mainIcon;
        }

        private void InitializeKeyboardLayoutMenu()
        {
            //Copy Image List
            cmsLeft.ImageList = imageList;

            //Create Turn Off Menu Item
            KToolStripMenuItem menuItem = new KToolStripMenuItem("Turn Off");
            menuItem.Checked = true;
            menuItem.Click += new EventHandler(cmsLeftMenuItem_Click);
            cmsLeft.Items.Add(menuItem);
        }

        private void CreateKeyboardDirectory()
        {
            try
            {
                //Create Keyboard Dir If needed
                Directory.CreateDirectory(keyboardDir);
            }
            catch (Exception)
            {

            }
        }

        #endregion

        #region Hook/Messaging with hook dll

        Process processX64;
        Process processX32;

        private void InitializeHook()
        {
            try
            {
                //IntPtr hHook;
                //IntPtr hModule;

                if (is64bit)
                {
                    //hHook = IntPtr.Zero;
                    //hModule = LoadLibrary("InputProcessor.x64.dll");
                    //RaiseWin32Exception(hModule != IntPtr.Zero);
                    //DllPtrFileToLoad = GetProcAddress(hModule, "fileNameToLoad");
                    //RaiseWin32Exception(DllPtrFileToLoad != IntPtr.Zero);
                    //DllPtrHotkeys = GetProcAddress(hModule, "Hotkeys");
                    //RaiseWin32Exception(DllPtrHotkeys != IntPtr.Zero);

                    //SetMainWindowsHandle64(Handle);
                    ProcessStartInfo psi;
                    
                    psi = new ProcessStartInfo("HookInput.x64.exe", Handle.ToString("X"));
                    //psi.WindowStyle = ProcessWindowStyle.Hidden;
                    processX64 = Process.Start(psi);

                    psi = new ProcessStartInfo("HookInput.x32.exe", Handle.ToString("X"));
                    //psi.WindowStyle = ProcessWindowStyle.Hidden;
                    processX32 = Process.Start(psi);

                    //SetMainDir64(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location));
                    //SetWindowsHooks64(hModule);
                    //RaiseWin32Exception(GetKeyProcHook64() != IntPtr.Zero);
                    //RaiseWin32Exception(GetWndProcHook64() != IntPtr.Zero);
                    //RaiseWin32Exception(GetGetMsgProcHook64() != IntPtr.Zero);
                }
            }
            catch (Win32Exception ex)
            {
                MessageBox.Show(this, ex.Message + "! Application will now exit.", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                Dispose();
            }
            catch (Exception)
            {
            }
        }

        //int GetKeyboardLayoutNameByIndex(uint index, IntPtr receiver, uint size)
        //{
        //    if (index > lvLayouts.Items.Count)
        //    {
        //        return -2;
        //    }

        //    String fileName = lvLayouts.Items[(int)index].Text;

        //    if (size < fileName.Length)
        //    {
        //        return -1;
        //    }

        //    UnicodeEncoding enc = new UnicodeEncoding();
        //    Marshal.Copy(enc.GetBytes(fileName), 0, receiver, enc.GetByteCount(fileName));
        //    Marshal.WriteByte(fileToLoad, enc.GetByteCount(fileName), 0);

        //    return fileName.Length;
        //}

        //public int GetHotKeys(uint count, IntPtr _hotkeys)
        //{
        //    if (count == 0)
        //    {
        //        return lvLayouts.Items.Count;
        //    }
        //    else if (count < lvLayouts.Items.Count)
        //    {
        //        return -1;
        //    }

        //    int hotkeysPtr = (int)_hotkeys;
        //    int colHotkeyIndex = lvLayouts.Columns.IndexOf(colHotkey);
        //    Hotkeys hotkeys = new Hotkeys();
        //    for (int i = 0; i < lvLayouts.Items.Count; i++)
        //    {
        //        hotkeys.Index = (uint)i;
        //        hotkeys.Hotkey = new Hotkey(lvLayouts.Items[i].SubItems[colHotkeyIndex].Text).ToInt();
        //        Marshal.StructureToPtr(hotkeys, (IntPtr)hotkeysPtr, false);
        //        hotkeysPtr += Marshal.SizeOf(hotkeys);
        //    }
        //    return lvLayouts.Items.Count;
        //}

        //private void UnHookWindowsHooks()
        //{
        //    RaiseWin32Exception(UnhookWindowsHookEx(GetKeyProcHook64()));
        //    RaiseWin32Exception(UnhookWindowsHookEx(GetWndProcHook64()));
        //    RaiseWin32Exception(UnhookWindowsHookEx(GetGetMsgProcHook64()));
        //}

        private void SetFileToLoad(string fileName)
        {
            UnicodeEncoding enc = new UnicodeEncoding();
            int count = enc.GetByteCount(fileName);
            IntPtr buffer = Marshal.AllocHGlobal(count);
            Marshal.Copy(enc.GetBytes(fileName), 0, buffer, count);

            NavtiveMethods.COPYDATASTRUCT copyData = new NavtiveMethods.COPYDATASTRUCT();
            copyData.dwData = 0xB0B0;
            copyData.lpData = buffer;
            copyData.cbData = count;
            
            IntPtr copyDataBuff = NavtiveMethods.IntPtrAlloc(copyData);
            
            NavtiveMethods.SendMessage(LastClientHandle, (uint)NavtiveMethods.WM.COPYDATA, IntPtr.Zero, copyDataBuff);
            
            NavtiveMethods.IntPtrFree(copyDataBuff);
            copyDataBuff = IntPtr.Zero;

            NavtiveMethods.IntPtrFree(buffer);
            buffer = IntPtr.Zero;
            //UnicodeEncoding enc = new UnicodeEncoding();
            //Marshal.Copy(enc.GetBytes(fileName), 0, DllPtrFileToLoad, enc.GetByteCount(fileName));
            //Marshal.WriteByte(DllPtrFileToLoad, enc.GetByteCount(fileName), 0);
        }

        #endregion

        #region Edit/Delete/Add Keyboard Layout

        private bool AddKeyboardLayoutToListWithFile(string fileName)
        {
            try
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
                    //cmsLeft.Items.Add(menuItem);
                }
                catch (UnauthorizedAccessException ex)
                {
                    if (!isAdmin)
                    {
                        AskToRunAsAdministrator("Access is denied and failed to copy keyboard layout file. Do you want to run KeyMagic as administrator?");
                    }
                    else
                    {
                        MessageBox.Show(this, ex.Message, "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                    return false;
                }
                catch (Exception ex)
                {
                    MessageBox.Show(this, ex.Message, "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show(this, ex.Message, "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            return true;
        }

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
                    AddKeyboardLayoutToListWithFile(FileName);
                }
                SaveKeyboardLayoutList();
                LoadKeyboardLayoutList();
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
                    LoadKeyboardLayoutList();
                }
            }
        }

        private void EditSelections()
        {
            int idxDisplay = lvLayouts.Columns.IndexOf(colDisplayText);
            int idxHotkey = lvLayouts.Columns.IndexOf(colHotkey);
            int idxDesc = lvLayouts.Columns.IndexOf(colDesc);

            if (lvLayouts.SelectedItems.Count == 0)
            {
                return;
            }

            ListViewItem lvItem = lvLayouts.SelectedItems[0];
            string name = lvItem.Text;
            string display = lvItem.SubItems[idxDisplay].Text;
            string hotkey = lvItem.SubItems[idxHotkey].Text;

            EditorDialog edlg = new EditorDialog(display, hotkey, lvItem.Group.Name == "Enabled");
            edlg.txtDesc.Text = lvItem.SubItems[idxDesc].Text;
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

        #endregion

        #region Loading/Saving Keyboard Layouts

        KeyboardLayoutList ActiveKeyboardList = new KeyboardLayoutList();
        KeyboardLayout DefaultLayout = new KeyboardLayout("", "", "", true);

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
                ActiveKeyboardList.Clear();
                ActiveKeyboardList.Add(DefaultLayout);
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

            //long lPtr = DllPtrHotkeys.ToInt64();

            foreach (KeyboardLayout layout in keyboardList)
            {
                InfoCollection infos = new InfoCollection(GetSaveKeyboardPath(layout.file));
                ListViewItem lvItem = new ListViewItem(layout.file);
                lvItem.SubItems.Add(layout.display);
                lvItem.SubItems.Add(layout.hotkey);
                lvItem.SubItems.Add(infos.GetDescription());
                using (Bitmap bmIcon = infos.GetIcon())
                {
                    if (bmIcon != null)
                    {
                        imageList.Images.Add(layout.file, bmIcon);
                        lvItem.ImageKey = layout.file;
                    }
                    if (layout.enable)
                    {
                        ActiveKeyboardList.Add(layout);

                        lvItem.Group = lvLayouts.Groups["Enabled"];
                        KToolStripMenuItem menuItem = new KToolStripMenuItem(layout.display);
                        if (bmIcon != null)
                            using (Icon icon = Icon.FromHandle(bmIcon.GetHicon()))
                            {
                                menuItem.Icon = icon;
                            }
                        menuItem.ImageKey = lvItem.ImageKey;
                        menuItem.ShortcutKeyDisplayString = layout.hotkey;
                        menuItem.Click += new EventHandler(cmsLeftMenuItem_Click);
                        cmsLeft.Items.Add(menuItem);

                        iconList[layout.file] = Icon.FromHandle(bmIcon.GetHicon());
                    }
                    else
                    {
                        lvItem.Group = lvLayouts.Groups["Disabled"];
                    }
                }
                lvLayouts.Items.Add(lvItem);
            }

            SendHotKeys();

            NavtiveMethods.PostMessage(new IntPtr(0xffff), (uint)DLLMSG.KM_LISTCHANGED, IntPtr.Zero, IntPtr.Zero);
        }

        private void SendHotKeys()
        {
            IntPtr HotKeys = Marshal.AllocHGlobal(sizeof(short) * ActiveKeyboardList.Count);
            int HotKeysPtr = HotKeys.ToInt32();
            foreach (KeyboardLayout kbLayout in ActiveKeyboardList)
            {
                Marshal.WriteInt16(new IntPtr(HotKeysPtr), (short)new Hotkey(kbLayout.hotkey).ToInt());
                HotKeysPtr += sizeof(short);
            }

            NavtiveMethods.COPYDATASTRUCT copyData = new NavtiveMethods.COPYDATASTRUCT();
            copyData.cbData = sizeof(short) * ActiveKeyboardList.Count;
            copyData.dwData = 0xBEEF;
            copyData.lpData = HotKeys;

            IntPtr copyDataBuff = NavtiveMethods.IntPtrAlloc(copyData);

            NavtiveMethods.SendMessage(LastClientHandle, (uint)NavtiveMethods.WM.COPYDATA, IntPtr.Zero, copyDataBuff);

            NavtiveMethods.IntPtrFree(copyDataBuff);
            copyDataBuff = IntPtr.Zero;

            NavtiveMethods.IntPtrFree(HotKeys);
            HotKeys = IntPtr.Zero;
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
            catch (UnauthorizedAccessException)
            {
                AskToRunAsAdministrator("Access is denied and failed to save keyboard layouts list. Do you want to run KeyMagic as administrator?");
            }
            catch (Exception)
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
            Application.Exit();
            //this.Dispose();
        }

        #endregion

        #region Right-click notify menu

        private void showToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Show();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AboutKeyMagic about = new AboutKeyMagic();
            about.ShowDialog(this);
        }

        #endregion

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
            this.Show();
            this.Activate();
        }

        #endregion

        #region Aero Glass Functions

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

        private void InitializeAeroGlass()
        {
            ResetDwmBlurBehind();
            m_glassMargins = new DwmApi.MARGINS(5, 28, 5, 5);

            if (DwmApi.DwmIsCompositionEnabled()) DwmApi.DwmExtendFrameIntoClientArea(this.Handle, m_glassMargins);
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

        #endregion

        #region Misc Functions

        private void AskToRunAsAdministrator(string msg)
        {
            AskToRunAsAdministrator(msg, "");
        }

        private void AskToRunAsAdministrator(string msg, string args)
        {
            string failedMessage = "Failed to create process.";
            if (MessageBox.Show(this, msg, "Error!", MessageBoxButtons.YesNo, MessageBoxIcon.Error) == DialogResult.Yes)
            {
                try
                {
                    ProcessStartInfo psi = new ProcessStartInfo(Environment.GetCommandLineArgs()[0]);
                    psi.Arguments = args;
                    psi.Verb = "runas";
                    Process p = Process.Start(psi);
                    if (p == null)
                    {
                        MessageBox.Show(this, failedMessage, "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return;
                    }
                    Close();
                }
                catch (Exception)
                {
                    MessageBox.Show(this, failedMessage, "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private string GetSaveKeyboardPath(string fileTitle)
        {
            return Path.Combine(keyboardDir, fileTitle + ".km2");
        }

        /// <summary>
        /// method for checking to see if the logged in user
        /// is in the Administrator's group
        /// </summary>
        /// <returns></returns>
        public static bool IsUserAdministrator()
        {
            //bool value to hold our return value
            bool isAdmin;
            try
            {
                //get the currently logged in user
                WindowsIdentity user = WindowsIdentity.GetCurrent();
                WindowsPrincipal principal = new WindowsPrincipal(user);
                isAdmin = principal.IsInRole(WindowsBuiltInRole.Administrator);
            }
            catch (UnauthorizedAccessException ex)
            {
                isAdmin = false;
                MessageBox.Show(ex.Message);
            }
            catch (Exception ex)
            {
                isAdmin = false;
                MessageBox.Show(ex.Message);
            }
            return isAdmin;
        }

        private void RaiseWin32Exception(bool condition)
        {
            if (condition == false)
            {
                //Returns the error code returned by the last unmanaged function called using platform invoke that has the DllImportAttribute.SetLastError flag set. 
                int errorCode = Marshal.GetLastWin32Error();
                //do cleanup

                //Initializes and throws a new instance of the Win32Exception class with the specified error. 
                throw new Win32Exception(errorCode);
            }
        }

        private int GetIndex(string ActiveFile)
        {
            foreach (ListViewItem item in lvLayouts.Items)
            {
                if (item.Text == ActiveFile)
                {
                    return lvLayouts.Items.IndexOf(item) + 1;
                }
            }
            return 0;
        }

        #endregion

        #region Keyboard Layout Menu Function

        private void ClearCheck(ToolStripItemCollection Items)
        {
            foreach (ToolStripMenuItem item in Items)
            {
                item.Checked = false;
            }
        }

        void cmsLeftMenuItem_Click(object sender, EventArgs e)
        {
            ClearCheck(cmsLeft.Items);

            KToolStripMenuItem thisItem = sender as KToolStripMenuItem;
            thisItem.Checked = true;

            int index = cmsLeft.Items.IndexOf(thisItem);
            if (index == 0)
            {
                SetFileToLoad("");
                if (NavtiveMethods.SendMessage(LastClientHandle, (uint)DLLMSG.KM_SETKEYBOARD, IntPtr.Zero, IntPtr.Zero) == IntPtr.Zero)
                {
                    nIcon.Icon = mainIcon;
                }
                return;
            }

            try
            {
                String fileName = ActiveKeyboardList[index].file;
                SetFileToLoad(fileName);

                if (NavtiveMethods.SendMessage(LastClientHandle, (uint)DLLMSG.KM_SETKEYBOARD, (IntPtr)index, IntPtr.Zero) == IntPtr.Zero)
                {
                    nIcon.Icon = iconList[fileName];
                    //using (Bitmap bm = new Bitmap(imageList.Images[fileName]))
                    //{
                    //    nIcon.Icon = Icon.FromHandle(bm.GetHicon());
                    //}
                }
            }
            catch (Exception)
            {
            }
        }

        #endregion

        private void btnCheckUpdate_Click(object sender, EventArgs e)
        {
            try
            {
                WebRequest httpReq = (HttpWebRequest)WebRequest.Create("http://ttkz-mbp/");
                WebResponse webResponse = httpReq.GetResponse();
                using (StreamReader sr = new StreamReader(webResponse.GetResponseStream()))
                {
                    string content = sr.ReadToEnd();
                }
                webResponse.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show(this, ex.Message, "Error!", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
        }

        private void chkRunAtLogon_CheckedChanged(object sender, EventArgs e)
        {
            SetLogonRun(chkRunAtLogon.Checked);
            Properties.Settings.Default.RunAtStartup = chkRunAtLogon.Checked = GetOnLogonRun();
        }
    }
}
