using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace KeyMagic
{
    class KeyEventHandler
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

        private void OnHotkeyMatched(uint index)
        {
            Trace.WriteLine("OnHotkeyMatched:" + index);
            if (HotkeyMatched != null) HotkeyMatched(this, new HotkeyMatchedEvent(index));
        }

        private IntPtr hhk;
        private KeyMagicDotNet.NetKeyMagicEngine engine;

        public KeyMagicDotNet.NetKeyMagicEngine Engine
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

        public void Install()
        {
            hookProc = new NativeMethods.HookProc(keyboardProc);

            using (Process process = Process.GetCurrentProcess())
            using (ProcessModule module = process.MainModule)
            {
                hhk = NativeMethods.SetWindowsHookEx(NativeMethods.HookType.WH_KEYBOARD_LL, hookProc, NativeMethods.GetModuleHandle(module.ModuleName), 0);
            }
        }

        public void UnInstall()
        {
            if (hhk != null)
            {
                NativeMethods.UnhookWindowsHookEx(hhk);
            }
        }

        void sendBackspace(int count)
        {

            if (count < 1)
                return;

            NativeMethods.KEYBOARDINPUT ip = new NativeMethods.KEYBOARDINPUT();
            ip.type = NativeMethods.INPUT_KEYBOARD;
            ip.dwExtraInfo = 0;
            ip.time = 0;

            for (int i = 0; i < count; i++)
            {

                ip.wScan = 255;
                ip.dwFlags = 0;
                ip.wVk = (ushort)NativeMethods.VirtualKey.VK_BACK;
                NativeMethods.SendInput(1, new NativeMethods.KEYBOARDINPUT[] { ip }, Marshal.SizeOf(typeof(NativeMethods.KEYBOARDINPUT)));

                ip.wScan = 0;
                ip.dwFlags = NativeMethods.KEYEVENTF.KEYUP;
                ip.wVk = (ushort)NativeMethods.VirtualKey.VK_BACK;
                NativeMethods.SendInput(1, new NativeMethods.KEYBOARDINPUT[] { ip }, Marshal.SizeOf(typeof(NativeMethods.KEYBOARDINPUT)));
            }
        }

        public void sendDifference(String contextBefore, String contextAfter)
        {
            if (contextBefore == contextAfter)
            {
                return;
            }

            int lengthBefore = contextBefore.Length;
            int lengthAfter = contextAfter.Length;

            if (contextBefore.Length > contextAfter.Length)
            {
                sendBackspace(contextBefore.Length - contextAfter.Length);
                lengthBefore = lengthAfter;
            }

            CharEnumerator bit, ait;
            int match = 0;
            for (
                bit = contextBefore.GetEnumerator(), ait = contextAfter.GetEnumerator();
                bit.MoveNext() && ait.MoveNext();
                match++)
            {
                if (bit.Current != ait.Current)
                {
                    break;
                }
            }

            if (match < lengthBefore)
            {
                sendBackspace(lengthBefore - match);
            }

            if (match < contextAfter.Length)
            {
                sendKeyStrokes(contextAfter.Substring(match));
            }
        }

        private uint sendKeyStrokes(String s)
        {
            uint cInputs = (uint)s.Length * 2; // need twice for up and down

            NativeMethods.KEYBOARDINPUT[] ip = new NativeMethods.KEYBOARDINPUT[cInputs];

            for (int i = 0, ii = 0; ii < cInputs; i++, ii++)
            {
                ip[ii].type = NativeMethods.INPUT_KEYBOARD;
                ip[ii].dwExtraInfo = 0xDEADC0DE;
                ip[ii].dwFlags = NativeMethods.KEYEVENTF.UNICODE;
                ip[ii].time = 0;
                ip[ii].wScan = s[i];
                ip[ii].wVk = 0;

                ii++;

                ip[ii].type = NativeMethods.INPUT_KEYBOARD;
                ip[ii].dwExtraInfo = 0;
                ip[ii].dwFlags = NativeMethods.KEYEVENTF.KEYUP | NativeMethods.KEYEVENTF.UNICODE;
                ip[ii].time = 0;
                ip[ii].wScan = s[i];
                ip[ii].wVk = 0;
            }

            uint cSent = NativeMethods.SendInput(cInputs, ip, Marshal.SizeOf(typeof(NativeMethods.KEYBOARDINPUT)));

            return cSent;
        }

        int keyboardProc(int code, IntPtr wParam, IntPtr lParam)
        {
            if (code < 0)
            {
                return NativeMethods.CallNextHookEx(hhk, code, wParam, lParam);
            }

            uint dwProcessId;
            uint threadId = NativeMethods.GetWindowThreadProcessId(NativeMethods.GetForegroundWindow(), out dwProcessId);
            if (threadId != NativeMethods.GetCurrentThreadId())
            {
                NativeMethods.AttachThreadInput(NativeMethods.GetCurrentThreadId(), threadId, true);
            }

            NativeMethods.KBDLLHOOKSTRUCT kbHooks = Marshal.PtrToStructure(lParam, typeof(NativeMethods.KBDLLHOOKSTRUCT)) as NativeMethods.KBDLLHOOKSTRUCT;

            if (kbHooks.flags != NativeMethods.KBDLLHOOKSTRUCTFlags.LLKHF_DOWN)
            {
                return NativeMethods.CallNextHookEx(hhk, code, wParam, lParam);
            }

            //Trace.WriteLine("code={0} wParam={1} vkCode={2} scanCode={3} flags={4} dwExtraInfo={5}",
            //    code, wParam, kbHooks.vkCode, kbHooks.scanCode, kbHooks.flags, kbHooks.dwExtraInfo);

            byte[] keys = new byte[256];

            NativeMethods.GetKeyboardState(keys);

            Trace.WriteLine(string.Format("{0} {1} {2}",
                keys[(int)NativeMethods.VirtualKey.VK_CONTROL],
                keys[(int)NativeMethods.VirtualKey.VK_MENU],
                keys[(int)NativeMethods.VirtualKey.VK_SHIFT]));

            int modifier = 0;
            bool CTRL, ALT, SHIFT;
            if (CTRL = (keys[(int)NativeMethods.VirtualKey.VK_CONTROL] & 0x80) != 0)
            {
                modifier |= (int)KeyMagicDotNet.NetKeyMagicEngine.ModifierMask.CTRL_MASK;
            }
            if (ALT = (keys[(int)NativeMethods.VirtualKey.VK_MENU] & 0x80) != 0)
            {
                modifier |= (int)KeyMagicDotNet.NetKeyMagicEngine.ModifierMask.ALT_MASK;
            }
            if (SHIFT = (keys[(int)NativeMethods.VirtualKey.VK_SHIFT] & 0x80) != 0)
            {
                modifier |= (int)KeyMagicDotNet.NetKeyMagicEngine.ModifierMask.SHIFT_MASK;
            }

            if (CheckHotkeys(kbHooks.vkCode, CTRL, ALT, SHIFT)) return 1;

            if (engine == null)
            {
                return NativeMethods.CallNextHookEx(hhk, code, wParam, lParam);
            }

            IntPtr HKLCurrent = NativeMethods.GetKeyboardLayout(NativeMethods.GetCurrentThreadId());

            uint scanCode = NativeMethods.MapVirtualKeyEx(kbHooks.vkCode, NativeMethods.MapVirtualKeyMapTypes.MAPVK_VK_TO_VSC, HKLCurrent);
            if (scanCode == 0)
            {
                return NativeMethods.CallNextHookEx(hhk, code, wParam, lParam);
            }

            StringBuilder TranslatedChar = new StringBuilder();

            uint vkey = NativeMethods.MapVirtualKeyEx(scanCode, NativeMethods.MapVirtualKeyMapTypes.MAPVK_VSC_TO_VK, (IntPtr)0x04090409);
            int ret = NativeMethods.ToUnicodeEx(vkey, scanCode, keys, TranslatedChar, 1, 0, (IntPtr)0x04090409);
            String contextBefore = engine.getContextText();

            if (engine.processKeyEvent((int)TranslatedChar[0], (int)vkey, modifier))
            {
                sendDifference(contextBefore, engine.getContextText());
                return 1;
            }
            else if (NativeMethods.GetKeyState(NativeMethods.VirtualKey.VK_CONTROL) != 0 || NativeMethods.GetKeyState(NativeMethods.VirtualKey.VK_MENU) != 0)
            {
                engine.reset();
            }

            //Console.WriteLine(engine.getContextText());
            return NativeMethods.CallNextHookEx(hhk, code, wParam, lParam);
        }

        private bool CheckHotkeys(uint key, bool CTRL, bool ALT, bool SHIFT)
        {
            Trace.WriteLine(string.Format("CheckHotkeys:{0} {1} {2} {3}", key, CTRL, ALT, SHIFT));
            uint index = 0;
            foreach (Hotkey hk in hotkeys)
            {
                index++;
                if (hk.keyChar != key) continue;
                if (hk.ctrl != CTRL) continue;
                if (hk.alt != ALT) continue;
                if (hk.shift != SHIFT) continue;

                OnHotkeyMatched(index);

                return true;
            }

            return false;
        }

        NativeMethods.HookProc hookProc = null;
    }
}
