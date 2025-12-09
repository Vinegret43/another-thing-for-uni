using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dnd
{
    public static class Config
    {
        // Must be a number calculated as 4*n + 1, where n is a natural number
        public const int DUNGEON_SIZE = 9;

        // These properties will be scaled linearly from MIN to MAX
        // as danger level rises from 0.0 to 1.0
        public const int MIN_ITEMS_IN_ROOM = 2;
        public const int MAX_ITEMS_IN_ROOM = 6;
        public const int MIN_ENEMIES_IN_ROOM = 1;
        public const int MAX_ENEMIES_IN_ROOM = 3;

        // === GENERAL LIMITS ===
        public const int MAX_DEXTERITY = 10;
        public const int MAX_STRENGTH = 10;

        // === PLAYER CHARACTER ===
        public const int MIN_STARTING_HELATH = 13;
        public const int MAX_STARTING_HELATH = 16;
        public const int MIN_STARTING_STR = 2;
        public const int MAX_STARTING_STR = 3;
        public const int MIN_STARTING_DEX = 2;
        public const int MAX_STARTING_DEX = 3;
    }
}
