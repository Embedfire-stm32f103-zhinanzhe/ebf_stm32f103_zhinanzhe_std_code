#! python3
#coding=utf-8

"""
Python版本：3.x
外部库：matplotlib1.5.3、numpy1.11.2

运行方式：
在命令行中输入：python sinWave.py 

运行结果：
命令行中会打印计算得的各点数据，
在当前目录下会生成py_dac_sinWav.c文件，包含上述数据，
并且会弹出描绘曲线的对话框。
"""

import matplotlib.pyplot as plt 
import numpy as np
import math

#修改本变量可以更改点数，如16、32、64等
POINT_NUM = 180
#正弦函数放大倍数
MULTIPLE = 2**9

#正弦函数X取值范围
X_VALUE = math.pi

# POINT_NUM 个点
n = np.linspace(0,X_VALUE,POINT_NUM)

#计算POINT_NUM个点的正弦值
a = map(math.sin,n)

r =[]
for i in a:
    temp = round(i*MULTIPLE)
    #得到序列
    r.append( temp )
   

print(list(map(int,r)))

#写入序列到文件
with open("py_pwm_sinWave.c",'w',encoding= 'gb2312') as f:
    print(list(map(int,r)),file= f)

#绘图
plt.plot(n,r,"-o")
plt.show()