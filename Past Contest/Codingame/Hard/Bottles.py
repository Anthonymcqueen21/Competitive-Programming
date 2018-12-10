import sys
import math

# Made with love by AntiSquid, Illedan and Wildum.
# You can help children learn to code while you participate by donating to CoderDojo.

my_team = int(input())
bush_and_spawn_point_count = int(input())  # usefrul from wood1, represents the number of bushes and the number of places where neutral units can spawn
for i in range(bush_and_spawn_point_count):
    # entity_type: BUSH, from wood1 it can also be SPAWN
    entity_type, x, y, radius = input().split()
    x = int(x)
    y = int(y)
    radius = int(radius)
item_count = int(input())  # useful from wood2
for i in range(item_count):
    # item_name: contains keywords such as BRONZE, SILVER and BLADE, BOOTS connected by "_" to help you sort easier
    # item_cost: BRONZE items have lowest cost, the most expensive items are LEGENDARY
    # damage: keyword BLADE is present if the most important item stat is damage
    # move_speed: keyword BOOTS is present if the most important item stat is moveSpeed
    # is_potion: 0 if it's not instantly consumed
    item_name, item_cost, damage, health, max_health, mana, max_mana, move_speed, mana_regeneration, is_potion = input().split()
    item_cost = int(item_cost)
    damage = int(damage)
    health = int(health)
    max_health = int(max_health)
    mana = int(mana)
    max_mana = int(max_mana)
    move_speed = int(move_speed)
    mana_regeneration = int(mana_regeneration)
    is_potion = int(is_potion)

# game loop
while True:
    gold = int(input())
    enemy_gold = int(input())
    round_type = int(input())  # a positive value will show the number of heroes that await a command
    entity_count = int(input())
    for i in range(entity_count):
        # unit_type: UNIT, HERO, TOWER, can also be GROOT from wood1
        # shield: useful in bronze
        # stun_duration: useful in bronze
        # count_down_1: all countDown and mana variables are useful starting in bronze
        # hero_type: DEADPOOL, VALKYRIE, DOCTOR_STRANGE, HULK, IRONMAN
        # is_visible: 0 if it isn't
        # items_owned: useful from wood1
        unit_id, team, unit_type, x, y, attack_range, health, max_health, shield, attack_damage, movement_speed, stun_duration, gold_value, count_down_1, count_down_2, count_down_3, mana, max_mana, mana_regeneration, hero_type, is_visible, items_owned = input().split()
        unit_id = int(unit_id)
        team = int(team)
        x = int(x)
        y = int(y)
        attack_range = int(attack_range)
        health = int(health)
        max_health = int(max_health)
        shield = int(shield)
        attack_damage = int(attack_damage)
        movement_speed = int(movement_speed)
        stun_duration = int(stun_duration)
        gold_value = int(gold_value)
        count_down_1 = int(count_down_1)
        count_down_2 = int(count_down_2)
        count_down_3 = int(count_down_3)
        mana = int(mana)
        max_mana = int(max_mana)
        mana_regeneration = int(mana_regeneration)
        is_visible = int(is_visible)
        items_owned = int(items_owned)

    # Write an action using print
    # To debug: print("Debug messages...", file=sys.stderr)


    # If roundType has a negative value then you need to output a Hero name, such as "DEADPOOL" or "VALKYRIE".
    # Else you need to output roundType number of any valid action, such as "WAIT" or "ATTACK unitId"
    print("WAIT")
