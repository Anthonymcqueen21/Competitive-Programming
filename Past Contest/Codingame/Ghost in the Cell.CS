using System;
using System.Collections.Generic;
using System.Linq;
class Factory
         {
    public int id;
    public int player;
    public int cyborgsNum;
    public int cyborgsProduction;
    public override string ToString() => $"F-{player}) {id} [{cyborgsNum}] ({cyborgsProduction})";
}

class Troop
{
    public int id;
    public int player;
    public int from;
    public int to;
    public int cyborgsNum;
    public int roundsToGo;
    public override string ToString() => $"T-{player}) {id} {from} -> {to} [{cyborgsNum}] ({roundsToGo})";
}

class Player
{
    #region Auxilary methods
    static void TryCatch(Action a) { try { a(); } catch (Exception) { } }
    static int TryGetInt(Func<int> a) { try { return a(); } catch (Exception) { return 0; } }
    static void Deb(object o) => Console.Error.WriteLine(o);
    static void DebList(IEnumerable<object> e) => Console.Error.WriteLine(e.Aggregate((x, y) => $"{x} {y}"));
    static void DebObjList(IEnumerable<object> e) => TryCatch(() => Console.Error.WriteLine(e.Aggregate((x, y) => $"{x}\n{y}")));
    static void DebDict(Dictionary<int, int> d)
    {
        foreach (var pair in d)
            Console.Error.WriteLine($"[{pair.Key}]:{pair.Value}");
    }
    #endregion

    #region Static global game state
    static int[,] MAP;
    static List<Factory> factories;
    static List<Troop> troops;
    static int BombesLeft = 2;
    static int BombesCoolDown = 0;
    static int Step = 0;
    static int RivalBaseId = 0;
    static int MyBaseId = 0;
    static int RivalProduction = 0;
    static int MyProduction = 0;
    static bool myUnderBombing = false;
    static List<int> underBombing = new List<int>();
    #endregion

    static bool CanBomb() => (BombesLeft > 0 && BombesCoolDown < 10);

    static int? NeedToBomb()
    {
        if (BombesLeft == 0) // No money, no party
            return null;
        var factoryDestinations = factories.Where(x => x.player == -1);
        if (factoryDestinations.Count() == 1 && BombesCoolDown <= 0)
        {
            BombesLeft--;
            BombesCoolDown = 11;
            return factoryDestinations.FirstOrDefault().id;
        }
        if (BombesCoolDown > 0)
        {
            BombesCoolDown--;
            return null;
        }
        else
        {
            int? target = null;
            int score = int.MinValue;
            foreach (var factory in factoryDestinations)
            {
                int tmpscore = factory.cyborgsProduction * 5 + factory.cyborgsProduction;
                if (tmpscore > score)
                {
                    score = tmpscore;
                    target = factory.id;
                }
            }
            if (target != null)
            {
                BombesLeft--;
                BombesCoolDown = 11;
                return target;
            }
        }
        return null;
    }

    static int? NeedToImprove()
    {
        try
        {
            int myScore = factories.Where(x => x.player == 1).Select(x => x.cyborgsNum).Aggregate((x, y) => x + y)
                          + TryGetInt(() => troops.Where(x => x.player == 1).Select(x => x.cyborgsNum).Aggregate((x, y) => x + y));
            int rivalScore = factories.Where(x => x.player == -1).Select(x => x.cyborgsNum).Aggregate((x, y) => x + y)
                          + TryGetInt(() => troops.Where(x => x.player == -1).Select(x => x.cyborgsNum).Aggregate((x, y) => x + y));
            if (myScore > 2 * rivalScore || Step > 25)
            {
                var myFractoriesToImprove = factories.Where(x => x.player == 1 && x.cyborgsNum > 11 && x.cyborgsProduction < 3).FirstOrDefault();
                return myFractoriesToImprove?.id;
            }
        }
        catch (Exception e) { Deb(e); }
        return null;
    }

    static Dictionary<int, int> BestAttackDestination()
    {
        var factoryDestinations = factories.Where(x => x.player != 1
                                                    && (x.cyborgsProduction > 0 || Step > 100)  // if STEP > 100 than probably the end of the game
                                                                                                // && (Step > 4 || MAP[x.id, RivalBaseId] >= MAP[x.id, MyBaseId])
                                                  );
        Deb("Attack calc");
        var result = new Dictionary<int, int>();
        foreach (var factory in factoryDestinations)
        {
            int newScore = 6 * factory.cyborgsProduction - ((factory.cyborgsProduction == 0) ? 1000 : 0) // want
                             - factory.cyborgsNum        // fear
                             + (factories.Where(x => x.player == 1).Sum(x => x.cyborgsNum - (int)(MAP[x.id, factory.id] * 2))); // can
            Deb($"[{factory.id}] = {5 * factory.cyborgsProduction - ((factory.cyborgsProduction == 0) ? 1000 : 0)} - {factory.cyborgsNum} + {(factories.Where(x => x.player == 1).Sum(x => x.cyborgsNum - (int)(MAP[x.id, factory.id])))} = {newScore}");
            result[factory.id] = newScore;
        }
        return result;
    }

    static Dictionary<int, int> BestDefenseDestination()
    {
        var factoryDestinations = factories.Where(x => x.player == 1);
        var result = new Dictionary<int, int>();
        Deb("Defense calc");
        foreach (var factory in factoryDestinations)
        {
            int newScore = TryGetInt(() => (troops.Where(x => x.player == -1 && x.to == factory.id).Sum(x => x.cyborgsNum))) // fear
                           - (factory.cyborgsProduction + ((factory.cyborgsProduction == 0) ? 1000 : 0)) // need
                           - factory.cyborgsNum;        // can
            Deb($"[{factory.id}] = {TryGetInt(() => (troops.Where(x => x.player == -1 && x.to == factory.id).Sum(x => x.cyborgsNum)))} - {(factory.cyborgsProduction + ((factory.cyborgsProduction == 0) ? 1000 : 0))} - {factory.cyborgsNum} = {newScore}");
            if (newScore > 0)
                result[factory.id] = newScore;
        }
        return result;
    }

    static List<string> ActionForDefenseMove(Dictionary<int, int> defenseMoves)
    {
        var actions = new List<string>();
        foreach (var factoryToDefend in defenseMoves.OrderByDescending(x => x.Value))
        {
            int needBots = factoryToDefend.Value;
            var myFactories = factories.Where(x => x.player == 1);
            foreach (var donor in myFactories)
            {
                if (donor.cyborgsNum > needBots)
                {
                    donor.cyborgsNum -= needBots;
                    needBots = 0;
                    if (donor.id != factoryToDefend.Key)
                        actions.Add($"MOVE {donor.id} {factoryToDefend.Key} {needBots}");
                }
                else if (donor.cyborgsNum > donor.cyborgsProduction)
                {
                    needBots -= donor.cyborgsNum - donor.cyborgsProduction;
                    if (donor.id != factoryToDefend.Key)
                        actions.Add($"MOVE {donor.id} {factoryToDefend.Key} {donor.cyborgsNum - donor.cyborgsProduction}");
                    donor.cyborgsNum = donor.cyborgsProduction;
                }
                if (needBots == 0)
                    break;
            }
        }
        return actions;
    }

    static List<string> ActionForAttackMove(Dictionary<int, int> attackMoves)
    {
        var actions = new List<string>();
        BombesCoolDown--;
        var myFactories = factories.Where(x => x.player == 1);
        foreach (var dest in attackMoves.OrderByDescending(x => x.Value))
        {
            int presentBots = factories.Where(x => x.id == dest.Key).FirstOrDefault().cyborgsNum;
            int productionBots = TryGetInt(() => factories.Where(x => x.id == dest.Key && x.player == -1).FirstOrDefault().cyborgsProduction * 3);
            int enemySendBots = TryGetInt(() => (troops.Where(x => x.player == -1 && x.to == dest.Key)?.Select(x => x.cyborgsNum)?.Aggregate((x, y) => x + y) ?? 0));
            int mySendBots = TryGetInt(() => (troops.Where(x => x.player == 1 && x.to == dest.Key)?.Select(x => x.cyborgsNum - x.roundsToGo)?.Aggregate((x, y) => x + y) ?? 0));
            int distance = TryGetInt(() => factories.Where(x => x.player == 1).Select(x => x.id).Aggregate((x, y) => x + MAP[y, dest.Key]) / myFactories.Count());
            //int stepAdd = Step / 40; // With time troops should be bigger 
            int neededBots = presentBots + productionBots + enemySendBots - mySendBots + distance;// + stepAdd;
            if (neededBots < 0)
                neededBots = 0;
            int initNeededBots = neededBots;
            int canSend = 0;
            var batchActions = new List<string>();

            bool willBomb = false;
            if (CanBomb() && neededBots > 9 && !underBombing.Contains(dest.Key) && (factories.Where(x => x.id == dest.Key).FirstOrDefault().player == -1))
            {
                //Deb($"CanBomb {CanBomb()}");
                batchActions.Add($"BOMB {factories.Where(x => x.player == 1).FirstOrDefault().id} {dest.Key}");
                neededBots -= 10;
                willBomb = true;
            }
            foreach (var myfactory in factories.Where(x => x.player == 1).OrderBy(x => MAP[x.id, dest.Key]))
            {
                if (myfactory.cyborgsNum > 2 * neededBots && neededBots > 4)
                {
                    batchActions.Add($"MOVE {myfactory.id} {dest.Key} {2 * neededBots}");
                    canSend += 2 * neededBots;
                    neededBots = 0;
                    myfactory.cyborgsNum -= 2 * neededBots;
                }
                else if (myfactory.cyborgsNum > neededBots + myfactory.cyborgsProduction + 1)
                {
                    batchActions.Add($"MOVE {myfactory.id} {dest.Key} {neededBots}");
                    canSend += neededBots;
                    neededBots = 0;
                    myfactory.cyborgsNum -= neededBots;
                }
                else if (myfactory.cyborgsNum > myfactory.cyborgsProduction + 1)
                {
                    batchActions.Add($"MOVE {myfactory.id} {dest.Key} {myfactory.cyborgsNum - myfactory.cyborgsProduction - 1}");
                    canSend += myfactory.cyborgsNum - myfactory.cyborgsProduction - 1;
                    neededBots -= myfactory.cyborgsNum - myfactory.cyborgsProduction - 1;
                    myfactory.cyborgsNum = myfactory.cyborgsProduction + 1;
                }
                if (neededBots == 0)
                    break;
            }
            if (canSend < initNeededBots && (factories.Where(x => x.id == dest.Key).FirstOrDefault()).player != -1)
            {
                willBomb = false;
                canSend = 0;
                batchActions = new List<string>();
            }
            if (willBomb)
            {
                underBombing.Add(dest.Key);
                BombesLeft--;
                BombesCoolDown = 10;
            }
            actions.AddRange(batchActions);
            Deb($"[{dest.Key}({dest.Value})]: Need {initNeededBots} ({presentBots}+{productionBots}+{enemySendBots}-{mySendBots}+1). Can send {canSend}");
        }
        /* Not top prio */
        int? improveId = NeedToImprove();
        if (improveId != null)
            actions.Add($"INC {improveId}");

        return actions;
    }

    static void Main(string[] args)
    {
        string[] inputs;
        int factoryCount = int.Parse(Console.ReadLine());
        MAP = new int[factoryCount, factoryCount];
        int linkCount = int.Parse(Console.ReadLine());
        for (int i = 0; i < linkCount; i++)
        {
            inputs = Console.ReadLine().Split(' ');
            int factory1 = int.Parse(inputs[0]);
            int factory2 = int.Parse(inputs[1]);
            int distance = int.Parse(inputs[2]);
            MAP[factory1, factory2] = MAP[factory2, factory1] = distance;
        }
        string message = "MSG All attack!!!";
        while (true)
        {
            factories = new List<Factory>();
            troops = new List<Troop>();
            int entityCount = int.Parse(Console.ReadLine());
            myUnderBombing = false;
            for (int i = 0; i < entityCount; i++)
            {
                inputs = Console.ReadLine().Split(' ');
                int entityId = int.Parse(inputs[0]);
                string entityType = inputs[1];
                int arg1 = int.Parse(inputs[2]);
                int arg2 = int.Parse(inputs[3]);
                int arg3 = int.Parse(inputs[4]);
                int arg4 = int.Parse(inputs[5]);
                int arg5 = int.Parse(inputs[6]);
                if (entityType == "FACTORY")
                    factories.Add(new Factory() { id = entityId, player = arg1, cyborgsNum = arg2, cyborgsProduction = arg3 });
                if (entityType == "TROOP")
                    troops.Add(new Troop() { id = entityId, player = arg1, from = arg2, to = arg3, cyborgsNum = arg4, roundsToGo = arg5 });
                if (entityType == "BOMB" && arg1 == -1)
                    myUnderBombing = true;
            }
            if (Step == 0)
            {
                RivalBaseId = factories.Where(x => x.player == -1).FirstOrDefault().id;
                MyBaseId = factories.Where(x => x.player == 1).FirstOrDefault().id;
                Deb($"My base {MyBaseId}; rival base {RivalBaseId}");
            }
            RivalProduction = TryGetInt(() => factories.Where(x => x.player == -1).Select(x => x.cyborgsProduction).Aggregate((x, y) => x + y));
            MyProduction = TryGetInt(() => factories.Where(x => x.player == 1).Select(x => x.cyborgsProduction).Aggregate((x, y) => x + y));
            message = $"MSG myProd {MyProduction}, rivalProd {RivalProduction}, b {myUnderBombing}";
            var defenseMoves = BestDefenseDestination();
            var attackMoves = BestAttackDestination();
            if (attackMoves.Count == 0 && defenseMoves.Count == 0)
                Console.WriteLine($"WAIT;{message}");
            else
            {
                var firstmoves = ActionForDefenseMove(defenseMoves);
                var secondarymoves = ActionForAttackMove(attackMoves);
                if (secondarymoves.Count == 0 && firstmoves.Count == 0)
                    Console.WriteLine($"WAIT;{message}");
                else
                {
                    firstmoves.AddRange(secondarymoves);
                    Console.WriteLine(firstmoves.Aggregate((x, y) => $"{x};{y}") + ";" + message);
                }
            }
            Step++;
        }
    }
}
