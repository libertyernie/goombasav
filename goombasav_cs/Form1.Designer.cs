namespace goombasav_cs {
	partial class Form1 {
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing) {
			if (disposing && (components != null)) {
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent() {
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
			this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.listBox1 = new System.Windows.Forms.ListBox();
			this.flowLayoutPanel2 = new System.Windows.Forms.FlowLayoutPanel();
			this.lblSize = new System.Windows.Forms.Label();
			this.lblSizeVal = new System.Windows.Forms.Label();
			this.lblType = new System.Windows.Forms.Label();
			this.lblTypeVal = new System.Windows.Forms.Label();
			this.flpConfigdata = new System.Windows.Forms.FlowLayoutPanel();
			this.lblBorder = new System.Windows.Forms.Label();
			this.lblBorderVal = new System.Windows.Forms.Label();
			this.lblPalette = new System.Windows.Forms.Label();
			this.lblPaletteVal = new System.Windows.Forms.Label();
			this.lblSleep = new System.Windows.Forms.Label();
			this.lblSleepVal = new System.Windows.Forms.Label();
			this.lblGamma = new System.Windows.Forms.Label();
			this.lblGammaVal = new System.Windows.Forms.Label();
			this.lblAutostate = new System.Windows.Forms.Label();
			this.lblAutostateVal = new System.Windows.Forms.Label();
			this.flpStateheader = new System.Windows.Forms.FlowLayoutPanel();
			this.lblUncompressedSize = new System.Windows.Forms.Label();
			this.lblUncompressedSizeVal = new System.Windows.Forms.Label();
			this.lblFramecount = new System.Windows.Forms.Label();
			this.lblFramecountVal = new System.Windows.Forms.Label();
			this.lblChecksum = new System.Windows.Forms.Label();
			this.lblChecksumVal = new System.Windows.Forms.Label();
			this.lblTitle = new System.Windows.Forms.Label();
			this.lblTitleVal = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.panel1 = new System.Windows.Forms.Panel();
			this.hashBox = new System.Windows.Forms.Label();
			this.lblHashVal = new System.Windows.Forms.Label();
			this.lblHash = new System.Windows.Forms.Label();
			this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
			this.btnReplace = new System.Windows.Forms.Button();
			this.btnExtract = new System.Windows.Forms.Button();
			this.menuStrip1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			this.flowLayoutPanel2.SuspendLayout();
			this.flpConfigdata.SuspendLayout();
			this.flpStateheader.SuspendLayout();
			this.panel1.SuspendLayout();
			this.flowLayoutPanel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// menuStrip1
			// 
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.helpToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(507, 28);
			this.menuStrip1.TabIndex = 0;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.saveAsToolStripMenuItem,
            this.toolStripMenuItem1,
            this.exitToolStripMenuItem});
			this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
			this.fileToolStripMenuItem.Size = new System.Drawing.Size(44, 24);
			this.fileToolStripMenuItem.Text = "File";
			// 
			// openToolStripMenuItem
			// 
			this.openToolStripMenuItem.Name = "openToolStripMenuItem";
			this.openToolStripMenuItem.Size = new System.Drawing.Size(175, 24);
			this.openToolStripMenuItem.Text = "Open";
			this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
			this.saveToolStripMenuItem.Size = new System.Drawing.Size(175, 24);
			this.saveToolStripMenuItem.Text = "Save";
			this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
			// 
			// saveAsToolStripMenuItem
			// 
			this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
			this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(175, 24);
			this.saveAsToolStripMenuItem.Text = "Save As...";
			this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.saveAsToolStripMenuItem_Click);
			// 
			// toolStripMenuItem1
			// 
			this.toolStripMenuItem1.Name = "toolStripMenuItem1";
			this.toolStripMenuItem1.Size = new System.Drawing.Size(172, 6);
			// 
			// exitToolStripMenuItem
			// 
			this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
			this.exitToolStripMenuItem.Size = new System.Drawing.Size(175, 24);
			this.exitToolStripMenuItem.Text = "Exit";
			this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
			// 
			// helpToolStripMenuItem
			// 
			this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
			this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
			this.helpToolStripMenuItem.Size = new System.Drawing.Size(53, 24);
			this.helpToolStripMenuItem.Text = "Help";
			// 
			// aboutToolStripMenuItem
			// 
			this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
			this.aboutToolStripMenuItem.Size = new System.Drawing.Size(119, 24);
			this.aboutToolStripMenuItem.Text = "About";
			this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
			// 
			// splitContainer1
			// 
			this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainer1.Location = new System.Drawing.Point(0, 28);
			this.splitContainer1.Name = "splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.listBox1);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.flowLayoutPanel2);
			this.splitContainer1.Panel2.Controls.Add(this.panel1);
			this.splitContainer1.Panel2.Controls.Add(this.flowLayoutPanel1);
			this.splitContainer1.Size = new System.Drawing.Size(507, 195);
			this.splitContainer1.SplitterDistance = 130;
			this.splitContainer1.TabIndex = 1;
			// 
			// listBox1
			// 
			this.listBox1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.listBox1.FormattingEnabled = true;
			this.listBox1.IntegralHeight = false;
			this.listBox1.ItemHeight = 16;
			this.listBox1.Location = new System.Drawing.Point(0, 0);
			this.listBox1.Name = "listBox1";
			this.listBox1.Size = new System.Drawing.Size(130, 195);
			this.listBox1.TabIndex = 0;
			// 
			// flowLayoutPanel2
			// 
			this.flowLayoutPanel2.Controls.Add(this.lblSize);
			this.flowLayoutPanel2.Controls.Add(this.lblSizeVal);
			this.flowLayoutPanel2.Controls.Add(this.lblType);
			this.flowLayoutPanel2.Controls.Add(this.lblTypeVal);
			this.flowLayoutPanel2.Controls.Add(this.flpConfigdata);
			this.flowLayoutPanel2.Controls.Add(this.flpStateheader);
			this.flowLayoutPanel2.Controls.Add(this.lblChecksum);
			this.flowLayoutPanel2.Controls.Add(this.lblChecksumVal);
			this.flowLayoutPanel2.Controls.Add(this.lblTitle);
			this.flowLayoutPanel2.Controls.Add(this.lblTitleVal);
			this.flowLayoutPanel2.Controls.Add(this.label2);
			this.flowLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
			this.flowLayoutPanel2.Location = new System.Drawing.Point(0, 0);
			this.flowLayoutPanel2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.flowLayoutPanel2.Name = "flowLayoutPanel2";
			this.flowLayoutPanel2.Size = new System.Drawing.Size(373, 142);
			this.flowLayoutPanel2.TabIndex = 6;
			// 
			// lblSize
			// 
			this.lblSize.Location = new System.Drawing.Point(0, 0);
			this.lblSize.Margin = new System.Windows.Forms.Padding(0);
			this.lblSize.Name = "lblSize";
			this.lblSize.Size = new System.Drawing.Size(45, 23);
			this.lblSize.TabIndex = 0;
			this.lblSize.Text = "Size:";
			// 
			// lblSizeVal
			// 
			this.lblSizeVal.Location = new System.Drawing.Point(45, 0);
			this.lblSizeVal.Margin = new System.Windows.Forms.Padding(0);
			this.lblSizeVal.Name = "lblSizeVal";
			this.lblSizeVal.Size = new System.Drawing.Size(100, 23);
			this.lblSizeVal.TabIndex = 1;
			// 
			// lblType
			// 
			this.lblType.Location = new System.Drawing.Point(145, 0);
			this.lblType.Margin = new System.Windows.Forms.Padding(0);
			this.lblType.Name = "lblType";
			this.lblType.Size = new System.Drawing.Size(51, 23);
			this.lblType.TabIndex = 2;
			this.lblType.Text = "Type:";
			// 
			// lblTypeVal
			// 
			this.flowLayoutPanel2.SetFlowBreak(this.lblTypeVal, true);
			this.lblTypeVal.Location = new System.Drawing.Point(196, 0);
			this.lblTypeVal.Margin = new System.Windows.Forms.Padding(0);
			this.lblTypeVal.Name = "lblTypeVal";
			this.lblTypeVal.Size = new System.Drawing.Size(100, 23);
			this.lblTypeVal.TabIndex = 3;
			// 
			// flpConfigdata
			// 
			this.flpConfigdata.Controls.Add(this.lblBorder);
			this.flpConfigdata.Controls.Add(this.lblBorderVal);
			this.flpConfigdata.Controls.Add(this.lblPalette);
			this.flpConfigdata.Controls.Add(this.lblPaletteVal);
			this.flpConfigdata.Controls.Add(this.lblSleep);
			this.flpConfigdata.Controls.Add(this.lblSleepVal);
			this.flpConfigdata.Controls.Add(this.lblGamma);
			this.flpConfigdata.Controls.Add(this.lblGammaVal);
			this.flpConfigdata.Controls.Add(this.lblAutostate);
			this.flpConfigdata.Controls.Add(this.lblAutostateVal);
			this.flpConfigdata.Location = new System.Drawing.Point(0, 23);
			this.flpConfigdata.Margin = new System.Windows.Forms.Padding(0);
			this.flpConfigdata.Name = "flpConfigdata";
			this.flpConfigdata.Size = new System.Drawing.Size(280, 48);
			this.flpConfigdata.TabIndex = 8;
			this.flpConfigdata.Visible = false;
			// 
			// lblBorder
			// 
			this.lblBorder.Location = new System.Drawing.Point(0, 0);
			this.lblBorder.Margin = new System.Windows.Forms.Padding(0);
			this.lblBorder.Name = "lblBorder";
			this.lblBorder.Size = new System.Drawing.Size(60, 23);
			this.lblBorder.TabIndex = 0;
			this.lblBorder.Text = "Border:";
			// 
			// lblBorderVal
			// 
			this.lblBorderVal.Location = new System.Drawing.Point(60, 0);
			this.lblBorderVal.Margin = new System.Windows.Forms.Padding(0);
			this.lblBorderVal.Name = "lblBorderVal";
			this.lblBorderVal.Size = new System.Drawing.Size(29, 23);
			this.lblBorderVal.TabIndex = 1;
			this.lblBorderVal.Text = "88";
			// 
			// lblPalette
			// 
			this.lblPalette.Location = new System.Drawing.Point(89, 0);
			this.lblPalette.Margin = new System.Windows.Forms.Padding(0);
			this.lblPalette.Name = "lblPalette";
			this.lblPalette.Size = new System.Drawing.Size(60, 23);
			this.lblPalette.TabIndex = 2;
			this.lblPalette.Text = "Palette:";
			// 
			// lblPaletteVal
			// 
			this.lblPaletteVal.Location = new System.Drawing.Point(149, 0);
			this.lblPaletteVal.Margin = new System.Windows.Forms.Padding(0);
			this.lblPaletteVal.Name = "lblPaletteVal";
			this.lblPaletteVal.Size = new System.Drawing.Size(29, 23);
			this.lblPaletteVal.TabIndex = 3;
			this.lblPaletteVal.Text = "88";
			// 
			// lblSleep
			// 
			this.lblSleep.Location = new System.Drawing.Point(178, 0);
			this.lblSleep.Margin = new System.Windows.Forms.Padding(0);
			this.lblSleep.Name = "lblSleep";
			this.lblSleep.Size = new System.Drawing.Size(51, 23);
			this.lblSleep.TabIndex = 9;
			this.lblSleep.Text = "Sleep:";
			// 
			// lblSleepVal
			// 
			this.flpConfigdata.SetFlowBreak(this.lblSleepVal, true);
			this.lblSleepVal.Location = new System.Drawing.Point(229, 0);
			this.lblSleepVal.Margin = new System.Windows.Forms.Padding(0);
			this.lblSleepVal.Name = "lblSleepVal";
			this.lblSleepVal.Size = new System.Drawing.Size(51, 23);
			this.lblSleepVal.TabIndex = 10;
			// 
			// lblGamma
			// 
			this.lblGamma.Location = new System.Drawing.Point(0, 23);
			this.lblGamma.Margin = new System.Windows.Forms.Padding(0);
			this.lblGamma.Name = "lblGamma";
			this.lblGamma.Size = new System.Drawing.Size(65, 25);
			this.lblGamma.TabIndex = 13;
			this.lblGamma.Text = "Gamma:";
			// 
			// lblGammaVal
			// 
			this.lblGammaVal.Location = new System.Drawing.Point(65, 23);
			this.lblGammaVal.Margin = new System.Windows.Forms.Padding(0);
			this.lblGammaVal.Name = "lblGammaVal";
			this.lblGammaVal.Size = new System.Drawing.Size(60, 23);
			this.lblGammaVal.TabIndex = 14;
			// 
			// lblAutostate
			// 
			this.lblAutostate.Location = new System.Drawing.Point(125, 23);
			this.lblAutostate.Margin = new System.Windows.Forms.Padding(0);
			this.lblAutostate.Name = "lblAutostate";
			this.lblAutostate.Size = new System.Drawing.Size(105, 23);
			this.lblAutostate.TabIndex = 11;
			this.lblAutostate.Text = "Autoload state:";
			// 
			// lblAutostateVal
			// 
			this.flpConfigdata.SetFlowBreak(this.lblAutostateVal, true);
			this.lblAutostateVal.Location = new System.Drawing.Point(230, 23);
			this.lblAutostateVal.Margin = new System.Windows.Forms.Padding(0);
			this.lblAutostateVal.Name = "lblAutostateVal";
			this.lblAutostateVal.Size = new System.Drawing.Size(40, 23);
			this.lblAutostateVal.TabIndex = 12;
			// 
			// flpStateheader
			// 
			this.flpStateheader.Controls.Add(this.lblUncompressedSize);
			this.flpStateheader.Controls.Add(this.lblUncompressedSizeVal);
			this.flpStateheader.Controls.Add(this.lblFramecount);
			this.flpStateheader.Controls.Add(this.lblFramecountVal);
			this.flowLayoutPanel2.SetFlowBreak(this.flpStateheader, true);
			this.flpStateheader.Location = new System.Drawing.Point(0, 71);
			this.flpStateheader.Margin = new System.Windows.Forms.Padding(0);
			this.flpStateheader.Name = "flpStateheader";
			this.flpStateheader.Size = new System.Drawing.Size(295, 48);
			this.flpStateheader.TabIndex = 5;
			// 
			// lblUncompressedSize
			// 
			this.lblUncompressedSize.Location = new System.Drawing.Point(0, 0);
			this.lblUncompressedSize.Margin = new System.Windows.Forms.Padding(0);
			this.lblUncompressedSize.Name = "lblUncompressedSize";
			this.lblUncompressedSize.Size = new System.Drawing.Size(140, 23);
			this.lblUncompressedSize.TabIndex = 0;
			this.lblUncompressedSize.Text = "Uncompressed size:";
			// 
			// lblUncompressedSizeVal
			// 
			this.flpStateheader.SetFlowBreak(this.lblUncompressedSizeVal, true);
			this.lblUncompressedSizeVal.Location = new System.Drawing.Point(140, 0);
			this.lblUncompressedSizeVal.Margin = new System.Windows.Forms.Padding(0);
			this.lblUncompressedSizeVal.Name = "lblUncompressedSizeVal";
			this.lblUncompressedSizeVal.Size = new System.Drawing.Size(120, 23);
			this.lblUncompressedSizeVal.TabIndex = 4;
			// 
			// lblFramecount
			// 
			this.lblFramecount.Location = new System.Drawing.Point(0, 23);
			this.lblFramecount.Margin = new System.Windows.Forms.Padding(0);
			this.lblFramecount.Name = "lblFramecount";
			this.lblFramecount.Size = new System.Drawing.Size(95, 23);
			this.lblFramecount.TabIndex = 5;
			this.lblFramecount.Text = "Frame count:";
			// 
			// lblFramecountVal
			// 
			this.flpStateheader.SetFlowBreak(this.lblFramecountVal, true);
			this.lblFramecountVal.Location = new System.Drawing.Point(95, 23);
			this.lblFramecountVal.Margin = new System.Windows.Forms.Padding(0);
			this.lblFramecountVal.Name = "lblFramecountVal";
			this.lblFramecountVal.Size = new System.Drawing.Size(165, 23);
			this.lblFramecountVal.TabIndex = 6;
			// 
			// lblChecksum
			// 
			this.lblChecksum.Location = new System.Drawing.Point(0, 119);
			this.lblChecksum.Margin = new System.Windows.Forms.Padding(0);
			this.lblChecksum.Name = "lblChecksum";
			this.lblChecksum.Size = new System.Drawing.Size(120, 23);
			this.lblChecksum.TabIndex = 7;
			this.lblChecksum.Text = "ROM checksum:";
			// 
			// lblChecksumVal
			// 
			this.flowLayoutPanel2.SetFlowBreak(this.lblChecksumVal, true);
			this.lblChecksumVal.Location = new System.Drawing.Point(120, 119);
			this.lblChecksumVal.Margin = new System.Windows.Forms.Padding(0);
			this.lblChecksumVal.Name = "lblChecksumVal";
			this.lblChecksumVal.Size = new System.Drawing.Size(140, 23);
			this.lblChecksumVal.TabIndex = 8;
			// 
			// lblTitle
			// 
			this.lblTitle.Location = new System.Drawing.Point(0, 142);
			this.lblTitle.Margin = new System.Windows.Forms.Padding(0);
			this.lblTitle.Name = "lblTitle";
			this.lblTitle.Size = new System.Drawing.Size(45, 23);
			this.lblTitle.TabIndex = 6;
			this.lblTitle.Text = "Title:";
			// 
			// lblTitleVal
			// 
			this.lblTitleVal.Location = new System.Drawing.Point(45, 142);
			this.lblTitleVal.Margin = new System.Windows.Forms.Padding(0);
			this.lblTitleVal.Name = "lblTitleVal";
			this.lblTitleVal.Size = new System.Drawing.Size(247, 23);
			this.lblTitleVal.TabIndex = 7;
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(295, 142);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(0, 17);
			this.label2.TabIndex = 9;
			// 
			// panel1
			// 
			this.panel1.Controls.Add(this.hashBox);
			this.panel1.Controls.Add(this.lblHashVal);
			this.panel1.Controls.Add(this.lblHash);
			this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.panel1.Location = new System.Drawing.Point(0, 142);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(373, 17);
			this.panel1.TabIndex = 0;
			// 
			// hashBox
			// 
			this.hashBox.Dock = System.Windows.Forms.DockStyle.Fill;
			this.hashBox.Location = new System.Drawing.Point(174, 0);
			this.hashBox.Name = "hashBox";
			this.hashBox.Size = new System.Drawing.Size(199, 17);
			this.hashBox.TabIndex = 1;
			// 
			// lblHashVal
			// 
			this.lblHashVal.AutoSize = true;
			this.lblHashVal.Dock = System.Windows.Forms.DockStyle.Left;
			this.lblHashVal.Location = new System.Drawing.Point(174, 0);
			this.lblHashVal.Name = "lblHashVal";
			this.lblHashVal.Size = new System.Drawing.Size(0, 17);
			this.lblHashVal.TabIndex = 2;
			// 
			// lblHash
			// 
			this.lblHash.AutoSize = true;
			this.lblHash.Dock = System.Windows.Forms.DockStyle.Left;
			this.lblHash.Location = new System.Drawing.Point(0, 0);
			this.lblHash.Name = "lblHash";
			this.lblHash.Size = new System.Drawing.Size(174, 17);
			this.lblHash.TabIndex = 0;
			this.lblHash.Text = "Hash of compressed data:";
			// 
			// flowLayoutPanel1
			// 
			this.flowLayoutPanel1.Controls.Add(this.btnReplace);
			this.flowLayoutPanel1.Controls.Add(this.btnExtract);
			this.flowLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.flowLayoutPanel1.Location = new System.Drawing.Point(0, 159);
			this.flowLayoutPanel1.Margin = new System.Windows.Forms.Padding(0);
			this.flowLayoutPanel1.Name = "flowLayoutPanel1";
			this.flowLayoutPanel1.Size = new System.Drawing.Size(373, 36);
			this.flowLayoutPanel1.TabIndex = 1;
			// 
			// btnReplace
			// 
			this.btnReplace.Location = new System.Drawing.Point(3, 3);
			this.btnReplace.Name = "btnReplace";
			this.btnReplace.Size = new System.Drawing.Size(100, 28);
			this.btnReplace.TabIndex = 0;
			this.btnReplace.Text = "Replace";
			this.btnReplace.UseVisualStyleBackColor = true;
			this.btnReplace.Click += new System.EventHandler(this.btnReplace_Click);
			// 
			// btnExtract
			// 
			this.btnExtract.Location = new System.Drawing.Point(109, 3);
			this.btnExtract.Name = "btnExtract";
			this.btnExtract.Size = new System.Drawing.Size(100, 28);
			this.btnExtract.TabIndex = 1;
			this.btnExtract.Text = "Extract";
			this.btnExtract.UseVisualStyleBackColor = true;
			this.btnExtract.Click += new System.EventHandler(this.btnExtract_Click);
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(507, 223);
			this.Controls.Add(this.splitContainer1);
			this.Controls.Add(this.menuStrip1);
			this.MainMenuStrip = this.menuStrip1;
			this.Name = "Form1";
			this.Text = "Form1";
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
			this.splitContainer1.ResumeLayout(false);
			this.flowLayoutPanel2.ResumeLayout(false);
			this.flowLayoutPanel2.PerformLayout();
			this.flpConfigdata.ResumeLayout(false);
			this.flpStateheader.ResumeLayout(false);
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.flowLayoutPanel1.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
		private System.Windows.Forms.SplitContainer splitContainer1;
		private System.Windows.Forms.ListBox listBox1;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
		private System.Windows.Forms.Label lblHash;
		private System.Windows.Forms.Label hashBox;
		private System.Windows.Forms.Button btnReplace;
		private System.Windows.Forms.Button btnExtract;
		private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel2;
		private System.Windows.Forms.Label lblSize;
		private System.Windows.Forms.Label lblSizeVal;
		private System.Windows.Forms.Label lblType;
		private System.Windows.Forms.Label lblTypeVal;
		private System.Windows.Forms.FlowLayoutPanel flpConfigdata;
		private System.Windows.Forms.Label lblBorder;
		private System.Windows.Forms.Label lblBorderVal;
		private System.Windows.Forms.Label lblPalette;
		private System.Windows.Forms.Label lblPaletteVal;
		private System.Windows.Forms.Label lblSleep;
		private System.Windows.Forms.Label lblSleepVal;
		private System.Windows.Forms.Label lblGamma;
		private System.Windows.Forms.Label lblGammaVal;
		private System.Windows.Forms.Label lblAutostate;
		private System.Windows.Forms.Label lblAutostateVal;
		private System.Windows.Forms.FlowLayoutPanel flpStateheader;
		private System.Windows.Forms.Label lblUncompressedSize;
		private System.Windows.Forms.Label lblUncompressedSizeVal;
		private System.Windows.Forms.Label lblFramecount;
		private System.Windows.Forms.Label lblFramecountVal;
		private System.Windows.Forms.Label lblChecksum;
		private System.Windows.Forms.Label lblChecksumVal;
		private System.Windows.Forms.Label lblTitle;
		private System.Windows.Forms.Label lblTitleVal;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label lblHashVal;
	}
}

