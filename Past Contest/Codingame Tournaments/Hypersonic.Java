import java.util.ArrayList;
import java.util.Scanner;
import java.util.Arrays;
import java.util.stream.Collectors;
import java.util.*;
import java.util.List;
class Player { public static void main(String[] args) { new Engine().start();}}
class AI {
  Grid grid;
  AI(Grid grid) {
    this.grid = grid;
  }

  boolean decision() {
    Tile item = closestItemTile(4);
    Tile bomb = bestBombTile();
    Tile safe = closestSafeTile();
    System.err.println(String.format("ITEM:%s\nBOMB:%s\nSAFE=%s", item, bomb, safe));
    Hero hero = grid.myHero();

    if (item == null && bomb == null && safe == null) {
      return stay("whoops");
    }
    if (item != null) {
      boolean canGo = true;
      for (Tile tile : grid.tilesIn(grid.tiles[hero.col][hero.row], 2)) {
        if (grid.boomMap[tile.col][tile.row] != Bomb.NO_EXPLODE) {
          canGo = false;
          break;
        }
      }
      if (canGo) {
        return move(item, String.format("item - %s/%s", item.col, item.row));
      }
    }

    if (bomb != null) {
      if (hero.col != bomb.col || hero.row != bomb.row) {
        return move(bomb, String.format("bomb - %s/%s", bomb.col, bomb.row));
      }
      if (hero.bombs > 0) {
        return bomb(bomb, String.format("bomb - %s/%s", bomb.col, bomb.row));
      }
    }

    if (safe != null) {
      return move(safe, String.format("safe - %s/%s", safe.col, safe.row));
    }

    return stay("" + (200 - grid.round));
  }

  Tile closestItemTile(int maxScanRadius) {
    List<Tile> items = new ArrayList<>(grid.moves)
        .stream()
        .filter(tile -> tile.type == Tile.Type.ITEM
            && tile.range < maxScanRadius)
        .collect(Collectors.toList());
    items.sort((tile1,tile2) -> tile1.range - tile2.range);
    if (items.isEmpty()) {
      return null;
    }
//    grid.debugList(items, 5, "ITEMS");
    return items.get(0);
  }

  Tile bestBombTile() {
    List<Tile> tiles = new ArrayList<>(grid.moves)
        .stream()
        .filter(tile -> grid.canBomb(tile))
        .collect(Collectors.toList());
    tiles.sort((tile1, tile2) -> {
      return (2 * grid.findExplodedBoxesCount(tile2) - tile2.range) - (2 * grid.findExplodedBoxesCount(tile1) - tile1.range);
    });
    if (tiles.isEmpty()) {
      return null;
    }
//    grid.debugList(tiles, 5, "BOMB TILES");
    return tiles.get(0);
  }

  Tile closestSafeTile() {
    List<Tile> safe = new ArrayList<>(grid.moves)
        .stream()
        .filter(move -> !grid.haveExplosionDanger(move))
        .collect(Collectors.toList());

    safe.sort((move1, move2) -> {
      int move1Boom = grid.boomMap[move1.col][move1.row];
      int move2Boom = grid.boomMap[move2.col][move2.row];
      if (move1Boom != move2Boom) {
        return move2Boom - move1Boom;
      }
      return move1.range - move2.range;
    });
    if (safe.isEmpty()) {
      return null;
    }
//    grid.debugList(safe, 5, "SAFE TILES");
    return safe.get(0);
  }

  //COMMANDS

  boolean stay(String message) {
    Hero my = grid.myHero();
    return move(my.col, my.row, message);
  }

  boolean stay() {
    return stay("");
  }

  boolean move(int col, int row, String message) {
    System.out.println(String.format("MOVE %s %s %s", col, row, message));
    return true;
  }

  boolean move(int col, int row) {
    return move(col, row, "");
  }

  boolean move(Tile target, String msg) {
    return move(target.col, target.row, msg);
  }

  boolean move(Tile tile) {
    return move(tile, tile.toString());
  }

  boolean bomb(int col, int row, String message) {
    System.out.println(String.format("BOMB %s %s %s", col, row, message));
    return true;
  }

  boolean bomb(int col, int row) {
    return bomb(col, row, "");
  }

  boolean bomb(Tile tile, String msg) {
    return bomb(tile.col, tile.row, msg);
  }

  boolean bomb(Tile tile) {
    return bomb(tile, tile.toString());
  }
}

class Bomb {

  static final int EXPLODE_NEXT_TURN = 1;
  static final int NO_EXPLODE = -10000;

  int col;
  int row;
  int owner;
  int time;//to explode
  int range;

  Bomb(int col, int row, int owner, int time, int range) {
    this.col = col;
    this.row = row;
    this.owner = owner;
    this.time = time;
    this.range = range;
  }

  @Override
  public String toString() {
    return String.format("(%s/%s[%s] - (%s, %s))", col, row, owner, time, range);
  }
}

class Engine {

  public void start() {
    Scanner scanner = new Scanner(System.in);
    int width = scanner.nextInt();
    int height = scanner.nextInt();
    int myId = scanner.nextInt();
    Grid grid = new Grid(width, height, myId);
    AI ai = new AI(grid);

    long time = System.nanoTime();
    int round = 0;

    while (true) {
      grid.updateMap(scanner);
      grid.updateEntities(scanner);
      grid.updateExplosionMap();
      grid.calculateAvailableTiles();

//      grid.debugMap();
//      grid.debugEntities();
//      grid.debugExplosions();
//      grid.debugPoints();
      grid.debugList(grid.moves, 5, "MOVES");

      ai.decision();

      System.err.println(String.format("=================%s ROUND COMPLETE====================", round));
      System.err.println(round + " round calc time in " + (System.nanoTime() - time) / 1000000 + " ms");
      time = System.nanoTime();
      round++;
      grid.round = round;
    }
  }
}

class Grid {
  int width;
  int height;
  int myID;

  Tile[][] tiles;
  int[][] boomMap;
  List<Hero> heroes = new ArrayList<>();
  List<Bomb> bombs = new ArrayList<>();
  List<Tile> moves = new ArrayList<>();
  int[] points = new int[4];

  EnumSet<Tile.Type> STOP_EXPLOSION = EnumSet.of(Tile.Type.BOX, Tile.Type.BOMB, Tile.Type.ITEM, Tile.Type.WALL);
  EnumSet<Tile.Type> NON_PASSABLE = EnumSet.of(Tile.Type.BOX, Tile.Type.BOMB, Tile.Type.WALL);
  int round;

  Grid(int width, int height, int myID) {
    this.width = width;
    this.height = height;
    this.myID = myID;
    tiles = new Tile[width][height];
    boomMap = new int[width][height];
    for (int col = 0; col < width; col++) {
      for (int row = 0; row < height; row++) {
        tiles[col][row] = new Tile(col, row);
      }
    }
  }

  void updateMap(Scanner scanner) {
    for (int row = 0; row < height; row++) {
      char[] line = scanner.next().toCharArray();
      for (int col = 0; col < line.length; col++) {
        switch (line[col]) {
          case '.':
            tiles[col][row].type = Tile.Type.FLOOR;
            break;
          case '0':
          case '1':
          case '2':
            tiles[col][row].type = Tile.Type.BOX;
            break;
          case 'X':
            tiles[col][row].type = Tile.Type.WALL;
            break;
        }

      }
    }
  }

  void updateEntities(Scanner scanner) {
    heroes.clear();
    bombs.clear();

    int entities = scanner.nextInt();
    for (int i = 0; i < entities; i++) {
      int entityType = scanner.nextInt();
      int owner = scanner.nextInt();
      int x = scanner.nextInt();
      int y = scanner.nextInt();
      int param1 = scanner.nextInt();
      int param2 = scanner.nextInt();

      switch (entityType) {
        case 0:
          heroes.add(new Hero(x, y, owner, param1, param2));
          break;
        case 1:
          bombs.add(new Bomb(x, y, owner, param1, param2));
          tiles[x][y].type = Tile.Type.BOMB;
          break;
        case 2:
          tiles[x][y].type = Tile.Type.ITEM;
          break;
      }
    }
  }

  void updateExplosionMap() {
    for (int col = 0; col < width; col++) {
      for (int row = 0; row < height; row++) {
        boomMap[col][row] = Bomb.NO_EXPLODE;
      }
    }

    //chains
    //todo make something about this ugly n^3
    for (Bomb bomb : bombs) {
      for (Tile exploded : findExplodedTiles(bomb)) {
        for (Bomb chained : bombs) {
          if (chained.col == exploded.col && chained.row == exploded.row) {
            int min = Math.min(bomb.time, chained.time);
            bomb.time = min;
            chained.time = min;
          }
        }
      }
    }

    for (Bomb bomb : bombs) {
      for (Tile exploded  : findExplodedTiles(bomb)) {
        if (boomMap[exploded.col][exploded.row] == Bomb.NO_EXPLODE) {
          boomMap[exploded.col][exploded.row] = bomb.time;
        } else {
          boomMap[exploded.col][exploded.row] = Math.min(bomb.time, boomMap[exploded.col][exploded.row]);
        }
        if (exploded.type == Tile.Type.BOX && bomb.time == Bomb.EXPLODE_NEXT_TURN) {
          points[bomb.owner]++;
        }
      }
    }
  }

  List<Tile> findExplodedTiles(Bomb bomb) {
    List<Tile> explode = new ArrayList<>();
    explode.add(tiles[bomb.col][bomb.row]);
    int lookCol, lookRow;
    Tile lookTile;
    //UP
    lookCol = bomb.col;
    lookRow = bomb.row - 1;
    while (lookRow >= 0 && bomb.row - lookRow < bomb.range) {
      lookTile = tiles[lookCol][lookRow];
      explode.add(lookTile);
      if (STOP_EXPLOSION.contains(lookTile.type)) {
        break;
      }
      lookRow--;
    }
    //DOWN
    lookCol = bomb.col;
    lookRow = bomb.row + 1;
    while (lookRow < height && lookRow - bomb.row < bomb.range) {
      lookTile = tiles[lookCol][lookRow];
      explode.add(lookTile);
      if (STOP_EXPLOSION.contains(lookTile.type)) {
        break;
      }
      lookRow++;
    }
    //LEFT
    lookCol = bomb.col - 1;
    lookRow = bomb.row;
    while (lookCol >= 0 && bomb.col - lookCol < bomb.range) {
      lookTile = tiles[lookCol][lookRow];
      explode.add(lookTile);
      if (STOP_EXPLOSION.contains(lookTile.type)) {
        break;
      }
      lookCol--;
    }
    //RIGHT
    lookCol = bomb.col + 1;
    lookRow = bomb.row;
    while (lookCol < width && lookCol - bomb.col < bomb.range) {
      lookTile = tiles[lookCol][lookRow];
      explode.add(lookTile);
      if (STOP_EXPLOSION.contains(lookTile.type)) {
        break;
      }
      lookCol++;
    }
    return explode;
  }

  int findExplodedBoxesCount(Bomb bomb) {
    return (int) findExplodedTiles(bomb).stream().filter(tile -> tile.type == Tile.Type.BOX).count();
  }

  int findExplodedBoxesCount(Tile tile) {
    Bomb newBomb = new Bomb(tile.col, tile.row, myID, 8, myHero().range);
    return findExplodedBoxesCount(newBomb);
  }

  boolean canBomb(Tile tile) {
    if (tile.type == Tile.Type.BOMB) {
      return false;
    }
    if (boomMap[tile.col][tile.row] != Bomb.NO_EXPLODE) {
      return false;
    }

    List<Tile> before = new ArrayList<>(moves);
    Bomb newBomb = new Bomb(tile.col, tile.row, myID, 8, myHero().range);
    before.removeAll(findExplodedTiles(newBomb));
//    if (tile.col == 1 && tile.row == 10) {
//      System.err.println("WILL EXPLODE->" + Arrays.toString(findExplodedTiles(newBomb).toArray()));
//      System.err.println("AFTER PREDICT->" + Arrays.toString(before.toArray()));
//    }
    return !before.isEmpty();
  }

  Hero myHero() {
    return heroes.stream().filter(hero -> hero.owner == myID).findFirst().get();
  }

  List<Tile> tilesIn(Tile from, int radius) {
    List<Tile> tiles = new ArrayList<>();
    int minCol = Math.max(from.col - radius, 0);
    int maxCol = Math.min(from.col + radius, width);
    int minRow = Math.max(from.row - radius, 0);
    int maxRow = Math.min(from.row + radius, height);
    for (int col = minCol; col < maxCol; col++) {
      for (int row = minRow; row < maxRow; row++) {
        for (Tile tile : moves) {
          if (tile.col == col && tile.row == row) {
            tiles.add(tile);
          }
        }
      }
    }
    return tiles;
  }

  void calculateAvailableTiles() {
    moves.clear();
    Set<Tile> visited = new HashSet<>();
    Set<Tile> unvisited = new HashSet<>();
    List<Tile> temp = new ArrayList<>();
    int roundsRange = 0;
    unvisited.add(tiles[myHero().col][myHero().row]);
    while (!unvisited.isEmpty()) {
      temp.clear();

      unvisited.forEach(temp::add);
      unvisited.clear();

      for (Tile tile : temp) {
        if (visited.contains(tile)) {
          continue;
        }
        visited.add(tile);
        tile.range = roundsRange;
        moves.add(tile);
        if (tile.col == 1 && tile.row == 6) {
          System.err.println("NEIGHT<-" + tile);
          System.err.println("NEIGH->" + Arrays.toString(neighbours(tile).toArray()));
        }
        for (Tile neigh : neighbours(tile)) {
          if (!visited.contains(neigh)) {
            unvisited.add(neigh);
          }
        }
      }

      roundsRange++;
    }
    moves.sort((o1, o2) -> o1.range - o2.range);
  }

  List<Tile> neighbours(Tile tile) {
    List<Tile> neighbours = new ArrayList<>();
    if (tile.col != 0) {
      Tile left = tiles[tile.col - 1][tile.row];
      if (!NON_PASSABLE.contains(left.type)
          && (boomMap[tile.col - 1][tile.row] != Bomb.EXPLODE_NEXT_TURN)
          && (boomMap[tile.col - 1][tile.row] != left.range)) {
        neighbours.add(left);
      }
    }
    if (tile.col != width - 1) {
      Tile right = tiles[tile.col + 1][tile.row];
      if (!NON_PASSABLE.contains(right.type)
          && (boomMap[tile.col + 1][tile.row] != Bomb.EXPLODE_NEXT_TURN)
          && (boomMap[tile.col + 1][tile.row] != right.range)) {
        neighbours.add(right);
      }
    }
    if (tile.row != 0) {
      Tile up = tiles[tile.col][tile.row - 1];
      if (!NON_PASSABLE.contains(up.type)
          && (boomMap[tile.col][tile.row - 1] != Bomb.EXPLODE_NEXT_TURN)
          && (boomMap[tile.col][tile.row - 1] != up.range)) {
        neighbours.add(up);
      }
    }
    if (tile.row != height - 1) {
      Tile bottom = tiles[tile.col][tile.row + 1];
      if (!NON_PASSABLE.contains(bottom.type)
          && (boomMap[tile.col][tile.row + 1] != Bomb.EXPLODE_NEXT_TURN)
          && (boomMap[tile.col][tile.row + 1] != bottom.range)) {
        neighbours.add(bottom);
      }
    }
    return neighbours;
  }

  boolean isReachable(int col, int row) {
    for (Tile move : moves) {
      if (move.col == col && move.row == row) {
        return true;
      }
    }
    return false;
  }

  boolean haveExplosionDanger(int col, int row) {
    return boomMap[col][row] != Bomb.NO_EXPLODE && boomMap[col][row] < 4;
  }

  boolean haveExplosionDanger(Tile tile) {
    return haveExplosionDanger(tile.col, tile.row);
  }

  void debugMap() {
    System.err.println("------GRID----------");
    StringBuilder builder = new StringBuilder();
    for (int row = 0; row < height; row++) {
      for (int col = 0; col < width; col++) {
        switch(tiles[col][row].type) {
          case BOMB:
            builder.append("B");
            break;
          case BOX:
            builder.append("0");
            break;
          case FLOOR:
            builder.append(".");
            break;
          case ITEM:
            builder.append("I");
            break;
          case WALL:
            builder.append("X");
            break;
        }
      }
      builder.append("\n");
    }
    System.err.println(builder.toString());
  }

  void debugEntities() {
    System.err.println("------ENTITITES----------");
    System.err.println("-- HEROES--" + Arrays.toString(heroes.toArray()));
    System.err.println("-- BOMBS--" + Arrays.toString(bombs.toArray()));
  }

  void debugExplosions() {
    System.err.println("------EXPLOSIONS----------");
    StringBuilder builder = new StringBuilder();
    for (int row = 0; row < height; row++) {
      for (int col = 0; col < width; col++) {
        int boomIn = boomMap[col][row];
        builder.append(boomIn == Bomb.NO_EXPLODE ? "X" : boomIn);
      }
      builder.append("\n");
    }
    System.err.println(builder.toString());
  }

  void debugPoints() {
    System.err.println("------POINTS----------");
    for (int owner = 0; owner < points.length; owner++) {
      System.err.println(String.format("%s have %s points", owner == myID ? "MY HERO" : "RIVAL", points[owner]));
    }
  }

  void debugList(List<Tile> list, int maxElements, String header) {
    System.err.println("----------" + header + "---------");
    StringBuilder builder = new StringBuilder();
    for (int i = 0; i < Math.min(list.size(), maxElements); i++) {
      builder.append(list.get(i)).append("\n");
    }
    System.err.println(builder.toString());
  }
}

class Hero {

  int col;//x-axis
  int row;//y-axis
  int owner;
  int bombs;
  int range;

  Hero(int col, int row, int owner, int bombs, int range) {
    this.col = col;
    this.row = row;
    this.owner = owner;
    this.bombs = bombs;
    this.range = range;
  }

  @Override
  public String toString() {
    return String.format("(pos(%s/%s) - owner[%s] - (%s, %s))", col, row, owner, bombs, range);
  }
}

class Tile {

  enum Type {
    FLOOR, BOX, BOMB, WALL, ITEM
  }

  int col;
  int row;
  Type type;

  int range = -1;//in rounds to hero, -1 not available

  Tile(int col, int row) {
    this.col = col;
    this.row = row;
  }

  @Override
  public boolean equals(Object o) {
    if (this == o) return true;
    if (o == null || getClass() != o.getClass()) return false;

    Tile tile = (Tile) o;

    if (col != tile.col) return false;
    if (row != tile.row) return false;
    return type == tile.type;

  }

  @Override
  public int hashCode() {
    int result = col;
    result = 31 * result + row;
    result = 31 * result + type.hashCode();
    return result;
  }

  @Override
  public String toString() {
    return String.format("(pos(%s/%s) - %s - range to hero %s", col, row, type, range == -1 ? "XX" : range);
  }
}
