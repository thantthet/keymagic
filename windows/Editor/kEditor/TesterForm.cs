using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace kEditor
{
    public partial class TesterForm : Form
    {
        KeyMagicDotNet.KeyMagicEngine Engine = null;
        public TesterForm(KeyMagicDotNet.KeyMagicEngine engine, Font f)
        {
            InitializeComponent();
            Engine = engine;
            txtInput.Font = f;
        }

        private void txtInput_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.A && e.Modifiers == Keys.Control)
            {
                txtInput.SelectAll();
                e.SuppressKeyPress = true;
            }
        }

        private void txtInput_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (Engine == null)
            {
                return;
            }

            int keyval = (int)e.KeyChar,
                keycode = 0,
                modifier = 0;

            if (KeyMagicDotNet.Util.GetKeyCodeAndModifier(keyval, ref keycode, ref modifier) == false)
            {
                keycode = keyval;
            }

            if (txtInput.SelectionLength != 0)
            {
                int sel = txtInput.SelectionStart;
                txtInput.Text = txtInput.Text.Remove(sel, txtInput.SelectionLength);
            }
            string oldContext = txtInput.Text;
            Engine.setContextText(oldContext);
            if (Engine.processKeyEvent(keyval, keycode, modifier))
            {
                string newContext = Engine.getContextText();

                if (oldContext != newContext)
                {
                    int fromEnd = oldContext.Length - txtInput.SelectionStart;

                    txtInput.Text = newContext;
                    txtInput.SelectionStart = newContext.Length - fromEnd;
                    txtInput.ScrollToCaret();
                    e.Handled = true;
                }
            }
        }
    }
}
