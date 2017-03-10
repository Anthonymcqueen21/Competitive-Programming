/**
 * Grab Snaffles and try to throw them through the opponent's goal!
 * Move towards a Snaffle and use your team id to determine where you need to throw it.
 **/
const optimalDistanceForFlipendo = 3500;

function findSnaffleToFlipendo(myWizard, snaffles) {
  const snaffleToFlipendo = snaffles.find(snaffle => {
    return getDistanceBetween(snaffle, myWizard) < optimalDistanceForFlipendo &&
    isSnaffleBetweenEnnemyGoal(myWizard, snaffle)
  });
  printErr('entityId', snaffleToFlipendo && snaffleToFlipendo.entityId);
  printErr('myWizard', myWizard.entityId);

  return snaffleToFlipendo && snaffleToFlipendo.entityId;
}

function isSnaffleBetweenEnnemyGoal(myWizard, snaffle) {
    return ((myWizard.x > snaffle.x && snaffle.x > enemyGoal.x)
      || (myWizard.x < snaffle.x && snaffle.x < enemyGoal.x)) &&
      ((myWizard.y > snaffle.y && snaffle.y > enemyGoal.y)
        || (myWizard.y < snaffle.y && snaffle.y < enemyGoal.y));
}

function getMyWizards(entities) {
  return entities.filter(entity => entity.entityType === 'WIZARD');
}

function getSnaffles(entities) {
  return entities.filter(entity => entity.entityType === 'SNAFFLE');
}

function getDistanceBetween(A, B) {
  return Math.sqrt((B.x - A.x)*(B.x - A.x) + (B.y - A.y)*(B.y - A.y));
}

function getClosestSnaffle(object, snaffles) {
  return snaffles.reduce((closestObject, snaffle) => {
    const newDistance = getDistanceBetween(object, snaffle);
    return newDistance < closestObject.distance ?
      Object.assign({}, snaffle, {distance: newDistance}):
      closestObject;
  }, {
    x: 0,
    y: 0,
    distance: 100000000000
  });
}

function targetSnaffle(snaffle) {
  print(`MOVE ${snaffle.x} ${snaffle.y} 150`);
}

var myTeamId = parseInt(readline()); // if 0 you need to score on the right of the map, if 1 you need to score on the left

var oppositeGoals = {
  0: {
    x: 16000,
    y: 3750
  },
  1: {
    x: 0,
    y: 3750
  }
};
var myGoals = {
  0: {
    x: 0,
    y: 3750
  },
  1: {
    x: 16000,
    y: 3750
  }
};

var enemyGoal = oppositeGoals[myTeamId];
var myGoal = myGoals[myTeamId];

let mana = 0;
// game loop
while (true) {
    var entities = parseInt(readline()); // number of entities still in game
    var entitiesTable = [];
    for (var i = 0; i < entities; i++) {
        var inputs = readline().split(' ');
        var entityId = parseInt(inputs[0]); // entity identifier
        var entityType = inputs[1]; // "WIZARD", "OPPONENT_WIZARD" or "SNAFFLE" (or "BLUDGER" after first league)
        var x = parseInt(inputs[2]); // position
        var y = parseInt(inputs[3]); // position
        var vx = parseInt(inputs[4]); // velocity
        var vy = parseInt(inputs[5]); // velocity
        var state = parseInt(inputs[6]); // 1 if the wizard is holding a Snaffle, 0 otherwise
        entitiesTable.push({
          entityId,
          entityType,
          x,
          y,
          vx,
          vy,
          state
        });
    }

    const myWizards = getMyWizards(entitiesTable);
    const snaffles = getSnaffles(entitiesTable);


    myWizards.forEach(function(myWizard) {
        printErr(`mana : ${mana}`);
        if (myWizard.state === 1) {
            print(`THROW ${enemyGoal.x} ${enemyGoal.y} 500`);
        }
        else {
          var closestSnaffle = getClosestSnaffle(myWizard, snaffles);
          // once we target a snaffle we remove it from the list
          var closestSnaffleIndex = snaffles.findIndex(snaffle => snaffle.x === closestSnaffle.x && snaffle.y === closestSnaffle.y);
          if (snaffles.length > 1) {
            snaffles.splice(closestSnaffleIndex, 1);
          }

          // TODO : extract method & check direction before flipendo
          if( mana >= 20) {
            const snaffleToFlipendoEntityId = findSnaffleToFlipendo(myWizard, snaffles);
            if (snaffleToFlipendoEntityId) {
              mana = mana - 20;
              print(`FLIPENDO ${snaffleToFlipendoEntityId}`);
            }
            else {
              targetSnaffle(closestSnaffle);
            }
          }
          else {
            targetSnaffle(closestSnaffle);
          }
        }
        mana = mana + 1;
        // Write an action using print()
        // To debug: printErr('Debug messages...');


        // Edit this line to indicate the action for each wizard (0 <= thrust <= 150, 0 <= power <= 500)
        // i.e.: "MOVE x y thrust" or "THROW x y power"
        // print('MOVE 8000 3750 100');
    })
}
