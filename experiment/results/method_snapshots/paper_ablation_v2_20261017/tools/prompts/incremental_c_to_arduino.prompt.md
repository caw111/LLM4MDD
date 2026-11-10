当前为增量更新模式: 需要在历史系统基础上做小幅修改，保持整体结构稳定。

历史系统JSON:
<<PRIOR_SYSTEM_JSON>>

增量修改约束:
1. 优先做最小改动，不重写整体架构。
2. 保持模块 ID、模块边界与 loop 调用顺序稳定。
3. 若涉及诊断修复，必须保留结构化日志格式：
[DIAG]|ts=<millis>|lvl=<INFO/WARN/ERROR>|code=<EVENT_CODE>|module=<module_id>|msg=<text>|k=v
4. 修复时至少说明并实现：根因假设对应的最小代码变更点。
5. 输出代码必须继续满足 `c_to_arduino.prompt.md` 中全部“可部署硬约束”“语法与可编译约束”“输出前自检”。
6. 若本次增量涉及 TM1637（或出现 `TM1637Display` / `display.`），必须确保：
	- 存在 `#include <TM1637Display.h>`；
	- 显示对象已合法声明后再使用；
	- 不得出现被拆行破坏的声明或其他无法编译语句。
7. 禁止引入自然语言占位语句（如 `if needed for debugging`、`TODO`、`to be implemented`、`replace with ...`）；若出现必须改写为可编译实现或删除。
8. 引脚不得以占位文本方式保留；模块涉及的引脚常量必须输出为可编辑 PARAM（带正确 module_id），并可被前端参数面板直接识别与修改。
9. MODULE 标记位置必须与主提示词一致：`start` 在函数定义前一行、`end` 在函数结束 `}` 后一行，且都为独立行，禁止行尾内联标记。
