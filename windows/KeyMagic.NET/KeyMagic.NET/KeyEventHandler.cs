using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.ComponentModel;

namespace KeyMagic
{
    class KeyEventHandler : Component
    {
        public class HotkeyMatchedEvent : EventArgs
        {
            public uint index;
            public HotkeyMatchedEvent(uint i)
            {
                index = i;
            }
        }
        public event EventHandler<HotkeyMatchedEvent> HotkeyMatched;
        public event EventHandler<System.Windows.Forms.MouseEventArgs> MouseDown;

        private void OnHotkeyMatched(uint index)
        {
            if (HotkeyMatched != null) HotkeyMatched(this, new HotkeyMatchedEvent(index));
        }

        private void OnMouseDown(System.Windows.Forms.MouseButtons mb, int clicks, int x, int y, int delta)
        {
            if (MouseDown != null) MouseDown(this, new System.Windows.Forms.MouseEventArgs(mb, clicks, x, y, delta));
        }

        private IntPtr hhk;
        private IntPtr hhm;
        private KeyMagicDotNet.KeyMagicEngine engine;

        public KeyMagicDotNet.KeyMagicEngine Engine
        {
            get { return engine; }
            set { engine = value; }
        }
        private Hotkey[] hotkeys = new Hotkey[1];

        public Hotkey[] Hotkeys
        {
            get { return hotkeys; }
            set { hotkeys = value; }
        }

        NativeMethods.HookProc khookProc;
        NativeMethods.HookProc mhookProc;

        public void Install()
        {
            khookProc = new NativeMethods.HookProc(KeyboardHookProc);
            mhookProc = new NativeMethods.HookProc(MouseHookProc);

            using (Process process = Process.GetCurrentProcess())
            using (ProcessModule module = process.MainModule)
            {
                hhk = NativeMethods.SetWindowsHookEx(NativeMethods.HookType.WH_KEYBOARD_LL, khookProc, NativeMethods.GetModuleHandle(module.ModuleName), 0);
                hhm = NativeMethods.SetWindowsHookEx(NativeMethods.HookType.WH_MOUSE_LL, mhookProc, NativeMethods.GetModuleHandle(module.ModuleName), 0);
            }
        }

        public void UnInstall()
        {
            if (hhk != null)
            {
                NativeMethods.UnhookWindowsHookEx(hhk);
                khookProc = null;
            }
            if (hhm != null)
            {
                NativeMethods.UnhookWindowsHookEx(hhm);
                mhookProc = null;
            }
        }

        int MouseHookProc(int code, IntPtr wParam, IntPtr lParam)
        {
            try
            {
                if (code < 0 || engine == null)
                {
                    return NativeMethods.CallNextHookEx(hhm, code, wParam, lParam);
                }

                NativeMethods.MSLLHOOKSTRUCT mHooks = Marshal.PtrToStructure(lParam, typeof(NativeMethods.MSLLHOOKSTRUCT)) as NativeMethods.MSLLHOOKSTRUCT;

                switch (wParam.ToInt32())
                {
                    case 0x0201:
                        OnMouseDown(System.Windows.Forms.MouseButtons.Left, 1, mHooks.pt.x, mHooks.pt.y, 0);
                        break;
                    case 0x0204:
                        OnMouseDown(System.Windows.Forms.MouseButtons.Right, 1, mHooks.pt.x, mHooks.pt.y, 0);
                        break;
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
                Debug.WriteLine(e.StackTrace);
            }
            return NativeMethods.CallNextHookEx(hhm, code, wParam, lParam);
        }

        int KeyboardHookProc(int code, IntPtr wParam, IntPtr lParam)
        {
            try
            {
                if (code < 0)
                {
                    return NativeMethods.CallNextHookEx(hhk, code, wParam, lParam);
                }

                NativeMethods.KBDLLHOOKSTRUCT kbHooks = Marshal.PtrToStructure(lParam, typeof(NativeMethods.KBDLLHOOKSTRUCT)) as NativeMethods.KBDLLHOOKSTRUCT;

                //Debug.Write(kbHooks.flags + ":");
                //Debug.WriteLine((System.Windows.Forms.Keys)kbHooks.vkCode);

                if ((kbHooks.flags & NativeMethods.KBDLLHOOKSTRUCTFlags.LLKHF_UP) == NativeMethods.KBDLLHOOKSTRUCTFlags.LLKHF_UP || kbHooks.vkCode == (uint)System.Windows.Forms.Keys.Packet)
                {
                    return NativeMethods.CallNextHookEx(hhk, code, wParam, lParam);
                }

                uint dwProcessId;
                uint threadId = NativeMethods.GetWindowThreadProcessId(NativeMethods.GetForegroundWindow(), out dwProcessId);
                //if (threadId != NativeMethods.GetCurrentThreadId())
                //{
                //    NativeMethods.AttachThreadInput(NativeMethods.GetCurrentThreadId(), threadId, true);
                //}

                byte[] keys = new byte[256];
                
                //NativeMethods.GetKeyboardState(keys);
                for (int i = 0; i < 256; i++)
                {
                    keys[i] = (byte)(NativeMethods.GetAsyncKeyState(i) >> 8);
                }

                int modifier = 0;
                bool CTRL, ALT, SHIFT;
                if (CTRL = (keys[(int)NativeMethods.VK.CONTROL] & 0x80) != 0)
                {
                    modifier |= (int)KeyMagicDotNet.KeyMagicEngine.ModifierMask.CTRL_MASK;
                }
                if (ALT = (keys[(int)NativeMethods.VK.MENU] & 0x80) != 0)
                {
                    modifier |= (int)KeyMagicDotNet.KeyMagicEngine.ModifierMask.ALT_MASK;
                }
                if (SHIFT = (keys[(int)NativeMethods.VK.SHIFT] & 0x80) != 0)
                {
                    modifier |= (int)KeyMagicDotNet.KeyMagicEngine.ModifierMask.SHIFT_MASK;
                }

                if (CheckHotkeys(kbHooks.vkCode, CTRL, ALT, SHIFT)) return 1;

                if (engine == null)
                {
                    //Debug.WriteLine("No Engine");
                    return NativeMethods.CallNextHookEx(hhk, code, wParam, lParam);
                }

                NativeMethods.VK[] modifierList = {
                                                              NativeMethods.VK.CONTROL,
                                                              NativeMethods.VK.MENU,
                                                              NativeMethods.VK.SHIFT,
                                                              NativeMethods.VK.LCONTROL,
                                                              NativeMethods.VK.RCONTROL,
                                                              NativeMethods.VK.LMENU,
                                                              NativeMethods.VK.RMENU,
                                                              NativeMethods.VK.LSHIFT, 
                                                              NativeMethods.VK.RSHIFT,
                                                              NativeMethods.VK.CAPITAL
                                                          };
                engine.SetKeyStates(new Byte[256]);
                foreach (NativeMethods.VK key in modifierList)
                {
                    engine.SetKeyState((int)key, keys[(int)key]);
                }

                IntPtr HKLCurrent = NativeMethods.GetKeyboardLayout(threadId);

                //uint scanCode = NativeMethods.MapVirtualKeyEx(kbHooks.vkCode, NativeMethods.MapVirtualKeyMapTypes.MAPVK_VK_TO_VSC, HKLCurrent);
                //if (scanCode == 0)
                //{
                //    Debug.WriteLine("No ScanCode");
                //    return NativeMethods.CallNextHookEx(hhk, code, wParam, lParam);
                //}

                StringBuilder TranslatedChar = new StringBuilder();

                uint vkey = kbHooks.vkCode;
                if (Engine.Options.posBased)
                {
                    vkey = NativeMethods.MapVirtualKeyEx(kbHooks.scanCode, NativeMethods.MapVirtualKeyMapTypes.MAPVK_VSC_TO_VK, (IntPtr)0x04090409);
                    NativeMethods.ToUnicodeEx(vkey, kbHooks.scanCode, keys, TranslatedChar, 1, 0, (IntPtr)0x04090409);
                }
                else
                {
                    NativeMethods.ToUnicodeEx(vkey, kbHooks.scanCode, keys, TranslatedChar, 1, 0, HKLCurrent);
                }

                Debug.WriteLine("TranslatedChar = " + TranslatedChar);

                String contextBefore = engine.GetContextText();

                int keyval = (int)(TranslatedChar.Length > 0 ? TranslatedChar[0] : 0);

                int keycode = 0, mod = 0;
                KeyMagicDotNet.Util.GetKeyCodeAndModifier(keyval, ref keycode, ref mod);

                Debug.WriteLine(string.Format("G:0x{0:X} 0x{1:X} {2}", keyval, keycode, mod));
                Debug.WriteLine(string.Format("M:0x{0:X} 0x{1:X} {2}", keyval, vkey, modifier));

                if (engine.ProcessKeyEvent((int)keyval, (int)vkey, modifier))
                {
                    SendInput.SendDifference(contextBefore, engine.GetContextText());
                    return 1;
                }
                else if ((NativeMethods.GetKeyState(NativeMethods.VK.CONTROL) & 0x80) != 0 || (NativeMethods.GetKeyState(NativeMethods.VK.MENU) & 0x80) != 0)
                {
                    Debug.WriteLine("Modifier key was pressed. Engine reset");
                    engine.Reset();
                }
                Debug.WriteLine("No Match");
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
                Debug.WriteLine(e.StackTrace);
            }
            return NativeMethods.CallNextHookEx(hhk, code, wParam, lParam);
        }

        private bool CheckHotkeys(uint key, bool CTRL, bool ALT, bool SHIFT)
        {
            switch (key)
            {
                case 0xA0:
                case 0xA1:
                case 0xA2:
                case 0xA3:
                case 0xA4:
                case 0xA5:
                    key = 0; // assume keycode as 0 if they are modifiers
                    break;
            }

            try
            {
                uint index = 0;
                foreach (Hotkey hk in hotkeys)
                {
                    index++;

                    if (hk.ToInt() == 0) continue;
                    if (hk.KeyChar != key) continue;
                    if (hk.Ctrl != CTRL) continue;
                    if (hk.Alt != ALT) continue;
                    if (hk.Shift != SHIFT) continue;

                    OnHotkeyMatched(index - 1);
                    return true;
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
                Debug.WriteLine(e.StackTrace);
            }
            return false;
        }
    }
}
