<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
<context>
    <name>AxisConfig</name>
    <message>
        <location filename="../BasicElement/axisconfig.h" line="419"/>
        <source>Undefined soft landing pos: %1</source>
        <translation>未定义的软着陆位置: %1</translation>
    </message>
</context>
<context>
    <name>AxisModuleConfig</name>
    <message>
        <location filename="../AxisModule/axismodule.h" line="134"/>
        <source>Can not cast pos &apos;%1&apos; to type &apos;%2&apos;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../AxisModule/axismodule.h" line="140"/>
        <source>Undefined module pos &apos;%1&apos; in module &apos;%2&apos;</source>
        <translation>未定义的模组位置 &apos;%1&apos; in module &apos;%2&apos;</translation>
    </message>
</context>
<context>
    <name>DummyDI</name>
    <message>
        <location filename="../BasicElement/dummydi.cpp" line="16"/>
        <source>Can not cast config to DummyIOConfig! Source config type: %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MasterMotionManager</name>
    <message>
        <location filename="../mastermotionmanager.cpp" line="145"/>
        <source>Enable move protection!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../mastermotionmanager.cpp" line="149"/>
        <source>Disable move protection!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../mastermotionmanager.cpp" line="126"/>
        <source>Disable move protection may cause dengerous behavior!
Are you sure to do that?</source>
        <translation>禁用运动保护可能导致危险的行为!
您确定要这样做吗?</translation>
    </message>
    <message>
        <location filename="../mastermotionmanager.cpp" line="195"/>
        <source>Are you sure to apply this config?
axisName: %1, softLandingPosName: %2, targetPos: %3</source>
        <translation>您确定要应用次配置嘛?
轴名: %1, 软着陆位置名: %2, 目标位置: %3</translation>
    </message>
    <message>
        <location filename="../mastermotionmanager.cpp" line="249"/>
        <source>Connection to motion manager server %1 is invalid!</source>
        <translation>与 motion manager server %1 的连接无效!</translation>
    </message>
    <message>
        <location filename="../mastermotionmanager.cpp" line="256"/>
        <source>Wait motion manager server response timeout! Server address: %1</source>
        <translation>等待 motion manager server 的响应超时! Server 地址: %1</translation>
    </message>
    <message>
        <location filename="../mastermotionmanager.cpp" line="261"/>
        <source>Motion manager server init failed! Server address: %1</source>
        <translation>Motion manager server 初始化失败! Server 地址: %1</translation>
    </message>
    <message>
        <location filename="../mastermotionmanager.cpp" line="294"/>
        <source>Waiting motion manager paused... Server address: %1</source>
        <translation>等待motion manager暂停... 服务器地址: %1</translation>
    </message>
</context>
<context>
    <name>MotionConfigManager</name>
    <message>
        <location filename="../MotionManager/motionconfigmanager.cpp" line="464"/>
        <source>Unknown axis module, Name: %1, Type: %2</source>
        <translation>位置的轴模组, 名称: %1, 类型: %2</translation>
    </message>
</context>
<context>
    <name>MotionManager</name>
    <message>
        <location filename="../MotionManager/motionmanager.cpp" line="118"/>
        <source>Instruction with uuid %1 was not watched!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../MotionManager/motionmanager.cpp" line="312"/>
        <source>Try to bind axis %1 to %2, but %2 is undefined!</source>
        <translation>试图绑定轴 %1 到 %2, 但 %2 未定义!</translation>
    </message>
    <message>
        <location filename="../MotionManager/motionmanager.cpp" line="320"/>
        <source>Bind axis %1 to %2 failed!</source>
        <translation>绑定轴 %1 到 %2 失败!</translation>
    </message>
    <message>
        <location filename="../MotionManager/motionmanager.cpp" line="366"/>
        <source>Waiting axis %1 paused...</source>
        <translation>等待轴 %1 暂停...</translation>
    </message>
    <message>
        <location filename="../MotionManager/motionmanager.cpp" line="634"/>
        <source>MotionManager is not inited!</source>
        <translation>MotionManager 未初始化!</translation>
    </message>
    <message>
        <location filename="../MotionManager/motionmanager.cpp" line="829"/>
        <source>Unknown axis module type: %1</source>
        <translation>未知的轴模组类型: %1</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="809"/>
        <location filename="../BasicElement/scaxis.cpp" line="840"/>
        <source>Axis %1 has been stopped! CurrentPos: %2</source>
        <translation>轴 %1 已被停止! 当前位置: %2</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="820"/>
        <source>Axis %1 wait soft land down finished timeout!</source>
        <translation>轴 %1 等待软着陆伸出完成超时!</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="851"/>
        <source>Axis %1 wait soft land up finished timeout!</source>
        <translation>轴 %1 等待软着陆缩回完成超时!</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="430"/>
        <source>Axis %1 wait inpos timeout!</source>
        <translation>轴 %1 等待到位超时!</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="397"/>
        <source>Axis %1 may be stopped! Target pos %2! Profiler pos: %3</source>
        <translation>轴 %1 可能已被停止! 目标位置 %2! 规划位置: %3</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="446"/>
        <source>Axis %1 wait arrived pos timeout!
Target pos: %2, Profiler pos: %3, Feedback pos: %4, Inpos precision %5</source>
        <translation>轴 %1 等待到达位置超时!
目标位置: %2, 规划位置: %3, 反馈位置: %4, 到位精度 %5</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="952"/>
        <source>Axis %1 was stopped! Wait greater than pos failed! Target pos: %2, Current pos: %3, Compare precision: %4</source>
        <translation>轴 %1 已停止! 等待大于位置失败! 目标位置: %2, 当前位置: %3, 比较精度: %4</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="964"/>
        <source>Axis %1 wait greater than pos timeout! Target pos:%2, Current pos: %3, Compare precision %4</source>
        <translation>轴 %1 等待大于位置超时! 目标位置: %2, 当前位置: %3, 比较精度: %4</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="988"/>
        <source>Axis %1 was stopped! Wait less than pos failed! Target pos: %2, Current pos: %3, Compare precision: %4</source>
        <translation>轴 %1 已停止! 等待小于位置失败! 目标位置: %2, 当前位置: %3, 比较精度: %4</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="1000"/>
        <source>Axis %1 wait less than pos timeout! Target pos:%2, Current pos: %3, Compare precision %4</source>
        <translation>轴 %1 等待小于位置超时! 目标位置: %2, 当前位置: %3, 比较精度: %4</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="1024"/>
        <source>Axis %1 was stopped! Wait approach pos failed! Target pos: %2, Current pos: %3, Compare precision: %4</source>
        <translation>轴 %1 已停止! 等待接近位置失败! 目标位置: %2, 当前位置: %3, 比较精度: %4</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="1036"/>
        <source>Axis %1 wait approach pos timeout! Target pos:%2, Current pos: %3, Compare precision %4</source>
        <translation>轴 %1 等待接近位置超时! 目标位置: %2, 当前位置: %3, 比较精度: %4</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="1051"/>
        <source>Axis %1 has not been inited!</source>
        <translation>轴 %1 已被初始化!</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="1055"/>
        <source>Axis %1 has not been enabled!</source>
        <translation>轴 %1 还未使能!</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="1059"/>
        <source>Axis %1 has not been homed!</source>
        <translation>轴 %1 未回零!</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="1063"/>
        <source>Axis %1 has Alarm!</source>
        <translation>轴 %1 已报警!</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="1071"/>
        <source>Axis %1 is running!</source>
        <translation>轴 %1 正在运行!</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="1243"/>
        <source>Axis %1 home failed!
%2</source>
        <translation>轴 %1 回零失败!
%2</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="1081"/>
        <source>Axis %1 target pos %2 exceeded limit [%3, %4]!</source>
        <translation>轴 %1 目标位置 %2 超出限制 [%3, %4]!</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="1128"/>
        <source>Axis %1 wait settling timeout! Settling window length: %2, window size: %3</source>
        <translation>轴 %1 等待整定超时! 整定窗口长度: %2, 窗口大小: %3</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="1236"/>
        <location filename="../BasicElement/scaxis.cpp" line="1247"/>
        <location filename="../BasicElement/scaxis.cpp" line="1284"/>
        <source>Axis %1 has been stopped!</source>
        <translation>轴 %1 已被停止!</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="1263"/>
        <location filename="../BasicElement/scaxis.cpp" line="1303"/>
        <source>Axis %1 wait home done timeout!</source>
        <translation>轴 %1 等待回零完成超时!</translation>
    </message>
    <message>
        <location filename="../CompositeElement/sccylinder.cpp" line="134"/>
        <source>Wait cylinder %1 being %2 state timeout!</source>
        <translation>等待气缸 %1 处于 %2 状态超时!</translation>
    </message>
    <message>
        <location filename="../CompositeElement/scvacuum.cpp" line="89"/>
        <source>Set vacuum %1 to %2 state timeout!</source>
        <translation>设置真空 %1 到 %2 状态超时!</translation>
    </message>
    <message>
        <location filename="../CompositeElement/scvacuum.cpp" line="110"/>
        <source>Wait vacuum %1 being %2 state timeout!</source>
        <translation>等待真空 %1 处于 %2 状态超时!</translation>
    </message>
    <message>
        <location filename="../MotionManager/motionelementcontainer.h" line="30"/>
        <source>Item %1 already existed!</source>
        <translation>Item %1 已存在!</translation>
    </message>
    <message>
        <location filename="../MotionManager/motionelementcontainer.h" line="53"/>
        <source>Item %1 has not registered!</source>
        <translation>Item %1 未被注册!</translation>
    </message>
</context>
<context>
    <name>SCAxis</name>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="38"/>
        <source>MoveProtection is not set!</source>
        <translation>运动保护未被设置!</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="46"/>
        <source>Positive limit must greater than negativeLimit! AxisName: %1</source>
        <translation>正限位必须大于负限位! 轴名: %1</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="57"/>
        <source>homeSettlingWindow must greater than 1 if homeSettlingPricision greater than 0! AxisName: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="61"/>
        <source>settlingWindow must greater than 1 if settlingByAppLayer! AxisName: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="85"/>
        <source>%1 home, just clear pos...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="148"/>
        <source>Disable %1 while running! Try to stop axis first!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="308"/>
        <source>StepCount must greater than 0!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="323"/>
        <source>Try to move timeout!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="665"/>
        <source>Can not move on velocity mode if useSoftLimit!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="735"/>
        <source>Bind to master axis failed! %1 did not have master axis!</source>
        <translation>绑定到主轴失败! %1 没有主轴!</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="1142"/>
        <source>%1 start home. Handled by driver.</source>
        <translation>%1 开始回零. 由驱动器进行回零.</translation>
    </message>
    <message>
        <location filename="../BasicElement/scaxis.cpp" line="1256"/>
        <location filename="../BasicElement/scaxis.cpp" line="1296"/>
        <source>Axis %1 home done!</source>
        <translation>轴 %1 回零完成!</translation>
    </message>
</context>
<context>
    <name>SCCylinder</name>
    <message>
        <location filename="../CompositeElement/sccylinder.cpp" line="18"/>
        <source>%1`s falseOutput and trueOutput can not be emtpy at the same time!</source>
        <translation>气缸 %1 的两个输出不能同时为空!</translation>
    </message>
    <message>
        <location filename="../CompositeElement/sccylinder.cpp" line="32"/>
        <source>If false input sensor is not attached, delayAfterMoveToFalse must greater than 50! CylinderName: %1</source>
        <translation type="unfinished">如果禁用了0状态感应器,&quot;运动到0状态后延时&quot; 必须大于60! 气缸名: %1 {50!?}</translation>
    </message>
    <message>
        <location filename="../CompositeElement/sccylinder.cpp" line="46"/>
        <source>If true input sensor is not attached, delayAfterMoveoToTrue must greater than 50! CylinderName: %1</source>
        <translation type="unfinished">如果禁用了1状态感应器,&quot;运动到1状态后延时&quot; 必须大于60! 气缸名: %1 {50!?}</translation>
    </message>
</context>
<context>
    <name>SCDI</name>
    <message>
        <location filename="../BasicElement/scdi.cpp" line="33"/>
        <source>Didital input %1 is not inited!</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SCDO</name>
    <message>
        <location filename="../BasicElement/scdo.cpp" line="19"/>
        <location filename="../BasicElement/scdo.cpp" line="41"/>
        <source>Didital output %1 is not inited!</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Transfer</name>
    <message>
        <location filename="../CompositeElement/transfer.cpp" line="39"/>
        <source>Load tray timeout!</source>
        <translation>上料盘超时!</translation>
    </message>
    <message>
        <location filename="../CompositeElement/transfer.cpp" line="99"/>
        <source>Unload tray timeout!</source>
        <translation>下料盘超时!</translation>
    </message>
    <message>
        <location filename="../CompositeElement/transfer.cpp" line="127"/>
        <source>Return back tray timeout!</source>
        <translation>返回料盘超时!</translation>
    </message>
</context>
<context>
    <name>TwoAxisPlanner</name>
    <message>
        <location filename="../CompositeElement/twoaxisplanner.cpp" line="46"/>
        <source>Axis %1 is not running!</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>XYZModule</name>
    <message>
        <location filename="../AxisModule/xyzmodule.cpp" line="69"/>
        <location filename="../AxisModule/xyzmodule.cpp" line="98"/>
        <location filename="../AxisModule/xyzmodule.cpp" line="119"/>
        <source>Unkown safetyMoveSeq %1(%2, %3)</source>
        <translation>未知的安全运动序列 %1(%2, %3)</translation>
    </message>
    <message>
        <location filename="../AxisModule/xyzmodule.cpp" line="76"/>
        <source>Try to move %1 x to safety pos, but useXSafetyPos is false!</source>
        <translation>试图移动 %1 x 到安全位, 但 &quot;使用x安全位&quot; 被禁用!</translation>
    </message>
    <message>
        <location filename="../AxisModule/xyzmodule.cpp" line="84"/>
        <source>Try to move %1 y to safety pos, but useYSafetyPos is false!</source>
        <translation>试图移动 %1 y 到安全位, 但 &quot;使用y安全位&quot; 被禁用!</translation>
    </message>
    <message>
        <location filename="../AxisModule/xyzmodule.cpp" line="92"/>
        <source>Try to move %1 z to safety pos, but useZSafetyPos is false!</source>
        <translation>试图移动 %1 z 到安全位, 但 &quot;使用z安全位&quot; 被禁用!</translation>
    </message>
    <message>
        <location filename="../AxisModule/xyzmodule.cpp" line="345"/>
        <source>Move to record pos, x,y,z:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
