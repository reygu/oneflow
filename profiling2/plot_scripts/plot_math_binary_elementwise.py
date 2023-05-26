import numpy as np
from matplotlib import pyplot as plt
import pandas as pd
import statistics
from scipy import stats
import numpy as np
from scipy import optimize  # 最小二乘法拟合
import matplotlib.pyplot as plt  # python matplotlib 绘图
from mpl_toolkits.mplot3d import Axes3D  # 3D 绘图
import sys
from scipy import optimize
import matplotlib.pyplot as plt
import numpy as np



df = pd.read_csv(r"results\profile_math_binary.20230524101016.csv")
df = df[df.op.apply(lambda x: not x.startswith("broadcast"))]

df['m'] = df['shape'].map(eval).map(lambda x:x[0][0])

log2_vars = ["m","time"]
for v in log2_vars:
    df[f"log2_{v}"] = df[v].map(np.log2)


grouped = dict(list(df.groupby('op')))



def piecewise_linear(x, x0, y0, k1, k2):
    # x<x0 ⇒ lambda x: k1*x + y0 - k1*x0
    # x>=x0 ⇒ lambda x: k2*x + y0 - k2*x0
    return np.piecewise(
        x,
        [x < x0, x >= x0],
        [lambda x: k1 * x + y0 - k1 * x0, lambda x: k2 * x + y0 - k2 * x0],
    )

plt.figure()

for op in grouped:
    x = np.array(grouped[op]["log2_m"])
    y = np.array(grouped[op]["log2_time"])
    p, e = optimize.curve_fit(piecewise_linear, x, y, bounds=(0, [20, 20, 2, 2]))
    xd = np.linspace(0, 30, 100)
    plt.title(op)
    plt.plot(x, y, "o")
    plt.plot(xd, piecewise_linear(xd, *p))
    print(op,'\t', ', '.join([f'{i:.5f}' for i in list(p)]))
plt.show()


for op in grouped:
    x = np.array(grouped[op]["log2_m"])
    y = np.array(grouped[op]["log2_time"])
    p, e = optimize.curve_fit(piecewise_linear, x, y, bounds=(0, [20, 20, 2, 2]))
    xd = np.linspace(0, 30, 100)
    plt.figure()
    plt.title(op)
    plt.plot(x, y, "o")
    plt.plot(xd, piecewise_linear(xd, *p))
    print(op,'\t', ', '.join([f'{i:.5f}' for i in list(p)]))
    plt.show()
   