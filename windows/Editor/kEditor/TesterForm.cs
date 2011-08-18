using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace kEditor
{
    public partial class TesterForm : Form
    {
        public TesterForm(KeyMagicDotNet.KeyMagicEngine engine, Font f)
        {
            InitializeComponent();

            txtInput.Font = f;
            txtInput.Engine = engine;
        }

        private void txtInput_KeyDown(object sender, KeyEventArgs e)
        {
            TextBox tbx = sender as TextBox;
            if (e.Control && e.KeyCode == Keys.A)
            {
                txtInput.Engine.Reset();
                tbx.SelectAll();
                e.SuppressKeyPress = true;
            }
        }
    }
}
