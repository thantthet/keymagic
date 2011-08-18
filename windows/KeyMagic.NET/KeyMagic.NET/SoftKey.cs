using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Diagnostics;
using System.Drawing.Drawing2D;
using System.ComponentModel;
using System.Windows.Forms.VisualStyles;
using System.Runtime.InteropServices;

namespace KeyMagic
{
    public class SoftKey : ButtonBase
    {
        private bool isDown = false;
        private bool isChecked = false;
        private bool isHovering = false;

        private IntPtr parentHandle;

        public IntPtr ParentHandle
        {
            get { return parentHandle; }
            set { parentHandle = value; }
        }

        public bool IsChecked
        {
            get { return isChecked && checkAble; }
            set
            {
                if (isChecked != value)
                {
                    isChecked = value;
                    Invalidate();
                }
            }
        }

        [Category("Behavior"), DefaultValue(false)]
        public bool CheckAble
        {
            get { return checkAble; }
            set { checkAble = value; }
        }
        bool checkAble = false;

        private uint scanCode;

        public uint ScanCode
        {
            get { return scanCode; }
            set { scanCode = value; }
        }

        private Color borderColor = Color.Gray;

        public Color BorderColor
        {
            get { return borderColor; }
            set
            {
                if (borderColor != value)
                {
                    borderColor = value;
                    Invalidate();
                }
            }
        }

        public SoftKey()
            : base()
        {
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.UserPaint, true);
            SetStyle(ControlStyles.DoubleBuffer, true);
            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            SetStyle(ControlStyles.ResizeRedraw, true);
            SetStyle(ControlStyles.SupportsTransparentBackColor, true);
            SetStyle(ControlStyles.Selectable, false);
            SetStyle(ControlStyles.SupportsTransparentBackColor, true);
        }

        Color[] normalColor = new Color[] { Color.White, Color.LightGray };

        public Color[] NormalColor
        {
            get { return normalColor; }
            set
            {
                normalColor = value;
                Invalidate();
            }
        }

        Color[] downColor = new Color[] { Color.FromArgb(229, 229, 229), Color.FromArgb(193, 193, 193) };

        public Color[] DownColor
        {
            get { return downColor; }
            set
            {
                downColor = value;
                Invalidate();
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            if (Width < 1 && Height < 1) return;

            Graphics g = e.Graphics;

            g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;

            if (DwmApi.DwmIsCompositionEnabled())
            {
                g.Clear(Color.Transparent);
            }
            else
            {
                g.Clear(SystemColors.ControlLightLight);
            }

            Rectangle border = Rectangle.FromLTRB(0, 0, Width - 1, Height - 1);

            using (GraphicsPath roundBorderPath = RoundedRectangle.Create(border))
            {
                if (isDown || (isChecked && checkAble))
                {
                    using (LinearGradientBrush gradientBrush = new LinearGradientBrush(border, downColor[0], downColor[1], LinearGradientMode.Vertical))
                    {
                        g.FillPath(gradientBrush, roundBorderPath);
                    }
                }
                else
                {
                    using (LinearGradientBrush gradientBrush = new LinearGradientBrush(border, normalColor[0], normalColor[1], LinearGradientMode.Vertical))
                    {
                        g.FillPath(gradientBrush, roundBorderPath);
                    }
                }

                if (isHovering)
                {
                    using (Pen p = new Pen(Color.FromArgb(80, SystemColors.Highlight), 3))
                    {
                        g.DrawPath(p, roundBorderPath);
                    }

                    using (Pen p = new Pen(Color.FromArgb(150, SystemColors.Highlight), 1))
                    {
                        g.DrawPath(p, roundBorderPath);
                    }
                }
                else
                {
                    using (Pen p = new Pen(Color.FromArgb(80, BorderColor), 2))
                    {
                        g.DrawPath(p, roundBorderPath);
                    }

                    using (Pen p = new Pen(Color.FromArgb(150, BorderColor), 1))
                    {
                        g.DrawPath(p, roundBorderPath);
                    }
                }
            }

            g.TextRenderingHint = System.Drawing.Text.TextRenderingHint.AntiAlias;
            TextFormatFlags tff = TextFormatFlags.VerticalCenter | TextFormatFlags.HorizontalCenter | TextFormatFlags.SingleLine | TextFormatFlags.NoPrefix;

            if (DwmApi.DwmIsCompositionEnabled())
            {
                DrawTextOnGlass(g, Text, Font, ClientRectangle, ForeColor, tff);
            }
            else
            {
                TextRenderer.DrawText(g, Text, Font, ClientRectangle, ForeColor, tff);
            }
        }

        private static int GetDTTextFormatFlags(TextFormatFlags flags)
        {
            if ((((ulong)flags) & 0xffffffffff000000L) == 0L)
            {
                return (int)flags;
            }
            return (((int)flags) & ((int)0xffffff));
        }

        //Taken from RibbonBar and changed some parameters
        private void DrawTextOnGlass(Graphics g, string Text, Font Font, Rectangle Rect, Color ForeColor, TextFormatFlags Format)
        {
            IntPtr destdc = g.GetHdc();
            IntPtr Memdc = NativeMethods.CreateCompatibleDC(destdc);   // Set up a memory DC where we'll draw the text.
            IntPtr bitmap;
            IntPtr bitmapOld = IntPtr.Zero;
            IntPtr logfnotOld;
            
            NativeMethods.BITMAPINFO dib = new NativeMethods.BITMAPINFO();
            dib.bmiHeader.biHeight = -Rect.Height;
            dib.bmiHeader.biWidth = Rect.Width;
            dib.bmiHeader.biPlanes = 1;
            dib.bmiHeader.biSize = Marshal.SizeOf(typeof(NativeMethods.BITMAPINFOHEADER));
            dib.bmiHeader.biBitCount = 32;
            dib.bmiHeader.biCompression = NativeMethods.BI_RGB;
            if (!(NativeMethods.SaveDC(Memdc) == 0))
            {
                bitmap = NativeMethods.CreateDIBSection(Memdc, ref dib, NativeMethods.DIB_RGB_COLORS, 0, IntPtr.Zero, 0);   // Create a 32-bit bmp for use in offscreen drawing when glass is on
                if (!(bitmap == IntPtr.Zero))
                {
                    bitmapOld = NativeMethods.SelectObject(Memdc, bitmap);
                    IntPtr hFont = Font.ToHfont();
                    logfnotOld = NativeMethods.SelectObject(Memdc, hFont);

                    NativeMethods.BitBlt(Memdc, Rect.Left, Rect.Top, Rect.Width, Rect.Height, destdc, 0, 0, NativeMethods.SRCCOPY);

                    VisualStyleRenderer renderer = new VisualStyleRenderer(VisualStyleElement.Window.Caption.Active);
                    
                    NativeMethods.DTTOPTS dttOpts = new NativeMethods.DTTOPTS();

                    dttOpts.dwSize = (uint)Marshal.SizeOf(typeof(NativeMethods.DTTOPTS));

                    dttOpts.dwFlags = NativeMethods.DTT_COMPOSITED;

                    NativeMethods.RECT rc2 = new NativeMethods.RECT(Rect.Left, Rect.Top, Rect.Right, Rect.Bottom);

                    int dtter = NativeMethods.DrawThemeTextEx(renderer.Handle, Memdc, 0, 0, Text, -1, GetDTTextFormatFlags(Format), ref rc2, ref dttOpts);

                    NativeMethods.BitBlt(destdc, Rect.Left, Rect.Top, Rect.Width, Rect.Height, Memdc, 0, 0, NativeMethods.SRCCOPY);

                    NativeMethods.SelectObject(Memdc, bitmapOld);
                    NativeMethods.SelectObject(Memdc, logfnotOld);

                    NativeMethods.DeleteObject(hFont);
                    NativeMethods.DeleteObject(bitmap);

                    renderer = null;
                }
            }

            NativeMethods.ReleaseDC(Memdc, -1);
            NativeMethods.DeleteDC(Memdc);

            g.ReleaseHdc(destdc);
        }

        protected override void OnKeyDown(KeyEventArgs e) { }
        protected override void OnKeyUp(KeyEventArgs e) { }
        protected override void OnKeyPress(KeyPressEventArgs e) { }

        protected override void OnMouseEnter(EventArgs eventargs)
        {
            isHovering = true;
            base.OnMouseEnter(eventargs);

            Invalidate();
        }

        protected override void OnMouseLeave(EventArgs eventargs)
        {
            isHovering = false;
            base.OnMouseLeave(eventargs);

            Invalidate();
        }

        protected override void OnClick(EventArgs e)
        {
            isChecked = !isChecked;

            Invalidate();

            base.OnClick(e);
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            isDown = true;

            base.OnMouseDown(e);
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            isDown = false;

            Invalidate();

            base.OnMouseUp(e);
        }
    }
}
