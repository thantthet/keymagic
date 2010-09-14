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
        ScintillaNet.Scintilla STLControl;
        Dictionary<string, int> nameToIndex;
        Dictionary<int, string> indexToName;
        Dictionary<int, styleCopy> originalStyles;

        struct styleCopy
        {
            public Font Font;
            public Color ForeColor;
            public  Color BackColor;
        }

        public ConfigStyles(ScintillaNet.Scintilla control, Dictionary<string, int> dict)
        {
            STLControl = control;
            nameToIndex = dict;

            originalStyles = new Dictionary<int, styleCopy>();
            indexToName = new Dictionary<int, string>();

            foreach (string name in dict.Keys)
            {
                ScintillaNet.Style thisSyle = STLControl.Styles[dict[name]];
                styleCopy copy = new styleCopy();

                copy.Font = thisSyle.Font;
                copy.ForeColor = thisSyle.ForeColor;
                copy.BackColor = thisSyle.BackColor;

                originalStyles.Add(dict[name], copy);
                indexToName.Add(dict[name], name);
            }

            InitializeComponent();
        }

        private void ConfigColor_Load(object sender, EventArgs e)
        {
            Label styleLabel;
            Button foreButton, backButton, fontButton;

            flowLayoutPanel.SuspendLayout();

            foreach (string name in nameToIndex.Keys)
            {
                int thisIndex = nameToIndex[name];
                ScintillaNet.Style thisStyle = STLControl.Styles[thisIndex];

                styleLabel = new Label();
                styleLabel.Text = name;
                styleLabel.Tag = thisIndex;
                styleLabel.Font = thisStyle.Font;
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
                backButton.Text = "Back Color";
                backButton.FlatStyle = FlatStyle.Popup;
                backButton.Size = new Size(70, 20);
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

            colorDlg.Color = STLControl.Styles[styleIndex].BackColor;
            colorDlg.FullOpen = true;

            if (colorDlg.ShowDialog(this) == System.Windows.Forms.DialogResult.OK)
            {
                STLControl.Styles[styleIndex].BackColor = colorDlg.Color;
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
            fontDlg.Font = STLControl.Styles[styleIndex].Font;

            if (fontDlg.ShowDialog(this) == System.Windows.Forms.DialogResult.OK)
            {
                STLControl.Styles[styleIndex].Font = fontDlg.Font;
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

            colorDlg.Color = STLControl.Styles[styleIndex].ForeColor;
            colorDlg.FullOpen = true;

            if (colorDlg.ShowDialog(this) == System.Windows.Forms.DialogResult.OK)
            {
                STLControl.Styles[styleIndex].ForeColor = colorDlg.Color;
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
                STLControl.Styles[index].Font = originalStyles[index].Font;
                STLControl.Styles[index].ForeColor = originalStyles[index].ForeColor;
                STLControl.Styles[index].BackColor = originalStyles[index].BackColor;
            }
            DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.Close();
        }

        private void btnDone_Click(object sender, EventArgs e)
        {
            DialogResult = System.Windows.Forms.DialogResult.OK;
            this.Close();
        }
    }
}
