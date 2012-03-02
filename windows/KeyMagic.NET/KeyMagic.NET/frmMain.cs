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
using System.Configuration;
using System.Xml;
using System.Threading;
using KeyMagicDotNet;

namespace KeyMagic
{
    public partial class frmMain : Form
    {
        string[] MyanmarUnicodeFontNames = new string[] { "Parabaik", "Yunghkio", "Myanmar3", "MyMyanmar Unicode", "WinUni Innwa", "Win Uni Innwa" };

        enum DLLMSG : int
        {
            //KM_SETKEYBOARD = 0x5401,
            KM_ERROR = 0x5401,
            KM_LOSTFOCUS = 0x5402,
            KM_GOTFOCUS = 0x5403,
            //KM_LISTCHANGED = 0x5404,
            //KM_GETKBLNAME = 0x5406
            KM_INPUTLANGCHANGE = 0x5404,
            KM_SENDKEYSTATES = 0x5405
        }

        //store additional icon for tray, this looks better for images with alpha when drawn
        Dictionary<String, Icon> iconList = new Dictionary<string, Icon>();

        Icon mainIcon;
        Icon defaultKeyboardIcon;

        static bool isAdmin = false;
        static bool is64bit = false;
        static bool isVistaOrLater = false;

        //IntPtr DllPtrFileToLoad = IntPtr.Zero;
        //IntPtr DllPtrHotkeys = IntPtr.Zero;
        IntPtr LastClientHandle = IntPtr.Zero;

        static string mainDir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
        static string keyboardDir = Path.Combine(mainDir, "KeyboardLayouts");
        static string layoutXMLFile = Path.Combine(mainDir, "Layouts.xml");

        Dictionary<string, Thread> threads = new Dictionary<string, Thread>();

        #region Main Form Events

        public frmMain()
        {
            KToolStripMenuItem.MyanmarFontFallback = GetFont(MyanmarUnicodeFontNames, SystemFonts.MenuFont);
            InitializeComponent();
            InitializeIcon();
            InitializeKeyboardLayoutMenu();
            ProcessCommandLineArgs();
            InitializeMisc();
            InitializeAeroGlass();
            RunHelperExe();
            InitializeLogonRun();
            ChangeWindowMessageFilterToReceive();

            keyEventHandler.Install();

            CreateKeyboardDirectory();
            LoadKeyboardLayoutList();

            lvLayouts.Font = GetFont(MyanmarUnicodeFontNames, lvLayouts.Font);

            softKeyboard = new SoftKeyboardWithEngine(keyEventHandler.Engine);
            softKeyboard.FormClosing += new FormClosingEventHandler(delegate(object fc_sender, FormClosingEventArgs fc_e)
            {
                if (fc_e.CloseReason == CloseReason.UserClosing)
                {
                    fc_e.Cancel = true;
                    softKeyboard.Hide();
                }
            });

            htkySoftKeyboard.Hotkey = new Hotkey(Properties.Settings.Default.SoftKeyboardHotkey);
            RegisterWindowsHotkey(softKeyboardHtkyId, htkySoftKeyboard.Hotkey);
        }

        private void ReportUserInfo()
        {
            if (Properties.Settings.Default.UserInfoReported)
            {
                Debug.WriteLine("UserInfoReported");
                return;
            }
            ThreadStart ts = new ThreadStart(delegate()
            {
                try
                {
                    //string url = "http://10.211.55.2/report.php";
                    string url = "http://ttkz.me/keymagic/report/report.php";

                    WebRequest request = (WebRequest)WebRequest.Create(url);
                    request.Method = "POST";
                    request.ContentType = "application/x-www-form-urlencoded";

                    Dictionary<string, string> postData = new Dictionary<string, string>();

                    postData["OSVersion"] = Environment.OSVersion.ToString();
                    postData["64bit"] = is64bit.ToString();

                    NativeMethods.MEMORYSTATUSEX mem = new NativeMethods.MEMORYSTATUSEX();
                    mem.dwLength = (uint)Marshal.SizeOf(typeof(NativeMethods.MEMORYSTATUSEX));

                    NativeMethods.GlobalMemoryStatusEx(ref mem);

                    postData["RAM"] = (mem.ullTotalPhys / 1024 / 1024).ToString() + " MB";

                    List<byte> postByte = new List<byte>();

                    foreach (var e in postData)
                    {
                        postByte.AddRange(Encoding.ASCII.GetBytes(e.Key + "=" + e.Value + "&"));
                    }

                    request.ContentLength = postByte.Count;

                    using (Stream req = request.GetRequestStream())
                    {
                        req.Write(postByte.ToArray(), 0, postByte.Count);

                        HttpWebResponse response = (HttpWebResponse)request.GetResponse();

                        if (response.StatusCode == HttpStatusCode.OK)
                        {
                            using (Stream res = response.GetResponseStream())
                            {
                                using (StreamReader sr = new StreamReader(res))
                                {
                                    string ret = sr.ReadToEnd();
                                    if (ret.Equals("SUCCESS"))
                                    {
                                        Properties.Settings.Default.UserInfoReported = true;
                                    }
                                    else
                                    {
                                        Properties.Settings.Default.UserInfoReported = false;
                                    }
                                }
                            }
                        }
                    }
                }
                catch (Exception e)
                {
                    Debug.WriteLine(e.Message);
                }
            });

            Thread t = new Thread(ts);
            t.Start();

            threads["ReportUserInfo"] = t;
        }

        private void ChangeWindowMessageFilterToReceive()
        {
            try
            {
                NativeMethods.ChangeWindowMessageFilter((uint)DLLMSG.KM_ERROR, NativeMethods.MessageFilterFlag.MSGFLT_ADD);
                NativeMethods.ChangeWindowMessageFilter((uint)DLLMSG.KM_GOTFOCUS, NativeMethods.MessageFilterFlag.MSGFLT_ADD);
                NativeMethods.ChangeWindowMessageFilter((uint)DLLMSG.KM_INPUTLANGCHANGE, NativeMethods.MessageFilterFlag.MSGFLT_ADD);
                NativeMethods.ChangeWindowMessageFilter((uint)DLLMSG.KM_LOSTFOCUS, NativeMethods.MessageFilterFlag.MSGFLT_ADD);
                NativeMethods.ChangeWindowMessageFilter((uint)NativeMethods.WM.COPYDATA, NativeMethods.MessageFilterFlag.MSGFLT_ADD);
            }
            catch
            {
            }
        }

        public static Font GetFont(String[] fontFamilies, Font prototype)
        {
            foreach (String fontFamily in fontFamilies)
            {
                Font f = GetFont(fontFamily, prototype);
                if (f != null)
                {
                    return f;
                }
            }
            return null;
        }

        public static Font GetFont(String fontFamily, Font prototype)
        {
            Font newFont = new Font(fontFamily, prototype.Size, prototype.Style); ;
            if (newFont.Name != fontFamily)
            {
                return null;
            }
            return newFont;
        }

        void htkyOnOff_ValueChanged(object sender, EventArgs e)
        {
            Debug.WriteLine("htkyOnOff_ValueChanged");
            KToolStripMenuItem item = cmsLeft.Items[0] as KToolStripMenuItem;
            Properties.Settings.Default.TurnOffHotkey = item.ShortcutKeyDisplayString = htkyOnOff.Hotkey.ToString();
            keyEventHandler.Hotkeys[0] = htkyOnOff.Hotkey;
        }

        void keyEventHandler_HotkeyMatched(object sender, KeyEventHandler.HotkeyMatchedEvent e)
        {
            try
            {
                ChangeKeyboardLayout((int)e.index);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                Debug.WriteLine(ex.StackTrace);
            }
        }

        void keyEventHandler_MouseDown(object sender, MouseEventArgs e)
        {
            IntPtr wnd = NativeMethods.WindowFromPoint(new NativeMethods.POINT(e.X, e.Y));

            if (wnd == NativeMethods.GetFocus())
            {
                keyEventHandler.Engine.Reset();
            }
        }

        private void ProcessCommandLineArgs()
        {
            string[] args = Environment.GetCommandLineArgs();
            var e = args.GetEnumerator();
            while (e.MoveNext())
            {
                string arg = e.Current as string;
                switch (arg)
                {
                    case "--RegRun":
                        KeyMagic.Properties.Settings.Default.RunAtStartup = true;
                        break;
                    case "--UnRegRun":
                        KeyMagic.Properties.Settings.Default.RunAtStartup = false;
                        break;
                    case "--ShowMain":
                        this.Visible = true;
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
            //SaveColumnWidths();
            //SoftKeyboardSize = softKeyboard.Size;
            //KeyMagic.Properties.Settings.Default.Save();

            //if (processX64 != null)
            //{
            //    processX64.Kill();
            //}
            //if (processX86 != null)
            //{
            //    processX86.Kill();
            //}

            //keyEventHandler.UnInstall();
        }

        protected override void DestroyHandle()
        {
            SaveColumnWidths();
            SoftKeyboardSize = softKeyboard.Size;
            KeyMagic.Properties.Settings.Default.Save();

            if (processX64 != null)
            {
                processX64.Kill();
                processX64 = null;
            }
            if (processX86 != null)
            {
                processX86.Kill();
                processX86 = null;
            }

            keyEventHandler.UnInstall();

            foreach (var thread in threads)
            {
                if (thread.Value != null)
                {
                    if (thread.Value.IsAlive)
                    {
                        thread.Value.Abort();
                    }
                }
            }

            base.DestroyHandle();
        }

        private void SaveColumnWidths()
        {
            KeyMagic.Properties.Settings.Default.ColFileNameWidth = lvLayouts.Columns[0].Width;
            KeyMagic.Properties.Settings.Default.ColDisplayWidth = lvLayouts.Columns[1].Width;
            KeyMagic.Properties.Settings.Default.ColHotkeyWidth = lvLayouts.Columns[2].Width;
            KeyMagic.Properties.Settings.Default.ColVersionWidth = lvLayouts.Columns[3].Width;
            KeyMagic.Properties.Settings.Default.ColDescWidth = lvLayouts.Columns[4].Width;
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {
            if (DwmApi.DwmIsCompositionEnabled())
            {
                BackColor = Color.Black;
            }
            else
            {
                BackColor = SystemColors.ControlLightLight;
            }

            base.OnPaintBackground(e);
        }

        #endregion

        #region LogonRun
        private void InitializeLogonRun()
        {
            SetLogonRun(Properties.Settings.Default.RunAtStartup);
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
                RegistryKey runKey = Registry.LocalMachine.OpenSubKey(@"SOFTWARE\Microsoft\Windows\CurrentVersion\Run", true);
                runKey.DeleteValue("KeyMagic");
            }
            catch (UnauthorizedAccessException)
            {
                DialogResult dr = AskToRunAsAdministrator(cantUnReg, "--UnRegRun");
                if (dr == DialogResult.No)
                {
                    Properties.Settings.Default.RunAtStartup = true;
                }
            }
        }

        private string cantUnReg = "Access is denied when setting KeyMagic to be removed on startup! Do you want to restart KeyMagic as Administrator?";
        private string cantReg = "Access is denied when setting KeyMagic to be run on startup! Do you want to restart KeyMagic as Administrator?";

        private void UnregisterTask()
        {
            Debug.WriteLine("RegisterTask:: Checking if unregistered");

            if (isTaskRegistered() == false)
            {
                return;
            }

            Debug.WriteLine("RegisterTask:: Trying to unregister");

            using (TaskService ts = new TaskService())
            {
                try
                {
                    ts.RootFolder.DeleteTask("KeyMagic");
                }
                catch (UnauthorizedAccessException)
                {
                    DialogResult dr = AskToRunAsAdministrator(cantUnReg, "--UnRegRun");
                    if (dr == DialogResult.No)
                    {
                        Properties.Settings.Default.RunAtStartup = true;
                    }
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
                    "KeyMagic", string.Format("\"{0}\" {1}", Assembly.GetExecutingAssembly().Location, "--run"), RegistryValueKind.String);
            }
            catch (UnauthorizedAccessException)
            {
                DialogResult dr = AskToRunAsAdministrator(cantReg, "--RegRun");
                if (dr == DialogResult.No)
                {
                    Properties.Settings.Default.RunAtStartup = false;
                }
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
            Debug.WriteLine("RegisterTask:: Checking if registered");

            if (isTaskRegistered())
            {
                return;
            }

            Debug.WriteLine("RegisterTask:: Trying to register");

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
                    td.Actions.Add(new ExecAction(string.Format("\"{0}\"", Assembly.GetExecutingAssembly().Location), "--task", null));

                    // Register the task in the root folder
                    ts.RootFolder.RegisterTaskDefinition(@"KeyMagic", td);

                    // Remove the task we just created
                    // ts.RootFolder.DeleteTask("KeyMagic");
                }
                catch (UnauthorizedAccessException)
                {
                    DialogResult dr = AskToRunAsAdministrator(cantReg, "--RegRun");
                    if (dr == DialogResult.No)
                    {
                        Properties.Settings.Default.RunAtStartup = false;
                    }
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

        private static bool InternalCheckIsWow64()
        {
            if ((Environment.OSVersion.Version.Major == 5 && Environment.OSVersion.Version.Minor >= 1) ||
                Environment.OSVersion.Version.Major >= 6)
            {
                using (Process p = Process.GetCurrentProcess())
                {
                    bool retVal;
                    try
                    {
                        if (!NativeMethods.IsWow64Process(p.Handle, out retVal))
                        {
                            return false;
                        }
                    }
                    catch
                    {
                        return false;
                    }
                    return retVal;
                }
            }
            else
            {
                return false;
            }
        }

        private void InitializeMisc()
        {
            bool beta = Properties.Settings.Default.BetaRelease;

            if (beta) this.Text += " (beta)";

            ActiveKeyboardList.Add(DefaultLayout);

            using (AboutKeyMagic aboutDlg = new AboutKeyMagic())
            {
                lblAboutTitle.Text = String.Format("About {0} {1} {2}", AboutKeyMagic.AssemblyTitle, AboutKeyMagic.AssemblyVersion, beta ? "(beta)" : "");
            }

            is64bit = InternalCheckIsWow64();

            if (Environment.OSVersion.Version.Major >= 6)
            {
                isVistaOrLater = true;
            }

            isAdmin = IsUserAdministrator();

            // Check every 15 days
            if (Properties.Settings.Default.LastCheckUpdateDate.Add(new TimeSpan(15, 0, 0, 0)) < DateTime.Now)
            {
                Debug.WriteLine("Will check for update. " + Properties.Settings.Default.LastCheckUpdateDate.Add(new TimeSpan(15, 0, 0, 0)) + " < " + DateTime.Now);
                Thread autoCheckUpdate = new Thread(new ThreadStart(delegate()
                {
                    // Wait for 5 minutes
                    Thread.Sleep(new TimeSpan(0, 5, 0));
                    CheckForUpdates(true);
                }));

                threads["AutoCheckUpdate"] = autoCheckUpdate;
                autoCheckUpdate.Start();
            }

            new Thread(new ThreadStart(delegate()
            {
                Thread.Sleep(new TimeSpan(0, 5, 0));
                ReportUserInfo();
            }));
        }

        private void InitializeIcon()
        {
            //Load default Icon
            mainIcon = new Icon(typeof(frmMain), "KeyMagic.ico");
            using (Bitmap bm = KeyMagic.Properties.Resources.DefaultIcon)
            {
                defaultKeyboardIcon = Icon.FromHandle(bm.GetHicon());
            }
            imageList.Images.Add("DefaultIcon", defaultKeyboardIcon);
            iconList[""] = mainIcon;
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

            htkyOnOff.Hotkey = new Hotkey(Properties.Settings.Default.TurnOffHotkey);
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

        Process processX64 = null;
        Process processX86 = null;

        private void RunHelperExe()
        {
            try
            {
                ProcessStartInfo psi;

                psi = new ProcessStartInfo(Path.Combine(mainDir, "HookInput.x32.exe"), Handle.ToString("X"));
                psi.WindowStyle = ProcessWindowStyle.Hidden;
                processX86 = Process.Start(psi);

                if (is64bit)
                {
                    psi = new ProcessStartInfo(Path.Combine(mainDir, "HookInput.x64.exe"), Handle.ToString("X"));
                    psi.WindowStyle = ProcessWindowStyle.Hidden;
                    processX64 = Process.Start(psi);
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

        //private void SetFileToLoad(string fileName)
        //{
        //    UnicodeEncoding enc = new UnicodeEncoding();
        //    int count = enc.GetByteCount(fileName);
        //    IntPtr buffer = Marshal.AllocHGlobal(count);
        //    Marshal.Copy(enc.GetBytes(fileName), 0, buffer, count);

        //    NativeMethods.COPYDATASTRUCT copyData = new NativeMethods.COPYDATASTRUCT();
        //    copyData.dwData = 0xB0B0;
        //    copyData.lpData = buffer;
        //    copyData.cbData = count;

        //    IntPtr copyDataBuff = NativeMethods.IntPtrAlloc(copyData);

        //    NativeMethods.SendMessage(LastClientHandle, (uint)NativeMethods.WM.COPYDATA, IntPtr.Zero, copyDataBuff);

        //    NativeMethods.IntPtrFree(copyDataBuff);
        //    copyDataBuff = IntPtr.Zero;

        //    NativeMethods.IntPtrFree(buffer);
        //    buffer = IntPtr.Zero;
        //    //UnicodeEncoding enc = new UnicodeEncoding();
        //    //Marshal.Copy(enc.GetBytes(fileName), 0, DllPtrFileToLoad, enc.GetByteCount(fileName));
        //    //Marshal.WriteByte(DllPtrFileToLoad, enc.GetByteCount(fileName), 0);
        //}

        #endregion

        #region Edit/Delete/Add Keyboard Layout

        private bool AddKeyboardLayoutToListWithFile(string fileName)
        {
            try
            {
                KeyMagicDotNet.InfoList infoList = KeyMagicDotNet.KeyMagicKeyboard.GetInfosFromKeyboardFile(fileName);
                if (infoList == null)
                {
                    MessageBox.Show(this, "Keyboard layout file cannot be loaded.", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
                string fileTitle = Path.GetFileNameWithoutExtension(fileName);
                ListViewItem lvItem = new ListViewItem(fileTitle);
                lvItem.Group = lvLayouts.Groups["Enabled"];

                Bitmap icon = infoList.GetIcon();
                if (icon != null)
                {
                    imageList.Images.Add(fileTitle, icon);
                    lvItem.ImageKey = fileTitle;
                }
                else
                {
                    lvItem.ImageKey = "DefaultIcon";
                }

                String name = infoList.GetName();
                if (name != null)
                {
                    lvItem.SubItems.Add(name);
                }
                else
                {
                    lvItem.SubItems.Add(fileTitle);
                }

                String hotkey = infoList.GetHotkey();
                if (hotkey != null)
                {
                    lvItem.SubItems.Add(new Hotkey(hotkey).ToString());
                }
                else
                {
                    lvItem.SubItems.Add("");
                }

                lvItem.SubItems.Add(KeyMagicDotNet.KeyMagicKeyboard.GetVersion(fileName).ToString());

                String desc = infoList.GetDescription();
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
                            MessageBox.Show(this, ex.Message, "Failed to delete!", MessageBoxButtons.OK, MessageBoxIcon.Error);
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

            FileStream fs = null;
            System.Xml.XmlReader reader = null;

            try
            {
                fs = new FileStream(layoutXMLFile, FileMode.Open, FileAccess.Read);
                reader = System.Xml.XmlReader.Create(fs);

                reader.MoveToContent();
                if (reader.LocalName == "Layouts")
                {
                    reader.ReadStartElement("Layouts");
                    keyboardList.ReadXml(reader);
                    reader.ReadEndElement();
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
            finally
            {
                if (reader != null) reader.Close();
                if (fs != null) fs.Close();
            }

            //long lPtr = DllPtrHotkeys.ToInt64();
            List<Hotkey> hotkeys = new List<Hotkey>();
            hotkeys.Add(htkyOnOff.Hotkey);

            foreach (KeyboardLayout layout in keyboardList)
            {
                KeyMagicDotNet.InfoList infoList = KeyMagicDotNet.KeyMagicKeyboard.GetInfosFromKeyboardFile(GetSaveKeyboardPath(layout.file));
                ListViewItem lvItem = new ListViewItem(layout.file);

                hotkeys.Add(new Hotkey(layout.hotkey));

                lvItem.SubItems.Add(layout.display);
                lvItem.SubItems.Add(layout.hotkey);
                lvItem.SubItems.Add(KeyMagicDotNet.KeyMagicKeyboard.GetVersion(GetSaveKeyboardPath(layout.file)).ToString());
                lvItem.SubItems.Add(infoList.GetDescription());
                using (Bitmap bmIcon = infoList.GetIcon())
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
                                iconList[layout.file] = menuItem.Icon = icon;
                            }
                        else iconList[layout.file] = defaultKeyboardIcon;

                        menuItem.ImageKey = lvItem.ImageKey;
                        menuItem.ShortcutKeyDisplayString = layout.hotkey;
                        String fontFamily = infoList.GetFontFamily();
                        menuItem.Click += new EventHandler(cmsLeftMenuItem_Click);
                        cmsLeft.Items.Add(menuItem);
                    }
                    else
                    {
                        lvItem.Group = lvLayouts.Groups["Disabled"];
                    }
                }
                lvLayouts.Items.Add(lvItem);
            }

            keyEventHandler.Hotkeys = hotkeys.ToArray();
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

            FileStream fs = null;
            System.Xml.XmlWriter writer = null;

            try
            {
                fs = new FileStream(layoutXMLFile, FileMode.Create);

                System.Xml.XmlWriterSettings settings = new System.Xml.XmlWriterSettings();
                settings.Indent = true;
                settings.IndentChars = ("    ");
                writer = System.Xml.XmlWriter.Create(fs, settings);

                writer.WriteStartElement("Layouts");
                keyboardList.WriteXml(writer);
                writer.WriteEndElement();
            }
            catch (UnauthorizedAccessException)
            {
                AskToRunAsAdministrator("Access is denied and failed to save keyboard layouts list. Do you want to run KeyMagic as administrator?");
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
            }
            finally
            {
                if (writer != null) writer.Close();
                if (fs != null) fs.Close();
            }
        }

        #endregion

        #region DialogButtons

        private void btnOK_Click(object sender, EventArgs e)
        {
            this.Hide();
            Properties.Settings.Default.Save();
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
            nIcon.Dispose();
            this.DestroyHandle();
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

        private DialogResult AskToRunAsAdministrator(string msg)
        {
            return AskToRunAsAdministrator(msg, "");
        }

        private DialogResult AskToRunAsAdministrator(string msg, string args)
        {
            string failedMessage = "Failed to create process.";
            DialogResult dr = MessageBox.Show(this, msg, "Error!", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Error);
            if (dr == DialogResult.Yes)
            {
                try
                {
                    ProcessStartInfo psi = new ProcessStartInfo(Environment.GetCommandLineArgs()[0]);
                    psi.Arguments = args;
                    psi.Verb = "runas";
                    Process p = Process.Start(psi);
                    if (p == null)
                    {
                        return MessageBox.Show(this, failedMessage, "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                    this.Dispose();
                }
                catch (Exception)
                {
                    MessageBox.Show(this, failedMessage, "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            return dr;
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
            if (index < 0) return;
            ChangeKeyboardLayout(index);
        }

        private int LastKeyboardLayoutIndex = Properties.Settings.Default.LastKeyboardLayoutIndex;

        private void ChangeKeyboardLayout(int index)
        {
            nIcon.Icon = mainIcon;

            try
            {
                if (!engines.ContainsKey(LastClientHandle))
                {
                    engines[LastClientHandle] = new LayoutInfo(index, new KeyMagicDotNet.KeyMagicEngine());
                }
                else if (index == 0 && engines[LastClientHandle].index == 0)
                {
                    index = LastKeyboardLayoutIndex;
                    engines[LastClientHandle] = new LayoutInfo(index, new KeyMagicDotNet.KeyMagicEngine());
                }
                else if (index == 0)
                {
                    Properties.Settings.Default.LastKeyboardLayoutIndex = LastKeyboardLayoutIndex = engines[LastClientHandle].index;
                    engines[LastClientHandle] = new LayoutInfo(0, null);
                }
                else if (engines[LastClientHandle].index != index)
                {
                    engines[LastClientHandle] = new LayoutInfo(index, new KeyMagicDotNet.KeyMagicEngine());
                }
                else if (engines[LastClientHandle].index == index)
                {
                    Debug.WriteLine(string.Format("engines[LastClientHandle].index == index # {0} == {1}", engines[LastClientHandle].index, index));
                    engines[LastClientHandle] = new LayoutInfo(0, null);
                    index = 0;
                }

                keyEventHandler.Engine = engines[LastClientHandle].engine;

                Debug.WriteLine(engines[LastClientHandle]);

                if (index != 0 && keyEventHandler.Engine != null)
                {
                    String fileName = ActiveKeyboardList[index].file;
                    bool success = keyEventHandler.Engine.LoadKeyboardFile(GetSaveKeyboardPath(fileName));
                    Debug.Assert(success == true, "Failed to load keyboard layout.");

                    SetKeyboardIcon(fileName);
                }

                SoftKeyboardEngine = keyEventHandler.Engine;
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.StackTrace);
            }
        }

        private void SetKeyboardIcon(string fileName)
        {
            if (iconList.ContainsKey(fileName))
            {
                nIcon.Icon = iconList[fileName];
            }
            else
            {
                nIcon.Icon = defaultKeyboardIcon;
            }
        }

        #endregion

        class Download
        {
            public string Version;
            public string DownloadURL;

            public override string ToString()
            {
                return string.Format("Version={0}, DownloadURL={1}", Version, DownloadURL);
            }
        }

        private void btnCheckUpdate_Click(object sender, EventArgs e)
        {
            CheckForUpdates(false);
        }

        private void CheckForUpdates(bool autoCheck)
        {
            ThreadStart ts = new ThreadStart(delegate()
            {
                string oldBtnText = null;
                if (btnCheckUpdate.InvokeRequired)
                {
                    btnCheckUpdate.Invoke(new MethodInvoker(delegate()
                    {
                        oldBtnText = btnCheckUpdate.Text;
                        btnCheckUpdate.Text = "Stop Checking";
                    }));
                }

                try
                {
                    String URLString = "http://dl.dropbox.com/u/176693/keymagic.xml";
                    //String URLString = "file://///psf/Home/Dropbox/Public/keymagic.xml";

                    WebRequest webReq = WebRequest.Create(URLString);
                    webReq.Proxy = WebRequest.GetSystemWebProxy();
                    webReq.CachePolicy = new System.Net.Cache.RequestCachePolicy(System.Net.Cache.RequestCacheLevel.NoCacheNoStore);
                    webReq.Proxy.Credentials = CredentialCache.DefaultCredentials;

                    XmlTextReader reader = new XmlTextReader(webReq.GetResponse().GetResponseStream());

                    Dictionary<string, Download> Downloads = new Dictionary<string, Download>();
                    String CurrentKey = null;

                    if (reader.Read() && !reader.Name.Equals("KeyMagic"))
                    {
                        throw new Exception("Invalid XML");
                    }

                    while (reader.Read())
                    {
                        switch (reader.NodeType)
                        {
                            case XmlNodeType.Element: // The node is an element.
                                if (string.IsNullOrEmpty(CurrentKey))
                                {
                                    CurrentKey = reader.Name;
                                    reader.Read();
                                    Downloads[CurrentKey] = new Download();
                                    Downloads[CurrentKey].Version = reader.Value;
                                }
                                else if (reader.Name.Equals("DownloadURL"))
                                {
                                    reader.Read();
                                    Downloads[CurrentKey].DownloadURL = reader.Value;

                                    CurrentKey = null;
                                }
                                break;
                        }
                    }

                    string latestVersion;

                    const string latestBetaVersion = "LatestBetaVersion";
                    const string latestStableVersion = "LatestStableVersion";

                    if (Properties.Settings.Default.BetaRelease)
                    {
                        latestVersion = latestBetaVersion;
                    }
                    else
                    {
                        latestVersion = latestStableVersion;
                    }

                    if (Downloads.ContainsKey(latestVersion))
                    {
                        while (Downloads[latestVersion].Version.StartsWith("="))
                        {
                            latestVersion = Downloads[latestVersion].Version.Substring(1);
                            Debug.WriteLine("Redirect to " + latestVersion);
                        }
                        if (new Version(Downloads[latestVersion].Version) > new Version(AboutKeyMagic.AssemblyVersion))
                        {
                            if (MessageBox.Show(string.Format("There is new update ({0}) available. Do you want to open download page now?", Downloads[latestVersion].Version), "Update Available",
                                MessageBoxButtons.YesNo, MessageBoxIcon.Information) == DialogResult.Yes)
                            {
                                Process.Start(Downloads[latestVersion].DownloadURL);
                            }
                        }
                        else if (new Version(Downloads[latestVersion].Version) < new Version(AboutKeyMagic.AssemblyVersion))
                        {
                            if (!autoCheck) MessageBox.Show("You have the future release version of KeyMagic which is not even released yet. Seems like you are using private release, do you?", "Ooh!", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                        }
                        else
                        {
                            // Check if there is stable release for this beta
                            if (Properties.Settings.Default.BetaRelease && Downloads.ContainsKey(latestStableVersion))
                            {
                                if (new Version(Downloads[latestStableVersion].Version) == new Version(AboutKeyMagic.AssemblyVersion))
                                {
                                    if (MessageBox.Show(string.Format("There is stable release ({0}) available for this beta version. Do you want to open download page now?", Downloads[latestStableVersion].Version), "Update Available",
                                        MessageBoxButtons.YesNo, MessageBoxIcon.Information) == DialogResult.Yes)
                                    {
                                        Process.Start(Downloads[latestStableVersion].DownloadURL);
                                    }
                                }
                                else
                                {
                                    if (!autoCheck) MessageBox.Show("You have the latest version of KeyMagic", "No Update Available", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                }
                            }
                            else
                            {
                                if (!autoCheck) MessageBox.Show("You have the latest version of KeyMagic", "No Update Available", MessageBoxButtons.OK, MessageBoxIcon.Information);
                            }
                        }
                    }
                }
                catch (ThreadAbortException)
                {
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "Error!", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                }

                if (btnCheckUpdate.InvokeRequired)
                {
                    btnCheckUpdate.Invoke(new MethodInvoker(delegate() { btnCheckUpdate.Text = oldBtnText; }));
                }

                Properties.Settings.Default.LastCheckUpdateDate = new DateTime();
            }
            );

            if (threads.ContainsKey("CheckForUpdate") == false)
            {
                threads["CheckForUpdate"] = null;
            }

            Thread updateThread = threads["CheckForUpdate"];

            if (updateThread != null)
            {
                if (updateThread.IsAlive)
                {
                    updateThread.Abort();
                }
                if (btnCheckUpdate.Text == "Stop Checking")
                {
                    btnCheckUpdate.Text = "Check For Updates";
                    return;
                }
            }

            updateThread = new Thread(ts);
            updateThread.Start();

            threads["CheckForUpdate"] = updateThread;
        }

        private void chkRunAtLogon_CheckedChanged(object sender, EventArgs e)
        {
            SetLogonRun(chkRunAtLogon.Checked);
            chkRunAtLogon.Checked = GetOnLogonRun();
        }

        private void mainTableLayoutPanel_Paint(object sender, PaintEventArgs e)
        {
            TableLayoutPanel t = mainTableLayoutPanel;
            Rectangle r = Rectangle.FromLTRB(t.Left, t.Top, t.Right, t.Top + 27);
            e.Graphics.FillRectangle(new SolidBrush(Color.Black), r);
        }

        private void fllBottom_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.FillRectangle(new SolidBrush(Color.Black), e.ClipRectangle);
        }

        SoftKeyboardWithEngine softKeyboard;

        public KeyMagicEngine SoftKeyboardEngine
        {
            get { return softKeyboard.Engine; }
            set { softKeyboard.Engine = value; }
        }

        public Size SoftKeyboardSize
        {
            get
            {
                return Properties.Settings.Default.SoftKeyboardSize;
            }
            set
            {
                Properties.Settings.Default.SoftKeyboardSize = value;
            }
        }

        private void softKeyboardToolStripMenuItem_Click(object sender, EventArgs e)
        {
            softKeyboard.Size = SoftKeyboardSize;
            softKeyboard.Show();
        }

        private void htkySoftKeyboard_ValueChanged(object sender, EventArgs e)
        {
            KToolStripMenuItem item = cmsRight.Items[1] as KToolStripMenuItem;
            Properties.Settings.Default.SoftKeyboardHotkey = item.ShortcutKeyDisplayString = htkySoftKeyboard.Hotkey.ToString();

            RegisterWindowsHotkey(softKeyboardHtkyId, htkySoftKeyboard.Hotkey);
        }

        private const int softKeyboardHtkyId = 0x001;

        private void RegisterWindowsHotkey(int id, Hotkey hotkey)
        {
            uint fsModifiers = hotkey.WinModifiers;
            NativeMethods.UnregisterHotKey(Handle, id);
            NativeMethods.RegisterHotKey(Handle, id, fsModifiers, hotkey.KeyChar);
        }
    }
}
