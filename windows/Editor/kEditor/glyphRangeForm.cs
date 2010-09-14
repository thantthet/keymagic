using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace kEditor
{
    public partial class glyphRangeForm : Form
    {
        public CharacterRange characterRange = new CharacterRange();
        public glyphRangeForm()
        {
            InitializeComponent();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            int first;
            int.TryParse(txtFrom.Text, System.Globalization.NumberStyles.AllowHexSpecifier, null, out first);

            int length;
            int.TryParse(txtLength.Text, out length);

            if (first == 0)
            {
                errorProvider.SetError(txtFrom, "Incorrect Value!");
            }
            else if (length == 0)
            {
                errorProvider.SetError(txtLength, "Incorrect Value!");
            }
            else if (first + length >= 0xffff)
            {
                errorProvider.SetError(txtLength, "Should not larger than " + (0xfffe - first));
            }
            else
            {
                characterRange.First = first;
                characterRange.Length = length;
                DialogResult = System.Windows.Forms.DialogResult.OK;
                this.Close();
            }
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.Close();
        }

        private void txtFrom_TextChanged(object sender, EventArgs e)
        {
            int first;
            int.TryParse(txtFrom.Text, System.Globalization.NumberStyles.AllowHexSpecifier, null, out first);
            characterRange.First = first;

            errorProvider.SetError(txtFrom, "");

            computeLast();
        }

        private void computeLast()
        {
            if (characterRange.First != 0 && characterRange.Length != 0 && (characterRange.First + characterRange.Length) <= 0xffff)
            {
                lblTo.Text = "- 0x" + (characterRange.First - 1 + characterRange.Length).ToString("X");
            }
        }

        private void txtLength_TextChanged(object sender, EventArgs e)
        {
            int length;
            int.TryParse(txtLength.Text, out length);
            characterRange.Length = length;

            errorProvider.SetError(txtLength, "");

            computeLast();
        }
    }
}
