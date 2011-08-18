namespace KeyMagic
{
    partial class SoftKeyboard
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SoftKeyboard));
            this.QWERRowPanel = new System.Windows.Forms.FlowLayoutPanel();
            this.NumRowPanel = new System.Windows.Forms.FlowLayoutPanel();
            this.DockedFlow = new System.Windows.Forms.FlowLayoutPanel();
            this.ASDFRowPanel = new System.Windows.Forms.FlowLayoutPanel();
            this.ZXCVRowPanel = new System.Windows.Forms.FlowLayoutPanel();
            this.SpaceRowPanel = new System.Windows.Forms.FlowLayoutPanel();
            this.DockedFlow.SuspendLayout();
            this.SuspendLayout();
            // 
            // QWERRowPanel
            // 
            this.DockedFlow.SetFlowBreak(this.QWERRowPanel, true);
            this.QWERRowPanel.Location = new System.Drawing.Point(3, 59);
            this.QWERRowPanel.Name = "QWERRowPanel";
            this.QWERRowPanel.Size = new System.Drawing.Size(840, 50);
            this.QWERRowPanel.TabIndex = 1;
            this.QWERRowPanel.WrapContents = false;
            // 
            // NumRowPanel
            // 
            this.DockedFlow.SetFlowBreak(this.NumRowPanel, true);
            this.NumRowPanel.Location = new System.Drawing.Point(3, 3);
            this.NumRowPanel.Name = "NumRowPanel";
            this.NumRowPanel.Size = new System.Drawing.Size(840, 50);
            this.NumRowPanel.TabIndex = 0;
            this.NumRowPanel.WrapContents = false;
            // 
            // DockedFlow
            // 
            this.DockedFlow.Controls.Add(this.NumRowPanel);
            this.DockedFlow.Controls.Add(this.QWERRowPanel);
            this.DockedFlow.Controls.Add(this.ASDFRowPanel);
            this.DockedFlow.Controls.Add(this.ZXCVRowPanel);
            this.DockedFlow.Controls.Add(this.SpaceRowPanel);
            this.DockedFlow.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DockedFlow.Location = new System.Drawing.Point(0, 0);
            this.DockedFlow.Name = "DockedFlow";
            this.DockedFlow.Size = new System.Drawing.Size(845, 281);
            this.DockedFlow.TabIndex = 0;
            this.DockedFlow.MouseEnter += new System.EventHandler(this.DockedFlow_MouseEnter);
            // 
            // ASDFRowPanel
            // 
            this.ASDFRowPanel.Location = new System.Drawing.Point(3, 115);
            this.ASDFRowPanel.Name = "ASDFRowPanel";
            this.ASDFRowPanel.Size = new System.Drawing.Size(840, 50);
            this.ASDFRowPanel.TabIndex = 2;
            this.ASDFRowPanel.WrapContents = false;
            // 
            // ZXCVRowPanel
            // 
            this.ZXCVRowPanel.Location = new System.Drawing.Point(3, 171);
            this.ZXCVRowPanel.Name = "ZXCVRowPanel";
            this.ZXCVRowPanel.Size = new System.Drawing.Size(840, 50);
            this.ZXCVRowPanel.TabIndex = 3;
            this.ZXCVRowPanel.WrapContents = false;
            // 
            // SpaceRowPanel
            // 
            this.SpaceRowPanel.Location = new System.Drawing.Point(3, 227);
            this.SpaceRowPanel.Name = "SpaceRowPanel";
            this.SpaceRowPanel.Size = new System.Drawing.Size(840, 50);
            this.SpaceRowPanel.TabIndex = 4;
            this.SpaceRowPanel.WrapContents = false;
            // 
            // SoftKeyboard
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.Color.Black;
            this.ClientSize = new System.Drawing.Size(845, 281);
            this.Controls.Add(this.DockedFlow);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(440, 200);
            this.Name = "SoftKeyboard";
            this.Text = "Soft Keyboard";
            this.TopMost = true;
            this.Resize += new System.EventHandler(this.SoftKeyboard_Resize);
            this.DockedFlow.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.FlowLayoutPanel QWERRowPanel;
        private System.Windows.Forms.FlowLayoutPanel DockedFlow;
        private System.Windows.Forms.FlowLayoutPanel NumRowPanel;
        private System.Windows.Forms.FlowLayoutPanel ASDFRowPanel;
        private System.Windows.Forms.FlowLayoutPanel ZXCVRowPanel;
        private System.Windows.Forms.FlowLayoutPanel SpaceRowPanel;
    }
}