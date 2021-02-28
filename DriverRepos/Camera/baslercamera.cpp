#include "baslercamera.h"

BaslerCamera::BaslerCamera(QString cameraName, CameraConfig *cameraConfig, QObject *parent) : SCCamera(cameraName, cameraConfig, parent)
{
    PylonInitialize();
}

void BaslerCamera::openImpl()
{
    bool foundCamera = false;
    CTlFactory &tlFactory = CTlFactory::GetInstance();
    DeviceInfoList_t devices;
    if (tlFactory.EnumerateDevices(devices) == 0)
    {
        throw SilicolAbort("No basler camera connected!", EX_LOCATION);
    }
    try
    {
        for (size_t i = 0; i < devices.size(); i++)
        {
            bool isFound = false;
            if (config()->cameraType() == CameraConfig::USB)
            {
                QString serialNumber = devices[i].GetSerialNumber().c_str();
                isFound = (serialNumber == config()->cameraAddress());
            }
            else
            {
                QString userDefinedCameraName = devices[i].GetUserDefinedName().c_str();
                isFound = (userDefinedCameraName == cameraName());
            }
            if (isFound)
            {
                foundCamera = true;
                camera.Attach(tlFactory.CreateDevice(devices[i]));
                camera.Open();
                camera.RegisterConfiguration(new CSoftwareTriggerConfiguration, RegistrationMode_ReplaceAll, Cleanup_Delete);

                INodeMap &cameraNodeMap = camera.GetNodeMap();
                if (config()->cameraType() == CameraConfig::GigE)
                {
                    CEnumerationPtr ptrTriggerSel = cameraNodeMap.GetNode("TriggerSelector");
                    ptrTriggerSel->FromString("FrameStart");
                }
                CEnumerationPtr ptrTrigger = cameraNodeMap.GetNode("TriggerMode");
                ptrTrigger->SetIntValue(1);
                CEnumerationPtr ptrTriggerSource = cameraNodeMap.GetNode("TriggerSource");
                ptrTriggerSource->FromString("Software");
                const char *exposureNodeName = config()->cameraType() == CameraConfig::GigE ? "ExposureTimeAbs" : "ExposureTime";
                CFloatPtr ptrExposureTime = cameraNodeMap.GetNode(exposureNodeName);
                ptrExposureTime->SetValue(config()->exposureTime());
                if (config()->cameraType() == CameraConfig::GigE)
                {
                    camera.StartGrabbing(GrabStrategy_UpcomingImage, GrabLoop_ProvidedByUser);
                }
                else
                {
                    camera.StartGrabbing(GrabStrategy_LatestImageOnly, GrabLoop_ProvidedByUser);
                }
                break;
            }
        }
    }
    catch (const GenericException &e)
    {
        throw SilicolAbort(e.what(), EX_LOCATION);
    }

    if (!foundCamera)
    {
        throw SilicolAbort(tr("Did not find camera: %1").arg(cameraName()), EX_LOCATION);
    }
}

void BaslerCamera::closeImpl()
{
    if (camera.IsGrabbing())
    {
        camera.StopGrabbing();
    }
    if (camera.IsOpen())
    {
        camera.Close();
    }
}

QImage BaslerCamera::getImageImpl()
{
    CGrabResultPtr grabResult;
    try
    {
        camera.WaitForFrameTriggerReady(200);
        camera.ExecuteSoftwareTrigger();
        camera.RetrieveResult(1000, grabResult);
    }
    catch (const GenericException &e)
    {
        throw GeneralError("Vision", e.what());
    }

    if (!grabResult->GrabSucceeded())
    {
        throw GeneralError("Vision", QString("Grab image failed! %1").arg(grabResult->GetErrorDescription().c_str()));
    }

    QImage image(grabResult->GetWidth(), grabResult->GetHeight(), QImage::Format_Indexed8);

    if (grabResult->GetPixelType() == EPixelType::PixelType_Mono8)
    {
        if (image.bytesPerLine() != image.width())
        {
            byte *bufferStart = reinterpret_cast<byte *>(grabResult->GetBuffer());
            for (int y = 0; y < image.height(); ++y)
            {
                memcpy(image.scanLine(y), bufferStart + y * image.width(), image.width());
            }
        }
        else
        {
            memcpy(image.bits(), grabResult->GetBuffer(), image.width() * image.height());
        }
    }

    return image;
}
