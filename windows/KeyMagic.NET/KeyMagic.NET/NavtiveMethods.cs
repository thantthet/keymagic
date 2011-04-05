using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace KeyMagic
{
    public class NavtiveMethods
    {
        #region WIN32 Imports

        [DllImport("user32.dll", SetLastError = true)]
        public static extern uint GetWindowThreadProcessId(IntPtr hWnd, out uint lpdwProcessId);

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern IntPtr SendMessage(IntPtr hWnd, UInt32 Msg, IntPtr wParam, IntPtr lParam);

        [return: MarshalAs(UnmanagedType.Bool)]
        [DllImport("user32.dll", SetLastError = true)]
        public static extern bool PostMessage(IntPtr hWnd, uint Msg, IntPtr wParam, IntPtr lParam);

        [DllImport("kernel32.dll", EntryPoint = "LoadLibraryW", SetLastError = true, CharSet = CharSet.Auto)]
        public static extern IntPtr LoadLibrary(String lpszLib);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, EntryPoint = "GetModuleHandleW")]
        public static extern IntPtr GetModuleHandle(String moduleName);

        [DllImport("kernel32.dll")]
        public static extern int GetLastError();

        public enum MessageFilterFlag : int
        {
            MSGFLT_ADD = 1,
            MSGFLT_REMOVE = 2
        }

        [DllImport("user32.dll")]
        public static extern bool ChangeWindowMessageFilter(uint message, MessageFilterFlag dwFlag);

        [DllImport("InputProcessor.x64.dll", EntryPoint = "GetKeyProcHook")]
        public static extern IntPtr GetKeyProcHook64();

        [DllImport("InputProcessor.x64.dll", EntryPoint = "GetWndProcHook")]
        public static extern IntPtr GetWndProcHook64();

        [DllImport("InputProcessor.x64.dll", EntryPoint = "GetGetMsgProcHook")]
        public static extern IntPtr GetGetMsgProcHook64();

        [DllImport("InputProcessor.x64.dll", EntryPoint = "SetWindowsHooks", SetLastError = true)]
        static extern void SetWindowsHooks64(IntPtr hModule);

        [DllImport("InputProcessor.x64.dll", EntryPoint = "SetMainWindowsHandle")]
        public static extern void SetMainWindowsHandle64(IntPtr data);

        [DllImport("InputProcessor.x64.dll", EntryPoint = "SetMainDir", CharSet = CharSet.Unicode)]
        public static extern void SetMainDir64(String dir);

        [DllImport("user32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool UnhookWindowsHookEx(IntPtr hhk);

        [DllImport("user32.dll", EntryPoint = "SendMessageW", CharSet = CharSet.Unicode)]
        public static extern IntPtr SendMessage(IntPtr hWnd, UInt32 Msg, IntPtr wParam, String lParam);

        [DllImport("kernel32.dll", CharSet = CharSet.Ansi, SetLastError = true)]
        public static extern IntPtr GetProcAddress(IntPtr hModule, String lpProcName);

        [DllImport("user32.dll", CharSet = CharSet.Unicode, EntryPoint = "GetClassNameW")]
        public static extern int GetClassName(IntPtr hWnd, StringBuilder lpClassName, int nMaxCount);

        public static IntPtr IntPtrAlloc<T>(T param)
        {
            IntPtr retval = Marshal.AllocHGlobal(Marshal.SizeOf(param));
            Marshal.StructureToPtr(param, retval, false);
            return (retval);
        }

        public static void IntPtrFree(IntPtr preAllocated)
        {
            if (IntPtr.Zero == preAllocated) throw (new Exception("Invalid IntPtr Value."));
            Marshal.FreeHGlobal(preAllocated); preAllocated = IntPtr.Zero;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct COPYDATASTRUCT
        {
            public long dwData;
            public int cbData;
            public IntPtr lpData;
        }

        public enum WM : uint
        {
            /// <summary>
            /// An application sends the WM_COPYDATA message to pass data to another application.
            /// </summary>
            COPYDATA = 0x004A
        }
        #endregion
    }
}
