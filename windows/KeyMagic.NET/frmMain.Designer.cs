namespace KeyMagic
{
    partial class frmMain
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.ListViewGroup listViewGroup1 = new System.Windows.Forms.ListViewGroup("Enabled", System.Windows.Forms.HorizontalAlignment.Left);
            System.Windows.Forms.ListViewGroup listViewGroup2 = new System.Windows.Forms.ListViewGroup("Disabled", System.Windows.Forms.HorizontalAlignment.Left);
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmMain));
            this.nIcon = new System.Windows.Forms.NotifyIcon(this.components);
            this.cmsRight = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.imageList = new System.Windows.Forms.ImageList(this.components);
            this.cmsLeft = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.tabControl = new KeyMagic.KTabControl();
            this.tabLayouts = new System.Windows.Forms.TabPage();
            this.tblLayouts = new System.Windows.Forms.TableLayoutPanel();
            this.lvLayouts = new System.Windows.Forms.ListView();
            this.colFileName = new System.Windows.Forms.ColumnHeader();
            this.colDisplayText = new System.Windows.Forms.ColumnHeader();
            this.colHotkey = new System.Windows.Forms.ColumnHeader();
            this.colDesc = new System.Windows.Forms.ColumnHeader();
            this.fllRight = new System.Windows.Forms.FlowLayoutPanel();
            this.btnEdit = new System.Windows.Forms.Button();
            this.btnAdd = new System.Windows.Forms.Button();
            this.btnRemove = new System.Windows.Forms.Button();
            this.fllBottom = new System.Windows.Forms.FlowLayoutPanel();
            this.btnExit = new System.Windows.Forms.Button();
            this.btnOK = new System.Windows.Forms.Button();
            this.tabSettings = new System.Windows.Forms.TabPage();
            this.showToolStripMenuItem = new KeyMagic.KToolStripMenuItem();
            this.aboutToolStripMenuItem = new KeyMagic.KToolStripMenuItem();
            this.exitToolStripMenuItem = new KeyMagic.KToolStripMenuItem();
            this.cmsRight.SuspendLayout();
            this.tabControl.SuspendLayout();
            this.tabLayouts.SuspendLayout();
            this.tblLayouts.SuspendLayout();
            this.fllRight.SuspendLayout();
            this.fllBottom.SuspendLayout();
            this.SuspendLayout();
            // 
            // nIcon
            // 
            this.nIcon.ContextMenuStrip = this.cmsRight;
            this.nIcon.Text = "KeyMagic";
            this.nIcon.Visible = true;
            this.nIcon.DoubleClick += new System.EventHandler(this.nIcon_DoubleClick);
            this.nIcon.MouseClick += new System.Windows.Forms.MouseEventHandler(this.nIcon_MouseClick);
            // 
            // cmsRight
            // 
            this.cmsRight.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.showToolStripMenuItem,
            this.aboutToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.cmsRight.Name = "menuNotify";
            this.cmsRight.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.cmsRight.Size = new System.Drawing.Size(151, 70);
            // 
            // imageList
            // 
            this.imageList.ColorDepth = System.Windows.Forms.ColorDepth.Depth8Bit;
            this.imageList.ImageSize = new System.Drawing.Size(16, 16);
            this.imageList.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // cmsLeft
            // 
            this.cmsLeft.Name = "cmsLeft";
            this.cmsLeft.Size = new System.Drawing.Size(61, 4);
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.tabLayouts);
            this.tabControl.Controls.Add(this.tabSettings);
            this.tabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl.Location = new System.Drawing.Point(0, 0);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(673, 372);
            this.tabControl.TabIndex = 0;
            this.tabControl.DrawItem += new System.Windows.Forms.DrawItemEventHandler(this.tabControl_DrawItem);
            // 
            // tabLayouts
            // 
            this.tabLayouts.Controls.Add(this.tblLayouts);
            this.tabLayouts.Location = new System.Drawing.Point(4, 25);
            this.tabLayouts.Name = "tabLayouts";
            this.tabLayouts.Padding = new System.Windows.Forms.Padding(3);
            this.tabLayouts.Size = new System.Drawing.Size(665, 343);
            this.tabLayouts.TabIndex = 0;
            this.tabLayouts.Text = "Keyboard Layouts";
            this.tabLayouts.UseVisualStyleBackColor = true;
            // 
            // tblLayouts
            // 
            this.tblLayouts.ColumnCount = 2;
            this.tblLayouts.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tblLayouts.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 90F));
            this.tblLayouts.Controls.Add(this.lvLayouts, 0, 0);
            this.tblLayouts.Controls.Add(this.fllRight, 1, 0);
            this.tblLayouts.Controls.Add(this.fllBottom, 0, 1);
            this.tblLayouts.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tblLayouts.Location = new System.Drawing.Point(3, 3);
            this.tblLayouts.Name = "tblLayouts";
            this.tblLayouts.RowCount = 2;
            this.tblLayouts.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tblLayouts.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 40F));
            this.tblLayouts.Size = new System.Drawing.Size(659, 337);
            this.tblLayouts.TabIndex = 0;
            // 
            // lvLayouts
            // 
            this.lvLayouts.AllowColumnReorder = true;
            this.lvLayouts.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.colFileName,
            this.colDisplayText,
            this.colHotkey,
            this.colDesc});
            this.lvLayouts.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lvLayouts.FullRowSelect = true;
            listViewGroup1.Header = "Enabled";
            listViewGroup1.Name = "Enabled";
            listViewGroup2.Header = "Disabled";
            listViewGroup2.Name = "Disabled";
            this.lvLayouts.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] {
            listViewGroup1,
            listViewGroup2});
            this.lvLayouts.Location = new System.Drawing.Point(3, 3);
            this.lvLayouts.Name = "lvLayouts";
            this.lvLayouts.Size = new System.Drawing.Size(563, 291);
            this.lvLayouts.SmallImageList = this.imageList;
            this.lvLayouts.TabIndex = 0;
            this.lvLayouts.UseCompatibleStateImageBehavior = false;
            this.lvLayouts.View = System.Windows.Forms.View.Details;
            this.lvLayouts.DoubleClick += new System.EventHandler(this.lvLayouts_DoubleClick);
            // 
            // colFileName
            // 
            this.colFileName.Text = "File Name";
            this.colFileName.Width = 98;
            // 
            // colDisplayText
            // 
            this.colDisplayText.Text = "Display Text";
            this.colDisplayText.Width = 127;
            // 
            // colHotkey
            // 
            this.colHotkey.Text = "Hotkey";
            this.colHotkey.Width = 81;
            // 
            // colDesc
            // 
            this.colDesc.Text = "Description";
            this.colDesc.Width = 282;
            // 
            // fllRight
            // 
            this.fllRight.Controls.Add(this.btnEdit);
            this.fllRight.Controls.Add(this.btnAdd);
            this.fllRight.Controls.Add(this.btnRemove);
            this.fllRight.Dock = System.Windows.Forms.DockStyle.Fill;
            this.fllRight.Location = new System.Drawing.Point(572, 3);
            this.fllRight.Name = "fllRight";
            this.fllRight.Size = new System.Drawing.Size(84, 291);
            this.fllRight.TabIndex = 1;
            // 
            // btnEdit
            // 
            this.fllRight.SetFlowBreak(this.btnEdit, true);
            this.btnEdit.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.btnEdit.Location = new System.Drawing.Point(3, 3);
            this.btnEdit.Name = "btnEdit";
            this.btnEdit.Size = new System.Drawing.Size(75, 23);
            this.btnEdit.TabIndex = 4;
            this.btnEdit.Text = "&Edit";
            this.btnEdit.UseVisualStyleBackColor = true;
            this.btnEdit.Click += new System.EventHandler(this.btnEdit_Click);
            // 
            // btnAdd
            // 
            this.fllRight.SetFlowBreak(this.btnAdd, true);
            this.btnAdd.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.btnAdd.Location = new System.Drawing.Point(3, 32);
            this.btnAdd.Name = "btnAdd";
            this.btnAdd.Size = new System.Drawing.Size(75, 23);
            this.btnAdd.TabIndex = 2;
            this.btnAdd.Text = "&Add";
            this.btnAdd.UseVisualStyleBackColor = true;
            this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
            // 
            // btnRemove
            // 
            this.fllRight.SetFlowBreak(this.btnRemove, true);
            this.btnRemove.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.btnRemove.Location = new System.Drawing.Point(3, 61);
            this.btnRemove.Name = "btnRemove";
            this.btnRemove.Size = new System.Drawing.Size(75, 23);
            this.btnRemove.TabIndex = 3;
            this.btnRemove.Text = "&Remove";
            this.btnRemove.UseVisualStyleBackColor = true;
            this.btnRemove.Click += new System.EventHandler(this.btnRemove_Click);
            // 
            // fllBottom
            // 
            this.tblLayouts.SetColumnSpan(this.fllBottom, 2);
            this.fllBottom.Controls.Add(this.btnExit);
            this.fllBottom.Controls.Add(this.btnOK);
            this.fllBottom.Dock = System.Windows.Forms.DockStyle.Fill;
            this.fllBottom.FlowDirection = System.Windows.Forms.FlowDirection.RightToLeft;
            this.fllBottom.Location = new System.Drawing.Point(3, 300);
            this.fllBottom.Name = "fllBottom";
            this.fllBottom.Size = new System.Drawing.Size(653, 34);
            this.fllBottom.TabIndex = 2;
            // 
            // btnExit
            // 
            this.btnExit.Location = new System.Drawing.Point(575, 3);
            this.btnExit.Name = "btnExit";
            this.btnExit.Size = new System.Drawing.Size(75, 23);
            this.btnExit.TabIndex = 0;
            this.btnExit.Text = "E&xit";
            this.btnExit.UseVisualStyleBackColor = true;
            this.btnExit.Click += new System.EventHandler(this.btnExit_Click);
            // 
            // btnOK
            // 
            this.btnOK.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.btnOK.Location = new System.Drawing.Point(494, 3);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(75, 23);
            this.btnOK.TabIndex = 0;
            this.btnOK.Text = "&OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // tabSettings
            // 
            this.tabSettings.Location = new System.Drawing.Point(4, 25);
            this.tabSettings.Name = "tabSettings";
            this.tabSettings.Padding = new System.Windows.Forms.Padding(3);
            this.tabSettings.Size = new System.Drawing.Size(665, 343);
            this.tabSettings.TabIndex = 1;
            this.tabSettings.Text = "Settings";
            this.tabSettings.UseVisualStyleBackColor = true;
            // 
            // showToolStripMenuItem
            // 
            this.showToolStripMenuItem.Name = "showToolStripMenuItem";
            this.showToolStripMenuItem.Size = new System.Drawing.Size(150, 22);
            this.showToolStripMenuItem.Text = "Show Window";
            this.showToolStripMenuItem.Click += new System.EventHandler(this.showToolStripMenuItem_Click);
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(150, 22);
            this.aboutToolStripMenuItem.Text = "About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(150, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(673, 372);
            this.Controls.Add(this.tabControl);
            this.DataBindings.Add(new System.Windows.Forms.Binding("Location", global::KeyMagic.Properties.Settings.Default, "frmMain_Location", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "frmMain";
            this.Text = "KeyMagic";
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.frmMain_Paint);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmMain_FormClosing);
            this.cmsRight.ResumeLayout(false);
            this.tabControl.ResumeLayout(false);
            this.tabLayouts.ResumeLayout(false);
            this.tblLayouts.ResumeLayout(false);
            this.fllRight.ResumeLayout(false);
            this.fllBottom.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.NotifyIcon nIcon;
        private System.Windows.Forms.TabPage tabLayouts;
        private System.Windows.Forms.TableLayoutPanel tblLayouts;
        private System.Windows.Forms.ListView lvLayouts;
        private System.Windows.Forms.ImageList imageList;
        private System.Windows.Forms.TabPage tabSettings;
        private System.Windows.Forms.ColumnHeader colFileName;
        private System.Windows.Forms.ColumnHeader colHotkey;
        private System.Windows.Forms.FlowLayoutPanel fllRight;
        private System.Windows.Forms.Button btnEdit;
        private System.Windows.Forms.Button btnAdd;
        private System.Windows.Forms.Button btnRemove;
        private System.Windows.Forms.FlowLayoutPanel fllBottom;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.ContextMenuStrip cmsRight;
        private System.Windows.Forms.ColumnHeader colDisplayText;
        private System.Windows.Forms.ColumnHeader colDesc;
        private System.Windows.Forms.Button btnExit;
        private System.Windows.Forms.ContextMenuStrip cmsLeft;
        private KTabControl tabControl;
        private KToolStripMenuItem aboutToolStripMenuItem;
        private KToolStripMenuItem showToolStripMenuItem;
        private KToolStripMenuItem exitToolStripMenuItem;

    }
}

