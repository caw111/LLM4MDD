## 噪音监控最小闭环（自然语言需求，图片整理版）

### 目标功能（最小原型）
- 采集环境噪声（麦克风采样）
- 计算声压级（SPL）或**近似强度/能量指标**（不要求物理标定的 dB SPL）
- 超过阈值触发报警/显示（至少给出 `alarm` 信号）
- 数码管显示当前噪声等级（至少显示一个连续量 `level` 或离散等级 `level_grade`）

### 明确不做（约束/排除项）
- 不做频谱分析（FFT/带通谱特征等）
- 不做复杂滤波（允许简单平滑/简单一阶滤波/滑动窗口均值）
- 不联网

### 功能模块划分
- `SoundSensor`：麦克风采样（输入源）
- `SignalProcess`：滤波 + 能量/强度计算（输出 `level`）
- `NoiseDecision`：阈值判断（输出 `alarm`）
- `Display`：数码管显示（消耗 `level` 与可选 `alarm`）

### 建议的可配置参数（便于自动化测试/迭代）
- `sample_rate_hz`：采样率（Hz）
- `window_size`：能量统计窗口长度（样本数）
- `threshold`：报警阈值（与 `level` 同量纲）
- `display_update_period_ms`：显示刷新周期（ms）
- `level_scale` / `level_offset`：可选的显示缩放（把 `level` 映射到数码管范围）

