namespace kEditor
{
    partial class mainFrame
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(mainFrame));
            this.mainMenu = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.recentFilesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
            this.clearRecentFileListToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
            this.closeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.undoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.redoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.cutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.copyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.pasteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deleteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem4 = new System.Windows.Forms.ToolStripSeparator();
            this.selectAllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.findToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.findAndReplaceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.goToToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lineNumbersToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem5 = new System.Windows.Forms.ToolStripSeparator();
            this.hexadecimalToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.setGlypeRangeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.formatToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.defaultFontToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.checkSyntaxToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.compileAndSaveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.configsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.changeColorsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fileAssociationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.defaultEditorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.forceAsDefaultEditorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openFileDlg = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDlg = new System.Windows.Forms.SaveFileDialog();
            this.fontDlg = new System.Windows.Forms.FontDialog();
            this.splitContainer = new System.Windows.Forms.SplitContainer();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.glyphTable = new kEditor.GlyphTable();
            this.txtFilter = new kEditor.FilterEditBox();
            this.lblGlyphName = new System.Windows.Forms.Label();
            this.SciEditor = new ScintillaNet.Scintilla();
            this.mainMenu.SuspendLayout();
            this.splitContainer.Panel1.SuspendLayout();
            this.splitContainer.Panel2.SuspendLayout();
            this.splitContainer.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.SciEditor)).BeginInit();
            this.SuspendLayout();
            // 
            // mainMenu
            // 
            this.mainMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.viewToolStripMenuItem,
            this.formatToolStripMenuItem,
            this.toolsToolStripMenuItem,
            this.configsToolStripMenuItem,
            this.helpToolStripMenuItem});
            resources.ApplyResources(this.mainMenu, "mainMenu");
            this.mainMenu.Name = "mainMenu";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.openToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.saveAsToolStripMenuItem,
            this.toolStripSeparator1,
            this.recentFilesToolStripMenuItem,
            this.toolStripMenuItem2,
            this.closeToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            resources.ApplyResources(this.fileToolStripMenuItem, "fileToolStripMenuItem");
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            resources.ApplyResources(this.newToolStripMenuItem, "newToolStripMenuItem");
            this.newToolStripMenuItem.Click += new System.EventHandler(this.newToolStripMenuItem_Click);
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            resources.ApplyResources(this.openToolStripMenuItem, "openToolStripMenuItem");
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            resources.ApplyResources(this.saveToolStripMenuItem, "saveToolStripMenuItem");
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            resources.ApplyResources(this.saveAsToolStripMenuItem, "saveAsToolStripMenuItem");
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.saveAsToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            resources.ApplyResources(this.toolStripSeparator1, "toolStripSeparator1");
            // 
            // recentFilesToolStripMenuItem
            // 
            this.recentFilesToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem3,
            this.clearRecentFileListToolStripMenuItem});
            this.recentFilesToolStripMenuItem.Name = "recentFilesToolStripMenuItem";
            resources.ApplyResources(this.recentFilesToolStripMenuItem, "recentFilesToolStripMenuItem");
            // 
            // toolStripMenuItem3
            // 
            this.toolStripMenuItem3.Name = "toolStripMenuItem3";
            resources.ApplyResources(this.toolStripMenuItem3, "toolStripMenuItem3");
            // 
            // clearRecentFileListToolStripMenuItem
            // 
            this.clearRecentFileListToolStripMenuItem.Name = "clearRecentFileListToolStripMenuItem";
            resources.ApplyResources(this.clearRecentFileListToolStripMenuItem, "clearRecentFileListToolStripMenuItem");
            this.clearRecentFileListToolStripMenuItem.Click += new System.EventHandler(this.clearRecentFileListToolStripMenuItem_Click);
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            resources.ApplyResources(this.toolStripMenuItem2, "toolStripMenuItem2");
            // 
            // closeToolStripMenuItem
            // 
            this.closeToolStripMenuItem.Name = "closeToolStripMenuItem";
            resources.ApplyResources(this.closeToolStripMenuItem, "closeToolStripMenuItem");
            this.closeToolStripMenuItem.Click += new System.EventHandler(this.closeToolStripMenuItem_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.undoToolStripMenuItem,
            this.redoToolStripMenuItem,
            this.toolStripSeparator2,
            this.cutToolStripMenuItem,
            this.copyToolStripMenuItem,
            this.pasteToolStripMenuItem,
            this.deleteToolStripMenuItem,
            this.toolStripMenuItem4,
            this.selectAllToolStripMenuItem,
            this.toolStripMenuItem1,
            this.findToolStripMenuItem,
            this.findAndReplaceToolStripMenuItem,
            this.goToToolStripMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            resources.ApplyResources(this.editToolStripMenuItem, "editToolStripMenuItem");
            // 
            // undoToolStripMenuItem
            // 
            this.undoToolStripMenuItem.Name = "undoToolStripMenuItem";
            resources.ApplyResources(this.undoToolStripMenuItem, "undoToolStripMenuItem");
            this.undoToolStripMenuItem.Click += new System.EventHandler(this.undoToolStripMenuItem_Click);
            // 
            // redoToolStripMenuItem
            // 
            this.redoToolStripMenuItem.Name = "redoToolStripMenuItem";
            resources.ApplyResources(this.redoToolStripMenuItem, "redoToolStripMenuItem");
            this.redoToolStripMenuItem.Click += new System.EventHandler(this.redoToolStripMenuItem_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            resources.ApplyResources(this.toolStripSeparator2, "toolStripSeparator2");
            // 
            // cutToolStripMenuItem
            // 
            this.cutToolStripMenuItem.Name = "cutToolStripMenuItem";
            resources.ApplyResources(this.cutToolStripMenuItem, "cutToolStripMenuItem");
            this.cutToolStripMenuItem.Click += new System.EventHandler(this.cutToolStripMenuItem_Click);
            // 
            // copyToolStripMenuItem
            // 
            this.copyToolStripMenuItem.Name = "copyToolStripMenuItem";
            resources.ApplyResources(this.copyToolStripMenuItem, "copyToolStripMenuItem");
            this.copyToolStripMenuItem.Click += new System.EventHandler(this.copyToolStripMenuItem_Click);
            // 
            // pasteToolStripMenuItem
            // 
            this.pasteToolStripMenuItem.Name = "pasteToolStripMenuItem";
            resources.ApplyResources(this.pasteToolStripMenuItem, "pasteToolStripMenuItem");
            this.pasteToolStripMenuItem.Click += new System.EventHandler(this.pasteToolStripMenuItem_Click);
            // 
            // deleteToolStripMenuItem
            // 
            this.deleteToolStripMenuItem.Name = "deleteToolStripMenuItem";
            resources.ApplyResources(this.deleteToolStripMenuItem, "deleteToolStripMenuItem");
            this.deleteToolStripMenuItem.Click += new System.EventHandler(this.deleteToolStripMenuItem_Click);
            // 
            // toolStripMenuItem4
            // 
            this.toolStripMenuItem4.Name = "toolStripMenuItem4";
            resources.ApplyResources(this.toolStripMenuItem4, "toolStripMenuItem4");
            // 
            // selectAllToolStripMenuItem
            // 
            this.selectAllToolStripMenuItem.Name = "selectAllToolStripMenuItem";
            resources.ApplyResources(this.selectAllToolStripMenuItem, "selectAllToolStripMenuItem");
            this.selectAllToolStripMenuItem.Click += new System.EventHandler(this.selectAllToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            resources.ApplyResources(this.toolStripMenuItem1, "toolStripMenuItem1");
            // 
            // findToolStripMenuItem
            // 
            this.findToolStripMenuItem.Name = "findToolStripMenuItem";
            resources.ApplyResources(this.findToolStripMenuItem, "findToolStripMenuItem");
            this.findToolStripMenuItem.Click += new System.EventHandler(this.findToolStripMenuItem_Click);
            // 
            // findAndReplaceToolStripMenuItem
            // 
            this.findAndReplaceToolStripMenuItem.Name = "findAndReplaceToolStripMenuItem";
            resources.ApplyResources(this.findAndReplaceToolStripMenuItem, "findAndReplaceToolStripMenuItem");
            this.findAndReplaceToolStripMenuItem.Click += new System.EventHandler(this.findAndReplaceToolStripMenuItem_Click);
            // 
            // goToToolStripMenuItem
            // 
            this.goToToolStripMenuItem.Name = "goToToolStripMenuItem";
            resources.ApplyResources(this.goToToolStripMenuItem, "goToToolStripMenuItem");
            this.goToToolStripMenuItem.Click += new System.EventHandler(this.goToToolStripMenuItem_Click);
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.lineNumbersToolStripMenuItem,
            this.toolStripMenuItem5,
            this.hexadecimalToolStripMenuItem,
            this.setGlypeRangeToolStripMenuItem});
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            resources.ApplyResources(this.viewToolStripMenuItem, "viewToolStripMenuItem");
            // 
            // lineNumbersToolStripMenuItem
            // 
            this.lineNumbersToolStripMenuItem.CheckOnClick = true;
            this.lineNumbersToolStripMenuItem.Name = "lineNumbersToolStripMenuItem";
            resources.ApplyResources(this.lineNumbersToolStripMenuItem, "lineNumbersToolStripMenuItem");
            this.lineNumbersToolStripMenuItem.Click += new System.EventHandler(this.lineNumbersToolStripMenuItem_Click);
            // 
            // toolStripMenuItem5
            // 
            this.toolStripMenuItem5.Name = "toolStripMenuItem5";
            resources.ApplyResources(this.toolStripMenuItem5, "toolStripMenuItem5");
            // 
            // hexadecimalToolStripMenuItem
            // 
            this.hexadecimalToolStripMenuItem.CheckOnClick = true;
            this.hexadecimalToolStripMenuItem.Name = "hexadecimalToolStripMenuItem";
            resources.ApplyResources(this.hexadecimalToolStripMenuItem, "hexadecimalToolStripMenuItem");
            this.hexadecimalToolStripMenuItem.Click += new System.EventHandler(this.hexadecimalToolStripMenuItem_Click);
            // 
            // setGlypeRangeToolStripMenuItem
            // 
            this.setGlypeRangeToolStripMenuItem.Name = "setGlypeRangeToolStripMenuItem";
            resources.ApplyResources(this.setGlypeRangeToolStripMenuItem, "setGlypeRangeToolStripMenuItem");
            this.setGlypeRangeToolStripMenuItem.Click += new System.EventHandler(this.setGlyphRangeToolStripMenuItem_Click);
            // 
            // formatToolStripMenuItem
            // 
            this.formatToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.defaultFontToolStripMenuItem});
            this.formatToolStripMenuItem.Name = "formatToolStripMenuItem";
            resources.ApplyResources(this.formatToolStripMenuItem, "formatToolStripMenuItem");
            // 
            // defaultFontToolStripMenuItem
            // 
            this.defaultFontToolStripMenuItem.Name = "defaultFontToolStripMenuItem";
            resources.ApplyResources(this.defaultFontToolStripMenuItem, "defaultFontToolStripMenuItem");
            this.defaultFontToolStripMenuItem.Click += new System.EventHandler(this.defaultFontToolStripMenuItem_Click);
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.checkSyntaxToolStripMenuItem,
            this.compileAndSaveToolStripMenuItem});
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            resources.ApplyResources(this.toolsToolStripMenuItem, "toolsToolStripMenuItem");
            this.toolsToolStripMenuItem.Click += new System.EventHandler(this.toolsToolStripMenuItem_Click);
            // 
            // checkSyntaxToolStripMenuItem
            // 
            this.checkSyntaxToolStripMenuItem.Name = "checkSyntaxToolStripMenuItem";
            resources.ApplyResources(this.checkSyntaxToolStripMenuItem, "checkSyntaxToolStripMenuItem");
            this.checkSyntaxToolStripMenuItem.Click += new System.EventHandler(this.checkSyntaxToolStripMenuItem_Click);
            // 
            // compileAndSaveToolStripMenuItem
            // 
            this.compileAndSaveToolStripMenuItem.Name = "compileAndSaveToolStripMenuItem";
            resources.ApplyResources(this.compileAndSaveToolStripMenuItem, "compileAndSaveToolStripMenuItem");
            this.compileAndSaveToolStripMenuItem.Click += new System.EventHandler(this.compileAndSaveToolStripMenuItem_Click);
            // 
            // configsToolStripMenuItem
            // 
            this.configsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.changeColorsToolStripMenuItem,
            this.fileAssociationToolStripMenuItem});
            this.configsToolStripMenuItem.Name = "configsToolStripMenuItem";
            resources.ApplyResources(this.configsToolStripMenuItem, "configsToolStripMenuItem");
            // 
            // changeColorsToolStripMenuItem
            // 
            this.changeColorsToolStripMenuItem.Name = "changeColorsToolStripMenuItem";
            resources.ApplyResources(this.changeColorsToolStripMenuItem, "changeColorsToolStripMenuItem");
            this.changeColorsToolStripMenuItem.Click += new System.EventHandler(this.changeStylesToolStripMenuItem_Click);
            // 
            // fileAssociationToolStripMenuItem
            // 
            this.fileAssociationToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.defaultEditorToolStripMenuItem,
            this.forceAsDefaultEditorToolStripMenuItem});
            this.fileAssociationToolStripMenuItem.Name = "fileAssociationToolStripMenuItem";
            resources.ApplyResources(this.fileAssociationToolStripMenuItem, "fileAssociationToolStripMenuItem");
            // 
            // defaultEditorToolStripMenuItem
            // 
            resources.ApplyResources(this.defaultEditorToolStripMenuItem, "defaultEditorToolStripMenuItem");
            this.defaultEditorToolStripMenuItem.Name = "defaultEditorToolStripMenuItem";
            // 
            // forceAsDefaultEditorToolStripMenuItem
            // 
            this.forceAsDefaultEditorToolStripMenuItem.CheckOnClick = true;
            this.forceAsDefaultEditorToolStripMenuItem.Name = "forceAsDefaultEditorToolStripMenuItem";
            resources.ApplyResources(this.forceAsDefaultEditorToolStripMenuItem, "forceAsDefaultEditorToolStripMenuItem");
            this.forceAsDefaultEditorToolStripMenuItem.Click += new System.EventHandler(this.forceAsDefaultEditorToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            resources.ApplyResources(this.helpToolStripMenuItem, "helpToolStripMenuItem");
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            resources.ApplyResources(this.aboutToolStripMenuItem, "aboutToolStripMenuItem");
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // openFileDlg
            // 
            resources.ApplyResources(this.openFileDlg, "openFileDlg");
            this.openFileDlg.RestoreDirectory = true;
            // 
            // saveFileDlg
            // 
            this.saveFileDlg.DefaultExt = "kms";
            resources.ApplyResources(this.saveFileDlg, "saveFileDlg");
            this.saveFileDlg.RestoreDirectory = true;
            // 
            // splitContainer
            // 
            this.splitContainer.BackColor = System.Drawing.SystemColors.ActiveBorder;
            resources.ApplyResources(this.splitContainer, "splitContainer");
            this.splitContainer.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainer.Name = "splitContainer";
            // 
            // splitContainer.Panel1
            // 
            this.splitContainer.Panel1.Controls.Add(this.tableLayoutPanel1);
            // 
            // splitContainer.Panel2
            // 
            this.splitContainer.Panel2.Controls.Add(this.SciEditor);
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            resources.ApplyResources(this.tableLayoutPanel1, "tableLayoutPanel1");
            this.tableLayoutPanel1.Controls.Add(this.glyphTable, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.txtFilter, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.lblGlyphName, 0, 1);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            // 
            // glyphTable
            // 
            this.glyphTable.AlternateGradientCellColorEnd = System.Drawing.Color.White;
            this.glyphTable.AlternateGradientCellColorStart = System.Drawing.Color.WhiteSmoke;
            resources.ApplyResources(this.glyphTable, "glyphTable");
            this.glyphTable.AutoValidate = System.Windows.Forms.AutoValidate.EnableAllowFocusChange;
            this.glyphTable.CellGradientColorEnd = System.Drawing.Color.WhiteSmoke;
            this.glyphTable.CellGradientColorStart = System.Drawing.Color.Gainsboro;
            this.glyphTable.FoucusForeColor = System.Drawing.Color.White;
            this.glyphTable.FoucusGradientColorEnd = System.Drawing.Color.Silver;
            this.glyphTable.FoucusGradientColorStart = System.Drawing.Color.DarkGray;
            this.glyphTable.FoucusHexForeColor = System.Drawing.Color.White;
            this.glyphTable.HexCodeColor = System.Drawing.Color.Black;
            this.glyphTable.HexNotation = false;
            this.glyphTable.Name = "glyphTable";
            this.glyphTable.NumberOfColumns = 6;
            this.glyphTable.NumberOfRows = 1;
            this.glyphTable.SelectedCell = -1;
            this.glyphTable.MouseLeave += new System.EventHandler(this.glyphTable_MouseLeave);
            this.glyphTable.SelectionChanged += new System.EventHandler(this.glyphTable_SelectionChanged);
            this.glyphTable.MouseMove += new System.Windows.Forms.MouseEventHandler(this.glyphTable_MouseMove);
            this.glyphTable.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.glyphTable_MouseDoubleClick);
            // 
            // txtFilter
            // 
            this.txtFilter.ForeColor = System.Drawing.Color.Gray;
            resources.ApplyResources(this.txtFilter, "txtFilter");
            this.txtFilter.Name = "txtFilter";
            this.txtFilter.TextChanged += new System.EventHandler(this.txtFilter_TextChanged);
            // 
            // lblGlyphName
            // 
            resources.ApplyResources(this.lblGlyphName, "lblGlyphName");
            this.lblGlyphName.Name = "lblGlyphName";
            // 
            // SciEditor
            // 
            this.SciEditor.AutoComplete.DropRestOfWord = true;
            this.SciEditor.AutoComplete.ListString = "";
            resources.ApplyResources(this.SciEditor, "SciEditor");
            this.SciEditor.Name = "SciEditor";
            this.SciEditor.Styles.BraceBad.FontName = "Verdana";
            this.SciEditor.Styles.BraceLight.FontName = "Verdana";
            this.SciEditor.Styles.ControlChar.FontName = "Verdana";
            this.SciEditor.Styles.Default.FontName = "Verdana";
            this.SciEditor.Styles.IndentGuide.FontName = "Verdana";
            this.SciEditor.Styles.LastPredefined.FontName = "Verdana";
            this.SciEditor.Styles.LineNumber.FontName = "Verdana";
            this.SciEditor.Styles.Max.FontName = "Verdana";
            this.SciEditor.TextChanged += new System.EventHandler<System.EventArgs>(this.SciEditor_TextChanged);
            this.SciEditor.CharAdded += new System.EventHandler<ScintillaNet.CharAddedEventArgs>(this.SciEditor_CharAdded);
            this.SciEditor.ModifiedChanged += new System.EventHandler(this.SciEditor_ModifiedChanged);
            this.SciEditor.SelectionChanged += new System.EventHandler(this.SciEditor_SelectionChanged);
            // 
            // mainFrame
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.splitContainer);
            this.Controls.Add(this.mainMenu);
            this.MainMenuStrip = this.mainMenu;
            this.Name = "mainFrame";
            this.Load += new System.EventHandler(this.mainForm_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.mainFrame_FormClosing);
            this.mainMenu.ResumeLayout(false);
            this.mainMenu.PerformLayout();
            this.splitContainer.Panel1.ResumeLayout(false);
            this.splitContainer.Panel2.ResumeLayout(false);
            this.splitContainer.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.SciEditor)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip mainMenu;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem closeToolStripMenuItem;
        private System.Windows.Forms.OpenFileDialog openFileDlg;
        private System.Windows.Forms.SaveFileDialog saveFileDlg;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem undoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem redoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem selectAllToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem findToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem goToToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem findAndReplaceToolStripMenuItem;
        private System.Windows.Forms.FontDialog fontDlg;
        private System.Windows.Forms.SplitContainer splitContainer;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem hexadecimalToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem recentFilesToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
        private System.Windows.Forms.ToolStripMenuItem clearRecentFileListToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem setGlypeRangeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cutToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem copyToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem pasteToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem4;
        private System.Windows.Forms.ToolStripMenuItem deleteToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem lineNumbersToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem5;
        private System.Windows.Forms.ToolStripMenuItem configsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fileAssociationToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem defaultEditorToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem forceAsDefaultEditorToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem changeColorsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem formatToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem defaultFontToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem compileAndSaveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem checkSyntaxToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private ScintillaNet.Scintilla SciEditor;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private GlyphTable glyphTable;
        private FilterEditBox txtFilter;
        private System.Windows.Forms.Label lblGlyphName;
    }
}

