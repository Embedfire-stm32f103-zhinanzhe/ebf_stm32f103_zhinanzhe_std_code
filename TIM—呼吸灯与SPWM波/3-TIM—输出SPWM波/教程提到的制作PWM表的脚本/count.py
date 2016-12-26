#! python3
#coding=utf-8

"""
Python版本：3.x
外部库：matplotlib1.5.3、numpy1.11.2

运行方式：
在命令行中输入：python index_wave.py 

运行结果：
命令行中会打印计算得的各点数据，
在当前目录下会生成py_index_wave.c文件，包含上述数据，
并且会弹出描绘曲线的对话框。
"""

import matplotlib.pyplot as plt 
import numpy as np
import math

#PWM点数
POINT_NUM = 180

#周期等级
PERIOD_CLASS = 1

#幅值等级
AMPLITUDE_CLASS = 256

TIMER_TIM_Period = 2**9
TIMER_TIM_Prescaler = 10

f_pclk = 72000000
t_pclk = 1/f_pclk

t_timer = t_pclk*TIMER_TIM_Prescaler*TIMER_TIM_Period

T_Point = t_timer * RGB_CLASS * PERIOD_CLASS


T_Up_Down_Cycle = T_Point * POINT_NUM

print ("呼吸周期：",T_Up_Down_Cycle)



