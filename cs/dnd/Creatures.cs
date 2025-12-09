using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dnd
{
    public abstract class Creature
    {
        protected int health;
        protected int maxHealth;
        protected int dexterity;
        protected int strength;
        protected int factionId;
        protected string name;
        protected List<IItem> inventory;
        protected IArmor? armor;
        protected IWeapon? weapon;
        protected Room? room;

        protected IWeapon default_weapon = new Weapon(4, "Кулаки");
        protected IArmor default_armor = new Armor(8, "Голышом");

        // Properties with getters and setters
        public int Health
        {
            get => health;
            set => health = Math.Min(value, maxHealth);
        }

        public int MaxHealth
        {
            get => maxHealth;
            set => maxHealth = value;
        }

        public int Dexterity
        {
            get => dexterity;
            set => dexterity = Math.Min(value, Config.MAX_DEXTERITY);
        }

        public int Strength
        {
            get => strength;
            set => strength = Math.Min(value, Config.MAX_STRENGTH);
        }

        public int FactionId
        {
            get => factionId;
            protected set => factionId = value;
        }

        public string Name
        {
            get => name;
            set => name = value;
        }

        public IReadOnlyList<IItem> Inventory => inventory.AsReadOnly();

        public IArmor Armor
        {
            get => armor ?? default_armor;
            set => armor = value;
        }

        public IWeapon Weapon
        {
            get => weapon ?? default_weapon;
            set => weapon = value;
        }

        public Room? Room
        {
            get => room;
            set => room = value;
        }

        public void RemoveItem(IItem item)
        {
            inventory.Remove(item);
        }

        public List<IItem> ClearInventory()
        {
            List<IItem> oldInventory = new List<IItem>(inventory);
            inventory.Clear();
            return oldInventory;
        }

        public virtual void Wound(int damage, Creature source)
        {
            health -= damage;
        }

        public bool IsAlive => health > 0;

        public abstract void MakeTurn();

        public override string ToString() => $"{Name} ({Health})";

        // Pass the result of your hit roll (With modifiers included) in here.
        // Will return true if attack was successful, after that you can call Wound()
        public virtual bool TryAttack(int hitRoll)
        {
            return hitRoll >= Armor.GetArmorClass();
        }

        public void UnsetWeapon()
        {
            if (Weapon != null)
                Weapon.Unequip(this);
            Weapon = null;
        }

        public void UnsetArmor()
        {
            if (Armor != null)
                Armor.Unequip(this);
            Armor = null;
        }

        // Constructor
        public Creature(int health, int strength, int dexterity, List<IItem> inventory,
                       IWeapon? weapon, IArmor? armor, int factionId = 0, string name = "")
        {
            this.health = health;
            this.maxHealth = health;
            this.strength = strength;
            this.dexterity = dexterity;
            this.inventory = inventory ?? new List<IItem>();
            this.weapon = weapon;
            this.armor = armor;
            this.factionId = factionId;
            this.name = name;
        }
        public void AddToInventory(IItem item)
        {
            inventory.Add(item);
        }
    }

    public abstract class Turn;
    public class AttackCreature : Turn { public required int CreatureIdx { get; set; } }
    public class UseItem : Turn { public required int ItemIdx { get; set; } }
    public class TakeItem : Turn { public required int ItemIdx { get; set; } }

    public class Player : Creature
    {
        Turn? turn;
        public Turn? Turn
        {
            get => turn;
            set => turn = value;
        }
        public override void MakeTurn()
        {
            switch (turn)
            {
                case AttackCreature t:
                    {
                        Creature target = room.Creatures[t.CreatureIdx];
                        int dmg = this.Weapon.Attack(this, target);
                        Program.form.PrintLine(
                            $"Ты совершаешь атаку по существу {target.Name}, нанося ему {dmg} урона"
                        );
                        if (!target.IsAlive)
                        {
                            Program.form.PrintLine(
                                "Бедный падает на землю без сознания."
                            );
                        }
                        break;
                    }
                case UseItem t:
                    {
                        IItem item = inventory[t.ItemIdx];
                        item.Use(this);
                        break;
                    }
                case TakeItem t:
                    {
                        IItem item = room.PopItem(t.ItemIdx);
                        inventory.Add(item);
                        break;
                    }
            }
        }

        public override void Wound(int damage, Creature source)
        {
            Program.form.PrintLine($"Тебя атаковал {source.Name}! Ты получаешь {damage} урона");
            health -= damage;
        }

        public static Player Generate()
        {
            int hlth = Rand.RandInt(Config.MIN_STARTING_HELATH, Config.MAX_STARTING_HELATH);
            int str = Rand.RandInt(Config.MIN_STARTING_STR, Config.MAX_STARTING_STR);
            int dex = Rand.RandInt(Config.MIN_STARTING_DEX, Config.MAX_STARTING_DEX);

            List<IItem> inv = new List<IItem>();
            Weapon weapon = new Weapon(4, "Кинжал");
            Armor armor = new Armor(12, "Проклёпанная кожа");
            inv.Add(weapon);
            inv.Add(armor);
            return new Player(hlth, str, dex, inv, weapon, armor);
        }

        public Player(int hlth, int str, int dex, List<IItem> inv, IWeapon wep, IArmor arm): 
            base(hlth, str, dex, inv, wep, arm, 1, "Player")
        {

        }
    }

    public abstract class Ork : Creature
    {
        protected Ork(int health, int strength, int dexterity, List<IItem> inventory,
                     IWeapon? weapon, IArmor? armor, string name)
            : base(health, strength, dexterity, inventory, weapon, armor, 0, name)
        {}

        public override void MakeTurn()
        {
            if (Room == null || Weapon == null) return;

            // Find first enemy creature in the room and attack it
            foreach (Creature creature in Room.Creatures)
            {
                if (creature.FactionId != FactionId)
                {
                    Weapon.Attack(this, creature);
                    break;
                }
            }
        }
    }

    public class OrkWarrior : Ork
    {
        public OrkWarrior(int health, int strength, int dexterity, List<IItem> inventory,
                         IWeapon? weapon, IArmor? armor, string name)
            : base(health, strength, dexterity, inventory, weapon, armor, name)
        {
        }

        public static OrkWarrior Generate(float dangerLevel)
        {
            int health = Rand.RandInt(6, (int)(6 * (dangerLevel + 1)));
            int strength = Math.Min(Rand.RandInt(1, (int)(2 * (dangerLevel + 1))), Config.MAX_STRENGTH);
            int dexterity = Math.Min(Rand.RandInt(1, (int)(2 * (dangerLevel + 1))), Config.MAX_DEXTERITY);

            var weapon = new Weapon(Rand.RandInt(3, (int)(4 * (dangerLevel + 1))), "Воинский меч");
            var armor = new Armor(Rand.RandInt(11, (int)(13 * (dangerLevel / 3 + 1))), "Кольчуга");

            var inventory = new List<IItem> { weapon, armor };

            return new OrkWarrior(health, strength, dexterity, inventory, weapon, armor, "Орк-воин");
        }
    }

    public class OrkArcher : Ork
    {
        public OrkArcher(int health, int strength, int dexterity, List<IItem> inventory,
                        IWeapon? weapon, IArmor? armor, string name)
            : base(health, strength, dexterity, inventory, weapon, armor, name)
        {
        }

        public static OrkArcher Generate(float dangerLevel)
        {
            int health = Rand.RandInt(4, (int)(4 * (dangerLevel + 1)));
            int strength = Math.Min(Rand.RandInt(1, 2), Config.MAX_STRENGTH);
            int dexterity = Math.Min(Rand.RandInt(2, (int)(2 * (dangerLevel + 1))), Config.MAX_DEXTERITY);

            var weapon = new Weapon(Rand.RandInt(4, (int)(5 * (dangerLevel + 1))), "Лук");
            var armor = new Armor(Rand.RandInt(7, (int)(8 * (dangerLevel / 2 + 1))), "Проклёпанная кожа");

            var inventory = new List<IItem> { weapon, armor };

            return new OrkArcher(health, strength, dexterity, inventory, weapon, armor, "Орк-лучник");
        }
    }
}
