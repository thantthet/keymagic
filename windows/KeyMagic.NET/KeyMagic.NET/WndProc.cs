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
        protected override void WndProc(ref Message msg)
        {
            base.WndProc(ref msg);

            const int WM_DWMCOMPOSITIONCHANGED = 0x031E;
            const int WM_NCHITTEST = 0x84;
            const int HTCLIENT = 0x01;

            switch (msg.Msg)
            {
                case (int)DLLMSG.KM_GETKBLNAME:
                    try
                    {
                        String fileName = ActiveKeyboardList[msg.LParam.ToInt32()].file;
                        SetFileToLoad(fileName);
                    }
                    catch (Exception)
                    {
                        Marshal.WriteByte(DllPtrFileToLoad, 0);
                    }
                    break;
                case (int)DLLMSG.KM_GOTFOCUS:

                    if (msg.LParam == Handle)
                    {
                        break;
                    }

                    uint pid;
                    GetWindowThreadProcessId(msg.LParam, out pid);
                    if (Process.GetCurrentProcess().Id == pid)
                    {
                        Debug.WriteLine("Same process with me so ignoring.");
                        break;
                    }

                    LastClientHandle = msg.LParam;
                    ClearCheck(cmsLeft.Items);

                    try
                    {
                        int index = (int)msg.WParam;

                        if (index != -1 && cmsLeft.Items.Count > index)
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
