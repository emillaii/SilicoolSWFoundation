#ifndef NAMEDEFINITION_H
#define NAMEDEFINITION_H

#include <QString>

namespace AxisName
{
    //弹夹
    const QString Elevator_X = "Elevator_X";//弹夹X
    const QString Elevator_Z = "Elevator_Z";//弹夹Z

    //剥单
    const QString Detaper_Z = "DTZ";//撕膜Z
    const QString ContactRollerL = "ContactRollerL";//detape传送A
    const QString ContactRollerR = "ContactRollerR";//detape传送B
    //const QString DTC = "DTC";//收胶纸 - 计划取消

    //PickArm
    const QString PickArm_X = "PickArm_X";
    const QString PickArm_Y = "PickArm_Y";
    const QString PickArmZ_L = "PickArmZ_L";
    const QString PickArmZ_R = "PickArmZ_R";
    const QString PickArmθ_L = "PickArmθ_L";//左侧吸嘴旋转
    const QString PickArmθ_R = "PickArmθ_R";//右侧吸嘴旋转
    const QString PickArmC_Z = "PickArmC_Z";//切刀

    //出料部分
    const QString TrackX = "TrackX";//夹子和轨道整体升降
    const QString TRX = "TrackZ";//夹子前后轴

    //    // TrayLoader
    //    const QString TLX = "TLX";
    //    const QString TLY = "TLY";

    //    // PickArm
    //    const QString PAX = "PAX";
    //    const QString PAY = "PAY";
    //    const QString PAZ = "PAZ";
}   // namespace AxisName

namespace DiName
{
    // 进料部分
    const QString MagKickerIn = "MagKickerIn";
    const QString MagKickerOut = "MagKickerOut";
    const QString MagFullSNR = "MagFullSNR";
    const QString CobCarrierSNR = "CobCarrierSNR";
    const QString DtStopperIn = "DtStopperIn";

    const QString DtStopperOut = "DtStopperOut";
    const QString DtGripperIn = "DtGripperIn";
    const QString DtGripperOut = "DtGripperOut";
    const QString DtKickerIn = "DtKickerIn";
    const QString DtKickerOut = "DtKickerOut";

    const QString VacPlatformIn ="VacPlatformIn";
    const QString VacPlatformOut = "VacPlatformOut";

    const QString PaVacSNR_L = "PaVacSNR_L";
    const QString PaVacSNR_R = "PaVacSNR_R";
    const QString CarrierExistSNR = "CarrierExistSNR";

    const QString TraySNR = "TraySNR";
    const QString PressureSNR = "PressureSNR";
    const QString OpenAlarm = "OpenAlarm";
    const QString IsExistMag = "IsExistMag";

}    // namespace DiName

namespace DoName
{
    //Feeding
    const QString MagGripper = "MagGripper";
    const QString MagKicker = "MagKicker";
    const QString DtStopper = "DtStopper";
    const QString DtGripper = "DtGripper";  //夹膜气缸
    const QString DtKicker = "DtKicker";    //空载片回弹夹
    const QString CollectSNR = "CollectSNR";
    const QString TrayClamp = "TrayClamp"; //压载板
    const QString PreventOverturnSNR = "PreventOverturnSNR";

    //Pick Arm
    const QString PickVac_L = "PickVac_L";
    const QString PickVac_R = "PickVac_R";
    const QString SensorVac = "SensorVac";//预留

    //Global
    const QString LightR = "LightR";
    const QString LightY = "LightY";
    const QString LightG = "LightG";
    const QString Beep = "Beep";

    const QString TrackStop_Z = "TrackStop_Z";
    const QString TrayGripper = "TrayGripper";
}   // namespace DoName

namespace VacuumName
{
    //Pick Arm
    const QString PickVac_L = "PickVac_L";
    const QString PickVac_R = "PickVac_R";
}    // namespace VacuumName

namespace CylName
{
    //Feeding
    const QString MagGripper = "MagGripper";
    const QString MagKicker = "MagKicker";
    const QString DtStopper = "DtStopper";
    const QString DtGripper = "DtGripper";  //夹膜气缸
    const QString DtKicker = "DtKicker";    //空载片回弹夹
    const QString CollectSNR = "CollectSNR";
    const QString TrayClamp = "TrayClamp";
    const QString PreventOverTurnSNR = "PreventOverturnSNR";


    //Discharge
    const QString TrayGripper = "TrayGripper";
    const QString TrackStop_Z = "TrackStop_Z";//出料轨道上下电机刹车

    // TrayLoader
    //const QString TLTrayClamp = "TLTrayClamp";
}   // namespace CylName

// single axis module
namespace SAxisModuleName
{
    //Elevator
    const QString ElevatorX = "ElevatorX";
	const QString ElevatorZ = "ElevatorZ";

    const QString DetaperZ = "DetaperZ";//剥料Z
    //    const QString DetaperX1 = "DetaperX1";
    //	const QString DetaperX2 = "DetaperX2";
    const QString ContactRollerL = "ContactRollerL";
    const QString ContactRollerR = "ContactRollerR";

    // PickArm
    const QString PAZL = "PAZL";
    const QString PAZR = "PAZR";
    const QString PAθL = "PAθL";
    const QString PAθR = "PAθR";
    const QString PACZ = "PACZ";//切料CZ

    const QString TrackX = "TrackX";
    const QString TrackZ = "TrackZ";

}    // namespace SAxisModuleName

namespace XYModuleName
{
    //Pick Arm
    const QString PAXY = "PAXY";

}    // namespace XYModuleName

namespace XYZModuleName
{
    // PickArm
    const QString CutXYZ = "CutXYZ";
}    // namespace XYZModuleName

namespace SAxisPosName
{
    //Public
    const QString StandbyPos = "StandbyPos";     //初始位

    //Elevator
    const QString ChangeBoardPos = "ChangeBoardPos";
    const QString SecondMagStandbyPos = "SecondMagStandbyPos";
    const QString DetaperZ_S = "DetaperZ_S";                //待夹胶纸的安全位
    const QString DetaperZ_ClampBlueFilmPos = "DetaperZ_ClampBlueFilmPos";//夹胶纸位
    const QString OutMagPos = "OutMagPos";//出弹夹位
    const QString FirstCarierBoardPos = "FirstCarierBoardPos";

    //Feeding
    const QString CutCarrierBoardPosZ = "CutCarrierBoardPosZ";

    // PAZ PAXY
    const QString PickPos = "PickPos";                //取料位
    const QString PlacePos = "PlacePos";                    //放料位
    const QString TakePicturePosForPick = "TakePicturePosForPick";
    const QString TakePicturePosForPlace = "TakePicturePosForPlace";

    //Discharge
    const QString GetTrayPos = "GetTrayPos";
    const QString GetTraySafePos = "GetTraySafePos";
    const QString PlaceTrayPos = "PlaceTrayPos";
    const QString PlaceTraySafePos = "PlaceTraySafePos";
    const QString OutTrayPos = "OutTrayPos";
    const QString PlaceMaterialPos = "PlaceMaterialPos";

    //calibrate
    const QString LPAPickCalibGlassPosUp = "LPAPickCalibGlassPosUp";
    const QString LPAPickCalibGlassPosDown = "LPAPickCalibGlassPosDown";
    const QString RPAPickCalibGlassPosUp = "RPAPickCalibGlassPosUp";
    const QString RPAPickCalibGlassPosDown = "RPAPickCalibGlassPosDown";

    const QString LPaPlaceMaterialPosUp = "LPaPlaceMaterialPosUp";
    const QString LPaPlaceMaterialPosDown = "LPaPlaceMaterialPosDown";
    const QString RPaPlaceMaterialPosUp = "RPaPlaceMaterialPosUp";
    const QString RPaPlaceMaterialPosDown = "RPaPlaceMaterialPosDown";

    const QString LPaPickMaterialPosUp = "LPaPickMaterialPosUp";
    const QString LPaPickMaterialPosDown = "LPaPickMaterialPosDown";
    const QString RPaPickMaterialPosUp = "RPaPickMaterialPosUp";
    const QString RPaPickMaterialPosDown = "RPaPickMaterialPosDown";
}   // namespace SAxisPosName

namespace XYPosName
{
    // Pick Arm
    const QString TakePictureForMaterialA = "TakePictureForMaterialA";
	const QString TakePictureForMaterialB = "TakePictureForMaterialB";
    const QString TakePictureForPickPos = "TakePictureForPickPos";

    const QString TakePictureForTrayPos = "TakePictureForTrayPos";
    const QString PlaceMaterialPos = "PlaceMaterialPos";

    //Calibration Pos
    const QString CalibrateCameraScalePos = "CalibrateCameraScalePos";
//    const QString CalibrateCameraToNozzlePosL = "CalibrateCameraToNozzlePosL";
//    const QString CalibrateCameraToNozzlePosR = "CalibrateCameraToNozzlePosR";
    const QString CalibrateCameraToNozzlePrPos = "CalibrateCameraToNozzlePrPos";
    const QString PA_GetCalibGlassPosL = "PA_GetCalibGlassPosL";
    const QString PA_GetCalibGlassPosR = "PA_GetCalibGlassPosR";

    const QString StandbyPos = "StandbyPos";

    const QString CutCarrierBoardPosXY = "CutCarrierBoardPosXY";

}   // namespace XYPosName

namespace XYZPosName
{
    // PAXYZ
    const QString StandbyPos = "StandbyPos";

}    // namespace XYZPosName

namespace CameraName
{
    const QString PickCamera = "PickCamera";
}    // namespace CameraName

namespace WorkerName
{
    const QString Feeding("Feeding");
    const QString Discharge("Discharge");
    const QString PickArm("PickArm");

    const QString TrayLoader("TrayLoader");
}    // namespace WorkerName

namespace VLName//vision Location Name
{
    const QString VisionLocationForScale = "VisionLocationForScale";
    const QString GetMaterialLocation = "GetMaterialLocation";
    const QString PlaceMaterialLocation = "PlaceMaterialLocation";
}    // namespace VLName

namespace CaliName
{
    const QString DutDownlookCalibrationL = "DutDownlookCalibrationL";
    const QString DutDownlookCalibrationR = "DutDownlookCalibrationR";
    const QString CalibrationForScale = "CalibrationForScale";
}    // namespace CaliName

#endif    // NAMEDEFINITION_H
