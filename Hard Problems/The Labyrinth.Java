import java.util.*;
import java.io.*;
import java.math.*;
/** Class representing a couple of cartesian coordinates*/
class Coords {
    int X, Y;
    
    public Coords(int X, int Y){
        this.X = X;
        this.Y = Y;
    }
    
    /** returns an array containing the 4 adjacent coordinates to this */
    public Coords[] neighbours() {
        Coords res[] = {new Coords(X, Y-1), new Coords(X+1, Y), new Coords(X, Y+1), new Coords(X-1, Y)};
        return res;
    }

    public boolean equals(Coords d) {
        return this.X == d.X && this.Y == d.Y;
    }
}
class Player {

    /** height of the labyrinth*/
    private static int height;
    /** width of the labyrinth*/
    private static int width;
    /** 2-dimensionnal array representing the labyrinth
     *  map[i][j] is the j-th cell of the i-th column, it can be 
     *  . : a hollow space
     *  # : a wall
     *  T : the starting position of kirk
     *  C : the control room
     *  ? : has not been scanned yet
     */
    private static char map[][];

    /** coordinates of Kirk*/
    private static Coords c;
    /** direction followed by kirk, which can be 1 of the 4 following constants*/
    private static int direction;
    private final static int UP = 0;
    private final static int RIGHT = 1;
    private final static int DOWN = 2;
    private final static int LEFT = 3;

    /** the 4 differents outputs possible*/
    private static final String output[] = {"UP", "RIGHT", "DOWN", "LEFT"};

    private static final Scanner in = new Scanner(System.in);

    /** For code clarity, returns map[s.Y][s.X]*/
    private static char map(Coords s) {
        return map[s.Y][s.X];
    }

    /** For code clarity, returns true if and only if s is in the map*/
    private static boolean isInMap(Coords s) {
        return 0 <= s.X && s.X < width && 0 <= s.Y && s.Y < height;
    }

    /** Returns true if and only if map(s) is one of the chars of 'avoid'*/
    private static boolean mustBeAvoided(Coords s, char[] avoid) {
        for (char ch : avoid)
            if (map(s) == ch)
                return true;
        return false;
    }

    /** Reads input for one game turn, updates kirk's coordinates,
     * and the part of the map newly discovered*/
    public static void readNewInfo() {
            c.Y = in.nextInt();
            c.X = in.nextInt();
            
            for (int y = 0; y < height; y++) {
                String ROW = in.next();
                if (c.Y-2 <= y && y <= c.Y+2)
                    for (int x = Math.max(c.X-2, 0); x < Math.min(c.X+3, width); x++)
                        map[y][x] = ROW.charAt(x);
            }

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    System.err.print(map[y][x]);
                }
                System.err.print("\n");
            }
    }
    
    /** Computes the shortest path from Kirk to a cell containing char 'target',
     * while avoiding characters contained by 'toAvoid'.
     * If onlyReveal is true, walks this path until the target cell is revealed (ie != '?')
     * Else, goes to the target cell.
     * @return false if no cell containing char 'target' can be found, true else.*/
    public static boolean goTo(char target, char[] toAvoid, boolean onlyReveal) {
        // initializes an array to remember predecessors during BFS
        int prec[][] = new int[height][width];
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
                prec[y][x] = -1;
            
        // initializes a queue to perform a BFS
        LinkedList<Coords> queue = new LinkedList<Coords>();
        queue.add(c);
        // perform a BFS, avoid characters contained by 'toAvoid', 
        // until character 'target is found'
        while(queue.size() > 0 && map(queue.element()) != target) {
            Coords[] neighbours = queue.remove().neighbours();
            for (int i = 0; i < neighbours.length; i++) {
                Coords n = neighbours[i];
                if (isInMap(n) && !mustBeAvoided(n, toAvoid) && prec[n.Y][n.X] == -1) {
                    prec[n.Y][n.X] = (i+2) % 4;
                    queue.add(n);
                }
            }
        }
    
        /* at this point, if queue is empty, no char 'target' is reachable 
        else, queue.element() is a target and path from Kirk to q.element() is generated
        by reading precursors*/

        if (queue.size() == 0)
            return false;
        else {
            Coords targetCell = queue.remove();
            Stack<Integer> path = new Stack<Integer>();
            Coords iter = targetCell;
            while (!iter.equals(c)) {
                path.push((prec[iter.Y][iter.X]+2) % 4);
                iter = iter.neighbours()[prec[iter.Y][iter.X]];  
            }
                
            // If we just want to reveal the target, we stop when it is
            // else we stop when Kirk is on it
            while ((onlyReveal && map(targetCell) == target) ||
                   (!onlyReveal && !path.empty())) { 
                System.out.println(output[path.pop()]);
                readNewInfo();
            }
            return true;
        }
    }

    public static void main(String args[]) {        
        height = in.nextInt();
        width = in.nextInt();
        direction = UP;
        c = new Coords(-1,-1);
        int A = in.nextInt();

        // Initializing the map
        map = new char[height][width];
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
                map[y][x] = '?';
        
        readNewInfo();

        // discovers the entire map
        char[] avoidWallsAndControl = { '#', 'C' };
        while (goTo('?', avoidWallsAndControl, true));

        // goes to control room and goes back to start
        char avoidWalls[] = { '#', '?' };
        goTo('C', avoidWalls, false);
        goTo('T', avoidWalls, false);
    }
}
   
