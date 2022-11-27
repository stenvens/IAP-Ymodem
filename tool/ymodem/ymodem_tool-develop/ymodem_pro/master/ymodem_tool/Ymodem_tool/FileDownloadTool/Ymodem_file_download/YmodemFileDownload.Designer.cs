namespace Ymodem_tool
{
    partial class YmodemFileDownload
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(YmodemFileDownload));
            this.skinGroupBox_DelayTimeAfterPowerUp = new CCWin.SkinControl.SkinGroupBox();
            this.customCombox_Baud = new Ymodem_tool.CustomCombox();
            this.label2 = new System.Windows.Forms.Label();
            this.customCombox_TransmitMode = new Ymodem_tool.CustomCombox();
            this.customCombox_FileDownload_Port = new Ymodem_tool.CustomCombox();
            this.button_UartPortScan = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label_FileDownload_Port = new System.Windows.Forms.Label();
            this.skinGroupBox2 = new CCWin.SkinControl.SkinGroupBox();
            this.label_UploadProgress = new System.Windows.Forms.Label();
            this.progressBar_Upload = new System.Windows.Forms.ProgressBar();
            this.skinButton_FileDownload_StartDownload = new CCWin.SkinControl.SkinButton();
            this.richTextBox_UoloadResultDisplay = new System.Windows.Forms.RichTextBox();
            this.skinGroupBox1 = new CCWin.SkinControl.SkinGroupBox();
            this.richTextBox_UpdateBinFileData = new System.Windows.Forms.RichTextBox();
            this.skinButton_OpenUpdateFile = new CCWin.SkinControl.SkinButton();
            this.skinGroupBox_DelayTimeAfterPowerUp.SuspendLayout();
            this.skinGroupBox2.SuspendLayout();
            this.skinGroupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // skinGroupBox_DelayTimeAfterPowerUp
            // 
            this.skinGroupBox_DelayTimeAfterPowerUp.BackColor = System.Drawing.Color.Transparent;
            this.skinGroupBox_DelayTimeAfterPowerUp.BorderColor = System.Drawing.Color.DodgerBlue;
            this.skinGroupBox_DelayTimeAfterPowerUp.Controls.Add(this.customCombox_Baud);
            this.skinGroupBox_DelayTimeAfterPowerUp.Controls.Add(this.label2);
            this.skinGroupBox_DelayTimeAfterPowerUp.Controls.Add(this.customCombox_TransmitMode);
            this.skinGroupBox_DelayTimeAfterPowerUp.Controls.Add(this.customCombox_FileDownload_Port);
            this.skinGroupBox_DelayTimeAfterPowerUp.Controls.Add(this.button_UartPortScan);
            this.skinGroupBox_DelayTimeAfterPowerUp.Controls.Add(this.label1);
            this.skinGroupBox_DelayTimeAfterPowerUp.Controls.Add(this.label_FileDownload_Port);
            this.skinGroupBox_DelayTimeAfterPowerUp.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.skinGroupBox_DelayTimeAfterPowerUp.ForeColor = System.Drawing.Color.Black;
            this.skinGroupBox_DelayTimeAfterPowerUp.Location = new System.Drawing.Point(11, 41);
            this.skinGroupBox_DelayTimeAfterPowerUp.Name = "skinGroupBox_DelayTimeAfterPowerUp";
            this.skinGroupBox_DelayTimeAfterPowerUp.Radius = 15;
            this.skinGroupBox_DelayTimeAfterPowerUp.RectBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(240)))), ((int)(((byte)(240)))), ((int)(((byte)(240)))));
            this.skinGroupBox_DelayTimeAfterPowerUp.RoundStyle = CCWin.SkinClass.RoundStyle.All;
            this.skinGroupBox_DelayTimeAfterPowerUp.Size = new System.Drawing.Size(362, 140);
            this.skinGroupBox_DelayTimeAfterPowerUp.TabIndex = 29;
            this.skinGroupBox_DelayTimeAfterPowerUp.TabStop = false;
            this.skinGroupBox_DelayTimeAfterPowerUp.Text = " 参数设置";
            this.skinGroupBox_DelayTimeAfterPowerUp.TitleBorderColor = System.Drawing.Color.DodgerBlue;
            this.skinGroupBox_DelayTimeAfterPowerUp.TitleRectBackColor = System.Drawing.Color.White;
            this.skinGroupBox_DelayTimeAfterPowerUp.TitleRoundStyle = CCWin.SkinClass.RoundStyle.All;
            // 
            // customCombox_Baud
            // 
            this.customCombox_Baud.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.customCombox_Baud.FormattingEnabled = true;
            this.customCombox_Baud.Location = new System.Drawing.Point(82, 99);
            this.customCombox_Baud.Name = "customCombox_Baud";
            this.customCombox_Baud.Size = new System.Drawing.Size(205, 25);
            this.customCombox_Baud.TabIndex = 70;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.Location = new System.Drawing.Point(12, 99);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(51, 20);
            this.label2.TabIndex = 69;
            this.label2.Text = "波特率";
            // 
            // customCombox_TransmitMode
            // 
            this.customCombox_TransmitMode.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.customCombox_TransmitMode.FormattingEnabled = true;
            this.customCombox_TransmitMode.Location = new System.Drawing.Point(82, 64);
            this.customCombox_TransmitMode.Name = "customCombox_TransmitMode";
            this.customCombox_TransmitMode.Size = new System.Drawing.Size(205, 25);
            this.customCombox_TransmitMode.TabIndex = 68;
            this.customCombox_TransmitMode.SelectedIndexChanged += new System.EventHandler(this.customCombox_TransmitMode_SelectedIndexChanged);
            // 
            // customCombox_FileDownload_Port
            // 
            this.customCombox_FileDownload_Port.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.customCombox_FileDownload_Port.FormattingEnabled = true;
            this.customCombox_FileDownload_Port.Location = new System.Drawing.Point(82, 29);
            this.customCombox_FileDownload_Port.Name = "customCombox_FileDownload_Port";
            this.customCombox_FileDownload_Port.Size = new System.Drawing.Size(205, 25);
            this.customCombox_FileDownload_Port.TabIndex = 29;
            // 
            // button_UartPortScan
            // 
            this.button_UartPortScan.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.button_UartPortScan.BackgroundImage = global::ymodem_tool.Properties.Resources.refresh_72px;
            this.button_UartPortScan.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.button_UartPortScan.Font = new System.Drawing.Font("微软雅黑", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button_UartPortScan.Location = new System.Drawing.Point(312, 26);
            this.button_UartPortScan.Name = "button_UartPortScan";
            this.button_UartPortScan.Size = new System.Drawing.Size(30, 30);
            this.button_UartPortScan.TabIndex = 53;
            this.button_UartPortScan.UseVisualStyleBackColor = false;
            this.button_UartPortScan.Click += new System.EventHandler(this.button_UartPortScan_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(12, 65);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(65, 20);
            this.label1.TabIndex = 26;
            this.label1.Text = "传输方式";
            // 
            // label_FileDownload_Port
            // 
            this.label_FileDownload_Port.AutoSize = true;
            this.label_FileDownload_Port.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label_FileDownload_Port.Location = new System.Drawing.Point(12, 29);
            this.label_FileDownload_Port.Name = "label_FileDownload_Port";
            this.label_FileDownload_Port.Size = new System.Drawing.Size(51, 20);
            this.label_FileDownload_Port.TabIndex = 6;
            this.label_FileDownload_Port.Text = "串口号";
            // 
            // skinGroupBox2
            // 
            this.skinGroupBox2.BackColor = System.Drawing.Color.Transparent;
            this.skinGroupBox2.BorderColor = System.Drawing.Color.DodgerBlue;
            this.skinGroupBox2.Controls.Add(this.label_UploadProgress);
            this.skinGroupBox2.Controls.Add(this.progressBar_Upload);
            this.skinGroupBox2.Controls.Add(this.skinButton_FileDownload_StartDownload);
            this.skinGroupBox2.Controls.Add(this.richTextBox_UoloadResultDisplay);
            this.skinGroupBox2.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.skinGroupBox2.ForeColor = System.Drawing.Color.Black;
            this.skinGroupBox2.Location = new System.Drawing.Point(11, 372);
            this.skinGroupBox2.Name = "skinGroupBox2";
            this.skinGroupBox2.Radius = 15;
            this.skinGroupBox2.RectBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(240)))), ((int)(((byte)(240)))), ((int)(((byte)(240)))));
            this.skinGroupBox2.RoundStyle = CCWin.SkinClass.RoundStyle.All;
            this.skinGroupBox2.Size = new System.Drawing.Size(362, 149);
            this.skinGroupBox2.TabIndex = 53;
            this.skinGroupBox2.TabStop = false;
            this.skinGroupBox2.Text = " 升级进度";
            this.skinGroupBox2.TitleBorderColor = System.Drawing.Color.DodgerBlue;
            this.skinGroupBox2.TitleRectBackColor = System.Drawing.Color.White;
            this.skinGroupBox2.TitleRoundStyle = CCWin.SkinClass.RoundStyle.All;
            // 
            // label_UploadProgress
            // 
            this.label_UploadProgress.AutoSize = true;
            this.label_UploadProgress.BackColor = System.Drawing.Color.Transparent;
            this.label_UploadProgress.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.label_UploadProgress.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label_UploadProgress.Location = new System.Drawing.Point(166, 32);
            this.label_UploadProgress.Name = "label_UploadProgress";
            this.label_UploadProgress.Size = new System.Drawing.Size(35, 19);
            this.label_UploadProgress.TabIndex = 73;
            this.label_UploadProgress.Text = "0 %";
            // 
            // progressBar_Upload
            // 
            this.progressBar_Upload.ForeColor = System.Drawing.SystemColors.WindowText;
            this.progressBar_Upload.Location = new System.Drawing.Point(11, 25);
            this.progressBar_Upload.Name = "progressBar_Upload";
            this.progressBar_Upload.Size = new System.Drawing.Size(340, 32);
            this.progressBar_Upload.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.progressBar_Upload.TabIndex = 74;
            // 
            // skinButton_FileDownload_StartDownload
            // 
            this.skinButton_FileDownload_StartDownload.BackColor = System.Drawing.Color.Transparent;
            this.skinButton_FileDownload_StartDownload.BackgroundImage = global::ymodem_tool.Properties.Resources.Green_button_64px;
            this.skinButton_FileDownload_StartDownload.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.skinButton_FileDownload_StartDownload.BaseColor = System.Drawing.Color.Transparent;
            this.skinButton_FileDownload_StartDownload.BorderColor = System.Drawing.Color.Transparent;
            this.skinButton_FileDownload_StartDownload.ControlState = CCWin.SkinClass.ControlState.Normal;
            this.skinButton_FileDownload_StartDownload.DownBack = null;
            this.skinButton_FileDownload_StartDownload.Font = new System.Drawing.Font("微软雅黑", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.skinButton_FileDownload_StartDownload.ForeColor = System.Drawing.Color.White;
            this.skinButton_FileDownload_StartDownload.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.skinButton_FileDownload_StartDownload.Location = new System.Drawing.Point(250, 69);
            this.skinButton_FileDownload_StartDownload.MouseBack = null;
            this.skinButton_FileDownload_StartDownload.Name = "skinButton_FileDownload_StartDownload";
            this.skinButton_FileDownload_StartDownload.NormlBack = null;
            this.skinButton_FileDownload_StartDownload.Size = new System.Drawing.Size(101, 69);
            this.skinButton_FileDownload_StartDownload.TabIndex = 72;
            this.skinButton_FileDownload_StartDownload.Text = "开始下载";
            this.skinButton_FileDownload_StartDownload.UseVisualStyleBackColor = false;
            this.skinButton_FileDownload_StartDownload.Click += new System.EventHandler(this.skinButton_FileDownload_StartDownload_Click);
            // 
            // richTextBox_UoloadResultDisplay
            // 
            this.richTextBox_UoloadResultDisplay.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.richTextBox_UoloadResultDisplay.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.richTextBox_UoloadResultDisplay.Location = new System.Drawing.Point(11, 69);
            this.richTextBox_UoloadResultDisplay.Name = "richTextBox_UoloadResultDisplay";
            this.richTextBox_UoloadResultDisplay.Size = new System.Drawing.Size(231, 69);
            this.richTextBox_UoloadResultDisplay.TabIndex = 22;
            this.richTextBox_UoloadResultDisplay.Text = "";
            // 
            // skinGroupBox1
            // 
            this.skinGroupBox1.BackColor = System.Drawing.Color.Transparent;
            this.skinGroupBox1.BorderColor = System.Drawing.Color.DodgerBlue;
            this.skinGroupBox1.Controls.Add(this.richTextBox_UpdateBinFileData);
            this.skinGroupBox1.Controls.Add(this.skinButton_OpenUpdateFile);
            this.skinGroupBox1.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.skinGroupBox1.ForeColor = System.Drawing.Color.Black;
            this.skinGroupBox1.Location = new System.Drawing.Point(11, 194);
            this.skinGroupBox1.Name = "skinGroupBox1";
            this.skinGroupBox1.Radius = 15;
            this.skinGroupBox1.RectBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(240)))), ((int)(((byte)(240)))), ((int)(((byte)(240)))));
            this.skinGroupBox1.RoundStyle = CCWin.SkinClass.RoundStyle.All;
            this.skinGroupBox1.Size = new System.Drawing.Size(362, 164);
            this.skinGroupBox1.TabIndex = 52;
            this.skinGroupBox1.TabStop = false;
            this.skinGroupBox1.Text = " 升级文件信息";
            this.skinGroupBox1.TitleBorderColor = System.Drawing.Color.DodgerBlue;
            this.skinGroupBox1.TitleRectBackColor = System.Drawing.Color.White;
            this.skinGroupBox1.TitleRoundStyle = CCWin.SkinClass.RoundStyle.All;
            // 
            // richTextBox_UpdateBinFileData
            // 
            this.richTextBox_UpdateBinFileData.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.richTextBox_UpdateBinFileData.Font = new System.Drawing.Font("新宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.richTextBox_UpdateBinFileData.Location = new System.Drawing.Point(11, 26);
            this.richTextBox_UpdateBinFileData.Name = "richTextBox_UpdateBinFileData";
            this.richTextBox_UpdateBinFileData.Size = new System.Drawing.Size(277, 130);
            this.richTextBox_UpdateBinFileData.TabIndex = 4;
            this.richTextBox_UpdateBinFileData.Text = "";
            // 
            // skinButton_OpenUpdateFile
            // 
            this.skinButton_OpenUpdateFile.BackColor = System.Drawing.Color.Transparent;
            this.skinButton_OpenUpdateFile.BackgroundImage = global::ymodem_tool.Properties.Resources.Dark_blue_button_64px;
            this.skinButton_OpenUpdateFile.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.skinButton_OpenUpdateFile.BaseColor = System.Drawing.Color.Transparent;
            this.skinButton_OpenUpdateFile.BorderColor = System.Drawing.Color.Transparent;
            this.skinButton_OpenUpdateFile.ControlState = CCWin.SkinClass.ControlState.Normal;
            this.skinButton_OpenUpdateFile.DownBack = null;
            this.skinButton_OpenUpdateFile.Font = new System.Drawing.Font("微软雅黑", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.skinButton_OpenUpdateFile.ForeColor = System.Drawing.Color.White;
            this.skinButton_OpenUpdateFile.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.skinButton_OpenUpdateFile.Location = new System.Drawing.Point(299, 25);
            this.skinButton_OpenUpdateFile.MouseBack = null;
            this.skinButton_OpenUpdateFile.Name = "skinButton_OpenUpdateFile";
            this.skinButton_OpenUpdateFile.NormlBack = null;
            this.skinButton_OpenUpdateFile.Size = new System.Drawing.Size(52, 131);
            this.skinButton_OpenUpdateFile.TabIndex = 58;
            this.skinButton_OpenUpdateFile.Text = "打  开  文  件";
            this.skinButton_OpenUpdateFile.UseVisualStyleBackColor = false;
            this.skinButton_OpenUpdateFile.Click += new System.EventHandler(this.skinButton_OpenUpdateFile_Click);
            // 
            // YmodemFileDownload
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(385, 535);
            this.Controls.Add(this.skinGroupBox2);
            this.Controls.Add(this.skinGroupBox1);
            this.Controls.Add(this.skinGroupBox_DelayTimeAfterPowerUp);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "YmodemFileDownload";
            this.Text = "IAP升级";
            this.Load += new System.EventHandler(this.YmodemFileDownload_Load);
            this.skinGroupBox_DelayTimeAfterPowerUp.ResumeLayout(false);
            this.skinGroupBox_DelayTimeAfterPowerUp.PerformLayout();
            this.skinGroupBox2.ResumeLayout(false);
            this.skinGroupBox2.PerformLayout();
            this.skinGroupBox1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private CCWin.SkinControl.SkinGroupBox skinGroupBox_DelayTimeAfterPowerUp;
        private CustomCombox customCombox_TransmitMode;
        private CustomCombox customCombox_FileDownload_Port;
        private System.Windows.Forms.Button button_UartPortScan;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label_FileDownload_Port;
        private CCWin.SkinControl.SkinGroupBox skinGroupBox2;
        private System.Windows.Forms.Label label_UploadProgress;
        private System.Windows.Forms.ProgressBar progressBar_Upload;
        private CCWin.SkinControl.SkinButton skinButton_FileDownload_StartDownload;
        private System.Windows.Forms.RichTextBox richTextBox_UoloadResultDisplay;
        private CCWin.SkinControl.SkinGroupBox skinGroupBox1;
        private System.Windows.Forms.RichTextBox richTextBox_UpdateBinFileData;
        private CCWin.SkinControl.SkinButton skinButton_OpenUpdateFile;
        private CustomCombox customCombox_Baud;
        private System.Windows.Forms.Label label2;
    }
}