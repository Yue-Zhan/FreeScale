#ifndef __MK60_MCG_H__
#define __MK60_MCG_H__

#include "common.h"

/********************************************************************/
typedef enum
{
    PLLUSR      ,  //自定义设置分频系数模式，直接加载 全局变量 mcg_div 的值
    PLL48,
    PLL64,
    PLL66,
    PLL68,
    PLL72,
    PLL80,
    PLL96,
    PLL128,
    PLL136,
    PLL148,
    PLL200,
    PLL220,

    PLL_MAX,
} PLL_e;

typedef struct
{
    uint16  clk;         //
    uint8   prdiv;       //外部晶振分频因子选项
    uint8   vdiv;        //外部晶振倍频因子选项
} mcg_cfg_t;

//时钟分频因子
typedef struct
{
    uint8 core_div;    //内核时钟分频因子
    uint8 bus_div;     //总线时钟分频因子  和 flash时钟分频因子
} mcg_div_t;


uint8 pll_init(PLL_e pll);

__RAMFUNC    void set_sys_dividers(uint32 outdiv1, uint32 outdiv2, uint32 outdiv3, uint32 outdiv4);

/********************************************************************/
#endif /* __MK60_MCG_H__ */
