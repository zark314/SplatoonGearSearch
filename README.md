# SplatoonGearSearch
喷喷喷装备制造辅助器

## 编译方式：
[splatoon_gear_search.exe](./splatoon_gear_search.exe) 在windows下可直接双击运行，无需编译。

其他平台需要先手动编译。[makefile](./makefile) 为样例。 以linux或macos为例：
```shell
make  # 编译
make run # 运行
```

## 运行：
运行后
1. 输入种子, 例如`0x915cfc31`
1. 输入当前装备品牌编号, 例如`12`
1. 输入想要的三个技能, 例如 `5 5 5`, 注意:
    * 输入`-1, -1, -1`返回所有可能的`.3`和`.2`组合
    * 如果只想要`.2`技能, 最后一个技能可设置为`-1`, 例如 `5 5 -1`
    * 目前不支持排列组合, 例如 `1 2 3` 和 `1 3 2`不同。
    * 程序可能会返回多个解，包括最快的解和饮料消耗较少的解
1. 输入另外的技能组合继续查询
1. `ctrl-c`结束程序


## 乱码问题：
[splatoon_gear_search.c](./splatoon_gear_search.c) 和 [constants.c](./constants.c) 可能采用gbk编码。[encoding/encoding.py](./encoding/encoding.py)是一个python3程序可以把相关代码转化为utf-8编码。

```shell
python3 encoding/encoding.py

# 输出为
# utils.h probably not gbk, skip
```

utf-8编码的代码另存在`encoding`目录下, 用来替换主目录的代码。替换后需要重新编译。
