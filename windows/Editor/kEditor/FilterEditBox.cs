using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace kEditor
{
    class FilterEditBox : TextBox
    {
        private string emptyText = "Type to filter";
        private System.Drawing.Color foreColor;

        public FilterEditBox()
        {
            Text = emptyText;
        }

        public string GetText()
        {
            if (Text == emptyText)
            {
                return "";
            }
            return Text;
        }

        private void SetForeColor()
        {
            foreColor = ForeColor;
            ForeColor = System.Drawing.Color.Gray;
        }

        private void RevertForeColor()
        {
            ForeColor = foreColor;
        }

        protected override void OnGotFocus(EventArgs e)
        {
            if (Text == emptyText)
            {
                Text = "";
                RevertForeColor();
            }
            base.OnGotFocus(e);
        }

        protected override void OnLostFocus(EventArgs e)
        {
            if (Text == emptyText)
            {
                SetForeColor();
            }
            else if (Text == "")
            {
                Text = emptyText;
                SetForeColor();
            }
            else
            {
                RevertForeColor();
            }
            base.OnLostFocus(e);
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            if (e.Control == true && e.KeyCode == Keys.A)
            {
                SelectAll();
            }
            base.OnKeyDown(e);
        }

        protected override void OnTextChanged(EventArgs e)
        {
            if (Text != emptyText)
            {
                RevertForeColor();
            }
            base.OnTextChanged(e);
        }
    }
}
