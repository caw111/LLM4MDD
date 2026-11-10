import matplotlib.pyplot as plt
import numpy as np

plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False

# 精细时间轴，让突变更顺滑
t = np.linspace(0, 80, 300)
noise = np.zeros_like(t)

# 1. 0-20s 低噪声：稳定20微波动
mask1 = t <= 20
noise[mask1] = 20 + np.random.normal(0, 0.3, sum(mask1))

# 2. 20-40s 交谈噪声：30~60平缓波动
mask2 = (t > 20) & (t <= 40)
noise[mask2] = np.random.uniform(32, 58, sum(mask2))

# 3. 40-42s 极速冲高（突发噪声，快速破阈值）
mask_rise = (t > 40) & (t <= 42)
noise[mask_rise] = np.linspace(60, 88, sum(mask_rise))

# 4. 42-48s 短时持续高噪声（核心报警区间，缩短时长）
mask_alarm = (t > 42) & (t <= 48)
noise[mask_alarm] = 85 + np.random.normal(0, 2, sum(mask_alarm))

# 5. 48-52s 极速回落（快速降回正常区间）
mask_fall = (t > 48) & (t <= 52)
noise[mask_fall] = np.linspace(80, 25, sum(mask_fall))

# 6. 52-80s 全程恢复安静
mask5 = t > 52
noise[mask5] = 20 + np.random.normal(0, 0.3, sum(mask5))

# 绘图
fig, ax = plt.subplots(figsize=(10, 6))
ax.plot(t, noise, color='#1f77b4', linewidth=2.8, label='实时噪声值')
ax.axhline(y=70, color='#ff4b4b', linestyle='--', linewidth=2, label='报警阈值 70')

# 仅标注真实短时报警区间
ax.axvspan(42, 48, alpha=0.25, color='orange', label='蜂鸣器短时报警区间')

# 首次超标触发点（精准定位破70的位置）
ax.scatter(40.6, 70, color='red', s=70, zorder=5)
ax.annotate('首次触发蜂鸣', xy=(40.6, 70), xytext=(44, 82), fontsize=10, color='red',
            arrowprops=dict(arrowstyle='->', color='red', lw=1.5))

# 下方标注，彻底无重叠
ax.text(10, 12, '低噪声\n(环境安静)', ha='center', fontsize=10)
ax.text(30, 12, '交谈噪声\n(30~60)', ha='center', fontsize=10)
ax.text(45, 18, '突发高噪声\n(短时快速报警)', ha='center', fontsize=10, color='darkred')
ax.text(66, 12, '噪声完全恢复\n(回归安静)', ha='center', fontsize=10)

ax.set_xlabel('时间 (s)', fontsize=12)
ax.set_ylabel('噪声数值', fontsize=12)
ax.set_xlim(0, 80)
ax.set_ylim(0, 95)
ax.grid(alpha=0.3)
ax.legend(loc='upper left')
plt.title('环境噪声监测全周期实验数据曲线图（短时突发高噪声）', fontsize=14, pad=15)
plt.tight_layout()
plt.savefig('noise_experiment_fast.png', dpi=300, bbox_inches='tight')
plt.show()