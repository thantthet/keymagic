using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace kEditor
{
    class FilterEditBox : TextBox
    {
        private string _emptyText = "Type to filter";
        private bool _isEmpty = true;
        private System.Drawing.Color _foreColor;

        public FilterEditBox()
        {
            setText();
        }

        public bool isEmpty()
        {
            return _isEmpty;
        }

        private void setText()
        {
            this.Text = _emptyText;
            _foreColor = this.ForeColor;
            this.ForeColor = System.Drawing.Color.Gray;
        }

        private void setColorBack()
        {
            this.ForeColor = _foreColor;
        }

        protected override void OnGotFocus(EventArgs e)
        {
            if (_isEmpty)
            {
                this.Text = "";
                setColorBack();
            }
            base.OnGotFocus(e);
        }

        protected override void OnLostFocus(EventArgs e)
        {
            if (_isEmpty)
            {
                setText();
                _isEmpty = true;
            }
            else
            {
                setColorBack();
            }
            base.OnLostFocus(e);
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            if (e.Control == true && e.KeyCode == Keys.A)
            {
                this.SelectAll();
            }
            base.OnKeyDown(e);
        }

        protected override void OnTextChanged(EventArgs e)
        {
            if (this.Text == "")
            {
                if (this.Focused == false)
                {
                    setText();
                }
                _isEmpty = true;
            }
            else
            {
                _isEmpty = false;
                setColorBack();
            }
            base.OnTextChanged(e);
        }

        public string GetText()
        {
            if (_isEmpty)
            {
                return "";
            }
            else
            {
                return this.Text;
            }
        }
    }
}
