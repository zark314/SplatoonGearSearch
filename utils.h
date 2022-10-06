#pragma once

#define MAX_NAME_LEN 400
#define NUM_ABILITY 14
#define NUM_BRAND 20
#define TRUE 1
#define FALSE 0
#define MAX_ITER 30

#define ERR_OUT                                                                \
    {                                                                          \
        printf("Error: should not achieve here: %s:%d\n", __FILE__, __LINE__); \
        exit(-1);                                                              \
    }

typedef int brand_t;
typedef unsigned seed_t;
typedef int ability_t;
typedef int drink_t;

typedef struct {
    seed_t seed;
    brand_t brand;
    ability_t ability;
} gear_t;

extern const char ability_order[NUM_ABILITY][MAX_NAME_LEN];
extern const char brand_name[NUM_BRAND][MAX_NAME_LEN];
extern const ability_t brand_ability[NUM_BRAND][2];

// return the positive / negative intention ability of the gear
// according to its brand.
ability_t get_gear_pos_ability(gear_t* const gear_ptr);
ability_t get_gear_neg_ability(gear_t* const gear_ptr);

// a helper function
// iterate all abilities to find a solution
// the sol can not be same as "drink"
// gear's brand intentions are used.
ability_t helper_iterate_abilities(int seed, drink_t drink, gear_t* const gear_ptr);

// Return TRUE if valid else return FALSE
int valid_targets(ability_t* targets);

// Output Sol
void output_sol(gear_t* const bkp_ptr, int init_gear_iter,
                gear_t* const init_ptr, int cnt,
                drink_t drink, drink_t drink1, drink_t drink2,
                ability_t* targets, int res_cost, int cost, int verbose);

void find_sol(gear_t init_gear, ability_t* targets, int is_full_search);

// return the next seed
seed_t get_next_seed(seed_t current_seed);

// get the current ability when no drink
ability_t get_curr_ability_wo_drink(gear_t* const gear_ptr);

// get the current ability when there is a drink
// but the drink "fails" to hit
ability_t get_curr_ability_drink_fail(gear_t* const gear_ptr, drink_t drink);

// get the next gear, if drink is used
gear_t get_next_gear_using_drink(gear_t* const gear_ptr, drink_t drink);

// expand the search tree once
// check for all possible drinks to find an ability match
// if possible to find a match; reutrn drink
// if not possible; return -1
// check prefered drink first
drink_t expand_once(gear_t* const gear_ptr, ability_t target, drink_t prefer);

// assumption: there's already a match in target0
// check if target1 and target2 are possible to match.
// if possible; return drink1 * 100 + drink2
// if not; return -1
// check prefered drink first
int expand_further(gear_t* const gear_ptr, ability_t target1, ability_t target2, drink_t prefer);

// 计算花费几张饮料券
int cost_solve(drink_t drink, drink_t drink1, drink_t drink2);

void print_const_message1();
void print_const_message2();
void print_const_message3();