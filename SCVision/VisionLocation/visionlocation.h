#ifndef VISION_LOCATION_H
#define VISION_LOCATION_H

#include "../sccamera.h"
#include "../scvision.h"
#include "Calibration/calibrationconfig.h"
#include "errorHandling/silicolerrorhandler.h"
#include "proffset.h"
#include "scvision_global.h"
#include "visionlocationconfig.h"
#include <QtConcurrent>

class SCVISIONSHARED_EXPORT VisionLocation : public QObject
{
    Q_OBJECT
public:
    VisionLocation(SCVision *vision, SCCamera *m_camera, VisionLocationConfig *config);

    ///
    /// \brief disable
    /// 禁用VisionLocation后，若PR执行失败，将PrOffset置为默认值，并返回True
    ///
    void disable()
    {
        isDisabled = true;
    }
    void enable()
    {
        isDisabled = false;
    }

    VisionLocationConfig *config() const
    {
        return m_config;
    }

    QString locationName() const
    {
        return m_config->locationName();
    }

    void setCalibrationResult(CalibrationResult *calibrationResult);

    void startContinuallyPerformPr();
    void stopContinuallyCalculation();

    bool performPR(PrOffset &offset);
    bool performPR(PrOffset &offset, int roiRowIndex, int roiColIndex);
    bool performPR(PRResultStruct &prResult);
    bool performPR(QImage &image, PRResultStruct &prResult);
    bool performPR(QImage &image, PrOffset &offset);
    bool performPR(QImage &image, PrOffset &offset, PRResultStruct &prResult);

    PrOffset getAvePrOffset(int times);
    PrOffset getAvePrOffset(int times, QList<PrOffset> &processData);

    void openLight();
    void closeLight();

    QImage getImage();

    QPointF getMechDistanceFromImageCenter(QPointF pixelPoint) const;
    bool result2Offset(const PRResultStruct &prResult, PrOffset &prOffset);

    void getXyPixelOffset(QPointF &xyPixel, QPointF &xyOffset);

    SCCamera *camera() const;

private slots:
    void onVisionLocationConfigLightBrightnessChanged(int lightBrightnessChanged);
    void onVisionLocationConfigSecondLightBrightnessChanged(int lightBrightnessChanged);

private:
    void reverseTheta(PRResultStruct &prResult);
    void showPrImage(QImage &image, const QString &imgProcessingResult = "");
    void savePrFailedImage(QImage &image);
    void drawSaveShowResultImage(QImage image, PRResultImageInfo *prResultImageInfo, QString imgProcessingResult = "");

private:
    bool isContinuallyCalculation = false;
    SCVision *vision;
    SCCamera *m_camera;
    VisionLocationConfig *m_config;
    CalibrationResult *calibration;

    bool isDisabled = false;
};

#endif    // VISION_LOCATION_H
