using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dnd
{
    public interface IItem
    {
        string GetName();
        void Use(Creature user);
    }

    public interface IWeapon : IItem
    {
        int Attack(Creature attacker, Creature target);
        int GetHitDiceSides();
        void Unequip(Creature user);
    }

    public interface IArmor : IItem
    {
        void Unequip(Creature user);
        int GetArmorClass();
    }

    public class Weapon : IWeapon
    {
        string name;
        int hitDiceSides;

        public int Attack(Creature attacker, Creature target)
        {
            int hitRoll = Rand.RandInt(1, 20) + attacker.Dexterity;

            if (target.TryAttack(hitRoll))
            {
                int damageRoll = Rand.RandInt(1, hitDiceSides) + attacker.Strength;
                target.Wound(damageRoll, attacker);
                return damageRoll;
            }

            return 0;
        }

        public int GetHitDiceSides()
        {
            return hitDiceSides;
        }

        public void Use(Creature user)
        {
            user.UnsetWeapon();
            user.Weapon = this;
        }

        public void Unequip(Creature user)
        {
            user.Weapon = null;
        }

        public string GetName()
        {
            return name;
        }

        public override string ToString()
        {
            return $"{name} ({hitDiceSides})";
        }

        public Weapon(int hit_dice_sides, string name)
        {
            this.hitDiceSides = hit_dice_sides;
            this.name = name;
        }
    }

    public class Armor : IArmor
    {
        int armorClass;
        string name;
        public int GetArmorClass()
        {
            return armorClass;
        }

        public void Use(Creature user)
        {
            user.UnsetArmor();
            user.Armor = this;
        }

        public void Unequip(Creature user)
        {
            user.Armor = null;
        }

        public override string ToString()
        {
            return $"{name} ({armorClass})";
        }

        public string GetName()
        {
            return name;
        }
        public Armor(int armorClass, string name)
        {
            this.armorClass = armorClass;
            this.name = name;
        }
    }

    public class MagicArmor : IWeapon, IArmor
    {
        private int armorClass;
        private int hitDiceSides;
        private string name;

        public int Attack(Creature attacker, Creature target)
        {
            int hitRoll = Rand.RandInt(1, 20) + attacker.Dexterity;

            if (target.TryAttack(hitRoll))
            {
                int damageRoll = Rand.RandInt(1, hitDiceSides) + attacker.Strength;
                target.Wound(damageRoll, attacker);
                return damageRoll;
            }

            return 0;
        }

        public int GetHitDiceSides()
        {
            return hitDiceSides;
        }

        public int GetArmorClass()
        {
            return armorClass;
        }

        // IItem implementation (from both IWeapon and IArmor)
        public void Use(Creature user)
        {
            // Unequip both current armor and weapon
            user.UnsetArmor();
            user.UnsetWeapon();

            // Equip this as both armor and weapon
            user.Armor = this;
            user.Weapon = this;
        }

        public string GetName()
        {
            return name;
        }

        // Unequip methods for both interfaces
        public void Unequip(Creature user)
        {
            user.Armor = null;
            user.Weapon = null;
        }

        public override string ToString()
        {
            return $"{name} (AC: {armorClass}, DMG: {hitDiceSides})";
        }

        public MagicArmor(int hitDiceSides, int armorClass, string name)
        {
            this.armorClass = armorClass;
            this.hitDiceSides = hitDiceSides;
            this.name = name;
        }
    }

    public class HealingPotion : IItem
    {
        private int hits;
        private string name;

        public string GetName()
        {
            return name;
        }

        private HealingPotion(int hits, string name)
        {
            this.hits = hits;
            this.name = name;
        }

        public static HealingPotion Generate(float dangerLevel)
        {
            int healingAmount = Rand.RandInt(4, (int)(8 * (dangerLevel + 1)));
            return new HealingPotion(healingAmount, "Зелье лечения");
        }

        public void Use(Creature user)
        {
            user.Health = user.Health + hits;
            user.RemoveItem(this);
        }

        public override string ToString()
        {
            return name;
        }
    }

    public class StrengthPotion : IItem
    {
        private int increase;
        private string name;

        public string GetName()
        {
            return name;
        }

        private StrengthPotion(int increase, string name)
        {
            this.increase = increase;
            this.name = name;
        }

        public static StrengthPotion Generate(float dangerLevel)
        {
            int increase = Rand.RandInt(1, 2);
            return new StrengthPotion(increase, "Зелье силы");
        }

        public void Use(Creature user)
        {
            user.Strength = user.Strength + increase;
            user.RemoveItem(this);
        }

        public override string ToString()
        {
            return name;
        }
    }

    public class DexterityPotion : IItem
    {
        private int increase;
        private string name;

        public string GetName()
        {
            return name;
        }

        private DexterityPotion(int increase, string name)
        {
            this.increase = increase;
            this.name = name;
        }

        public static DexterityPotion Generate(float dangerLevel)
        {
            int increase = Rand.RandInt(1, 2);
            return new DexterityPotion(increase, "Зелье ловкости");
        }

        public void Use(Creature user)
        {
            user.Dexterity = user.Dexterity + increase;
            user.RemoveItem(this);
        }

        public override string ToString()
        {
            return name;
        }
    }
}
