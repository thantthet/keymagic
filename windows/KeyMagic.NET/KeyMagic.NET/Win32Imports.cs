using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace KeyMagic
{
    public partial class frmMain
    {
        #region WIN32 Imports

        [DllImport("user32.dll", SetLastError = true)]
        static extern uint GetWindowThreadProcessId(IntPtr hWnd, out uint lpdwProcessId);

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        static extern IntPtr SendMessage(IntPtr hWnd, UInt32 Msg, IntPtr wParam, IntPtr lParam);

        [return: MarshalAs(UnmanagedType.Bool)]
        [DllImport("user32.dll", SetLastError = true)]
        static extern bool PostMessage(IntPtr hWnd, uint Msg, IntPtr wParam, IntPtr lParam);

        [DllImport("kernel32.dll", EntryPoint = "LoadLibraryW", SetLastError = true, CharSet = CharSet.Auto)]
        static extern IntPtr LoadLibrary(String lpszLib);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, EntryPoint = "GetModuleHandleW")]
        static extern IntPtr GetModuleHandle(String moduleName);

        [DllImport("kernel32.dll")]
        static extern int GetLastError();

        enum MessageFilterFlag : int
        {
            MSGFLT_ADD = 1,
            MSGFLT_REMOVE = 2
        }

        [DllImport("user32.dll")]
        static extern bool ChangeWindowMessageFilter(uint message, MessageFilterFlag dwFlag);

        [DllImport("InputProcessor.x64.dll", EntryPoint = "GetKeyProcHook")]
        static extern IntPtr GetKeyProcHook64();

        [DllImport("InputProcessor.x64.dll", EntryPoint = "GetWndProcHook")]
        static extern IntPtr GetWndProcHook64();

        [DllImport("InputProcessor.x64.dll", EntryPoint = "GetGetMsgProcHook")]
        static extern IntPtr GetGetMsgProcHook64();

        [DllImport("InputProcessor.x64.dll", EntryPoint = "SetWindowsHooks", SetLastError = true)]
        static extern void SetWindowsHooks64(IntPtr hModule);

        [DllImport("InputProcessor.x64.dll", EntryPoint = "SetMainWindowsHandle")]
        static extern void SetMainWindowsHandle64(IntPtr data);

        [DllImport("InputProcessor.x64.dll", EntryPoint = "SetMainDir", CharSet = CharSet.Unicode)]
        static extern void SetMainDir64(String dir);

        [DllImport("InputProcessor.x64.dll", EntryPoint = "GetHotkeyCount", CharSet = CharSet.Unicode)]
        static extern uint GetHotkeyCount();

        [DllImport("user32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool UnhookWindowsHookEx(IntPtr hhk);

        [DllImport("user32.dll", EntryPoint = "SendMessageW", CharSet = CharSet.Unicode)]
        static extern IntPtr SendMessage(IntPtr hWnd, UInt32 Msg, IntPtr wParam, String lParam);

        [DllImport("kernel32.dll", CharSet = CharSet.Ansi, SetLastError = true)]
        static extern IntPtr GetProcAddress(IntPtr hModule, String lpProcName);

        [DllImport("user32.dll", CharSet = CharSet.Unicode, EntryPoint = "GetClassNameW")]
        static extern int GetClassName(IntPtr hWnd, StringBuilder lpClassName, int nMaxCount);

        #endregion
    }
}
