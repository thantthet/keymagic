using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.Windows.Forms.VisualStyles;
using System.Runtime.InteropServices;

namespace KeyMagic
{
    public partial class SoftKeyboard : Form
    {
        #region Modifier Flags
        private bool lShift = false;

        public bool LShift
        {
            get { return lShift; }
            set
            {
                if (lShift != value)
                {
                    lShift = value;
                    LShiftKey.IsChecked = value;
                    Refresh(RefreshFor.Label);
                }
            }
        }
        private bool rShift = false;

        public bool RShift
        {
            get { return rShift; }
            set
            {
                if (rShift != value)
                {
                    rShift = value;
                    RShiftKey.IsChecked = value;
                    Refresh(RefreshFor.Label);
                }
            }
        }

        public bool Shift
        {
            get { return lShift || rShift; }
            set
            {
                if (lShift != value || rShift != value)
                {
                    lShift = rShift = value;
                    LShiftKey.IsChecked = value;
                    RShiftKey.IsChecked = value;
                    Refresh(RefreshFor.Label);
                }
            }
        }

        private bool lControl = false;

        public bool LControl
        {
            get { return lControl; }
            set
            {
                if (lControl != value)
                {
                    lControl = value;
                    LControlKey.IsChecked = value;
                    Refresh(RefreshFor.Label);
                }
            }
        }

        private bool rControl = false;

        public bool RControl
        {
            get { return rControl; }
            set
            {
                if (rControl != value)
                {
                    rControl = value;
                    RControlKey.IsChecked = value;
                    Refresh(RefreshFor.Label);
                }
            }
        }

        public bool Control
        {
            get { return rControl || lControl; }
            set
            {
                if (rControl != value || lControl != value)
                {
                    rControl = lControl = value;
                    LControlKey.IsChecked = value;
                    RControlKey.IsChecked = value;
                    Refresh(RefreshFor.Label);
                }
            }
        }

        private bool lAlt = false;

        public bool LAlt
        {
            get { return lAlt; }
            set
            {
                if (lAlt != value)
                {
                    lAlt = value;
                    LAltKey.IsChecked = value;
                    Refresh(RefreshFor.Label);
                }
            }
        }

        private bool rAlt = false;

        public bool RAlt
        {
            get { return rAlt; }
            set
            {
                if (rAlt != value)
                {
                    rAlt = value;
                    RAltKey.IsChecked = value;
                    Refresh(RefreshFor.Label);
                }
            }
        }

        public bool Alt
        {
            get { return rAlt || lAlt; }
            set
            {
                if (rAlt != value || lAlt != value)
                {
                    rAlt = lAlt = value;
                    LAltKey.IsChecked = value;
                    RAltKey.IsChecked = value;
                    Refresh(RefreshFor.Label);
                }
            }
        }

        private bool capsLock = false;

        public bool CapsLock
        {
            get { return capsLock; }
            set
            {
                if (capsLock != value)
                {
                    Debug.WriteLine("LAlt = RAlt = " + value);
                    capsLock = value;
                    Refresh(RefreshFor.Label);
                }
            }
        }

        #endregion

        #region Special Keys
        private SoftKey CapsLockKey
        {
            get { return ASDFRowPanel.Controls[0] as SoftKey; }
        }

        private SoftKey LShiftKey
        {
            get { return ZXCVRowPanel.Controls[0] as SoftKey; }
        }

        private SoftKey RShiftKey
        {
            get { return ZXCVRowPanel.Controls[ZXCVRowPanel.Controls.Count - 1] as SoftKey; }
        }

        private SoftKey LControlKey
        {
            get { return SpaceRowPanel.Controls[0] as SoftKey; }
        }

        private SoftKey RControlKey
        {
            get { return SpaceRowPanel.Controls[SpaceRowPanel.Controls.Count - 1] as SoftKey; }
        }

        private SoftKey LAltKey
        {
            get { return SpaceRowPanel.Controls[2] as SoftKey; }
        }

        private SoftKey RAltKey
        {
            get { return SpaceRowPanel.Controls[SpaceRowPanel.Controls.Count - 2] as SoftKey; }
        }

        #endregion

        public enum RefreshFor
        {
            Font = 1,
            Label = 1 << 1,
            Size = 1 << 2,
            All = Font | Label | Size
        }

        public enum KeyType
        {
            Normal = 1,
            Toggled = 1 << 1,
            SwitchKey = 1 << 2,
            DeadKey = 1 << 2
        }

        #region Dynamic Font Generation Based on Size

        private String buttonFontFamily;

        public String ButtonFontFamily
        {
            get
            {
                if (buttonFontFamily == null)
                {
                    buttonFontFamily = Font.FontFamily.Name;
                }
                return buttonFontFamily;
            }
            set
            {
                if (value == null)
                {
                    value = Font.FontFamily.Name;
                }

                if (buttonFontFamily != value)
                {
                    buttonFontFamily = value;

                    Refresh(RefreshFor.Font);
                }
            }
        }

        public float FontSize
        {
            get
            {
                if (NumRowPanel.Size.IsEmpty)
                {
                    return 0.1f;
                }
                Size normalKeySize = NumberSize(NumRowPanel.Size)[0];
                float size = (float)(Math.Min(normalKeySize.Width, normalKeySize.Height) * 0.3);
                if (size < 0.1f)
                {
                    return 0.1f;
                }
                return size;
            }
        }

        private Font buttonFont;

        private Font ButtonFont
        {
            get
            {
                if (buttonFont == null)
                {
                    buttonFont = Font;
                }
                if (buttonFont.FontFamily.Name != buttonFontFamily || buttonFont.Size != FontSize)
                {
                    buttonFont = new Font(ButtonFontFamily, FontSize, FontStyle.Bold);
                }
                return buttonFont;
            }
        }

        #endregion

        #region Button Labels and VK codes

        private string deleteLabel = "del";

        public string DeleteLabel
        {
            get { return deleteLabel; }
            set { deleteLabel = value; }
        }
        private string tabLabel = "tab";

        public string TabLabel
        {
            get { return tabLabel; }
            set { tabLabel = value; }
        }
        private string capsLabel = "caps";

        public string CapsLabel
        {
            get { return capsLabel; }
            set { capsLabel = value; }
        }
        private string returnLabel = "return";

        public string ReturnLabel
        {
            get { return returnLabel; }
            set { returnLabel = value; }
        }
        private string shiftLabel = "shift";

        public string ShiftLabel
        {
            get { return shiftLabel; }
            set { shiftLabel = value; }
        }
        private string spaceLabel = "space";

        public string SpaceLabel
        {
            get { return spaceLabel; }
            set { spaceLabel = value; }
        }
        private string altLabel = "Alt";

        public string AltLabel
        {
            get { return altLabel; }
            set { altLabel = value; }
        }
        private string ctrlLabel = "Ctrl";

        public string CtrlLabel
        {
            get { return ctrlLabel; }
            set { ctrlLabel = value; }
        }
        private string winLabel = "Win";

        public string WinLabel
        {
            get { return winLabel; }
            set { winLabel = value; }
        }

        public uint[] NumRowScancodes
        {
            get
            {
                return new uint[] { 0x29, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE };
            }
        }

        public uint[] QWERRowScancodes
        {
            get
            {
                return new uint[] { 0xF, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x2B };
            }
        }

        public uint[] ASDFRowScancodes
        {
            get
            {
                return new uint[] { 0x3A, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x1C };
            }
        }

        public uint[] ZXCVRowScancodes
        {
            get
            {
                return new uint[] { 0x2A, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36 };
            }
        }

        public uint[] SpaceRowScancodes
        {
            get
            {
                return new uint[] { 0x1D, 0x5B, 0x38, 0x39, 0xE038, 0xE01D };
            }
        }

        #endregion

        private struct KeyLocation
        {
            public Panel Panel;
            public int KeyIndex;

            public KeyLocation(Panel Panel, int KeyIndex)
            {
                this.Panel = Panel;
                this.KeyIndex = KeyIndex;
            }
        }

        KeyLocation[] SpeicalKeys;

        public SoftKeyboard()
        {
            InitializeComponent();
            InitializeAeroGlass();

            SetStyle(ControlStyles.Selectable, false);

            SpeicalKeys = new KeyLocation[] {
                new KeyLocation(NumRowPanel, -1),
                new KeyLocation(QWERRowPanel, 0),
                new KeyLocation(ASDFRowPanel, 0),
                new KeyLocation(ASDFRowPanel, -1),
                new KeyLocation(ZXCVRowPanel, 0),
                new KeyLocation(ZXCVRowPanel, -1),
                new KeyLocation(SpaceRowPanel, 0),
                new KeyLocation(SpaceRowPanel, 1),
                new KeyLocation(SpaceRowPanel, 2),
                new KeyLocation(SpaceRowPanel, 3),
                new KeyLocation(SpaceRowPanel, 4),
                new KeyLocation(SpaceRowPanel, 5)
            };

            CreateButtons();
            Refresh(RefreshFor.All);

            GetKeyStatesTimer.Interval = 50;
            GetKeyStatesTimer.Tick += new EventHandler(GetKeyStatesTimer_Tick);
        }

        Timer GetKeyStatesTimer = new Timer();

        protected override void OnVisibleChanged(EventArgs e)
        {
            Debug.WriteLine("OnVisibleChanged");
            base.OnVisibleChanged(e);

            if (Visible)
            {
                GetKeyStatesTimer.Start();
            }
            else
            {
                GetKeyStatesTimer.Stop();
            }
        }

        protected virtual void GetKeyStatesTimer_Tick(object sender, EventArgs e)
        {
            SetCapsLockState(CapsLockKey);

            LShift = (NativeMethods.GetAsyncKeyState(NativeMethods.VK.LSHIFT) & 0x8000) == 0x8000;
            RShift = (NativeMethods.GetAsyncKeyState(NativeMethods.VK.RSHIFT) & 0x8000) == 0x8000;

            LControl = (NativeMethods.GetAsyncKeyState(NativeMethods.VK.LCONTROL) & 0x8000) == 0x8000;
            RControl = (NativeMethods.GetAsyncKeyState(NativeMethods.VK.RCONTROL) & 0x8000) == 0x8000;

            LAlt = (NativeMethods.GetAsyncKeyState(NativeMethods.VK.LMENU) & 0x8000) == 0x8000;
            RAlt = (NativeMethods.GetAsyncKeyState(NativeMethods.VK.RMENU) & 0x8000) == 0x8000;
        }

        //protected override void OnShown(EventArgs e)
        //{
        //    long exStyle = NativeMethods.GetWindowLong(this.Handle, NativeMethods.GWL_EXSTYLE);
        //    exStyle |= 0x08000000L;
        //    Debug.WriteLine(exStyle.ToString("X"));
        //    exStyle = NativeMethods.SetWindowLong(this.Handle, NativeMethods.GWL_EXSTYLE, exStyle);
        //    Debug.WriteLine(exStyle.ToString("X"));

        //    base.OnShown(e);
        //}

        private void CreateButtons()
        {
            CreateNumberRow(NumRowPanel);
            CreateQWERRow(QWERRowPanel);
            CreateASDFRow(ASDFRowPanel);
            CreateZXCVRow(ZXCVRowPanel);
            CreateSpaceRow(SpaceRowPanel);
        }

        #region Generating Display String

        protected virtual string GetDisplayStringFromVK(uint scanCode)
        {
            byte[] keys = GetVirtualizedKeyboardState();
            keys[(int)NativeMethods.VK.CONTROL] = 0;
            keys[(int)NativeMethods.VK.MENU] = 0;

            uint keyval = GetKeyValue(scanCode, keys, IntPtr.Zero);

            switch (scanCode)
            {
                case 0x1D:
                case 0xE01D:
                    return ctrlLabel;
                case 0x38:
                case 0xE038:
                    return altLabel;
                case 0x2A:
                case 0x36:
                    return shiftLabel;
                case 0x3A:
                    return capsLabel;
                case 0x39:
                    return spaceLabel;
                case 0x5B:
                    return winLabel;
                case 0x0E:
                    return deleteLabel;
                case 0x0F:
                    return tabLabel;
                case 0x1C:
                    return returnLabel;
            }

            //if (keyval == 0)
            //{
            //    switch (scanCode)
            //    {
            //        case 0x1D:
            //        case 0xE01D:
            //            return ctrlLabel;
            //        case 0x38:
            //        case 0xE038:
            //            return altLabel;
            //        case 0x2A:
            //        case 0x36:
            //            return shiftLabel;
            //        case 0x3A:
            //            return capsLabel;
            //        case 0x39:
            //            return spaceLabel;
            //        case 0x5B:
            //            return winLabel;
            //        case 0x0E:
            //            return deleteLabel;
            //        case 0x0F:
            //            return tabLabel;
            //        case 0x1C:
            //            return returnLabel;
            //    }
            //}
            //else if (keyval == 0x08 && scanCode == 0x0E)
            //{
            //    return deleteLabel;
            //}
            //else if (keyval == 0x09 && scanCode == 0x0F)
            //{
            //    return tabLabel;
            //}
            //else if (keyval == 0x0D && scanCode == 0x1C)
            //{
            //    return returnLabel;
            //}

            return ((char)keyval).ToString();
        }

        public uint GetKeyValue(uint scanCode, byte[] keys, IntPtr keyboard)
        {
            bool deadkey;
            return GetKeyValue(scanCode, keys, keyboard, out deadkey);
        }

        public uint GetKeyValue(uint scanCode, byte[] keys, IntPtr keyboard, out bool deadkey)
        {
            if (keys == null)
            {
                keys = GetVirtualizedKeyboardState();
            }

            if (keyboard == IntPtr.Zero)
            {
                uint dwProcessId;
                uint threadId = NativeMethods.GetWindowThreadProcessId(NativeMethods.GetForegroundWindow(), out dwProcessId);
                keyboard = NativeMethods.GetKeyboardLayout(threadId);
            }

            uint vkey = GetVK(scanCode, keyboard);
            keys[vkey] = 0x80;

            StringBuilder TranslatedChar = new StringBuilder();

            int result = NativeMethods.ToUnicodeEx(vkey, scanCode, keys, TranslatedChar, 1, 0, keyboard);

            deadkey = (result == -1) ? true : false;

            return (uint)(TranslatedChar.Length > 0 ? TranslatedChar[0] : 0);
        }

        public uint GetVK(uint scanCode, IntPtr keyboard)
        {
            return NativeMethods.MapVirtualKeyEx(scanCode, NativeMethods.MapVirtualKeyMapTypes.MAPVK_VSC_TO_VK_EX, keyboard);
        }

        public byte[] GetVirtualizedKeyboardState()
        {
            byte[] keyboardState = new byte[256];

            if (LShift) keyboardState[(uint)NativeMethods.VK.LSHIFT] = 0x80;
            if (RShift) keyboardState[(uint)NativeMethods.VK.RSHIFT] = 0x80;
            if (Shift) keyboardState[(uint)NativeMethods.VK.SHIFT] = 0x80;

            if (LControl) keyboardState[(uint)NativeMethods.VK.LCONTROL] = 0x80;
            if (RControl) keyboardState[(uint)NativeMethods.VK.RCONTROL] = 0x80;
            if (Control) keyboardState[(uint)NativeMethods.VK.CONTROL] = 0x80;

            if (LAlt) keyboardState[(uint)NativeMethods.VK.LMENU] = 0x80;
            if (RAlt) keyboardState[(uint)NativeMethods.VK.RMENU] = 0x80;
            if (Alt) keyboardState[(uint)NativeMethods.VK.MENU] = 0x80;

            if (CapsLock) keyboardState[(uint)NativeMethods.VK.CAPITAL] = 0x81;

            return keyboardState;
        }

        #endregion

        #region Adding Buttons

        private void CreateNumberRow(FlowLayoutPanel currentFlow)
        {
            SoftKey b = null;

            currentFlow.Controls.Clear();

            Padding p = new Padding(2, 3, 2, 3);

            Size[] sizes = NumberSize(currentFlow.Size);

            for (int i = 0; i < sizes.Length; i++)
            {
                b = CreateSoftKey() as SoftKey;
                b.ScanCode = NumRowScancodes[i];
                currentFlow.Controls.Add(b);
            }
        }

        private void CreateQWERRow(FlowLayoutPanel currentFlow)
        {
            SoftKey b = null;

            currentFlow.Controls.Clear();

            Padding p = new Padding(2, 3, 2, 3);

            Size[] sizes = QWERSize(currentFlow.Size);

            for (int i = 0; i < sizes.Length; i++)
            {
                b = CreateSoftKey() as SoftKey;
                b.ScanCode = QWERRowScancodes[i];
                currentFlow.Controls.Add(b);
            }
        }

        private void CreateASDFRow(FlowLayoutPanel currentFlow)
        {
            SoftKey b = null;

            currentFlow.Controls.Clear();

            Padding p = new Padding(2, 3, 2, 3);

            Size[] sizes = ASDFSize(currentFlow.Size);

            for (int i = 0; i < sizes.Length; i++)
            {
                b = CreateSoftKey() as SoftKey;
                b.ScanCode = ASDFRowScancodes[i];
                currentFlow.Controls.Add(b);
            }
        }

        private void CreateZXCVRow(FlowLayoutPanel currentFlow)
        {
            SoftKey b = null;

            currentFlow.Controls.Clear();

            Padding p = new Padding(2, 3, 2, 3);

            Size[] sizes = ZXCVSize(currentFlow.Size);

            for (int i = 0; i < sizes.Length; i++)
            {
                b = CreateSoftKey() as SoftKey;
                if (i == 0)
                {
                    b.CheckAble = true;
                }
                b.ScanCode = ZXCVRowScancodes[i];
                currentFlow.Controls.Add(b);
            }

            if (b != null)
            {
                b.CheckAble = true;
            }
        }

        private void CreateSpaceRow(FlowLayoutPanel currentFlow)
        {
            SoftKey b = null;

            currentFlow.Controls.Clear();

            Padding p = new Padding(2, 3, 2, 3);

            Size[] sizes = SpaceSize(currentFlow.Size);

            for (int i = 0; i < sizes.Length; i++)
            {
                b = CreateSoftKey() as SoftKey;
                switch (i)
                {
                    case 0:
                    case 2:
                    case 4:
                    case 5:
                        b.CheckAble = true;
                        break;
                }
                b.ScanCode = SpaceRowScancodes[i];
                currentFlow.Controls.Add(b);
            }
        }

        #endregion

        #region Sizing

        private Size[] NumberSize(Size containerSize)
        {
            List<Size> sizes = new List<Size>();

            double totalNormalKeyWidth = containerSize.Width * 0.9;
            double normalKeyWidth = Math.Round(totalNormalKeyWidth / 13) - (buttonMargin.Left + buttonMargin.Right);

            for (int i = 0; i < 13; i++)
            {
                sizes.Add(new Size((int)normalKeyWidth, containerSize.Height - buttonMargin.Top));
            }

            sizes.Add(new Size((int)(containerSize.Width * 0.1) - (buttonMargin.Left + buttonMargin.Right), containerSize.Height - buttonMargin.Top));

            return sizes.ToArray();
        }

        private Size[] QWERSize(Size containerSize)
        {
            List<Size> sizes = new List<Size>();

            double totalNormalKeyWidth = containerSize.Width * 0.9;
            double normalKeyWidth = Math.Round(totalNormalKeyWidth / 13) - (buttonMargin.Left + buttonMargin.Right);

            sizes.Add(new Size((int)(containerSize.Width * 0.1) - (buttonMargin.Left + buttonMargin.Right), containerSize.Height - buttonMargin.Top));

            for (int i = 0; i < 13; i++)
            {
                sizes.Add(new Size((int)normalKeyWidth, containerSize.Height - buttonMargin.Top));
            }

            return sizes.ToArray();
        }

        private Size[] ASDFSize(Size containerSize)
        {
            List<Size> sizes = new List<Size>();

            double totalNormalKeyWidth = containerSize.Width * 0.759;
            double normalKeyWidth = Math.Round(totalNormalKeyWidth / 11) - (buttonMargin.Left + buttonMargin.Right);

            sizes.Add(new Size((int)(containerSize.Width * 0.116) - (buttonMargin.Left + buttonMargin.Right), containerSize.Height - buttonMargin.Top));

            for (int i = 0; i < 11; i++)
            {
                sizes.Add(new Size((int)normalKeyWidth, containerSize.Height - 3));
            }

            sizes.Add(new Size((int)(containerSize.Width * 0.125) - (buttonMargin.Left + buttonMargin.Right), containerSize.Height - buttonMargin.Top));

            return sizes.ToArray();
        }

        private Size[] ZXCVSize(Size containerSize)
        {
            List<Size> sizes = new List<Size>();

            double totalNormalKeyWidth = containerSize.Width * 0.694;
            double normalKeyWidth = Math.Round(totalNormalKeyWidth / 10) - (buttonMargin.Left + buttonMargin.Right);

            sizes.Add(new Size((int)(containerSize.Width * 0.153) - (buttonMargin.Left + buttonMargin.Right), containerSize.Height - buttonMargin.Top));

            for (int i = 0; i < 10; i++)
            {
                sizes.Add(new Size((int)normalKeyWidth, containerSize.Height - 3));
            }

            sizes.Add(new Size((int)(containerSize.Width * 0.153) - (buttonMargin.Left + buttonMargin.Right), containerSize.Height - buttonMargin.Top));

            return sizes.ToArray();
        }

        private Size[] SpaceSize(Size containerSize)
        {
            List<Size> sizes = new List<Size>();

            double totalNormalKeyWidth = containerSize.Width * 0.44285;
            double normalKeyWidth = Math.Round(totalNormalKeyWidth / 5) - (buttonMargin.Left + buttonMargin.Right);

            for (int i = 0; i < 3; i++)
            {
                sizes.Add(new Size((int)normalKeyWidth, containerSize.Height - buttonMargin.Top));
            }

            sizes.Add(new Size((int)(containerSize.Width * 0.55715) - (buttonMargin.Left + buttonMargin.Right), containerSize.Height - buttonMargin.Top));

            for (int i = 0; i < 2; i++)
            {
                sizes.Add(new Size((int)normalKeyWidth, containerSize.Height - buttonMargin.Top));
            }

            return sizes.ToArray();
        }

        #endregion

        #region Resizing

        private void SoftKeyboard_Resize(object sender, EventArgs e)
        {
            NumRowPanel.Width = QWERRowPanel.Width = ASDFRowPanel.Width = ZXCVRowPanel.Width = SpaceRowPanel.Width = DockedFlow.Width - 5;
            NumRowPanel.Height = QWERRowPanel.Height = ASDFRowPanel.Height = ZXCVRowPanel.Height = SpaceRowPanel.Height = (DockedFlow.Height - 30) / 5;

            Refresh(RefreshFor.Font | RefreshFor.Size);
        }

        #endregion

        #region Refreshing

        public void Refresh(RefreshFor modify)
        {
            try
            {
                RefreshNumberRow(NumRowPanel, modify);
                RefreshQWERRow(QWERRowPanel, modify);
                RefreshASDFRow(ASDFRowPanel, modify);
                RefreshZXCVRow(ZXCVRowPanel, modify);
                RefreshSpaceRow(SpaceRowPanel, modify);


                //Speical Case for Special Keys
                if ((modify & RefreshFor.Font) == RefreshFor.Font)
                {
                    Font smallFont = new Font(ButtonFont.FontFamily, ButtonFont.Size);
                    foreach (KeyLocation loc in SpeicalKeys)
                    {
                        int index = loc.KeyIndex;
                        if (index == -1)
                        {
                            index = loc.Panel.Controls.Count - 1;
                        }
                        loc.Panel.Controls[index].Font = smallFont;
                    }
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine(new StackTrace(e));
            }
        }

        private void RefreshNumberRow(FlowLayoutPanel panel, RefreshFor modify)
        {
            if (NumRowScancodes.Length != panel.Controls.Count)
            {
                return;
            }

            if ((modify & RefreshFor.Label) == RefreshFor.Label)
            {
                for (int i = 0; i < panel.Controls.Count; i++)
                {
                    SoftKey key = panel.Controls[i] as SoftKey;
                    ChangeStyleForKey(key);
                    key.Text = GetDisplayStringFromVK(key.ScanCode);
                }
            }

            if ((modify & RefreshFor.Font) == RefreshFor.Font)
            {
                for (int i = 0; i < panel.Controls.Count; i++)
                {
                    panel.Controls[i].Font = ButtonFont;
                }
            }

            if ((modify & RefreshFor.Size) == RefreshFor.Size)
            {
                panel.SuspendLayout();

                Size[] sizes = NumberSize(panel.Size);
                if (sizes.Length != panel.Controls.Count)
                {
                    return;
                }

                for (int i = 0; i < sizes.Length; i++)
                {
                    panel.Controls[i].Size = sizes[i];
                }

                panel.ResumeLayout();

                Control b = panel.Controls[panel.Controls.Count - 1];
                b.Width = panel.Width - b.Location.X - (buttonMargin.Left + buttonMargin.Right);
            }
        }

        private void ChangeStyleForKey(SoftKey key)
        {
            if (IsSwitchKey(key.ScanCode))
            {
                SetStyleForKeyType(key, KeyType.SwitchKey);
            }
            else
            {
                SetStyleForKeyType(key, KeyType.Normal);
            }
        }

        protected virtual void SetStyleForKeyType(SoftKey key, KeyType keyType)
        {
            if ((keyType & KeyType.Toggled) == KeyType.Toggled)
            {
                key.NormalColor = new Color[] { Color.Orange, Color.OrangeRed };
                key.ForeColor = Color.White;
            }
            else if ((keyType & KeyType.Normal) == KeyType.Normal)
            {
                key.NormalColor = new Color[] { Color.White, Color.LightGray };
                key.ForeColor = Color.Black;
            }
            else if ((keyType & KeyType.SwitchKey) == KeyType.SwitchKey)
            {
                key.NormalColor = new Color[] { Color.FromArgb(177, 216, 250), Color.FromArgb(105, 180, 255) };
                key.ForeColor = Color.Black;
            }
        }

        protected virtual bool IsSwitchKey(uint scanCode)
        {
            bool deadkey;
            GetKeyValue(scanCode, null, IntPtr.Zero, out deadkey);
            Debug.WriteLineIf(deadkey, "DeadKey:" + scanCode);
            return deadkey;
        }

        private void RefreshQWERRow(FlowLayoutPanel panel, RefreshFor modify)
        {
            if (QWERRowScancodes.Length != panel.Controls.Count)
            {
                return;
            }

            if ((modify & RefreshFor.Label) == RefreshFor.Label)
            {
                for (int i = 0; i < panel.Controls.Count; i++)
                {
                    SoftKey key = panel.Controls[i] as SoftKey;
                    ChangeStyleForKey(key);
                    key.Text = GetDisplayStringFromVK(key.ScanCode);
                }
            }

            if ((modify & RefreshFor.Font) == RefreshFor.Font)
            {
                for (int i = 0; i < panel.Controls.Count; i++)
                {
                    panel.Controls[i].Font = ButtonFont;
                }
            }

            if ((modify & RefreshFor.Size) == RefreshFor.Size)
            {
                panel.SuspendLayout();

                Size[] sizes = QWERSize(panel.Size);
                if (sizes.Length != panel.Controls.Count)
                {
                    return;
                }

                for (int i = 0; i < sizes.Length; i++)
                {
                    panel.Controls[i].Size = sizes[i];
                }

                panel.ResumeLayout();

                Control b = panel.Controls[panel.Controls.Count - 1];
                b.Width = panel.Width - b.Location.X - (buttonMargin.Left + buttonMargin.Right);
            }
        }

        private void RefreshASDFRow(FlowLayoutPanel panel, RefreshFor modify)
        {
            if (ASDFRowScancodes.Length != panel.Controls.Count)
            {
                return;
            }

            if ((modify & RefreshFor.Label) == RefreshFor.Label)
            {
                for (int i = 0; i < panel.Controls.Count; i++)
                {
                    SoftKey key = panel.Controls[i] as SoftKey;
                    ChangeStyleForKey(key);
                    key.Text = GetDisplayStringFromVK(key.ScanCode);
                    if (key.ScanCode == 0x14)
                    {
                        SetCapsLockState(key);
                        continue;
                    }
                }
            }

            if ((modify & RefreshFor.Font) == RefreshFor.Font)
            {
                for (int i = 0; i < panel.Controls.Count; i++)
                {
                    panel.Controls[i].Font = ButtonFont;
                }
            }

            if ((modify & RefreshFor.Size) == RefreshFor.Size)
            {
                panel.SuspendLayout();

                Size[] sizes = ASDFSize(panel.Size);
                if (sizes.Length != panel.Controls.Count)
                {
                    return;
                }

                for (int i = 0; i < sizes.Length; i++)
                {
                    panel.Controls[i].Size = sizes[i];
                }

                panel.ResumeLayout();

                Control b = panel.Controls[panel.Controls.Count - 1];
                b.Width = panel.Width - b.Location.X - (buttonMargin.Left + buttonMargin.Right);
            }
        }

        private void RefreshZXCVRow(FlowLayoutPanel panel, RefreshFor modify)
        {
            if (ZXCVRowScancodes.Length != panel.Controls.Count)
            {
                return;
            }

            if ((modify & RefreshFor.Label) == RefreshFor.Label)
            {
                for (int i = 0; i < panel.Controls.Count; i++)
                {
                    SoftKey key = panel.Controls[i] as SoftKey;
                    ChangeStyleForKey(key);
                    key.Text = GetDisplayStringFromVK(key.ScanCode);
                }
            }

            if ((modify & RefreshFor.Font) == RefreshFor.Font)
            {
                for (int i = 0; i < panel.Controls.Count; i++)
                {
                    panel.Controls[i].Font = ButtonFont;
                }
            }

            if ((modify & RefreshFor.Size) == RefreshFor.Size)
            {
                panel.SuspendLayout();

                Size[] sizes = ZXCVSize(panel.Size);
                if (sizes.Length != panel.Controls.Count)
                {
                    return;
                }

                for (int i = 0; i < sizes.Length; i++)
                {
                    panel.Controls[i].Size = sizes[i];
                }

                panel.ResumeLayout();

                Control b = panel.Controls[panel.Controls.Count - 1];
                b.Width = panel.Width - b.Location.X - (buttonMargin.Left + buttonMargin.Right);
            }
        }

        private void RefreshSpaceRow(FlowLayoutPanel panel, RefreshFor modify)
        {
            if (SpaceRowScancodes.Length != panel.Controls.Count)
            {
                return;
            }

            if ((modify & RefreshFor.Label) == RefreshFor.Label)
            {
                for (int i = 0; i < panel.Controls.Count; i++)
                {
                    SoftKey key = panel.Controls[i] as SoftKey;
                    ChangeStyleForKey(key);
                    key.Text = GetDisplayStringFromVK(key.ScanCode);
                }
            }

            if ((modify & RefreshFor.Font) == RefreshFor.Font)
            {
                for (int i = 0; i < panel.Controls.Count; i++)
                {
                    panel.Controls[i].Font = ButtonFont;
                }
            }

            if ((modify & RefreshFor.Size) == RefreshFor.Size)
            {
                panel.SuspendLayout();

                Size[] sizes = SpaceSize(panel.Size);
                if (sizes.Length != panel.Controls.Count)
                {
                    return;
                }

                for (int i = 0; i < sizes.Length; i++)
                {
                    panel.Controls[i].Size = sizes[i];
                }

                panel.ResumeLayout();

                Control b = panel.Controls[panel.Controls.Count - 1];
                b.Width = panel.Width - b.Location.X - (buttonMargin.Left + buttonMargin.Right);
            }
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
            m_glassMargins = new DwmApi.MARGINS(-1, -1, -1, -1);
            //m_glassMargins = new DwmApi.MARGINS(1, 1, 1, 1);

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

        ToolTip tt = new ToolTip();

        void SoftKey_MouseUp(object sender, MouseEventArgs e)
        {
            SoftKey key = sender as SoftKey;

            if (SetForegroundWindow() == false)
            {
                tt.RemoveAll();
                tt.AutomaticDelay = 0;
                tt.Show("Move the cursor where you want to enter text", key);
            }

            SimulateKeyPress(key);

            if (key.ScanCode == 0x14)
            {
                SetCapsLockState(key);
            }
        }

        protected virtual void SimulateKeyPress(SoftKey key)
        {
            SendInput.SendSingleScanCode((ushort)key.ScanCode, NativeMethods.KEYEVENTF.KEYDOWN);

            if (key.IsChecked == false)
            {
                SendInput.SendSingleScanCode((ushort)key.ScanCode, NativeMethods.KEYEVENTF.KEYUP);
            }

            //ResetModifiersAsNeeded((int)key.VirutalKeyCodeValue);
            ResetModifiersAsNeeded((int)key.ScanCode);
        }

        // we can't reset modifiers key immediately as modifier keys need to be pressed until client processed the input
        delegate void ResetModifiersAsNeededMethod(int scanCode);
        public void ResetModifiersAsNeeded(int scanCode)
        {
            // If called from same thread
            if (InvokeRequired == false)
            {
                // we called ourself by creating async caller
                ResetModifiersAsNeededMethod d = new ResetModifiersAsNeededMethod(ResetModifiersAsNeeded);
                d.BeginInvoke(scanCode, null, null);
                return;
            }

            // we are in seperated thread, sleep and wait
            System.Threading.Thread.Sleep(50);

            int[] autoResetVKs = new int[] {
                (int)NativeMethods.VK.LCONTROL,
                (int)NativeMethods.VK.RCONTROL,
                (int)NativeMethods.VK.LMENU,
                (int)NativeMethods.VK.RMENU,
                (int)NativeMethods.VK.LSHIFT,
                (int)NativeMethods.VK.RSHIFT
            };

            int[] autoResetScanCodes = new int[] { 0x2A, 0x36, 0x1D, 0x38, 0xE038, 0xE01D };

            if (!autoResetScanCodes.Contains(scanCode))
            {
                foreach (int vkey in autoResetVKs)
                {
                    if ((NativeMethods.GetAsyncKeyState(vkey) & 0x8000) == 0x8000)
                    {
                        SendInput.SendSingleVK((ushort)vkey, NativeMethods.KEYEVENTF.KEYUP);
                    }
                }
            }
        }

        protected void SetCapsLockState(SoftKey key)
        {
            if ((NativeMethods.GetKeyState(NativeMethods.VK.CAPITAL) & 0x1) != 0x1)
            {
                SetStyleForKeyType(key, KeyType.Normal);
                CapsLock = false;
            }
            else
            {
                SetStyleForKeyType(key, KeyType.Toggled);
                CapsLock = true;
            }
        }

        private Padding buttonMargin = new Padding(2, 3, 2, 3);

        public Padding ButtonMargin
        {
            get { return buttonMargin; }
            set { buttonMargin = value; }
        }

        private ButtonBase CreateSoftKey()
        {
            SoftKey b = new SoftKey();

            b.Margin = buttonMargin;

            b.UseMnemonic = false;

            b.BackColor = Color.Black;

            b.ParentHandle = this.Handle;

            b.BackColor = this.BackColor;

            b.MouseUp += new MouseEventHandler(SoftKey_MouseUp);

            return b;
        }

        #region Active Window Handeling

        IntPtr lastActiveHandle = IntPtr.Zero;

        public IntPtr LastActiveHandle
        {
            get { return lastActiveHandle; }
            set { lastActiveHandle = value; }
        }

        private void DockedFlow_MouseEnter(object sender, EventArgs e)
        {
            SetForegroundWindow();
        }

        private bool SetForegroundWindow()
        {
            IntPtr currentWindow = NativeMethods.GetForegroundWindow();

            if (currentWindow == Handle || currentWindow == IntPtr.Zero)
            {
                if (lastActiveHandle == IntPtr.Zero)
                {
                    return false;
                }
                else if (NativeMethods.IsWindow(lastActiveHandle))
                {
                    Debug.WriteLine("SetForegroundWindow");
                    return NativeMethods.SetForegroundWindow(lastActiveHandle);
                }
                else
                {
                    return false;
                }
            }
            return true;
        }

        protected override void WndProc(ref Message m)
        {
            const int WM_DWMCOMPOSITIONCHANGED = 0x031E;

            switch (m.Msg)
            {
                case 0x0084:
                    IntPtr currentActiveHandle = NativeMethods.GetForegroundWindow();
                    if (NativeMethods.IsWindow(currentActiveHandle) && currentActiveHandle != this.Handle)
                    {
                        lastActiveHandle = currentActiveHandle;
                    }
                    break;
                case 0x0021:
                    int hitTestResult = (int)m.LParam & 0xff;

                    // hit client
                    if (hitTestResult == 1)
                    {
                        m.Result = new IntPtr(3);
                        return;
                    }
                    break;
                case WM_DWMCOMPOSITIONCHANGED:
                    if (!DwmApi.DwmIsCompositionEnabled())
                    {
                        m_glassMargins = null;
                    }
                    else
                    {
                        InitializeAeroGlass();
                    }
                    break;
            }
            base.WndProc(ref m);
        }

        #endregion
    }
}
