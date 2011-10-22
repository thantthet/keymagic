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
        private CharacterRange m_GlyphRange = new CharacterRange();
        public CharacterRange GlyphRange
        {
            get
            {
                return m_GlyphRange;
            }
            set
            {
                m_GlyphRange = value;
                txtFrom.Text = m_GlyphRange.First.ToString("X4");
                txtLength.Text = m_GlyphRange.Length.ToString();
            }
        }
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
                m_GlyphRange.First = first;
                m_GlyphRange.Length = length;
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
            m_GlyphRange.First = first;

            errorProvider.SetError(txtFrom, "");

            computeLast();
        }

        private void computeLast()
        {
            if (m_GlyphRange.First != 0 && m_GlyphRange.Length != 0 && (m_GlyphRange.First + m_GlyphRange.Length) <= 0xffff)
            {
                lblTo.Text = "- 0x" + (m_GlyphRange.First - 1 + m_GlyphRange.Length).ToString("X");
            }
        }

        private void txtLength_TextChanged(object sender, EventArgs e)
        {
            int length;
            int.TryParse(txtLength.Text, out length);
            m_GlyphRange.Length = length;

            errorProvider.SetError(txtLength, "");

            computeLast();
        }
    }
}
