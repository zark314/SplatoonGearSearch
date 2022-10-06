#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

int main() {
    ability_t targets[3] = {3, 3, 3};
    gear_t init_gear = {0, 0, 0};

    print_const_message1();
    scanf("%x", &init_gear.seed);
    print_const_message2();
    scanf("%d", &init_gear.brand);

    while (1) {
        print_const_message3();
        scanf("%d %d %d", &targets[0], &targets[1], &targets[2]);

        if (valid_targets(targets) == FALSE) {
            continue;
        }

        if (targets[0] == -1) {
            // 0.3
            for (ability_t i = 0; i < NUM_ABILITY; i++) {
                targets[0] = i;
                targets[1] = i;
                targets[2] = i;
                printf("%d: <%s> ", i, ability_order[i]);
                find_sol(init_gear, targets, FALSE);
            }
            printf("\n\n");
            // 0.2
            for (ability_t i = 0; i < NUM_ABILITY; i++) {
                targets[0] = i;
                targets[1] = i;
                targets[2] = -1;
                printf("%d: <%s> ", i, ability_order[i]);
                find_sol(init_gear, targets, FALSE);
            }
            continue;
        }

        find_sol(init_gear, targets, TRUE);
    }

    return 0;
}

void find_sol(gear_t init_gear_bkp, ability_t* targets, int is_full_search) {
    gear_t init_gear = init_gear_bkp;
    int res_cost = 4, cost;
    for (int init_gear_iter = 0; init_gear_iter < MAX_ITER; init_gear_iter++) {
        for (int cnt = 0; cnt < 3; cnt++) {
            for (drink_t drink = 14; drink >= 0; drink--) {
                gear_t gear = init_gear;
                for (int i = 0; i <= cnt; i++) { gear = get_next_gear_using_drink(&gear, drink); }

                if (gear.ability != targets[0]) { continue; }
                int combined_return = expand_further(&gear, targets[1], targets[2], drink);
                drink_t drink1 = combined_return / 100;
                drink_t drink2 = combined_return % 100;
                if (combined_return != -1) {
                    cost = cost_solve(drink, drink1, drink2);
                    
                    // discard if not a better sol.
                    if (res_cost <= cost) {  continue; }

                    output_sol(&init_gear_bkp, init_gear_iter, &init_gear, cnt, drink, drink1, drink2, targets, res_cost, cost, is_full_search);
                    res_cost = cost;
                    if (is_full_search == FALSE) { return; }
                }
            }
        }

        init_gear = get_next_gear_using_drink(&init_gear, -1);
    }
    if (res_cost == 4) {
        printf("没能找到合适的解\n");
    }
}

seed_t get_next_seed(seed_t current_seed) {
    current_seed ^= current_seed << 13;
    current_seed ^= current_seed >> 17;
    current_seed ^= current_seed << 5;
    return current_seed;
}

ability_t get_curr_ability_wo_drink(gear_t* const gear_ptr) {
    ability_t i;
    if (gear_ptr == NULL) {
        ERR_OUT;
    }

    // if brand has no intention
    if (gear_ptr->brand > 16 || gear_ptr->brand < 0) {
        return (gear_ptr->seed % 28) / 2;
    }

    // brand has an intention
    // -1 means no need to check for drink in the helper
    return helper_iterate_abilities(gear_ptr->seed % 35, -1, gear_ptr);
}

ability_t get_curr_ability_drink_fail(gear_t* gear_ptr, drink_t drink) {
    int mod;

    // determine "mod"
    if (get_gear_pos_ability(gear_ptr) == -1) {  // gear's brand has no intention
        mod = 26;
    } else if (drink == get_gear_pos_ability(gear_ptr)) {  // drink matches pos ability
        mod = 25;
    } else if (drink == get_gear_neg_ability(gear_ptr)) {  // drink matches neg ability
        mod = 34;
    } else {  // drink not related to gear's intention
        mod = 33;
    }

    return helper_iterate_abilities(gear_ptr->seed % mod, drink, gear_ptr);
}

gear_t get_next_gear_using_drink(gear_t* const gear_ptr, drink_t drink) {
    gear_t to_return = *gear_ptr;
    // advance seed
    to_return.seed = get_next_seed(gear_ptr->seed);

    // if no drink or invalid drink
    if (drink == -1 || drink == 14) {
        to_return.ability = get_curr_ability_wo_drink(&to_return);
        return to_return;
    }

    // check if drink gives an hit
    if ((to_return.seed % 100) < 30) {
        to_return.ability = drink;
        return to_return;
    }

    // advance seed again
    to_return.seed = get_next_seed(to_return.seed);
    to_return.ability = get_curr_ability_drink_fail(&to_return, drink);
    return to_return;
}

drink_t expand_once(gear_t* const gear_ptr, ability_t target, drink_t prefer) {
    gear_t next_gear;
    drink_t drink;

    for (int i = 0; i < 15; i++) {
        drink = (prefer + i) % 15;
        next_gear = get_next_gear_using_drink(gear_ptr, drink);
        if (next_gear.ability == target) {
            return drink;
        }
    }
    return -1;
}

int expand_further(gear_t* const gear_ptr, ability_t target1,
                   ability_t target2, drink_t prefer) {
    drink_t drink1, drink2;
    gear_t next_gear;
    for (int i = 0; i < 15; i++) {
        drink1 = (prefer + i) % 15;
        next_gear = get_next_gear_using_drink(gear_ptr, drink1);
        if (next_gear.ability == target1) {
            if (target2 == -1) {
                return drink1 * 100 + 14;
            }
            drink2 = expand_once(&next_gear, target2, drink1);
            if (drink2 != -1) {
                return drink1 * 100 + drink2;
            }
        }
    }
    return -1;
}

// 计算花费几张饮料券
int cost_solve(drink_t drink, drink_t drink1, drink_t drink2) {
    int cost = 0;

    if (drink != 14) {
        cost++;
    }
    if (drink != drink1 && drink1 != 14) {
        cost++;
    }
    if (drink1 != drink2 && drink2 != 14) {
        cost++;
    }

    return cost;
}

//////////////// helper

ability_t get_gear_pos_ability(gear_t* const gear_ptr) {
    return brand_ability[gear_ptr->brand][0];
}
ability_t get_gear_neg_ability(gear_t* const gear_ptr) {
    return brand_ability[gear_ptr->brand][1];
}
ability_t helper_iterate_abilities(int seed, drink_t drink, gear_t* const gear_ptr) {
    ability_t i;
    for (i = 0; i < NUM_ABILITY; i++) {
        if (i == drink) {
            continue;
        }
        if (i == get_gear_pos_ability(gear_ptr)) {
            seed = seed - 10;
        } else if (i == get_gear_neg_ability(gear_ptr)) {
            seed = seed - 1;
        } else {
            seed = seed - 2;
        }
        if (seed < 0) {
            return i;
        }
    }
    ERR_OUT;
}

int valid_targets(ability_t* targets) {
    for (int i = 0; i < 3; i++) {
        if (targets[i] < -1 || targets[i] >= NUM_ABILITY) {
            printf("%d 为非正常输入\n", targets[i]);
            return FALSE;
        }
    }
    if (targets[2] != -1) {
        if (targets[0] == -1 || targets[1] == -1) {
            printf("-1 需要在最后面\n");
            return FALSE;
        }
    } else {
        if (targets[1] != -1 && targets[0] == -1) {
            printf("-1 需要在最后面\n");
            return FALSE;
        }
    }
    return TRUE;
}

void output_sol(gear_t* const bkp_ptr, int init_gear_iter,
                gear_t* const init_ptr, int cnt,
                drink_t drink, drink_t drink1, drink_t drink2,
                ability_t* targets, int res_cost, int cost, int verbose) {
    if (verbose == FALSE) {
        if (targets[2] == -1) { 
            printf("0.2 最少需要刷 %d 个技能，需要 ", init_gear_iter + cnt + 2);
        } else { 
            printf("0.3 最少需要刷 %d 个技能，需要 ", init_gear_iter + cnt + 3);
        }
        printf("%d 张饮料券 分别是 |", cost);
        if (drink != 14) {
            printf("%s|", ability_order[drink]);
        }
        if (drink != drink1 && drink1 != 14) {
            printf("%s|", ability_order[drink1]);
        }
        if (targets[2] != -1 && drink1 != drink2 && drink2 != 14) {
            printf("%s|", ability_order[drink2]);
        }
        printf("\n");
        return;
    }
    if (res_cost == 4) {
        printf("这是  <<最快>>  的方案，需要消耗 %d 张饮料券\n", cost);
    } else {
        printf("此方案需要消耗 %d 张饮料券\n", cost);
    }
    
    gear_t gear_temp = *bkp_ptr;
    int output_idx = 1;
    for (int temp = 0; temp < init_gear_iter; temp++) {
        gear_temp = get_next_gear_using_drink(&gear_temp, -1);
        printf("  第 %d 个技能产生 %s 此时的种子为 0x%x\n", output_idx, ability_order[gear_temp.ability], gear_temp.seed);
        output_idx++;
    }
    if (drink != 14) {
        printf("<<<喝 %s 饮料>>>\n", ability_order[drink]);
    }
    gear_temp = *init_ptr;
    for (int temp = 0; temp < cnt; temp++) {
        gear_temp = get_next_gear_using_drink(&gear_temp, drink);
        printf("  第 %d 个技能产生 %s 此时的种子为 0x%x\n", output_idx, ability_order[gear_temp.ability], gear_temp.seed);
        output_idx++;
    }
    gear_t gear = get_next_gear_using_drink(&gear_temp, drink);
    printf("<<0.3的洗衣服>> <<0.2的按需打孔>> ，接下来的技能就是目标序列\n");
    printf("  第 %d 个技能产生 %s 此时的种子为 0x%x\n", output_idx++, ability_order[gear.ability], gear.seed);
    if (drink != drink1) {
        if (drink1 == 14) {
            printf("<<<取消>>>饮料效果\n");
        } else {
            printf("<<<喝 %s 饮料>>>\n", ability_order[drink1]);
        }
    }
    gear_t gear1 = get_next_gear_using_drink(&gear, drink1);
    gear_t gear2 = get_next_gear_using_drink(&gear1, drink2);
    printf("  第 %d 个技能产生 %s 此时的种子为  0x%x\n", output_idx++, ability_order[gear1.ability], gear1.seed);
    if (targets[2] != -1) {
        if (drink1 != drink2) {
            if (drink2 == 14) {
                printf("<<<取消>>>饮料效果\n");
            } else {
                printf("<<<喝 %s 饮料>>>\n", ability_order[drink2]);
            }
        }
        printf("  第 %d 个技能产生 %s 此时的种子为  0x%x\n", output_idx++, ability_order[gear2.ability], gear2.seed);
    }
    printf("\n\n\n");
}