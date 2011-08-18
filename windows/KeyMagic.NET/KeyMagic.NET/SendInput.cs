using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace KeyMagic
{
    class SendInput
    {
        public static uint SendSingleScanCode(ushort scanCode, NativeMethods.KEYEVENTF dwFlags)
        {
            NativeMethods.KEYBOARDINPUT ip = new NativeMethods.KEYBOARDINPUT();

            ip.type = NativeMethods.INPUT_KEYBOARD;
            ip.wScan = scanCode;
            ip.dwExtraInfo = 0;
            ip.dwFlags = dwFlags | NativeMethods.KEYEVENTF.SCANCODE;
            if ((scanCode & 0xE000) == 0xE000)
            {
                ip.dwFlags |= NativeMethods.KEYEVENTF.EXTENDEDKEY;
            }
            ip.time = 0;
            ip.wVk = 0;

            return NativeMethods.SendInput(1, new NativeMethods.KEYBOARDINPUT[] { ip }, Marshal.SizeOf(typeof(NativeMethods.KEYBOARDINPUT)));
        }

        public static uint SendSingleVK(ushort wVk, NativeMethods.KEYEVENTF dwFlags)
        {
            NativeMethods.KEYBOARDINPUT ip = new NativeMethods.KEYBOARDINPUT();

            ip.type = NativeMethods.INPUT_KEYBOARD;
            ip.wScan = 0;
            ip.dwExtraInfo = 0;
            ip.dwFlags = dwFlags;
            ip.time = 0;
            ip.wVk = wVk;

            return NativeMethods.SendInput(1, new NativeMethods.KEYBOARDINPUT[] { ip }, Marshal.SizeOf(typeof(NativeMethods.KEYBOARDINPUT)));
        }

        public static void SendBackspace(int count)
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
                ip.wVk = (ushort)NativeMethods.VK.BACK;
                NativeMethods.SendInput(1, new NativeMethods.KEYBOARDINPUT[] { ip }, Marshal.SizeOf(typeof(NativeMethods.KEYBOARDINPUT)));

                ip.wScan = 0;
                ip.dwFlags = NativeMethods.KEYEVENTF.KEYUP;
                ip.wVk = (ushort)NativeMethods.VK.BACK;
                NativeMethods.SendInput(1, new NativeMethods.KEYBOARDINPUT[] { ip }, Marshal.SizeOf(typeof(NativeMethods.KEYBOARDINPUT)));
            }
        }

        public static void SendDifference(String contextBefore, String contextAfter)
        {
            if (contextBefore == contextAfter)
            {
                return;
            }

            int lengthBefore = contextBefore.Length;
            int lengthAfter = contextAfter.Length;

            if (contextBefore.Length > contextAfter.Length)
            {
                SendBackspace(contextBefore.Length - contextAfter.Length);
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
                SendBackspace(lengthBefore - match);
            }

            if (match < contextAfter.Length)
            {
                SendKeyStrokes(contextAfter.Substring(match));
            }
        }

        private static uint SendKeyStrokes(String s)
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

            return cSent / 2;
        }
    }
}
