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
            public uint index;
            public KeyMagicDotNet.NetKeyMagicEngine engine;

            public LayoutInfo(uint i, KeyMagicDotNet.NetKeyMagicEngine e)
            {
                index = i;
                engine = e;
            }
        }

        Dictionary<IntPtr, LayoutInfo> engines = new Dictionary<IntPtr, LayoutInfo>();

        protected override void WndProc(ref Message msg)
        {
            base.WndProc(ref msg);

            const int WM_DWMCOMPOSITIONCHANGED = 0x031E;
            const int WM_NCHITTEST = 0x84;
            const int HTCLIENT = 0x01;

            switch (msg.Msg)
            {
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

                    if (msg.LParam == Handle)
                    {
                        break;
                    }

                    Debug.WriteLine("KM_GOTFOCUS=" + msg.LParam.ToString("X"));

                    uint pid;
                    NativeMethods.GetWindowThreadProcessId(msg.LParam, out pid);
                    if (Process.GetCurrentProcess().Id == pid)
                    {
                        Debug.WriteLine("Same process with main process so ignoring.");
                        break;
                    }

                    LastClientHandle = msg.LParam;
                    ClearCheck(cmsLeft.Items);

                    try
                    {
                        int index = 0;
                        if (engines.ContainsKey(msg.LParam))
                        {
                            handler.Engine = engines[msg.LParam].engine;
                            index = (int)engines[msg.LParam].index;
                        }
                        else
                        {
                            engines[LastClientHandle] = new LayoutInfo(0, new KeyMagicDotNet.NetKeyMagicEngine());
                        }

                        if (cmsLeft.Items.Count > index)
                        {
                            String ActiveFile = ActiveKeyboardList[index].file;
                            ToolStripMenuItem item = cmsLeft.Items[index] as ToolStripMenuItem;
                            item.Checked = true;
                        
                            if (iconList.ContainsKey(ActiveFile))
                            {
                                nIcon.Icon = iconList[ActiveFile];
                            }

                            else
                            {
                                nIcon.Icon = mainIcon;
                            }
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
                    m_glassMargins = null;
                    break;
            }
        }
    }
}
