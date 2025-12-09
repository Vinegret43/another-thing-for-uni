using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dnd
{
    public static class Generators
    {
        public static Dungeon GenerateDungeon()
        {
            Player player = Player.Generate();
            int center = Config.DUNGEON_SIZE / 2; // Integer division in C#
            Dungeon dungeon = new Dungeon(player);
            Room startingRoom = new Room(dungeon, (center, center));
            startingRoom.SpawnCreature(player);

            // Initialize grid with null values
            Room?[,] roomGrid = new Room?[Config.DUNGEON_SIZE, Config.DUNGEON_SIZE];

            // Prim's algorithm
            roomGrid[center, center] = startingRoom;
            List<(int x, int y)> savedWalls = new List<(int, int)>
        {
            (center + 1, center),
            (center - 1, center),
            (center, center + 1),
            (center, center - 1)
        };

            Random random = new Random();

            while (savedWalls.Count > 0)
            {
                int chosenIndex = Rand.RandInt(0, savedWalls.Count - 1);
                (int x, int y) = savedWalls[chosenIndex];
                savedWalls.RemoveAt(chosenIndex);

                (int dx, int dy) dir = (0, 0);

                // Check for neighboring rooms
                if (x > 0 && roomGrid[x - 1, y] != null)
                {
                    if (dir != (0, 0)) continue;
                    dir = (-1, 0);
                }
                if (x < Config.DUNGEON_SIZE - 1 && roomGrid[x + 1, y] != null)
                {
                    if (dir != (0, 0)) continue;
                    dir = (1, 0);
                }
                if (y > 0 && roomGrid[x, y - 1] != null)
                {
                    if (dir != (0, 0)) continue;
                    dir = (0, -1);
                }
                if (y < Config.DUNGEON_SIZE - 1 && roomGrid[x, y + 1] != null)
                {
                    if (dir != (0, 0)) continue;
                    dir = (0, 1);
                }

                if (dir == (0, 0)) continue; // No neighboring room found

                // Create room at wall position
                roomGrid[x, y] = new Room(dungeon, (x, y));

                // Create room opposite direction (as per original algorithm)
                (int newX, int newY) = (x - dir.dx, y - dir.dy);
                roomGrid[newX, newY] = new Room(dungeon, (newX, newY));

                // Add new walls
                if (newX > 0 && roomGrid[newX - 1, newY] == null)
                {
                    savedWalls.Add((newX - 1, newY));
                }
                if (newX < Config.DUNGEON_SIZE - 1 && roomGrid[newX + 1, newY] == null)
                {
                    savedWalls.Add((newX + 1, newY));
                }
                if (newY > 0 && roomGrid[newX, newY - 1] == null)
                {
                    savedWalls.Add((newX, newY - 1));
                }
                if (newY < Config.DUNGEON_SIZE - 1 && roomGrid[newX, newY + 1] == null)
                {
                    savedWalls.Add((newX, newY + 1));
                }
            }

            // Convert to RoomGrid
            RoomGrid finalGrid = new RoomGrid(Config.DUNGEON_SIZE);
            for (int i = 0; i < Config.DUNGEON_SIZE; i++)
            {
                for (int j = 0; j < Config.DUNGEON_SIZE; j++)
                {
                    finalGrid[i, j] = roomGrid[i, j];
                }
            }
            dungeon.SetGrid(finalGrid);

            // Populate rooms with items and creatures
            PopulateRooms(dungeon, finalGrid, center);

            return dungeon;
        }

        private static void PopulateRooms(Dungeon dungeon, RoomGrid roomGrid, int center)
        {
            LootTable<IItem> itemLootTable = Generators.GetItemLootTable();
            LootTable<Creature> creatureLootTable = Generators.GetCreatureLootTable();

            for (int i = 0; i < Config.DUNGEON_SIZE; i++)
            {
                for (int j = 0; j < Config.DUNGEON_SIZE; j++)
                {
                    Room? room = roomGrid[i, j];
                    if (room != null && !room.IsExit())
                    {
                        (int x, int y) = room.Position;
                        float danger = (Math.Abs(x - center) + Math.Abs(y - center)) /
                                       (float)(center + center);

                        // Add items
                        int numItems = (int)Math.Round(
                            Config.MIN_ITEMS_IN_ROOM +
                            (Config.MAX_ITEMS_IN_ROOM - Config.MIN_ITEMS_IN_ROOM) * danger
                        );

                        for (int k = 0; k < numItems; k++)
                        {
                            IItem? item = itemLootTable.GenItem(danger);
                            if (item != null)
                            {
                                room.AddItem(item);
                            }
                        }

                        // Add creatures
                        int numCreatures = (int)Math.Round(
                            Config.MIN_ENEMIES_IN_ROOM +
                            (Config.MAX_ENEMIES_IN_ROOM - Config.MIN_ENEMIES_IN_ROOM) * danger
                        );

                        for (int k = 0; k < numCreatures; k++)
                        {
                            Creature? creature = creatureLootTable.GenItem(danger);
                            if (creature != null)
                            {
                                room.SpawnCreature(creature);
                            }
                        }
                    }
                }
            }
        }

        public static LootTable<IItem> GetItemLootTable()
        {
            LootTable<IItem> lootTable = new LootTable<IItem>();

            // Potions
            lootTable.AddItem(0.0f, 20, d => HealingPotion.Generate(d));
            lootTable.AddItem(0.1f, 5, d => StrengthPotion.Generate(d));
            lootTable.AddItem(0.1f, 5, d => DexterityPotion.Generate(d));

            // Weapons
            lootTable.AddItem(0.1f, 2, d => new Weapon(6, "Короткий меч"));
            lootTable.AddItem(0.2f, 3, d => new Weapon(8, "Длинный меч"));
            lootTable.AddItem(0.4f, 4, d => new Weapon(10, "Алебарда"));
            lootTable.AddItem(0.4f, 5, d => new Weapon(12, "Секира"));

            // Armor
            lootTable.AddItem(0.0f, 2, d => new Armor(13, "Кольчужная рубаха"));
            lootTable.AddItem(0.2f, 3, d => new Armor(15, "Полулаты"));
            lootTable.AddItem(0.4f, 4, d => new Armor(17, "Наборный доспех"));
            lootTable.AddItem(0.6f, 5, d => new Armor(19, "Латы"));

            // Magic Armor
            lootTable.AddItem(0.6f, 5, d => new MagicArmor(12, 20, "Магическая броня"));

            return lootTable;
        }

        public static LootTable<Creature> GetCreatureLootTable()
        {
            LootTable<Creature> lt = new LootTable<Creature>();
            lt.AddItem(0.01f, 10, d => OrkWarrior.Generate(d));
            lt.AddItem(0.01f, 10, d => OrkArcher.Generate(d));
            return lt;
        }
    }

    public class LootTable<T>
    {
        private readonly List<Tuple<float, int, Func<float, T>>> items = new List<Tuple<float, int, Func<float, T>>>();

        public void AddItem(float minDanger, int weight, Func<float, T> generator)
        {
            items.Add(Tuple.Create(minDanger, weight, generator));
        }

        public T? GenItem(float danger)
        {
            var validItems = new List<Tuple<float, int, Func<float, T>>>();
            int weightSum = 0;

            foreach (var item in items)
            {
                if (danger >= item.Item1)
                {
                    validItems.Add(item);
                    weightSum += item.Item2;
                }
            }

            if (weightSum == 0)
                return default(T);

            int r = Rand.RandInt(0, weightSum - 1);

            foreach (var item in validItems)
            {
                if (r < item.Item2)
                {
                    return item.Item3(danger);
                }
                r -= item.Item2;
            }

            throw new InvalidOperationException("Failed to generate item"); // Unreachable
        }
    }
    public static class Rand
    {
        private static readonly Random random = new Random();

        public static int RandInt(int min, int max)
        {
            return random.Next(min, max + 1);
        }
    }
}
