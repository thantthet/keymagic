using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.Drawing;
using System.Runtime.InteropServices;

namespace KeyMagic
{
    public partial class frmMain
    {
        public struct LayoutInfo
        {
            public int index;
            public KeyMagicDotNet.KeyMagicEngine engine;

            public LayoutInfo(int i, KeyMagicDotNet.KeyMagicEngine e)
            {
                index = i;
                engine = e;
            }

            public override string ToString()
            {
                return string.Format("index={0},engine={1}", index, engine);
            }
        }

        Dictionary<IntPtr, LayoutInfo> engines = new Dictionary<IntPtr, LayoutInfo>();

        protected override void WndProc(ref Message msg)
        {
            base.WndProc(ref msg);

            const int WM_DWMCOMPOSITIONCHANGED = 0x031E;
            const int WM_NCHITTEST = 0x84;
            const int HTCLIENT = 0x01;
            const int WM_HOTKEY = 0x0312;

            switch (msg.Msg)
            {
                case WM_HOTKEY:
                    if (msg.WParam.ToInt32() == softKeyboardHtkyId)
                    {
                        softKeyboard.Visible = !softKeyboard.Visible; 
                    }
                    break;
                case (int)DLLMSG.KM_INPUTLANGCHANGE:
                    softKeyboard.Refresh(SoftKeyboard.RefreshFor.Label);
                    break;
                //case (int)DLLMSG.KM_GETKBLNAME:
                //    try
                //    {
                //        String fileName = ActiveKeyboardList[msg.LParam.ToInt32()].file;
                //        SetFileToLoad(fileName);
                //    }
                //    catch (Exception)
                //    {
                //        //Marshal.WriteByte(DllPtrFileToLoad, 0);
                //    }
                //    break;
                case (int)DLLMSG.KM_GOTFOCUS:
                    if (msg.LParam == Handle || msg.LParam == NativeMethods.GetParent(msg.LParam))
                    {
                    }
                    else
                    {
                        uint pid;
                        NativeMethods.GetWindowThreadProcessId(msg.LParam, out pid);
                        if (Process.GetCurrentProcess().Id == pid)
                        {
                            break;
                        }
                    }

                    LastClientHandle = msg.LParam;
                    ClearCheck(cmsLeft.Items);

                    uint dwProcessId;
                    uint threadId = NativeMethods.GetWindowThreadProcessId(NativeMethods.GetForegroundWindow(), out dwProcessId);
                    NativeMethods.AttachThreadInput(NativeMethods.GetCurrentThreadId(), threadId, true);

                    try
                    {
                        int index = 0;
                        if (engines.ContainsKey(msg.LParam))
                        {
                            handler.Engine = engines[msg.LParam].engine;
                            if (handler.Engine != null)
                            {
                                handler.Engine.Reset();
                            }
                            SoftKeyboardEngine = handler.Engine;
                            index = engines[msg.LParam].index;
                        }
                        else
                        {
                            engines[LastClientHandle] = new LayoutInfo(0, null);
                            SoftKeyboardEngine = null;
                        }

                        if (cmsLeft.Items.Count > index)
                        {
                            String ActiveFile = ActiveKeyboardList[index].file;
                            ToolStripMenuItem item = cmsLeft.Items[index] as ToolStripMenuItem;
                            item.Checked = true;

                            SetKeyboardIcon(ActiveFile);
                        }
                    }
                    catch (Exception ex)
                    {
                        Trace.WriteLine(ex.Message, "Exception");
                    }
                    break;
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
        }
    }
}
