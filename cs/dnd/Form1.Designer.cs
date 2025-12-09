namespace dnd
{
    partial class Form1
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
            components = new System.ComponentModel.Container();
            buttonTop = new Button();
            buttonBottom = new Button();
            buttonRight = new Button();
            buttonLeft = new Button();
            contextMenuStrip1 = new ContextMenuStrip(components);
            listBoxEnemies = new ListBox();
            label1 = new Label();
            buttonAttack = new Button();
            listBoxItems = new ListBox();
            label2 = new Label();
            buttonTake = new Button();
            label3 = new Label();
            label4 = new Label();
            label5 = new Label();
            labelHealth = new Label();
            progressBar1 = new ProgressBar();
            textBoxLog = new TextBox();
            label6 = new Label();
            label7 = new Label();
            labelDex = new Label();
            labelStr = new Label();
            label8 = new Label();
            label9 = new Label();
            labelWeapon = new Label();
            labelArmor = new Label();
            listBoxInventory = new ListBox();
            label10 = new Label();
            buttonUse = new Button();
            label11 = new Label();
            SuspendLayout();
            // 
            // buttonTop
            // 
            buttonTop.Location = new Point(1068, 789);
            buttonTop.Margin = new Padding(3, 4, 3, 4);
            buttonTop.Name = "buttonTop";
            buttonTop.Size = new Size(121, 45);
            buttonTop.TabIndex = 0;
            buttonTop.Text = "Вверх";
            buttonTop.UseVisualStyleBackColor = true;
            buttonTop.Click += buttonUp_Click;
            // 
            // buttonBottom
            // 
            buttonBottom.Location = new Point(1068, 842);
            buttonBottom.Margin = new Padding(3, 4, 3, 4);
            buttonBottom.Name = "buttonBottom";
            buttonBottom.Size = new Size(121, 45);
            buttonBottom.TabIndex = 1;
            buttonBottom.Text = "Вниз";
            buttonBottom.UseVisualStyleBackColor = true;
            buttonBottom.Click += buttonDown_Click;
            // 
            // buttonRight
            // 
            buttonRight.Location = new Point(1195, 789);
            buttonRight.Margin = new Padding(3, 4, 3, 4);
            buttonRight.Name = "buttonRight";
            buttonRight.Size = new Size(137, 98);
            buttonRight.TabIndex = 2;
            buttonRight.Text = "Направо";
            buttonRight.UseVisualStyleBackColor = true;
            buttonRight.Click += buttonRight_Click;
            // 
            // buttonLeft
            // 
            buttonLeft.Location = new Point(933, 789);
            buttonLeft.Margin = new Padding(3, 4, 3, 4);
            buttonLeft.Name = "buttonLeft";
            buttonLeft.Size = new Size(129, 98);
            buttonLeft.TabIndex = 3;
            buttonLeft.Text = "Налево";
            buttonLeft.UseVisualStyleBackColor = true;
            buttonLeft.Click += buttonLeft_Click;
            // 
            // contextMenuStrip1
            // 
            contextMenuStrip1.ImageScalingSize = new Size(32, 32);
            contextMenuStrip1.Name = "contextMenuStrip1";
            contextMenuStrip1.Size = new Size(61, 4);
            // 
            // listBoxEnemies
            // 
            listBoxEnemies.FormattingEnabled = true;
            listBoxEnemies.Location = new Point(933, 44);
            listBoxEnemies.Name = "listBoxEnemies";
            listBoxEnemies.Size = new Size(399, 260);
            listBoxEnemies.TabIndex = 5;
            listBoxEnemies.DoubleClick += buttonAttack_Click;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(933, 9);
            label1.Name = "label1";
            label1.Size = new Size(77, 32);
            label1.TabIndex = 6;
            label1.Text = "Враги";
            // 
            // buttonAttack
            // 
            buttonAttack.Location = new Point(933, 311);
            buttonAttack.Margin = new Padding(3, 4, 3, 4);
            buttonAttack.Name = "buttonAttack";
            buttonAttack.Size = new Size(399, 45);
            buttonAttack.TabIndex = 7;
            buttonAttack.Text = "Атаковать";
            buttonAttack.UseVisualStyleBackColor = true;
            buttonAttack.Click += buttonAttack_Click;
            // 
            // listBoxItems
            // 
            listBoxItems.FormattingEnabled = true;
            listBoxItems.Location = new Point(933, 414);
            listBoxItems.Name = "listBoxItems";
            listBoxItems.Size = new Size(399, 260);
            listBoxItems.TabIndex = 8;
            listBoxItems.DoubleClick += buttonTake_Click;
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(933, 379);
            label2.Name = "label2";
            label2.Size = new Size(128, 32);
            label2.TabIndex = 9;
            label2.Text = "Предметы";
            // 
            // buttonTake
            // 
            buttonTake.Location = new Point(933, 681);
            buttonTake.Margin = new Padding(3, 4, 3, 4);
            buttonTake.Name = "buttonTake";
            buttonTake.Size = new Size(399, 45);
            buttonTake.TabIndex = 10;
            buttonTake.Text = "Взять";
            buttonTake.UseVisualStyleBackColor = true;
            buttonTake.Click += buttonTake_Click;
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(933, 753);
            label3.Name = "label3";
            label3.Size = new Size(175, 32);
            label3.TabIndex = 11;
            label3.Text = "Перемещение";
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new Point(12, 9);
            label4.Name = "label4";
            label4.Size = new Size(70, 32);
            label4.TabIndex = 12;
            label4.Text = "Воин";
            // 
            // label5
            // 
            label5.AutoSize = true;
            label5.Location = new Point(12, 58);
            label5.Name = "label5";
            label5.Size = new Size(133, 32);
            label5.TabIndex = 13;
            label5.Text = "ЗДОРОВЬЕ";
            // 
            // labelHealth
            // 
            labelHealth.AutoSize = true;
            labelHealth.Location = new Point(163, 58);
            labelHealth.Name = "labelHealth";
            labelHealth.Size = new Size(78, 32);
            labelHealth.TabIndex = 14;
            labelHealth.Text = "label6";
            // 
            // progressBar1
            // 
            progressBar1.Location = new Point(20, 93);
            progressBar1.Name = "progressBar1";
            progressBar1.Size = new Size(338, 29);
            progressBar1.TabIndex = 15;
            progressBar1.Value = 50;
            // 
            // textBoxLog
            // 
            textBoxLog.Location = new Point(20, 580);
            textBoxLog.Multiline = true;
            textBoxLog.Name = "textBoxLog";
            textBoxLog.ReadOnly = true;
            textBoxLog.ScrollBars = ScrollBars.Vertical;
            textBoxLog.Size = new Size(846, 307);
            textBoxLog.TabIndex = 16;
            // 
            // label6
            // 
            label6.AutoSize = true;
            label6.Location = new Point(20, 164);
            label6.Name = "label6";
            label6.Size = new Size(62, 32);
            label6.TabIndex = 17;
            label6.Text = "ЛОВ";
            // 
            // label7
            // 
            label7.AutoSize = true;
            label7.Location = new Point(211, 164);
            label7.Name = "label7";
            label7.Size = new Size(63, 32);
            label7.TabIndex = 18;
            label7.Text = "СИЛ";
            // 
            // labelDex
            // 
            labelDex.AutoSize = true;
            labelDex.Location = new Point(88, 164);
            labelDex.Name = "labelDex";
            labelDex.Size = new Size(78, 32);
            labelDex.TabIndex = 19;
            labelDex.Text = "label8";
            // 
            // labelStr
            // 
            labelStr.AutoSize = true;
            labelStr.Location = new Point(280, 164);
            labelStr.Name = "labelStr";
            labelStr.Size = new Size(78, 32);
            labelStr.TabIndex = 20;
            labelStr.Text = "label8";
            // 
            // label8
            // 
            label8.AutoSize = true;
            label8.Location = new Point(20, 251);
            label8.Name = "label8";
            label8.Size = new Size(110, 32);
            label8.TabIndex = 21;
            label8.Text = "ОРУЖИЕ";
            // 
            // label9
            // 
            label9.AutoSize = true;
            label9.Location = new Point(20, 359);
            label9.Name = "label9";
            label9.Size = new Size(125, 32);
            label9.TabIndex = 22;
            label9.Text = "ДОСПЕХИ";
            // 
            // labelWeapon
            // 
            labelWeapon.AutoSize = true;
            labelWeapon.Location = new Point(20, 283);
            labelWeapon.Name = "labelWeapon";
            labelWeapon.Size = new Size(110, 32);
            labelWeapon.TabIndex = 23;
            labelWeapon.Text = "ОРУЖИЕ";
            // 
            // labelArmor
            // 
            labelArmor.AutoSize = true;
            labelArmor.Location = new Point(20, 391);
            labelArmor.Name = "labelArmor";
            labelArmor.Size = new Size(77, 32);
            labelArmor.TabIndex = 24;
            labelArmor.Text = "armor";
            // 
            // listBoxInventory
            // 
            listBoxInventory.FormattingEnabled = true;
            listBoxInventory.Location = new Point(459, 44);
            listBoxInventory.Name = "listBoxInventory";
            listBoxInventory.Size = new Size(399, 452);
            listBoxInventory.TabIndex = 25;
            listBoxInventory.DoubleClick += buttonUse_Click;
            // 
            // label10
            // 
            label10.AutoSize = true;
            label10.Location = new Point(459, 9);
            label10.Name = "label10";
            label10.Size = new Size(134, 32);
            label10.TabIndex = 26;
            label10.Text = "Инвентарь";
            // 
            // buttonUse
            // 
            buttonUse.Location = new Point(459, 503);
            buttonUse.Margin = new Padding(3, 4, 3, 4);
            buttonUse.Name = "buttonUse";
            buttonUse.Size = new Size(399, 45);
            buttonUse.TabIndex = 27;
            buttonUse.Text = "Использовать";
            buttonUse.UseVisualStyleBackColor = true;
            buttonUse.Click += buttonUse_Click;
            // 
            // label11
            // 
            label11.AutoSize = true;
            label11.Location = new Point(20, 545);
            label11.Name = "label11";
            label11.Size = new Size(144, 32);
            label11.TabIndex = 28;
            label11.Text = "Сообщения";
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(13F, 32F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1349, 922);
            Controls.Add(label11);
            Controls.Add(buttonUse);
            Controls.Add(label10);
            Controls.Add(listBoxInventory);
            Controls.Add(labelArmor);
            Controls.Add(labelWeapon);
            Controls.Add(label9);
            Controls.Add(label8);
            Controls.Add(labelStr);
            Controls.Add(labelDex);
            Controls.Add(label7);
            Controls.Add(label6);
            Controls.Add(textBoxLog);
            Controls.Add(progressBar1);
            Controls.Add(labelHealth);
            Controls.Add(label5);
            Controls.Add(label4);
            Controls.Add(label3);
            Controls.Add(buttonTake);
            Controls.Add(label2);
            Controls.Add(listBoxItems);
            Controls.Add(buttonAttack);
            Controls.Add(label1);
            Controls.Add(listBoxEnemies);
            Controls.Add(buttonLeft);
            Controls.Add(buttonRight);
            Controls.Add(buttonBottom);
            Controls.Add(buttonTop);
            Margin = new Padding(3, 4, 3, 4);
            Name = "Form1";
            StartPosition = FormStartPosition.WindowsDefaultBounds;
            Text = "D&D";
            ResumeLayout(false);
            PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonTop;
        private System.Windows.Forms.Button buttonBottom;
        private Button buttonRight;
        private Button buttonLeft;
        private ContextMenuStrip contextMenuStrip1;
        private ListBox listBoxEnemies;
        private Label label1;
        private Button buttonAttack;
        private ListBox listBoxItems;
        private Label label2;
        private Button buttonTake;
        private Label label3;
        private Label label4;
        private Label label5;
        private Label labelHealth;
        private ProgressBar progressBar1;
        private TextBox textBoxLog;
        private Label label6;
        private Label label7;
        private Label labelDex;
        private Label labelStr;
        private Label label8;
        private Label label9;
        private Label labelWeapon;
        private Label labelArmor;
        private ListBox listBoxInventory;
        private Label label10;
        private Button buttonUse;
        private Label label11;
    }
}

