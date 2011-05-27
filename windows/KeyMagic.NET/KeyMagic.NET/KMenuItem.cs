using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace KeyMagic
{
    class KToolStripMenuItem : ToolStripMenuItem
    {
        //We store additional ICON to display better for icons with alpha
        private Icon _icon;

        public Icon Icon
        {
            set
            {
                _icon = value;
            }
            get
            {
                return _icon;
            }
        }

        public KToolStripMenuItem()
            : base()
        {
        }

        public KToolStripMenuItem(Image image)
            : base(image)
        {
        }

        public KToolStripMenuItem(String text)
            : base(text)
        {
        }

        public KToolStripMenuItem(String text, Image image)
            : base(text, image)
        {
        }

        public KToolStripMenuItem(string text, Image image, params ToolStripItem[] dropDownItems)
            : base(text, image, dropDownItems)
        {
        }

        public KToolStripMenuItem(string text, Image image, EventHandler onClick)
            : base(text, image, onClick)
        {
        }

        public KToolStripMenuItem(string text, Image image, EventHandler onClick, string name)
            : base(text, image, onClick, name)
        {
        }

        public KToolStripMenuItem(string text, Image image, EventHandler onClick, Keys shortcutKeys)
            : base(text, image, onClick)
        {
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            Rectangle r = e.ClipRectangle;
            Rectangle br = r;
            br.Inflate(-1, -1);

            Graphics g = e.Graphics;
            int leftPadding = 30;
            Brush textBrush = Brushes.Black;
            Font textFont = new Font(Font, FontStyle.Bold);
            int imgLeft = (leftPadding / 2) - 8;
            int imgTop = (r.Height / 2) - 8;

            g.FillRectangle(new SolidBrush(SystemColors.ControlLightLight), r);

            if (this.Checked)
            {
                textBrush = Brushes.White;
                LinearGradientBrush lgb = new LinearGradientBrush(r, ColorTranslator.FromWin32(0xf4a88b), ColorTranslator.FromWin32(0xf1895e), LinearGradientMode.Vertical);
                g.FillRectangle(lgb, br);
                g.DrawRectangle(new Pen(ColorTranslator.FromWin32(0xeda184)), br);
            }

            if (this.Selected)
            {
                textBrush = Brushes.White;
                LinearGradientBrush lgb = new LinearGradientBrush(r, ColorTranslator.FromWin32(0xf08a65), ColorTranslator.FromWin32(0xed6228), LinearGradientMode.Vertical);
                g.FillRectangle(lgb, br);
                g.DrawRectangle(new Pen(ColorTranslator.FromWin32(0xe7815b)), br);
            }

            if (_icon != null)
            {
                g.DrawIcon(_icon, imgLeft, imgTop);
            }
            else if (Image != null)
            {
                g.DrawImage(Image, imgLeft, imgTop, 16, 16);
            }
            else if (ImageIndex != -1 && Parent.ImageList != null)
            {
                g.DrawImage(Parent.ImageList.Images[ImageIndex], imgLeft, imgTop, 16, 16);
            }
            else if (ImageKey != "" && Parent.ImageList != null)
            {
                g.DrawImage(Parent.ImageList.Images[ImageKey], imgLeft, imgTop, 16, 16);
            }

            g.DrawString(this.Text, textFont, textBrush, r.Left + leftPadding, r.Top + 2);
            //TextRenderer.DrawText(e.Graphics, Text, textFont, new Point(r.Left + leftPadding, r.Top + 2), Color.Black);

            StringFormat sf = new StringFormat();
            sf.Alignment = StringAlignment.Far;
            Rectangle rect = Rectangle.FromLTRB(r.Left, r.Top + 2, r.Right - 10, r.Bottom);
            g.DrawString(this.ShortcutKeyDisplayString, textFont, textBrush, rect, sf);
        }
    }
}
