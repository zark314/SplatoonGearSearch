#include <stdio.h>

typedef struct
{
    unsigned int seed;
    int brand;
    int ability;
} seed_struct;

char ability_order[14][70] = {
    "主省",
    "副省",
    "墨回",
    "走速",
    "游速",
    "大招减量",
    "sp死亡减轻",
    "大招增强",
    "快速复活",
    "超级跳",
    "副强",
    "安全鞋",
    "防爆",
    "行动强化",
};
char brand_name[20][70] = {
    "战斗鱿鱼",
    "钢铁先锋",
    "海月",
    "罗肯贝格",
    "泽酷",
    "锻品",
    "暖流",
    "帆立",
    "寺门",
    "时雨",
    "艾洛眼",
    "暇古",
    "无法无天",
    "鱿皇",
    "剑尖鱿",
    "散寿司",
    "七轮",
    "熊先生商会",
    "鱼干制造",
    "amiibo",
};
int brand_ability[20][2] = {
    {11, 0},
    {9, 8},
    {4, 12},
    {3, 4},
    {6, 5},
    {7, 1},
    {1, 2},
    {8, 6},
    {0, 3},
    {12, 13},
    {2, 9},
    {5, 7},
    {1, 6},
    {10, 11},
    {0, 10},
    {13, 10},
    {13, 5},
    {-1, -1},
    {-1, -1},
    {-1, -1},
};

int printfb(unsigned int a)
{
    int i = 0;
    while (1)
    {
        printf("%d", a % 2);
        i++;
        a = a / 2;
        if (a == 0)
            break;
    }
    printf("\n");
    printf("%d\n", i);
}

// 计算下一步的种子
unsigned int seed_advance(unsigned int seed)
{
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed;
}

//计算当前品牌种子对应的技能 28 = 2*14 35 = 12 * 2 + 1 + 10
int ability_solve(unsigned int seed, int brand)
{
    int i;
    int seed_;
    if (brand > 16 || brand < 0)
    {
        seed_ = seed % 28;
        for (i = 0; i < 14; i++)
        {
            seed_ = seed_ - 2;
            if (seed_ < 0)
                return i;
        }
    }
    seed_ = seed % 35;
    for (i = 0; i < 14; i++)
    {
        if (i == brand_ability[brand][0])
            seed_ = seed_ - 10;
        else if (i == brand_ability[brand][1])
            seed_ = seed_ - 1;
        else
            seed_ = seed_ - 2;
        if (seed_ < 0)
            return i;
    }
}

//计算有饮料的情况下，而且饮料没有中的情况下的技能
int ability_solve_dd(seed_struct seed, int drink)
{
    int n, seed_, i;
    if (brand_ability[seed.brand][0] == -1)
    {
        n = 26;
    }
    else if (brand_ability[seed.brand][0] == drink)
    {
        n = 25;
    }
    else if (brand_ability[seed.brand][1] == drink)
    {
        n = 34;
    }
    else
        n = 33;
    seed_ = seed.seed % n;
    for (i = 0; i < 14; i++)
    {
        if (i == drink)
            continue;
        if (i == brand_ability[seed.brand][0])
            seed_ = seed_ - 10;
        else if (i == brand_ability[seed.brand][1])
            seed_ = seed_ - 1;
        else
            seed_ = seed_ - 2;
        if (seed_ < 0)
            return i;
    }
}

//计算有饮料的情况下的种子计算
seed_struct ability_solve_drink(seed_struct seed, int drink)
{
    int ability, seed_;
    seed.seed = seed_advance(seed.seed);
    if (drink == -1 || drink == 14)
    {
        seed.ability = ability_solve(seed.seed, seed.brand);
        return seed;
    }
    seed_ = seed.seed % 100;
    if (seed_ < 30)
    {
        seed.ability = drink;
        return seed;
    }
    seed.seed = seed_advance(seed.seed);
    seed.ability = ability_solve_dd(seed, drink);
    return seed;
}

//查找下一位种子是否符合序列条件
int search_seed_one(seed_struct seed, int target, int drink)
{
    int i = drink;
    seed_struct seed_next;
    while (1)
    {
        seed_next = ability_solve_drink(seed, i);
        if (seed_next.ability == target)
            return i;
        i = (i + 1) % 15;
        if (i == drink)
        {
            return -2;
        }
    }
}
//在所有的种子序列中寻找目标技能序列
int search_seed(seed_struct seed, int target1, int target2, int drink)
{
    int i = drink;
    int j;
    int k[2] = {0, 0};
    seed_struct seed_next;
    while (1)
    {
        seed_next = ability_solve_drink(seed, i);
        if (seed_next.ability == target1)
        {
            k[0] = i;
            if (target2 == -1)
                return k[0] * 100 + 14;
            k[1] = search_seed_one(seed_next, target2, i);
            if (k[1] != -2)
                return k[0] * 100 + k[1];
        }
        i = (i + 1) % 15;
        if (i == drink)
        {
            return -2;
        }
    }
}

//计算花费几张饮料券
int cost_solve(int i, int two)
{
    int cost = 0;
    int d1, d2;
    d1 = two / 100;
    d2 = two % 100;
    if (i != 14)
        cost++;
    if (i != d1 && d1 != 14)
        cost++;
    if (d1 != d2 && d2 != 14)
        cost++;
    return cost;
}

/*
大概思路就是先用种子衍化没有饮料的情况下种子衍化序列
从序号0开始衍化，每衍化一个就在这个种子的基础上，喝13种不同的饮料衍化三个接下来的序列
在这些演化出来的14×3个种子里面寻找与目标技能第一个相同的技能
如果找到，那就在这个种子的基础上继续寻找喝所有饮料的可能性下的下一个种子与技能
如果第二个第三个都与目标序列匹配
则代表找到目标序列
并且计算输出输出此时花费的饮料券
之后原始序列继续衍化，同种花费的饮料券只输出路线最少的情况
*/
int main()
{
    int i, j, k, ii, jj, kk;
    int two_step;
    int cost;
    int res_cost = 4;
    int target_code[3] = {3, 3, 3};
    seed_struct seed_begin, seed_ori, seed_brance;

    //一些输入程序
    printf("欢迎使用splatoon装备辅助程序\n此程序由本人一人完成,学艺不精,有bug实属正常,请您见谅\n也请您使用生成的方案之前验证一下\n");
    printf("本程序由消耗饮料券从多到少排序，每种消耗的饮料券只会生成一种结果\n");
    printf("如有问题请反馈给我\nQQ:1464243589\nB站ID:要历练的风\n");
    printf("请输入当前装备种子(例子:0x915cfc31):\n");
    scanf("%x", &seed_begin.seed);
    printf("请按照下面的装备品牌编号，输入当前装备品牌编号：\n");
    for (i = 0; i < 20; i++)
    {
        printf("%d : %s\n", i, brand_name[i]);
    }
    scanf("%d", &seed_begin.brand);
    printf("请按照下面的技能编号，输入三个目标技能序列(例:0 0 0)\n");
    printf("如果只需要两个技能,要自己打孔，最后一个位置填-1(例:1 2 -1)\n");
    printf("如果不知道刷什么技能请输入-1 -1 -1 查看刷所有技能的消耗\n");
    printf("三个数之间请用空格隔开\n");
    for (i = 0; i < 14; i++)
    {
        printf("%d : %s\n", i, ability_order[i]);
    }

    //这就是目标序列
    // target_loop_num的作用是在寻找全技能路线时标志寻找到哪个技能的数字
    scanf("%d %d %d", &target_code[0], &target_code[1], &target_code[2]);
    int target_loop_num = -1;
    if (target_code[0] == -1)
    {
        target_loop_num = 0;
        target_code[0] = target_loop_num;
        target_code[1] = target_loop_num;
        target_code[2] = target_loop_num;
    }
    seed_begin.ability = -1;
    int target_num = 15;
    int max_num = target_num * 2;
    seed_ori = seed_begin;
    seed_struct seed_res[max_num];
    //这是寻找所有技能可能性的程序，分别将目标序列设置成所有技能的0.3与0.2进行运行程序
    if (target_loop_num != -1)
    {
        int find = 0;
        int drink_num = 0;
        int drink_all[4];
        while (1)
        {
            drink_num = 0;
            find = 0;
            seed_ori = seed_begin;
            for (j = 0; j < max_num - 5; j++)
            {
                if (find != 0)
                    break;
                for (jj = 0; jj < 3; jj++)
                {
                    if (find != 0)
                        break;
                    for (i = 14; i > -1; i--)
                    {
                        if (find != 0)
                            break;
                        seed_brance = seed_ori;
                        for (ii = -1; ii < jj; ii++)
                        {
                            seed_brance = ability_solve_drink(seed_brance, i); //在喝饮料的情况下进行种子衍化
                        }
                        if (seed_brance.ability == target_code[0])
                        {
                            two_step = search_seed(seed_brance, target_code[1], target_code[2], i);
                            if (two_step != -2 || ((two_step / 100 == two_step % 100) && (two_step / 100 == i)))
                            {
                                find = 1;
                                drink_num = 0;
                                cost = cost_solve(i, two_step);
                                if (i != 14)
                                {
                                    drink_all[drink_num] = i;
                                    drink_num++;
                                }
                                if (two_step / 100 != 14 && two_step / 100 != i)
                                {
                                    drink_all[drink_num] = two_step / 100;
                                    drink_num++;
                                }
                                if (two_step % 100 != 14 && two_step / 100 != two_step % 100)
                                {
                                    drink_all[drink_num] = two_step % 100;
                                    drink_num++;
                                }
                                if (cost != drink_num)
                                {
                                    printf("error drink number\n");
                                    printf("cost = %d", cost);
                                    printf("drink_num = %d", drink_num);
                                    printf("i = %d\n", i);
                                    printf("two_step = %d\n", two_step);
                                    printf("drink_all = %d %d %d\n", drink_all[0], drink_all[1], drink_all[2]);
                                    return 0;
                                }
                                if (target_loop_num < 14)
                                {
                                    printf("%d: <%s> 0.3最少需要刷 %d 个技能，需要 %d 张饮料券 分别是 \\", target_loop_num, ability_order[target_loop_num], j + jj + 3, drink_num);
                                    for (ii = 0; ii < drink_num; ii++)
                                    {
                                        printf("%s\\", ability_order[drink_all[ii]]);
                                    }
                                    printf("\n");
                                    if (target_loop_num == 13)
                                        printf("\n\n");
                                }
                                if (target_loop_num > 13)
                                {
                                    printf("%d: <%s> 0.2最少需要刷 %d 个技能，需要 %d 张饮料券 分别是 \\", target_loop_num - 14, ability_order[target_loop_num - 14], j + jj + 2, drink_num);
                                    for (ii = 0; ii < drink_num; ii++)
                                    {
                                        printf("%s\\", ability_order[drink_all[ii]]);
                                    }
                                    printf("\n");
                                }
                            }
                        }
                    }
                }
                seed_ori = ability_solve_drink(seed_ori, -1);
                seed_res[j] = seed_ori;
            }
            target_loop_num = target_loop_num + 1;
            if (target_loop_num == 28)
            {
                scanf("%d", &i);
                return 0;
            }
            if (target_loop_num < 14)
            {
                target_code[0] = target_loop_num;
                target_code[1] = target_loop_num;
                target_code[2] = target_loop_num;
            }
            else
            {
                target_code[0] = target_loop_num - 14;
                target_code[1] = target_loop_num - 14;
                target_code[2] = -1;
            }
        }
    }

    //这是寻找特定的目标序列的程序
    for (j = 0; j < max_num - 5; j++)
    {
        //对每一个原始种子序列都要衍化十四次接下来的可能性，分别衍化三次，寻找是否包含目标序列
        for (jj = 0; jj < 3; jj++)
        {
            for (i = 14; i > -1; i--)
            {
                seed_brance = seed_ori; //进入支脉寻找
                for (ii = -1; ii < jj; ii++)
                {
                    seed_brance = ability_solve_drink(seed_brance, i); //在喝饮料的情况下进行种子衍化
                    seed_res[j + ii + 1] = seed_brance;                //记录可能的路线
                }
                if (seed_brance.ability == target_code[0])
                {                                                                           //匹配了目标序列的第一个技能
                    two_step = search_seed(seed_brance, target_code[1], target_code[2], i); //寻找后两个技能是否匹配
                    if (two_step != -2 || ((two_step / 100 == two_step % 100) && (two_step / 100 == i)))
                    {
                        cost = cost_solve(i, two_step); //计算方案花费饮料数量
                        if (res_cost <= cost)           //判断此时花费的饮料数量是否少于之前的方案，如果不少于，则丢弃
                        {
                            continue;
                        }
                        seed_res[j + jj + 1] = ability_solve_drink(seed_res[j + jj], two_step / 100);
                        seed_res[j + jj + 2] = ability_solve_drink(seed_res[j + jj + 1], two_step % 100);
                        if (res_cost == 4)
                        {
                            printf("这是  <<最快>>  的方案，需要消耗 %d 张饮料券\n", cost);
                        }
                        else
                        {
                            printf("此方案需要消耗 %d 张饮料券\n", cost);
                        }
                        res_cost = cost;
                        for (ii = 0; ii < j; ii++)
                        {
                            printf("  第 %d 个技能产生 %s   此时的种子为  0x%x\n", ii + 1, ability_order[seed_res[ii].ability], seed_res[ii].seed);
                        }
                        if (i != 14)
                            printf("<<<喝 %s 饮料>>>\n", ability_order[i]);
                        for (ii = 0; ii < jj; ii++)
                        {
                            printf("  第 %d 个技能产生 %s   此时的种子为  0x%x\n", j + ii + 1, ability_order[seed_res[j + ii].ability], seed_res[j + ii].seed);
                        }
                        printf("<<0.3的洗衣服>> <<0.2的按需打孔>> ，接下来的技能就是目标序列\n");
                        printf("  第 %d 个技能产生 %s   此时的种子为  0x%x\n", j + jj + 1, ability_order[seed_res[j + jj].ability], seed_res[j + ii].seed);
                        if (two_step / 100 != i)
                        {
                            if (two_step / 100 == 14)
                                printf("<<<取消>>>饮料效果\n");
                            else
                                printf("<<<喝 %s 饮料>>>\n", ability_order[two_step / 100]);
                        }
                        printf("  第 %d 个技能产生 %s   此时的种子为  0x%x\n", j + jj + 2, ability_order[seed_res[j + jj + 1].ability], seed_res[j + jj + 1].seed);
                        if (target_code[2] != -1)
                        {
                            if (two_step % 100 != two_step / 100)
                            {
                                if (two_step % 100 == 14)
                                    printf("<<<取消>>>饮料效果\n");
                                else
                                    printf("<<<喝 %s 饮料>>>\n", ability_order[two_step % 100]);
                            }
                            printf("  第 %d 个技能产生 %s   此时的种子为  0x%x\n", j + jj + 3, ability_order[seed_res[j + ii + 2].ability], seed_res[j + jj + 2].seed);
                        }
                        printf("\n\n\n");
                    }
                }
            }
        }
        // printf("0x%x \n", seed_ori.seed);
        seed_ori = ability_solve_drink(seed_ori, -1);
        seed_res[j] = seed_ori;
    }
    if (res_cost == 4)
    {
        printf("Sorry,找不到目标组合");
    }
    scanf("%d", &i);
    return 0;
}
