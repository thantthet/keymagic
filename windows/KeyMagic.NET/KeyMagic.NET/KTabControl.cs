using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Drawing.Drawing2D;

namespace KeyMagic
{
    class KTabControl : TabControl
    {
        int mouseIsOn = -1;
        bool initializationComplete = false;
        BufferedGraphicsContext backbufferContext;
        BufferedGraphics backbufferGraphics;
        Graphics drawingGraphics;

        public KTabControl()
            : base()
        {
            this.DoubleBuffered = false;
            this.SetStyle(ControlStyles.UserPaint, true);
            this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            this.SetStyle(ControlStyles.ResizeRedraw, true);
            this.SetStyle(ControlStyles.SupportsTransparentBackColor, true);
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, true);

            backbufferContext = BufferedGraphicsManager.Current;
            initializationComplete = true;

            RecreateBuffers();
        }

        protected override void OnResize(EventArgs e)
        {
            base.OnResize(e);
            RecreateBuffers();
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            mouseIsOn = -1;
            for (int i = 0; i < TabCount; i++)
            {
                Point point = e.Location;
                if (GetTabRect(i).Contains(point))
                {
                    mouseIsOn = i;
                    break;
                }
            }
            base.OnMouseMove(e);
        }

        protected override void OnMouseLeave(EventArgs e)
        {
            mouseIsOn = -1;
            base.OnMouseLeave(e);
        }

        private void RecreateBuffers()
        {
            // Check initialization has completed so we know backbufferContext has been assigned.
            // Check that we aren't disposing or this could be invalid.
            if (!initializationComplete)
                return;

            // We recreate the buffer with a width and height of the control. The "+ 1"
            // guarantees we never have a buffer with a width or height of 0.
            backbufferContext.MaximumBuffer = new Size(this.Width + 1, this.Height + 1);

            // Dispose of old backbufferGraphics (if one has been created already)
            if (backbufferGraphics != null)
                backbufferGraphics.Dispose();

            // Create new backbufferGrpahics that matches the current size of buffer.
            backbufferGraphics = backbufferContext.Allocate(this.CreateGraphics(),
            new Rectangle(0, 0, Math.Max(this.Width, 1), Math.Max(this.Height, 1)));

            // Assign the Graphics object on backbufferGraphics to "drawingGraphics" for easy reference elsewhere.
            drawingGraphics = backbufferGraphics.Graphics;

            // This is a good place to assign drawingGraphics.SmoothingMode if you want a better anti-aliasing technique.

            // Invalidate the control so a repaint gets called somewhere down the line.
            this.Invalidate();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            DrawControl(drawingGraphics);
            backbufferGraphics.Render(e.Graphics);

            base.OnPaint(e);
        }

        public enum Corners
        {
            None = 0,
            NorthWest = 2,
            NorthEast = 4,
            SouthEast = 8,
            SouthWest = 16,
            All = NorthWest | NorthEast | SouthEast | SouthWest,
            North = NorthWest | NorthEast,
            South = SouthEast | SouthWest,
            East = NorthEast | SouthEast,
            West = NorthWest | SouthWest
        }

        internal void DrawControl(Graphics g)
        {
            if (DwmApi.DwmIsCompositionEnabled())
            {
                g.Clear(Color.Transparent);
            }
            else
            {
                g.Clear(SystemColors.ControlLightLight);
            }
            if (TabCount > 0)
            {
                g.FillRectangle(Brushes.White, GetTabRect(0).X, GetTabRect(0).Bottom, ClientRectangle.Width, ClientRectangle.Height);
                Rectangle contentRect = Rectangle.FromLTRB(ClientRectangle.X, GetTabRect(0).Bottom, ClientRectangle.Right - 1, ClientRectangle.Bottom - 1);
                g.DrawRectangle(Pens.Gray, contentRect);
            }

            for (int i = 0; i < TabCount; i++)
            {
                TabPage tab = TabPages[i];
                Rectangle tabRect = GetTabRect(i);
                tabRect.Inflate(-2, 0);

                //draw shadow
                Rectangle shadowRect = tabRect;
                shadowRect.Offset(2, 2);

                using (GraphicsPath path = CreateCompleteTabPath(shadowRect))
                {
                    Rectangle tabBar = Rectangle.FromLTRB(ClientRectangle.Left, ClientRectangle.Top,
                        ClientRectangle.Right, tabRect.Bottom);

                    Region region = g.Clip;
                    g.SetClip(tabBar);
                    SolidBrush shadow = new SolidBrush(Color.FromArgb(100, Color.Gray));
                    g.FillPath(shadow, path);
                    g.Clip = region;
                }

                if (tab == SelectedTab)
                {
                    using (GraphicsPath path = CreateTabPath(tabRect))
                    {
                        g.SmoothingMode = SmoothingMode.AntiAlias;
                        g.FillPath(Brushes.White, path);
                        g.DrawPath(Pens.Gray, path);
                    }

                    Rectangle fillRect = tabRect;
                    fillRect.Offset(0, 2);
                    using (GraphicsPath path = CreateCompleteTabPath(fillRect))
                    {
                        g.FillPath(Brushes.White, path);
                    }
                }
                else
                {
                    using (GraphicsPath path = CreateCompleteTabPath(tabRect))
                    {
                        g.SmoothingMode = SmoothingMode.AntiAlias;
                        g.FillPath(Brushes.White, path);
                        g.DrawPath(Pens.Gray, path);
                    }
                }

                if (i == mouseIsOn)
                {
                    using (GraphicsPath path = CreateTabPath(tabRect))
                    {
                        using (Pen p = new Pen(Color.FromArgb(80, SystemColors.Highlight)))
                        {
                            p.Width = 4;
                            g.DrawPath(p, path);
                        }

                        using (Pen p = new Pen(Color.FromArgb(150, SystemColors.Highlight)))
                        {
                            p.Width = 2;
                            g.DrawPath(p, path);
                        }
                    }
                }


                StringFormat stringFormat = new StringFormat();
                stringFormat.Alignment = StringAlignment.Center;
                stringFormat.LineAlignment = StringAlignment.Center;

                g.DrawString(tab.Text, tab.Font, new SolidBrush(tab.ForeColor), tabRect, stringFormat);
            }
        }

        #region TabPath Creation - Used functions from RibbonBar
        public GraphicsPath CreateCompleteTabPath(Rectangle r)
        {
            GraphicsPath path = new GraphicsPath();
            int corner = 6;
            int rightOffset = 1;

            path.AddLine(
                r.Left, r.Bottom,
                r.Left, r.Top + corner);
            path.AddArc(
                new Rectangle(
                r.Left, r.Top,
                corner, corner),
                180, 90);
            path.AddLine(
                r.Left + corner, r.Top,
                r.Right - corner - rightOffset, r.Top);
            path.AddArc(
                new Rectangle(
                r.Right - corner - rightOffset, r.Top,
                corner, corner),
                -90, 90);
            path.AddLine(
                r.Right - rightOffset, r.Top + corner,
                r.Right - rightOffset, r.Bottom);

            path.CloseFigure();

            return path;
        }

        public GraphicsPath CreateTabPath(Rectangle r)
        {
            GraphicsPath path = new GraphicsPath();
            int corner = 6;
            int rightOffset = 1;

            path.AddLine(
                r.Left, r.Bottom,
                r.Left, r.Top + corner);
            path.AddArc(
                new Rectangle(
                r.Left, r.Top,
                corner, corner),
                180, 90);
            path.AddLine(
                r.Left + corner, r.Top,
                r.Right - corner - rightOffset, r.Top);
            path.AddArc(
                new Rectangle(
                r.Right - corner - rightOffset, r.Top,
                corner, corner),
                -90, 90);
            path.AddLine(
                r.Right - rightOffset, r.Top + corner,
                r.Right - rightOffset, r.Bottom);


            return path;
        }
        #endregion
    }
}
