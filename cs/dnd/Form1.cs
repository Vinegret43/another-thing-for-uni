using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace dnd
{
    public partial class Form1 : Form
    {
        Dungeon dungeon;
        Player player;

        public void PrintLine(string str)
        {
            textBoxLog.Text += str + Environment.NewLine;
            textBoxLog.SelectionStart = textBoxLog.TextLength;
            textBoxLog.ScrollToCaret();
        }

        public Form1()
        {
            InitializeComponent();
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);

            dungeon = Generators.GenerateDungeon();
            player = dungeon.Player;

            UpdateUI();
        }

        private void buttonDown_Click(object sender, EventArgs e)
        {
            Room? r = player.Room!.GetBottomNeighbor();
            MoveToRoom(r);
            UpdateUI();
            CheckForGameEnd();
        }

        private void buttonUp_Click(object sender, EventArgs e)
        {
            Room? r = player.Room!.GetTopNeighbor();
            MoveToRoom(r);
            UpdateUI();
            CheckForGameEnd();
        }

        private void UpdateUI()
        {
            labelHealth.Text = $"{player.Health}/{player.MaxHealth}";
            int percent = (int)((float)player.Health / (float)player.MaxHealth * 100.0);
            progressBar1.Value = Math.Max(percent, 0);
            labelDex.Text = $"{player.Dexterity}";
            labelStr.Text = $"{player.Strength}";
            labelWeapon.Text = player.Weapon.ToString();
            labelArmor.Text = player.Armor.ToString();

            listBoxInventory.DataSource = null;
            listBoxEnemies.DataSource = null;
            listBoxItems.DataSource = null;
            listBoxInventory.DataSource = player.Inventory;
            listBoxEnemies.DataSource = player.Room!.Npcs;
            listBoxItems.DataSource = player.Room!.Items;

            buttonLeft.Enabled = player.Room.HasLeftNeighbor();
            buttonRight.Enabled = player.Room.HasRightNeighbor();
            buttonTop.Enabled = player.Room.HasTopNeighbor();
            buttonBottom.Enabled = player.Room.HasBottomNeighbor();
        }

        private void buttonAttack_Click(object sender, EventArgs e)
        {
            int idx = listBoxEnemies.SelectedIndex;
            if (idx < 0) return;
            player.Turn = new AttackCreature { CreatureIdx = idx };
            dungeon.PlayRound();
            UpdateUI();
            CheckForGameEnd();
        }

        private void buttonUse_Click(object sender, EventArgs e)
        {
            int idx = listBoxInventory.SelectedIndex;
            if (idx < 0) return;
            player.Turn = new UseItem { ItemIdx = idx };
            dungeon.PlayRound();
            UpdateUI();
            CheckForGameEnd();
        }

        private void buttonTake_Click(object sender, EventArgs e)
        {
            int idx = listBoxItems.SelectedIndex;
            if (idx < 0) return;
            player.Turn = new TakeItem { ItemIdx = idx };
            dungeon.PlayRound();
            UpdateUI();
            CheckForGameEnd();
        }

        private void CheckForGameEnd()
        {
            if (player.Health <= 0)
            {
                MessageBox.Show("Ты умер, здоровье не выдержало. Надо было витаминов побольше есть");
                Application.Exit();
            }
            if (player.Room!.IsExit())
            {
                MessageBox.Show("Ты выбрался из подземелья. Победа. Пойди похвастайся перед друзьями");
                Application.Exit();
            }
        }

        private void buttonLeft_Click(object sender, EventArgs e)
        {
            Room? r = player.Room!.GetLeftNeighbor();
            MoveToRoom(r);
            UpdateUI();
            CheckForGameEnd();
        }

        private void buttonRight_Click(object sender, EventArgs e)
        {
            Room? r = player.Room!.GetRightNeighbor();
            MoveToRoom(r);
            UpdateUI();
            CheckForGameEnd();
        }
        private void MoveToRoom(Room? dest)
        {
            if (dest == null) return;
            Room src = player.Room!;
            if (src.Creatures.Count > 1 && dest.Creatures.Count > 1)
            {
                PrintLine("Проход загорожен монстрами. Туда не пройти");
                return;
            }
            src.RemoveCreature(player);
            dest.SpawnCreature(player);
        }
    }
}
