using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dnd
{
    public class Dungeon
    {
        private RoomGrid rooms;
        private Player player;

        public RoomGrid Grid => rooms;
        public Player Player => player;

        public void PlayRound()
        {
            player.Room.PlayRound();
        }

        public void ClearDeadCreatures()
        {
            player.Room.ClearDeadCreatures();
        }

        public void SetGrid(RoomGrid grid)
        {
            this.rooms = grid;
        }

        public Room GetRoom((int, int) pos)
        {
            if (Math.Min(pos.Item1, pos.Item2) < 0 ||
                Math.Max(pos.Item1, pos.Item2) >= Config.DUNGEON_SIZE)
            {
                return null;
            }
            return rooms[pos.Item1, pos.Item2];
        }

        public Dungeon(Player player)
        {
            this.player = player;
            this.rooms = new RoomGrid(Config.DUNGEON_SIZE);
        }
    }

    public class Room
    {
        private List<IItem> items;
        private List<Creature> creatures;
        private (int, int) pos;
        private Dungeon dungeon;

        public List<IItem> Items => items;
        public List<Creature> Creatures => creatures;

        public List<Creature> Npcs => creatures.Where(c => !(c is Player)).ToList();
        public (int X, int Y) Position => pos;
        public Dungeon Dungeon => dungeon;

        public void SpawnCreature(Creature creature)
        {
            creature.Room = this;
            creatures.Add(creature);
        }

        public void RemoveCreature(Creature creature)
        {
            creatures.Remove(creature);
        }

        public void AddItem(IItem item)
        {
            items.Add(item);
        }

        public IItem PopItem(int index)
        {
            IItem item = items[index];
            items.RemoveAt(index);
            return item;
        }

        public void PlayRound()
        {
            foreach (Creature creature in creatures)
            {
                if (creature.IsAlive)
                {
                    creature.MakeTurn();
                }
            }
            ClearDeadCreatures();
        }

        public void ClearDeadCreatures()
        {
            for (int i = creatures.Count - 1; i >= 0; i--)
            {
                Creature creature = creatures[i];
                if (!creature.IsAlive)
                {
                    creatures.RemoveAt(i);
                    List<IItem> inv = creature.ClearInventory();
                    items.AddRange(inv);
                    // In C#, objects are garbage collected, no delete needed
                }
            }
        }

        public bool IsExit()
        {
            if (pos.Item1 == 0 || pos.Item1 == Config.DUNGEON_SIZE - 1 ||
                pos.Item2 == 0 || pos.Item2 == Config.DUNGEON_SIZE - 1)
            {
                if (CountNeighbours() == 1)
                {
                    return true;
                }
            }
            return false;
        }

        // Direction methods
        public Room? GetLeftNeighbor()
        {
            if (Position.X > 0)
                return dungeon.Grid[Position.X - 1, Position.Y];
            return null;
        }

        public Room? GetRightNeighbor()
        {
            if (Position.X < Config.DUNGEON_SIZE - 1)
                return dungeon.Grid[Position.X + 1, Position.Y];
            return null;
        }

        public Room? GetTopNeighbor()
        {
            if (Position.Y > 0)
                return dungeon.Grid[Position.X, Position.Y - 1];
            return null;
        }

        public Room? GetBottomNeighbor()
        {
            if (Position.Y < Config.DUNGEON_SIZE - 1)
                return dungeon.Grid[Position.X, Position.Y + 1];
            return null;
        }

        public bool HasLeftNeighbor() => GetLeftNeighbor() != null;
        public bool HasRightNeighbor() => GetRightNeighbor() != null;
        public bool HasTopNeighbor() => GetTopNeighbor() != null;
        public bool HasBottomNeighbor() => GetBottomNeighbor() != null;

        private int CountNeighbours()
        {
            int count = 0;

            if (GetLeftNeighbor() != null) count++;
            if (GetRightNeighbor() != null) count++;
            if (GetTopNeighbor() != null) count++;
            if (GetBottomNeighbor() != null) count++;

            return count;
        }

        public Room(Dungeon dungeon, (int, int) pos)
        {
            this.dungeon = dungeon;
            this.pos = pos;
            this.items = new List<IItem>();
            this.creatures = new List<Creature>();
        }
    }

    public class RoomGrid
    {
        private Room?[,] rooms;

        public Room? this[int x, int y]
        {
            get => rooms[x, y];
            set => rooms[x, y] = value;
        }

        public int Size => rooms.GetLength(0);

        public RoomGrid(int size)
        {
            rooms = new Room?[size, size];
        }

        // Helper method to get non-null rooms
        public IEnumerable<Room> GetNonNullRooms()
        {
            for (int i = 0; i < Size; i++)
            {
                for (int j = 0; j < Size; j++)
                {
                    if (rooms[i, j] != null)
                    {
                        yield return rooms[i, j]!;
                    }
                }
            }
        }
    }
}
