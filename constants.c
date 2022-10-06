#include <stdio.h>
#include "utils.h"

const char ability_order[NUM_ABILITY][MAX_NAME_LEN] = {
    "主省 / 提升墨汁效率(主要武器)",
    "副省 / 提升墨汁效率(次要武器)",
    "墨回 / 提升墨汁槽的回复速度",
    "走速 / 提升人类状态移动速度",
    "游速 / 提升鱿鱼游动速度",
    "大招减量 / 提升特殊武器增加量",
    "sp死亡减轻 / 降低特殊武器减少量",
    "大招增强 / 提升特殊武器的性能",
    "快速复活 / 缩短复活时间",
    "超级跳 / 缩短超级跳跃的时间",
    "副强 / 提升次要武器的性能",
    "安全鞋 / 减轻对手墨汁影响",
    "防爆 / 减轻次要武器影响",
    "行动强化",
};

const char brand_name[NUM_BRAND][MAX_NAME_LEN] = {
    "战斗鱿鱼 / SquidForce",
    "钢铁先锋 / Zink",
    "海月 / Krak-On",
    "罗肯贝格 / Rockenberg",
    "泽酷 / Zekko",
    "锻品 / Forge",
    "暖流 / Firefin",
    "帆立 / Skalop",
    "寺门 / Splash Mob",
    "时雨 / Barazushi",
    "艾洛眼 / Tentatek",
    "暇古 / Takoroka",
    "无法无天 / Annaki",
    "鱿皇 / Enperry",
    "剑尖鱿 / Toni Kensa",
    "散寿司 / Barazushi",
    "七轮 / Emberz",
    "熊先生商会 / Grizzco",
    "鱼干制造 / Cuttlegear",
    "amiibo",
};
const ability_t brand_ability[20][2] = {
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

void print_const_message1() {
    printf("欢迎使用splatoon装备辅助程序\n此程序由本人一人完成,学艺不精,有bug实属正常,请您见谅\n也请您使用生成的方案之前验证一下\n");
    printf("本程序由消耗饮料券从多到少排序，每种消耗的饮料券只会生成一种结果\n");
    printf("如有问题请反馈给我\nQQ:1464243589\nB站ID:要历练的风\n");
    printf("请输入当前装备种子(例子:0x915cfc31):\n");
}

void print_const_message2() {
    printf("请按照下面的装备品牌编号，输入当前装备品牌编号：\n");
    for (brand_t i = 0; i < NUM_BRAND; i++) {
        printf("%d : %s\n", i, brand_name[i]);
    }
}

void print_const_message3() {
    printf("================== 准备开始搜索:\n");
    printf("请按照下面的技能编号，输入三个目标技能序列(例:0 0 0)\n");
    printf("如果只需要两个技能,要自己打孔，最后一个位置填-1(例:1 2 -1)\n");
    printf("如果不知道刷什么技能请输入-1 -1 -1 查看刷所有技能的消耗\n");
    printf("三个数之间请用空格隔开\n");
    for (brand_t i = 0; i < NUM_ABILITY; i++) {
        printf("%d : %s\n", i, ability_order[i]);
    }
}