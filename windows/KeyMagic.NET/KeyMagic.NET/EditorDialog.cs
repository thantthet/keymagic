using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace KeyMagic
{
    public partial class EditorDialog : Form
    {
        public EditorDialog()
        {
            InitializeComponent();
        }

        public EditorDialog(string display, string hotkey, bool enabled)
        {
            InitializeComponent();

            txtDisplay.Text = display;
            chkEnable.Checked = enabled;

            hkHotkey.Hotkey = new Hotkey(hotkey);
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
        }
    }
}
