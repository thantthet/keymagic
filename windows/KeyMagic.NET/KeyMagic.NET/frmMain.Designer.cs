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
            KeyMagic.Hotkey hotkey1 = new KeyMagic.Hotkey();
            KeyMagic.Hotkey hotkey2 = new KeyMagic.Hotkey();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmMain));
            this.nIcon = new System.Windows.Forms.NotifyIcon(this.components);
            this.cmsRight = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.showToolStripMenuItem = new KeyMagic.KToolStripMenuItem();
            this.softKeyboardToolStripMenuItem = new KeyMagic.KToolStripMenuItem();
            this.aboutToolStripMenuItem = new KeyMagic.KToolStripMenuItem();
            this.exitToolStripMenuItem = new KeyMagic.KToolStripMenuItem();
            this.imageList = new System.Windows.Forms.ImageList(this.components);
            this.cmsLeft = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.fllBottom = new System.Windows.Forms.FlowLayoutPanel();
            this.btnExit = new System.Windows.Forms.Button();
            this.btnOK = new System.Windows.Forms.Button();
            this.mainTableLayoutPanel = new System.Windows.Forms.TableLayoutPanel();
            this.tabControl = new KeyMagic.KTabControl();
            this.tabLayouts = new System.Windows.Forms.TabPage();
            this.tblLayouts = new System.Windows.Forms.TableLayoutPanel();
            this.lvLayouts = new System.Windows.Forms.ListView();
            this.colFileName = new System.Windows.Forms.ColumnHeader();
            this.colDisplayText = new System.Windows.Forms.ColumnHeader();
            this.colHotkey = new System.Windows.Forms.ColumnHeader();
            this.colVersion = new System.Windows.Forms.ColumnHeader();
            this.colDesc = new System.Windows.Forms.ColumnHeader();
            this.fllRight = new System.Windows.Forms.FlowLayoutPanel();
            this.btnEdit = new System.Windows.Forms.Button();
            this.btnAdd = new System.Windows.Forms.Button();
            this.btnRemove = new System.Windows.Forms.Button();
            this.tabSettings = new System.Windows.Forms.TabPage();
            this.grupSettings = new System.Windows.Forms.GroupBox();
            this.tableSettings = new System.Windows.Forms.TableLayoutPanel();
            this.htkyOnOff = new KeyMagic.HotkeyControl();
            this.lblOnOffKey = new System.Windows.Forms.Label();
            this.chkRunAtLogon = new System.Windows.Forms.CheckBox();
            this.htkySoftKeyboard = new KeyMagic.HotkeyControl();
            this.lblSoftKeyboard = new System.Windows.Forms.Label();
            this.tabAbout = new System.Windows.Forms.TabPage();
            this.tableAbout = new System.Windows.Forms.TableLayoutPanel();
            this.txtLicense = new System.Windows.Forms.TextBox();
            this.btnCheckUpdate = new System.Windows.Forms.Button();
            this.lblAboutTitle = new System.Windows.Forms.Label();
            this.cmsRight.SuspendLayout();
            this.fllBottom.SuspendLayout();
            this.mainTableLayoutPanel.SuspendLayout();
            this.tabControl.SuspendLayout();
            this.tabLayouts.SuspendLayout();
            this.tblLayouts.SuspendLayout();
            this.fllRight.SuspendLayout();
            this.tabSettings.SuspendLayout();
            this.grupSettings.SuspendLayout();
            this.tableSettings.SuspendLayout();
            this.tabAbout.SuspendLayout();
            this.tableAbout.SuspendLayout();
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
            this.softKeyboardToolStripMenuItem,
            this.aboutToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.cmsRight.Name = "menuNotify";
            this.cmsRight.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.cmsRight.Size = new System.Drawing.Size(151, 92);
            // 
            // showToolStripMenuItem
            // 
            this.showToolStripMenuItem.Icon = null;
            this.showToolStripMenuItem.Name = "showToolStripMenuItem";
            this.showToolStripMenuItem.Size = new System.Drawing.Size(150, 22);
            this.showToolStripMenuItem.Text = "Show Window";
            this.showToolStripMenuItem.Click += new System.EventHandler(this.showToolStripMenuItem_Click);
            // 
            // softKeyboardToolStripMenuItem
            // 
            this.softKeyboardToolStripMenuItem.Icon = null;
            this.softKeyboardToolStripMenuItem.Name = "softKeyboardToolStripMenuItem";
            this.softKeyboardToolStripMenuItem.Size = new System.Drawing.Size(150, 22);
            this.softKeyboardToolStripMenuItem.Text = "Soft Keyboard";
            this.softKeyboardToolStripMenuItem.Click += new System.EventHandler(this.softKeyboardToolStripMenuItem_Click);
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Icon = null;
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(150, 22);
            this.aboutToolStripMenuItem.Text = "About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Icon = null;
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(150, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
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
            // fllBottom
            // 
            this.fllBottom.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.fllBottom.Controls.Add(this.btnExit);
            this.fllBottom.Controls.Add(this.btnOK);
            this.fllBottom.Dock = System.Windows.Forms.DockStyle.Fill;
            this.fllBottom.FlowDirection = System.Windows.Forms.FlowDirection.RightToLeft;
            this.fllBottom.Location = new System.Drawing.Point(0, 349);
            this.fllBottom.Margin = new System.Windows.Forms.Padding(0);
            this.fllBottom.Name = "fllBottom";
            this.fllBottom.Size = new System.Drawing.Size(669, 40);
            this.fllBottom.TabIndex = 3;
            // 
            // btnExit
            // 
            this.btnExit.Location = new System.Drawing.Point(591, 3);
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
            this.btnOK.Location = new System.Drawing.Point(510, 3);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(75, 23);
            this.btnOK.TabIndex = 0;
            this.btnOK.Text = "&OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // mainTableLayoutPanel
            // 
            this.mainTableLayoutPanel.ColumnCount = 1;
            this.mainTableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.mainTableLayoutPanel.Controls.Add(this.tabControl, 0, 0);
            this.mainTableLayoutPanel.Controls.Add(this.fllBottom, 0, 1);
            this.mainTableLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainTableLayoutPanel.Location = new System.Drawing.Point(0, 0);
            this.mainTableLayoutPanel.Name = "mainTableLayoutPanel";
            this.mainTableLayoutPanel.RowCount = 2;
            this.mainTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.mainTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 40F));
            this.mainTableLayoutPanel.Size = new System.Drawing.Size(669, 389);
            this.mainTableLayoutPanel.TabIndex = 4;
            this.mainTableLayoutPanel.Paint += new System.Windows.Forms.PaintEventHandler(this.mainTableLayoutPanel_Paint);
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.tabLayouts);
            this.tabControl.Controls.Add(this.tabSettings);
            this.tabControl.Controls.Add(this.tabAbout);
            this.tabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl.Location = new System.Drawing.Point(0, 0);
            this.tabControl.Margin = new System.Windows.Forms.Padding(0);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(669, 349);
            this.tabControl.TabIndex = 0;
            // 
            // tabLayouts
            // 
            this.tabLayouts.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.tabLayouts.Controls.Add(this.tblLayouts);
            this.tabLayouts.Location = new System.Drawing.Point(4, 25);
            this.tabLayouts.Name = "tabLayouts";
            this.tabLayouts.Padding = new System.Windows.Forms.Padding(3);
            this.tabLayouts.Size = new System.Drawing.Size(661, 320);
            this.tabLayouts.TabIndex = 0;
            this.tabLayouts.Text = "Keyboard Layouts";
            // 
            // tblLayouts
            // 
            this.tblLayouts.ColumnCount = 2;
            this.tblLayouts.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tblLayouts.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 90F));
            this.tblLayouts.Controls.Add(this.lvLayouts, 0, 0);
            this.tblLayouts.Controls.Add(this.fllRight, 1, 0);
            this.tblLayouts.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tblLayouts.Location = new System.Drawing.Point(3, 3);
            this.tblLayouts.Name = "tblLayouts";
            this.tblLayouts.RowCount = 1;
            this.tblLayouts.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tblLayouts.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 314F));
            this.tblLayouts.Size = new System.Drawing.Size(655, 314);
            this.tblLayouts.TabIndex = 0;
            // 
            // lvLayouts
            // 
            this.lvLayouts.AllowColumnReorder = true;
            this.lvLayouts.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.colFileName,
            this.colDisplayText,
            this.colHotkey,
            this.colVersion,
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
            this.lvLayouts.Size = new System.Drawing.Size(559, 308);
            this.lvLayouts.SmallImageList = this.imageList;
            this.lvLayouts.Sorting = System.Windows.Forms.SortOrder.Ascending;
            this.lvLayouts.TabIndex = 0;
            this.lvLayouts.UseCompatibleStateImageBehavior = false;
            this.lvLayouts.View = System.Windows.Forms.View.Details;
            this.lvLayouts.DoubleClick += new System.EventHandler(this.lvLayouts_DoubleClick);
            // 
            // colFileName
            // 
            this.colFileName.Text = "File Name";
            this.colFileName.Width = global::KeyMagic.Properties.Settings.Default.ColFileNameWidth;
            // 
            // colDisplayText
            // 
            this.colDisplayText.Text = "Display Text";
            this.colDisplayText.Width = global::KeyMagic.Properties.Settings.Default.ColDisplayWidth;
            // 
            // colHotkey
            // 
            this.colHotkey.Text = "Hotkey";
            this.colHotkey.Width = global::KeyMagic.Properties.Settings.Default.ColHotkeyWidth;
            // 
            // colVersion
            // 
            this.colVersion.Text = "Version";
            this.colVersion.Width = global::KeyMagic.Properties.Settings.Default.ColVersionWidth;
            // 
            // colDesc
            // 
            this.colDesc.Text = "Description";
            this.colDesc.Width = global::KeyMagic.Properties.Settings.Default.ColDescWidth;
            // 
            // fllRight
            // 
            this.fllRight.Controls.Add(this.btnEdit);
            this.fllRight.Controls.Add(this.btnAdd);
            this.fllRight.Controls.Add(this.btnRemove);
            this.fllRight.Dock = System.Windows.Forms.DockStyle.Fill;
            this.fllRight.Location = new System.Drawing.Point(568, 3);
            this.fllRight.Name = "fllRight";
            this.fllRight.Size = new System.Drawing.Size(84, 308);
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
            // tabSettings
            // 
            this.tabSettings.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.tabSettings.Controls.Add(this.grupSettings);
            this.tabSettings.Location = new System.Drawing.Point(4, 25);
            this.tabSettings.Name = "tabSettings";
            this.tabSettings.Padding = new System.Windows.Forms.Padding(3);
            this.tabSettings.Size = new System.Drawing.Size(661, 320);
            this.tabSettings.TabIndex = 1;
            this.tabSettings.Text = "Settings";
            // 
            // grupSettings
            // 
            this.grupSettings.Controls.Add(this.tableSettings);
            this.grupSettings.Dock = System.Windows.Forms.DockStyle.Fill;
            this.grupSettings.Location = new System.Drawing.Point(3, 3);
            this.grupSettings.Name = "grupSettings";
            this.grupSettings.Size = new System.Drawing.Size(655, 314);
            this.grupSettings.TabIndex = 2;
            this.grupSettings.TabStop = false;
            this.grupSettings.Text = "Settings";
            // 
            // tableSettings
            // 
            this.tableSettings.ColumnCount = 2;
            this.tableSettings.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 138F));
            this.tableSettings.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableSettings.Controls.Add(this.htkyOnOff, 1, 0);
            this.tableSettings.Controls.Add(this.lblOnOffKey, 0, 0);
            this.tableSettings.Controls.Add(this.chkRunAtLogon, 1, 2);
            this.tableSettings.Controls.Add(this.htkySoftKeyboard, 1, 1);
            this.tableSettings.Controls.Add(this.lblSoftKeyboard, 0, 1);
            this.tableSettings.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableSettings.Location = new System.Drawing.Point(3, 16);
            this.tableSettings.Name = "tableSettings";
            this.tableSettings.RowCount = 3;
            this.tableSettings.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableSettings.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableSettings.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableSettings.Size = new System.Drawing.Size(649, 295);
            this.tableSettings.TabIndex = 0;
            // 
            // htkyOnOff
            // 
            this.htkyOnOff.Dock = System.Windows.Forms.DockStyle.Fill;
            hotkey1.Alt = false;
            hotkey1.Ctrl = false;
            hotkey1.KeyChar = '\0';
            hotkey1.KeyString = null;
            hotkey1.Shift = false;
            this.htkyOnOff.Hotkey = hotkey1;
            this.htkyOnOff.Location = new System.Drawing.Point(141, 3);
            this.htkyOnOff.Name = "htkyOnOff";
            this.htkyOnOff.Size = new System.Drawing.Size(505, 29);
            this.htkyOnOff.TabIndex = 2;
            // 
            // lblOnOffKey
            // 
            this.lblOnOffKey.AutoSize = true;
            this.lblOnOffKey.Location = new System.Drawing.Point(3, 10);
            this.lblOnOffKey.Margin = new System.Windows.Forms.Padding(3, 10, 3, 0);
            this.lblOnOffKey.Name = "lblOnOffKey";
            this.lblOnOffKey.Size = new System.Drawing.Size(105, 13);
            this.lblOnOffKey.TabIndex = 3;
            this.lblOnOffKey.Text = "Turn On/Off Hotkey:";
            // 
            // chkRunAtLogon
            // 
            this.chkRunAtLogon.AutoSize = true;
            this.chkRunAtLogon.Checked = global::KeyMagic.Properties.Settings.Default.RunAtStartup;
            this.chkRunAtLogon.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkRunAtLogon.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::KeyMagic.Properties.Settings.Default, "RunAtStartup", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.chkRunAtLogon.Location = new System.Drawing.Point(141, 73);
            this.chkRunAtLogon.Name = "chkRunAtLogon";
            this.chkRunAtLogon.Size = new System.Drawing.Size(137, 17);
            this.chkRunAtLogon.TabIndex = 1;
            this.chkRunAtLogon.Text = "Run KeyMagic at logon";
            this.chkRunAtLogon.UseVisualStyleBackColor = true;
            this.chkRunAtLogon.CheckedChanged += new System.EventHandler(this.chkRunAtLogon_CheckedChanged);
            // 
            // htkySoftKeyboard
            // 
            this.htkySoftKeyboard.Dock = System.Windows.Forms.DockStyle.Fill;
            hotkey2.Alt = false;
            hotkey2.Ctrl = false;
            hotkey2.KeyChar = '\0';
            hotkey2.KeyString = null;
            hotkey2.Shift = false;
            this.htkySoftKeyboard.Hotkey = hotkey2;
            this.htkySoftKeyboard.Location = new System.Drawing.Point(141, 38);
            this.htkySoftKeyboard.Name = "htkySoftKeyboard";
            this.htkySoftKeyboard.Size = new System.Drawing.Size(505, 29);
            this.htkySoftKeyboard.TabIndex = 4;
            this.htkySoftKeyboard.ValueChanged += new System.EventHandler(this.htkySoftKeyboard_ValueChanged);
            // 
            // lblSoftKeyboard
            // 
            this.lblSoftKeyboard.AutoSize = true;
            this.lblSoftKeyboard.Location = new System.Drawing.Point(3, 45);
            this.lblSoftKeyboard.Margin = new System.Windows.Forms.Padding(3, 10, 3, 0);
            this.lblSoftKeyboard.Name = "lblSoftKeyboard";
            this.lblSoftKeyboard.Size = new System.Drawing.Size(114, 13);
            this.lblSoftKeyboard.TabIndex = 5;
            this.lblSoftKeyboard.Text = "Soft Keyboard Hotkey:";
            // 
            // tabAbout
            // 
            this.tabAbout.Controls.Add(this.tableAbout);
            this.tabAbout.Location = new System.Drawing.Point(4, 25);
            this.tabAbout.Name = "tabAbout";
            this.tabAbout.Padding = new System.Windows.Forms.Padding(3);
            this.tabAbout.Size = new System.Drawing.Size(661, 320);
            this.tabAbout.TabIndex = 2;
            this.tabAbout.Text = "About";
            this.tabAbout.UseVisualStyleBackColor = true;
            // 
            // tableAbout
            // 
            this.tableAbout.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.tableAbout.ColumnCount = 1;
            this.tableAbout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableAbout.Controls.Add(this.txtLicense, 0, 1);
            this.tableAbout.Controls.Add(this.btnCheckUpdate, 0, 2);
            this.tableAbout.Controls.Add(this.lblAboutTitle, 0, 0);
            this.tableAbout.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableAbout.Location = new System.Drawing.Point(3, 3);
            this.tableAbout.Name = "tableAbout";
            this.tableAbout.RowCount = 3;
            this.tableAbout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 34F));
            this.tableAbout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableAbout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 31F));
            this.tableAbout.Size = new System.Drawing.Size(655, 314);
            this.tableAbout.TabIndex = 0;
            // 
            // txtLicense
            // 
            this.txtLicense.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtLicense.Location = new System.Drawing.Point(3, 37);
            this.txtLicense.Multiline = true;
            this.txtLicense.Name = "txtLicense";
            this.txtLicense.ReadOnly = true;
            this.txtLicense.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtLicense.Size = new System.Drawing.Size(649, 243);
            this.txtLicense.TabIndex = 1;
            this.txtLicense.Text = resources.GetString("txtLicense.Text");
            // 
            // btnCheckUpdate
            // 
            this.btnCheckUpdate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnCheckUpdate.Location = new System.Drawing.Point(537, 286);
            this.btnCheckUpdate.Name = "btnCheckUpdate";
            this.btnCheckUpdate.Size = new System.Drawing.Size(115, 23);
            this.btnCheckUpdate.TabIndex = 2;
            this.btnCheckUpdate.Text = "Check For Update";
            this.btnCheckUpdate.UseVisualStyleBackColor = true;
            this.btnCheckUpdate.Click += new System.EventHandler(this.btnCheckUpdate_Click);
            // 
            // lblAboutTitle
            // 
            this.lblAboutTitle.AutoSize = true;
            this.lblAboutTitle.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblAboutTitle.Location = new System.Drawing.Point(3, 8);
            this.lblAboutTitle.Margin = new System.Windows.Forms.Padding(3, 8, 3, 0);
            this.lblAboutTitle.Name = "lblAboutTitle";
            this.lblAboutTitle.Size = new System.Drawing.Size(162, 19);
            this.lblAboutTitle.TabIndex = 3;
            this.lblAboutTitle.Text = "KeyMagic {VERSION}";
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Black;
            this.ClientSize = new System.Drawing.Size(669, 389);
            this.Controls.Add(this.mainTableLayoutPanel);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "frmMain";
            this.Text = "KeyMagic";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.frmMain_FormClosed);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmMain_FormClosing);
            this.cmsRight.ResumeLayout(false);
            this.fllBottom.ResumeLayout(false);
            this.mainTableLayoutPanel.ResumeLayout(false);
            this.tabControl.ResumeLayout(false);
            this.tabLayouts.ResumeLayout(false);
            this.tblLayouts.ResumeLayout(false);
            this.fllRight.ResumeLayout(false);
            this.tabSettings.ResumeLayout(false);
            this.grupSettings.ResumeLayout(false);
            this.tableSettings.ResumeLayout(false);
            this.tableSettings.PerformLayout();
            this.tabAbout.ResumeLayout(false);
            this.tableAbout.ResumeLayout(false);
            this.tableAbout.PerformLayout();
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
        private System.Windows.Forms.ContextMenuStrip cmsRight;
        private System.Windows.Forms.ColumnHeader colDisplayText;
        private System.Windows.Forms.ColumnHeader colDesc;
        private System.Windows.Forms.ContextMenuStrip cmsLeft;
        private KTabControl tabControl;
        private KToolStripMenuItem aboutToolStripMenuItem;
        private KToolStripMenuItem showToolStripMenuItem;
        private KToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.TabPage tabAbout;
        private System.Windows.Forms.TableLayoutPanel tableAbout;
        private System.Windows.Forms.TextBox txtLicense;
        private System.Windows.Forms.Button btnCheckUpdate;
        private System.Windows.Forms.Label lblAboutTitle;
        private System.Windows.Forms.TableLayoutPanel tableSettings;
        private System.Windows.Forms.CheckBox chkRunAtLogon;
        private System.Windows.Forms.GroupBox grupSettings;
        private HotkeyControl htkyOnOff;
        private System.Windows.Forms.Label lblOnOffKey;
        private System.Windows.Forms.FlowLayoutPanel fllBottom;
        private System.Windows.Forms.Button btnExit;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.TableLayoutPanel mainTableLayoutPanel;
        private System.Windows.Forms.ColumnHeader colVersion;
        private KToolStripMenuItem softKeyboardToolStripMenuItem;
        private HotkeyControl htkySoftKeyboard;
        private System.Windows.Forms.Label lblSoftKeyboard;

    }
}

