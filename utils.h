#pragma once

#define MAX_NAME_LEN 70
#define NUM_ABILITY 14
#define NUM_BRAND 20

extern const char ability_order[NUM_ABILITY][MAX_NAME_LEN];
extern const char brand_name[NUM_BRAND][MAX_NAME_LEN];
extern const int brand_ability[NUM_BRAND][2];

typedef struct {
    unsigned int seed;
    int brand;
    int ability;
} gear_t;

void printfb(unsigned int a);