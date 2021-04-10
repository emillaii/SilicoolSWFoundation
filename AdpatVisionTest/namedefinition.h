#ifndef NAMEDEFINITION_H
#define NAMEDEFINITION_H

#include <QString>

namespace AxisName
{
    // TrayLoader
    const QString TLX = "TLX";
    const QString TLY = "TLY";

    // PickArm
    const QString PAX = "PAX";
    const QString PAY = "PAY";
    const QString PAZ = "PAZ";
}    // namespace AxisName

namespace DiName
{
    // TrayLoader
    const QString TLTraySNR = "TLTraySNR";
}    // namespace DiName

namespace DoName
{
    const QString DemoDO = "DemoDO";
}    // namespace DoName

namespace CylName
{
    // TrayLoader
    const QString TLTrayClamp = "TLTrayClamp";
}    // namespace CylName

namespace VacuumName
{
    // PickArm
    const QString PAVacuum = "PAVacuum";
}    // namespace VacuumName

// single axis module
namespace SAxisModuleName
{
    // PickArm
    const QString PAZ = "PAZ";
}    // namespace SAxisModuleName

namespace XYModuleName
{
    // TrayLoader
    const QString TLXY = "TLXY";
}    // namespace XYModuleName

namespace XYZModuleName
{
    // PickArm
    const QString PAXYZ = "PAXYZ";
}    // namespace XYZModuleName

namespace SAxisPosName
{
    // PAZ
    const QString StandbyPos = "StandbyPos";
    const QString PickPos = "PickPos";
}    // namespace SAxisPosName

namespace XYPosName
{
    // TrayLoader
    const QString StandbyPos = "StandbyPos";
    const QString PrLeftTop = "PrLeftTop";
    const QString ChangeTrayPos = "ChangeTrayPos";
}    // namespace XYPosName

namespace XYZPosName
{
    // PAXYZ
    const QString StandbyPos = "StandbyPos";
    const QString PlacePos = "PlacePos";
}    // namespace XYZPosName

namespace CameraName
{
    const QString PickCamera = "PickCamera";
}    // namespace CameraName

namespace WorkerName
{
    const QString TrayLoader("TrayLoader");
    const QString PickArm("PickArm");
}    // namespace WorkerName

namespace VLName
{
    const QString DutDownlookLocation = "DutDownlookLocation";
}    // namespace VLName

namespace CaliName
{
    const QString DutDownlookCalibration = "DutDownlookCalibration";
}    // namespace CaliName

#endif    // NAMEDEFINITION_H
