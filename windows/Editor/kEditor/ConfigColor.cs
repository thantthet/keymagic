using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace kEditor
{
    public partial class ConfigStyles : Form
    {
        Dictionary<int, string> indexToName;
        Dictionary<int, Styler.StyleCopy> originalStyles;
        Styler styler = Styler.shared;

        public ConfigStyles()
        {
            originalStyles = new Dictionary<int, Styler.StyleCopy>();
            indexToName = new Dictionary<int, string>();

            foreach (string name in styler.GetStyleNameIndex().Keys)
            {
                Styler.StyleCopy copy = styler.GetStyle(name);

                originalStyles.Add(styler.GetStyleIndex(name), copy);
                indexToName.Add(styler.GetStyleIndex(name), name);
            }

            InitializeComponent();
        }

        private void ConfigColor_Load(object sender, EventArgs e)
        {
            Label styleLabel;
            Button foreButton, backButton, fontButton;

            flowLayoutPanel.SuspendLayout();

            foreach (string name in styler.GetStyleNameIndex().Keys)
            {
                int thisIndex = styler.GetStyleIndex(name);
                Styler.StyleCopy thisStyle = styler.GetStyle(name);

                styleLabel = new Label();
                styleLabel.Text = name;
                styleLabel.Tag = thisIndex;
                styleLabel.Font = SciStyleFont.GetFont(thisStyle);
                styleLabel.ForeColor = thisStyle.ForeColor;
                styleLabel.BackColor = thisStyle.BackColor;
                styleLabel.Margin = new Padding(0, 3, 0, 0);
                styleLabel.Size = new System.Drawing.Size(170, styleLabel.Size.Height);
                flowLayoutPanel.Controls.Add(styleLabel);

                fontButton = new Button();
                fontButton.Text = "Font";
                fontButton.FlatStyle = FlatStyle.Popup;
                fontButton.Size = new Size(40, 20);
                fontButton.Tag = thisIndex;
                fontButton.Click += new EventHandler(fontButton_Click);
                flowLayoutPanel.Controls.Add(fontButton);

                foreButton = new Button();
                foreButton.Text = "Text Color";
                foreButton.FlatStyle = FlatStyle.Popup;
                foreButton.Size = new Size(70, 20);
                foreButton.Tag = thisIndex;
                foreButton.Click += new EventHandler(foreButton_Click);
                flowLayoutPanel.Controls.Add(foreButton);

                backButton = new Button();
                backButton.Text = "Background";
                backButton.FlatStyle = FlatStyle.Popup;
                backButton.Size = new Size(80, 20);
                backButton.Tag = thisIndex;
                backButton.Click += new EventHandler(backButton_Click);
                flowLayoutPanel.Controls.Add(backButton);

                flowLayoutPanel.SetFlowBreak(backButton, true);
            }

            flowLayoutPanel.Controls.Remove(btnCancel);
            flowLayoutPanel.Controls.Remove(btnDone);

            flowLayoutPanel.Controls.Add(btnCancel);
            flowLayoutPanel.Controls.Add(btnDone);

            Size preferSize = flowLayoutPanel.PreferredSize;
            preferSize.Width += 20;
            preferSize.Height += 40;
            this.Size = preferSize;

            flowLayoutPanel.ResumeLayout();
        }

        void backButton_Click(object sender, EventArgs e)
        {
            Button thisControl = sender as Button;
            ColorDialog colorDlg = new ColorDialog();
            int styleIndex = (int)thisControl.Tag;

            var style = styler.GetStyle(styleIndex);
            colorDlg.Color = style.BackColor;
            colorDlg.FullOpen = true;

            if (colorDlg.ShowDialog(this) == System.Windows.Forms.DialogResult.OK)
            {
                style.BackColor = colorDlg.Color;
                styler.SetStyle(styleIndex, style);
                foreach (Control ctrl in flowLayoutPanel.Controls)
                {
                    if ((int)ctrl.Tag == styleIndex)
                    {
                        ctrl.BackColor = colorDlg.Color;
                        break;
                    }
                }
            }
        }

        void fontButton_Click(object sender, EventArgs e)
        {
            Button thisControl = sender as Button;
            FontDialog fontDlg = new FontDialog();
            int styleIndex = (int)thisControl.Tag;

            var style = styler.GetStyle(styleIndex);
            fontDlg.Font = SciStyleFont.GetFont(style);

            if (fontDlg.ShowDialog(this) == System.Windows.Forms.DialogResult.OK)
            {
                style.Font = fontDlg.Font.Name;
                style.Size = fontDlg.Font.Size;
                style.Bold = fontDlg.Font.Bold;
                style.Italic = fontDlg.Font.Italic;
                style.Underline = fontDlg.Font.Underline;
                styler.SetStyle(styleIndex, style);
                foreach (Control ctrl in flowLayoutPanel.Controls)
                {
                    if ((int)ctrl.Tag == styleIndex)
                    {
                        ctrl.Font = fontDlg.Font;
                        break;
                    }
                }
            }
        }

        void foreButton_Click(object sender, EventArgs e)
        {
            Button thisControl = sender as Button;
            ColorDialog colorDlg = new ColorDialog();
            int styleIndex = (int)thisControl.Tag;

            var style = styler.GetStyle(styleIndex);
            colorDlg.Color = style.ForeColor;
            colorDlg.FullOpen = true;

            if (colorDlg.ShowDialog(this) == System.Windows.Forms.DialogResult.OK)
            {
                style.ForeColor = colorDlg.Color;
                styler.SetStyle(styleIndex, style);
                foreach (Control ctrl in flowLayoutPanel.Controls)
                {
                    if ((int)ctrl.Tag == styleIndex)
                    {
                        ctrl.ForeColor = colorDlg.Color;
                        break;
                    }
                }
            }
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            foreach (int index in originalStyles.Keys)
            {
                styler.SetStyle(index, originalStyles[index]);
            }
            DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.Close();
        }

        private void btnDone_Click(object sender, EventArgs e)
        {
            styler.SaveStyles();
            DialogResult = System.Windows.Forms.DialogResult.OK;
            this.Close();
        }
    }
}
