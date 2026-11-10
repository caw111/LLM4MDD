根据需求生成符合 OMG SysML v2 文本语法的模型和需求测试映射。
仅返回 JSON，包含 system_sysml2 字符串和 req_test_map 对象。
req_test_map.requirements_to_testpoints 每项包含非空 requirement_id、requirement_text、test_point_id、test_method、stimulus、oracle。
每条需求必须有测试点。不能为了通过编译删除需求或把未知行为替换为常量。

使用 private import ScalarValues::*;，标量类型用 Real / Integer / Boolean。
使用 part def 定义模块、part 实例化模块，并声明唯一的 part system : 系统定义名;
输入输出使用 in attribute / out attribute。模块间用 bind child.input = source; 连接。
用属性的值表达式完整定义计算逻辑，支持算术、比较、布尔和 if 条件表达式。
需要跨周期记忆时用 attribute count : Integer := 0; 初始化状态，并在单个 perform action tick 内用 assign count := ...; 更新。
多条动作必须显式 first start; then assign ...; then assign ...; 排序，条件动作使用 if 条件 { assign ...; } else { assign ...; }。
执行约定：初始化一次，每个采样周期执行 tick，然后计算输出。不能把状态行为改成无记忆的公式。
不要输出 C 代码、value type、param、箭头伪连接或未实现行为的注释占位符。
编译器不支持的构造将产生诊断；修复时必须保持原需求含义。硬件采样/驱动应与模型的输入/输出对应。

合法示例：
package Example {
    private import ScalarValues::*;
    part def Processor {
        in attribute sample : Real;
        out attribute level : Real = sample * sample;
    }
    part def Monitor {
        in attribute mic : Real;
        in attribute threshold : Real;
        part processing : Processor;
        bind processing.sample = mic;
        out attribute level : Real = processing.level;
        out attribute alarm : Boolean = level > threshold;
    }
    part system : Monitor;
}

建模边界与修复规则（必须遵守）：
- 系统所有持久状态更新集中在唯一一个 perform action tick 中；不要在多个模块分别声明 perform action。
- first start; 必须位于该动作体内，后续每个顺序动作由 then 连接；不要在 part def 的属性之间写 first start。
- assign 只更新用 := 初始化的持久属性；输出用 out attribute 的值表达式引用更新后的状态。纯条件逻辑优先使用属性值表达式，不要直接照搬 C 语法。
- 不要用 state、action、part 等语言保留字作未转义属性名，可用 modeCode、ledOn 等描述性名称。
- 当前数值模型不支持 String、打印、toString、硬件库调用或数组。设备采样作为 in attribute 输入，执行器指令作为 out attribute 输出；串口或显示文本在 Arduino 适配层根据明确的输出状态码映射，需求测试映射必须保留原文本和时序要求。
- 时钟由采样输入表示（如 elapsedMilliseconds），把到期判断和状态转移放在模型中。不可把控制规则全部移到 Arduino 适配层，或用固定输出假装实现需求。

已通过参考校验器和当前编译器的状态动作示例：
package Stateful {
    private import ScalarValues::*;
    part def Counter {
        in attribute reset : Boolean;
        attribute count : Integer := 0;
        perform action tick {
            if reset { assign count := 0; }
            else { assign count := count + 1; }
        }
        out attribute value : Integer = count;
    }
    part system : Counter;
}


已通过参考校验器和当前编译器的状态动作示例：
package Sequential {
    private import ScalarValues::*;
    part def Controller {
        attribute a : Integer := 0;
        attribute b : Integer := 0;
        perform action tick {
            first start;
            then assign a := a + 1;
            then assign b := a + 2;
        }
        out attribute value : Integer = b;
    }
    part system : Controller;
}


组合逻辑与输入绑定规则：
没有跨周期记忆的需求必须优先使用纯表达式，不要为它构造状态动作。条件表达式语法是 if 条件 ? 真值 else 假值（不是 C 三目表达式，也不是 if/then/else 语句）。根系统定义声明所有外部采样输入；子部件的输入通过 bind 连接这些根输入，不能把一个没有驱动的子部件输入直接当作传感器来源。package 层只放类型定义和唯一的 system 实例，其他部件实例放在根系统定义内部。

合法的组合条件示例：
package GatedSignal {
    private import ScalarValues::*;
    part def Controller {
        in attribute enabled : Boolean;
        in attribute requested : Real;
        out attribute selected : Real = if enabled ? requested else 0.0;
    }
    part system : Controller;
}

输入需求：
<<USER_REQUIREMENTS>>

交付前逐项核对原需求中的设备、输入输出、单位、阈值、时间间隔、状态转换与初始状态。
需求测试映射只提供追踪关系，不代表独立验证已通过；不得声称功能测试通过。
若附带独立接口契约，必须逐字保留接口名称、类型、单位、采样周期和设备映射。
例如每5秒输出环境报告：温湿度和实际经过时间是模型输入；模型累计时间，输出测量值及单步 reportDue 信号。
Arduino层只负责传感器采样、控制步调度、按 reportDue 格式化串口文本；5000ms判断必须在模型中。
未指定硬件参数应显式标记待确定；不能为了通过校验删掉设备或用固定传感器值占位。

顺序规则适用于每个嵌套动作块：外层 then if 并不会给分支内多条 assign 排序。
下面的分支内顺序示例已经通过参考校验器和当前编译器：
package ConditionalOrder {
    private import ScalarValues::*;
    part def Controller {
        in attribute reset : Boolean;
        attribute count : Integer := 0;
        attribute doubled : Integer := 0;
        perform action tick {
            if reset {
                first start;
                then assign count := 0;
                then assign doubled := 0;
            } else {
                first start;
                then assign count := count + 1;
                then assign doubled := count * 2;
            }
        }
        out attribute value : Integer = doubled;
    }
    part system : Controller;
}
