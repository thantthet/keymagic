namespace KeyMagic
{
    partial class HotkeyControl
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.flowHotkey = new System.Windows.Forms.FlowLayoutPanel();
            this.chkCtrl = new System.Windows.Forms.CheckBox();
            this.chkShift = new System.Windows.Forms.CheckBox();
            this.chkAlt = new System.Windows.Forms.CheckBox();
            this.cboKey = new System.Windows.Forms.ComboBox();
            this.flowHotkey.SuspendLayout();
            this.SuspendLayout();
            // 
            // flowHotkey
            // 
            this.flowHotkey.Controls.Add(this.chkCtrl);
            this.flowHotkey.Controls.Add(this.chkShift);
            this.flowHotkey.Controls.Add(this.chkAlt);
            this.flowHotkey.Controls.Add(this.cboKey);
            this.flowHotkey.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flowHotkey.Location = new System.Drawing.Point(0, 0);
            this.flowHotkey.Name = "flowHotkey";
            this.flowHotkey.Size = new System.Drawing.Size(202, 28);
            this.flowHotkey.TabIndex = 9;
            this.flowHotkey.WrapContents = false;
            // 
            // chkCtrl
            // 
            this.chkCtrl.AutoSize = true;
            this.chkCtrl.Location = new System.Drawing.Point(3, 6);
            this.chkCtrl.Margin = new System.Windows.Forms.Padding(3, 6, 3, 3);
            this.chkCtrl.Name = "chkCtrl";
            this.chkCtrl.Size = new System.Drawing.Size(41, 17);
            this.chkCtrl.TabIndex = 0;
            this.chkCtrl.Text = "Ctrl";
            this.chkCtrl.UseVisualStyleBackColor = true;
            // 
            // chkShift
            // 
            this.chkShift.AutoSize = true;
            this.chkShift.Location = new System.Drawing.Point(50, 6);
            this.chkShift.Margin = new System.Windows.Forms.Padding(3, 6, 3, 3);
            this.chkShift.Name = "chkShift";
            this.chkShift.Size = new System.Drawing.Size(47, 17);
            this.chkShift.TabIndex = 2;
            this.chkShift.Text = "Shift";
            this.chkShift.UseVisualStyleBackColor = true;
            // 
            // chkAlt
            // 
            this.chkAlt.AutoSize = true;
            this.chkAlt.Location = new System.Drawing.Point(103, 6);
            this.chkAlt.Margin = new System.Windows.Forms.Padding(3, 6, 3, 3);
            this.chkAlt.Name = "chkAlt";
            this.chkAlt.Size = new System.Drawing.Size(38, 17);
            this.chkAlt.TabIndex = 1;
            this.chkAlt.Text = "Alt";
            this.chkAlt.UseVisualStyleBackColor = true;
            // 
            // cboKey
            // 
            this.cboKey.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboKey.FormattingEnabled = true;
            this.cboKey.Items.AddRange(new object[] {
            "",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "0",
            "A",
            "B",
            "C",
            "D",
            "E",
            "F",
            "G",
            "H",
            "I",
            "J",
            "K",
            "L",
            "M",
            "N",
            "O",
            "P",
            "Q",
            "R",
            "S",
            "T",
            "U",
            "V",
            "W",
            "X",
            "Y",
            "Z",
            "F1",
            "F2",
            "F3",
            "F4",
            "F5",
            "F6",
            "F7",
            "F8",
            "F9",
            "F10",
            "F11",
            "F12",
            "`",
            "-",
            "=",
            "[",
            "]",
            "\\",
            ";",
            "\'",
            ",",
            ".",
            "/"});
            this.cboKey.Location = new System.Drawing.Point(147, 3);
            this.cboKey.Name = "cboKey";
            this.cboKey.Size = new System.Drawing.Size(44, 21);
            this.cboKey.TabIndex = 3;
            // 
            // HotkeyControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.flowHotkey);
            this.Name = "HotkeyControl";
            this.Size = new System.Drawing.Size(202, 28);
            this.flowHotkey.ResumeLayout(false);
            this.flowHotkey.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.FlowLayoutPanel flowHotkey;
        private System.Windows.Forms.CheckBox chkCtrl;
        private System.Windows.Forms.CheckBox chkAlt;
        private System.Windows.Forms.CheckBox chkShift;
        private System.Windows.Forms.ComboBox cboKey;
    }
}
