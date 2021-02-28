#ifndef TWOAXISPLANNER_H
#define TWOAXISPLANNER_H

#include <QObject>
#include <BasicElement/scaxis.h>

class SCMOTIONSHARED_EXPORT TwoAxisPlanner : public QObject
{
    Q_OBJECT

public:
    enum CmpOperation
    {
        LessThan,
        GreaterThan
    };

    explicit TwoAxisPlanner(QObject *parent = nullptr);

    void init(SCAxis* axis1, SCAxis* axis2);

    ///
    /// \brief moveTo 规划两轴运动，逻辑为：
    /// 1. 将轴1运动到目标位置axis1TargetPos（若needMoveAxis1为False，表示调用者已开始运动轴1，则此函数不再运动轴1）
    /// 2. 将轴2运动到过渡点axis2TransisionPos
    /// 3. 循环查询轴1当前位置，若满足条件（轴1当前位置 axis1TriggerCondition axis1TriggerPos），
    ///     将轴2运动到axis2TargetPos（若轴2正在运动，在线改变轴2的目标位置，若已停止（表示已到达将轴2运动到过渡点axis2TransisionPos），启动轴2运动到目标位置）
    /// 4. 若轴1已到达目标位置axis1TargetPos附近，还未满足轴2运动的触发条件，也会将轴2运动到目标位置
    /// 5. 等待两轴到达目标位置(由waitDone决定）
    ///
    void moveTo(double axis1TargetPos, double axis2TargetPos, double axis2TransisionPos, double axis1TriggerPos, CmpOperation axis1TriggerCondition, bool needMoveAxis1=true, bool waitDone=true);

private:
    bool meetCondition(double currentPos, double triggerPos, CmpOperation triggerCondition)
    {
        if(triggerCondition == LessThan)
        {
            return currentPos < triggerPos;
        }
        else
        {
            return currentPos > triggerPos;
        }
    }

private:
    SCAxis* axis1;
    SCAxis* axis2;
};

#endif // TWOAXISPLANNER_H
