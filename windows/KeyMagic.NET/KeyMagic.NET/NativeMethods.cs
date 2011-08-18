using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Windows.Forms;

namespace KeyMagic
{
    public class NativeMethods
    {
        #region WIN32 Imports

        public enum HookType : int
        {
            WH_JOURNALRECORD = 0,
            WH_JOURNALPLAYBACK = 1,
            WH_KEYBOARD = 2,
            WH_GETMESSAGE = 3,
            WH_CALLWNDPROC = 4,
            WH_CBT = 5,
            WH_SYSMSGFILTER = 6,
            WH_MOUSE = 7,
            WH_HARDWARE = 8,
            WH_DEBUG = 9,
            WH_SHELL = 10,
            WH_FOREGROUNDIDLE = 11,
            WH_CALLWNDPROCRET = 12,
            WH_KEYBOARD_LL = 13,
            WH_MOUSE_LL = 14
        }

        [StructLayout(LayoutKind.Sequential)]
        public class KBDLLHOOKSTRUCT
        {
            public uint vkCode;
            public uint scanCode;
            public KBDLLHOOKSTRUCTFlags flags;
            public uint time;
            public UIntPtr dwExtraInfo;
        }

        [Flags]
        public enum KBDLLHOOKSTRUCTFlags : uint
        {
            LLKHF_DOWN = 0x0,
            LLKHF_EXTENDED = 0x01,
            LLKHF_INJECTED = 0x10,
            LLKHF_ALTDOWN = 0x20,
            LLKHF_UP = 0x80,
        }

        [StructLayout(LayoutKind.Sequential)]
        public class MSLLHOOKSTRUCT
        {
            public POINT pt;
            public uint mouseData;
            public uint flags;
            public uint time;
            public UIntPtr dwExtraInfo;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct KEYBOARDINPUT
        {
            public uint type;
            public ushort wVk;
            public ushort wScan;
            public KEYEVENTF dwFlags;
            public uint time;
            public uint dwExtraInfo;
            uint unused1;
            uint unused2;
        }

        public const uint INPUT_KEYBOARD = 1;
        public enum KEYEVENTF : uint
        {
            KEYDOWN = 0x000,
            EXTENDEDKEY = 0x001,
            KEYUP = 0x002,
            UNICODE = 0x004,
            SCANCODE = 0x008

        }

        public enum VK : uint
        {
            BACK = Keys.Back,

            SHIFT = Keys.ShiftKey,
            CONTROL = Keys.ControlKey,
            MENU = Keys.Menu,

            LSHIFT = Keys.LShiftKey,
            RSHIFT = Keys.RShiftKey,

            LCONTROL = Keys.LControlKey,
            RCONTROL = Keys.RControlKey,

            LMENU = Keys.LMenu,
            RMENU = Keys.RMenu,

            CAPITAL = Keys.Capital
        }

        /// <summary>
        /// The set of valid MapTypes used in MapVirtualKey
        /// </summary>
        public enum MapVirtualKeyMapTypes : uint
        {
            /// <summary>
            /// uCode is a virtual-key code and is translated into a scan code.
            /// If it is a virtual-key code that does not distinguish between left- and
            /// right-hand keys, the left-hand scan code is returned.
            /// If there is no translation, the function returns 0.
            /// </summary>
            MAPVK_VK_TO_VSC = 0x00,

            /// <summary>
            /// uCode is a scan code and is translated into a virtual-key code that
            /// does not distinguish between left- and right-hand keys. If there is no
            /// translation, the function returns 0.
            /// </summary>
            MAPVK_VSC_TO_VK = 0x01,

            /// <summary>
            /// uCode is a virtual-key code and is translated into an unshifted
            /// character value in the low-order word of the return value. Dead keys (diacritics)
            /// are indicated by setting the top bit of the return value. If there is no
            /// translation, the function returns 0.
            /// </summary>
            MAPVK_VK_TO_CHAR = 0x02,

            /// <summary>
            /// Windows NT/2000/XP: uCode is a scan code and is translated into a
            /// virtual-key code that distinguishes between left- and right-hand keys. If
            /// there is no translation, the function returns 0.
            /// </summary>
            MAPVK_VSC_TO_VK_EX = 0x03,

            /// <summary>
            /// Not currently documented
            /// </summary>
            MAPVK_VK_TO_VSC_EX = 0x04
        }

        [DllImport("kernel32.dll")]
        public static extern uint GetCurrentThreadId();

        [DllImport("kernel32.dll", SetLastError = true, CallingConvention = CallingConvention.Winapi)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool IsWow64Process([In] IntPtr hProcess, [Out] out bool wow64Process);

        [DllImport("user32.dll", SetLastError = true)]
        public static extern uint SendInput(uint cInputs, /* [MarshalAs(UnmanagedType.LPArray)] */ KEYBOARDINPUT[] inputs, int cbSize);

        [DllImport("user32.dll")]
        public static extern IntPtr GetKeyboardLayout(uint idThread);

        [DllImport("user32.dll")]
        public static extern uint MapVirtualKeyEx(uint uCode, MapVirtualKeyMapTypes uMapType, IntPtr dwhkl);

        [DllImport("user32.dll")]
        public static extern int ToUnicodeEx(uint wVirtKey, uint wScanCode, byte[]
           lpKeyState, [Out, MarshalAs(UnmanagedType.LPWStr)] StringBuilder pwszBuff,
           int cchBuff, uint wFlags, IntPtr dwhkl);

        public delegate int HookProc(int code, IntPtr wParam, IntPtr lParam);

        [DllImport("user32.dll")]
        public static extern short GetKeyState(VK nVirtKey);

        [DllImport("user32.dll")]
        public static extern short GetKeyState(Int32 nVirtKey);

        [DllImport("user32.dll")]
        public static extern IntPtr GetForegroundWindow();

        [DllImport("user32.dll")]
        public static extern bool AttachThreadInput(uint idAttach, uint idAttachTo, bool fAttach);

        [DllImport("User32.dll")]
        public static extern short GetAsyncKeyState(Int32 vKey);

        [DllImport("User32.dll")]
        public static extern short GetAsyncKeyState(VK vKey);

        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool GetKeyboardState(byte[] lpKeyState);

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
        public static extern void SetWindowsHooks64(IntPtr hModule);

        [DllImport("InputProcessor.x64.dll", EntryPoint = "SetMainWindowsHandle")]
        public static extern void SetMainWindowsHandle64(IntPtr data);

        [DllImport("InputProcessor.x64.dll", EntryPoint = "SetMainDir", CharSet = CharSet.Unicode)]
        public static extern void SetMainDir64(String dir);

        [DllImport("user32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool UnhookWindowsHookEx(IntPtr hhk);

        [DllImport("user32.dll", SetLastError = true)]
        public static extern IntPtr SetWindowsHookEx(HookType hookType, HookProc lpfn, IntPtr hMod, uint dwThreadId);

        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public static extern int CallNextHookEx(IntPtr hhk, int nCode, IntPtr wParam, IntPtr lParam);

        [DllImport("user32.dll", EntryPoint = "SendMessageW", CharSet = CharSet.Unicode)]
        public static extern IntPtr SendMessage(IntPtr hWnd, UInt32 Msg, IntPtr wParam, String lParam);

        [DllImport("kernel32.dll", CharSet = CharSet.Ansi, SetLastError = true)]
        public static extern IntPtr GetProcAddress(IntPtr hModule, String lpProcName);

        [DllImport("user32.dll", CharSet = CharSet.Unicode, EntryPoint = "GetClassNameW")]
        public static extern int GetClassName(IntPtr hWnd, StringBuilder lpClassName, int nMaxCount);

        [DllImport("user32.dll")]
        public static extern IntPtr GetParent(IntPtr hWnd);

        [DllImport("user32.dll")]
        public static extern IntPtr WindowFromPoint(POINT pt);

        public const int GWL_EXSTYLE = (-20);

        [DllImport("user32.dll")]
        public static extern long GetWindowLong(IntPtr hWnd, int nIndex);

        [DllImport("user32.dll")]
        public static extern long SetWindowLong(IntPtr hWnd, int nIndex, long dwNewLong);

        [DllImport("user32.dll")]
        public static extern IntPtr GetWindow(IntPtr hWnd, uint wCmd);

        [DllImport("user32.dll")]
        public static extern bool SetForegroundWindow(IntPtr hWnd);

        [DllImport("user32.dll")]
        public static extern bool IsWindow(IntPtr hWnd);

        [DllImport("user32.dll")]
        public static extern IntPtr GetFocus();

        [DllImport("user32.dll")]
        public static extern bool RegisterHotKey(IntPtr hWnd, int id, uint fsModifiers, uint vk);

        [DllImport("user32.dll")]
        public static extern bool UnregisterHotKey(IntPtr hWnd, int id);

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

        [StructLayout(LayoutKind.Sequential)]
        public struct CREATESTRUCT
        {
            public IntPtr lpCreateParams;
            public IntPtr hInstance;
            public IntPtr hMenu;
            public IntPtr hwndParent;
            public int cy;
            public int cx;
            public int y;
            public int x;
            public long style;
            public IntPtr lpszName;
            public IntPtr lpszClass;
            public ulong dwExStyle;

            public override string ToString()
            {
                return string.Format("{0} {1} {2} {3} 0x{4:X} 0x{5:X} 0x{6:X} 0x{7:X}", cy, cx, y, x, style, lpszName, lpszClass, dwExStyle);
            }
        }

        public enum WM : uint
        {
            /// <summary>
            /// An application sends the WM_COPYDATA message to pass data to another application.
            /// </summary>
            COPYDATA = 0x004A
        }



        /// <summary>
        /// An uncompressed format.
        /// </summary>
        public const int BI_RGB = 0;

        /// <summary>
        /// The BITMAPINFO structure contains an array of literal RGB values.
        /// </summary>
        public const int DIB_RGB_COLORS = 0;

        /// <summary>
        /// Copies the source rectangle directly to the destination rectangle.
        /// </summary>
        public const int SRCCOPY = 0x00CC0020;

        /// <summary>
        /// Represents a point
        /// </summary>
        public struct POINT
        {
            public int x;
            public int y;

            public POINT(int x, int y)
            {
                this.x = x;
                this.y = y;
            }
        }

        /// <summary>
        /// Defines the options for the DrawThemeTextEx function.
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public struct DTTOPTS
        {
            public uint dwSize;
            public uint dwFlags;
            public uint crText;
            public uint crBorder;
            public uint crShadow;
            public int iTextShadowType;
            public POINT ptShadowOffset;
            public int iBorderSize;
            public int iFontPropId;
            public int iColorPropId;
            public int iStateId;
            public int fApplyOverlay;
            public int iGlowSize;
            public IntPtr pfnDrawTextCallback;
            public int lParam;
        }

        /// <summary>
        /// This structure describes a color consisting of relative intensities of red, green, and blue.
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public struct RGBQUAD
        {
            public byte rgbBlue;
            public byte rgbGreen;
            public byte rgbRed;
            public byte rgbReserved;
        }


        /// <summary>
        /// This structure contains information about the dimensions and color format of a device-independent bitmap (DIB).
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public struct BITMAPINFOHEADER
        {
            public int biSize;
            public int biWidth;
            public int biHeight;
            public short biPlanes;
            public short biBitCount;
            public int biCompression;
            public int biSizeImage;
            public int biXPelsPerMeter;
            public int biYPelsPerMeter;
            public int biClrUsed;
            public int biClrImportant;
        }

        /// <summary>
        /// This structure defines the dimensions and color information of a Windows-based device-independent bitmap (DIB).
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public struct BITMAPINFO
        {
            public BITMAPINFOHEADER bmiHeader;
            public RGBQUAD bmiColors;
        }

        /// <summary>
        /// Describes the width, height, and location of a rectangle.
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public struct RECT
        {
            public int left, top, right, bottom;

            public RECT(int left, int top, int right, int bottom)
            {
                this.left = left; this.top = top; this.right = right; this.bottom = bottom;
            }
        }

        /// <summary>
        /// The CreateDIBSection function creates a DIB that applications can write to directly.
        /// </summary>
        /// <param name="hdc"></param>
        /// <param name="pbmi"></param>
        /// <param name="iUsage"></param>
        /// <param name="ppvBits"></param>
        /// <param name="hSection"></param>
        /// <param name="dwOffset"></param>
        /// <returns></returns>
        [DllImport("gdi32.dll")]
        public static extern IntPtr CreateDIBSection(IntPtr hdc, ref BITMAPINFO pbmi, uint iUsage, int ppvBits, IntPtr hSection, uint dwOffset);

        /// <summary>
        /// This function transfers pixels from a specified source rectangle to a specified destination rectangle, altering the pixels according to the selected raster operation (ROP) code.
        /// </summary>
        /// <param name="hdc"></param>
        /// <param name="nXDest"></param>
        /// <param name="nYDest"></param>
        /// <param name="nWidth"></param>
        /// <param name="nHeight"></param>
        /// <param name="hdcSrc"></param>
        /// <param name="nXSrc"></param>
        /// <param name="nYSrc"></param>
        /// <param name="dwRop"></param>
        /// <returns></returns>
        [DllImport("gdi32.dll")]
        public static extern bool BitBlt(IntPtr hdc, int nXDest, int nYDest, int nWidth, int nHeight, IntPtr hdcSrc, int nXSrc, int nYSrc, uint dwRop);

        /// <summary>
        /// This function creates a memory device context (DC) compatible with the specified device.
        /// </summary>
        /// <param name="hDC"></param>
        /// <returns></returns>
        [DllImport("gdi32.dll")]
        public static extern IntPtr CreateCompatibleDC(IntPtr hDC);

        /// <summary>
        /// This function selects an object into a specified device context. The new object replaces the previous object of the same type.
        /// </summary>
        /// <param name="hDC"></param>
        /// <param name="hObject"></param>
        /// <returns></returns>
        [DllImport("gdi32.dll")]
        public static extern IntPtr SelectObject(IntPtr hDC, IntPtr hObject);

        /// <summary>
        /// The DeleteObject function deletes a logical pen, brush, font, bitmap, region, or palette, freeing all system resources associated with the object. After the object is deleted, the specified handle is no longer valid.
        /// </summary>
        /// <param name="hObject"></param>
        /// <returns></returns>
        [DllImport("gdi32.dll")]
        public static extern bool DeleteObject(IntPtr hObject);

        /// <summary>
        /// The DeleteDC function deletes the specified device context (DC).
        /// </summary>
        /// <param name="hdc"></param>
        /// <returns></returns>
        [DllImport("gdi32.dll")]
        public static extern bool DeleteDC(IntPtr hdc);

        /// <summary>
        /// The SaveDC function saves the current state of the specified device context (DC) by copying data describing selected objects and graphic modes
        /// </summary>
        /// <param name="hdc"></param>
        /// <returns></returns>
        [DllImport("gdi32.dll")]
        public static extern int SaveDC(IntPtr hdc);

        /// <summary>
        /// This function releases a device context (DC), freeing it for use by other applications. The effect of ReleaseDC depends on the type of device context.
        /// </summary>
        /// <param name="hdc"></param>
        /// <param name="state"></param>
        /// <returns></returns>
        [DllImport("user32.dll")]
        public static extern int ReleaseDC(IntPtr hdc, int state);

        /// <summary>
        /// This function retrieves a handle to a display device context (DC) for the client area of the specified window.
        /// </summary>
        /// <param name="hdc"></param>
        /// <returns></returns>
        [DllImport("user32.dll")]
        public static extern IntPtr GetDC(IntPtr hdc);

        /// <summary>
        /// Draws text using the color and font defined by the visual style. Extends DrawThemeText by allowing additional text format options.
        /// </summary>
        /// <param name="hTheme"></param>
        /// <param name="hdc"></param>
        /// <param name="iPartId"></param>
        /// <param name="iStateId"></param>
        /// <param name="text"></param>
        /// <param name="iCharCount"></param>
        /// <param name="dwFlags"></param>
        /// <param name="pRect"></param>
        /// <param name="pOptions"></param>
        /// <returns></returns>
        [DllImport("UxTheme.dll", CharSet = CharSet.Unicode)]
        public static extern int DrawThemeTextEx(IntPtr hTheme, IntPtr hdc, int iPartId, int iStateId, string text, int iCharCount, int dwFlags, ref RECT pRect, ref DTTOPTS pOptions);

        [DllImport("user32.dll")]
        public static extern int SetBkMode(IntPtr hdc, int iBkMode);

        public const int DTT_COMPOSITED = (int)(1UL << 13);
        public const int DTT_GLOWSIZE = (int)(1UL << 11); 

        [StructLayout(LayoutKind.Sequential)]
        public class DRAWTEXTPARAMS
        {
            private int cbSize;
            public int iTabLength;
            public int iLeftMargin;
            public int iRightMargin;
            public int uiLengthDrawn;
            public DRAWTEXTPARAMS()
            {
                this.cbSize = Marshal.SizeOf(typeof(DRAWTEXTPARAMS));
            }

            public DRAWTEXTPARAMS(DRAWTEXTPARAMS original)
            {
                this.cbSize = Marshal.SizeOf(typeof(DRAWTEXTPARAMS));
                this.iLeftMargin = original.iLeftMargin;
                this.iRightMargin = original.iRightMargin;
                this.iTabLength = original.iTabLength;
            }

            public DRAWTEXTPARAMS(int leftMargin, int rightMargin)
            {
                this.cbSize = Marshal.SizeOf(typeof(DRAWTEXTPARAMS));
                this.iLeftMargin = leftMargin;
                this.iRightMargin = rightMargin;
            }
        }

        [DllImport("user32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        public static extern int DrawTextExW(HandleRef hDC, string lpszString, int nCount, ref RECT lpRect, int nFormat, [In, Out] DRAWTEXTPARAMS lpDTParams);

        #endregion
    }
}
