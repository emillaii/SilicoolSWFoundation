#include "glueinspection.h"
#include "commonutils.h"

glueInspection::glueInspection()
{

}

bool glueInspection::glueCheck(QImage &imageBefore,
                               QImage &imageAfter,
                               double resolution,
                               QImage &resultImage,
                               double &inMinGlueWidthSpec,
                               double &inMaxGlueWidthSpec,
                               double &inAvgGlueWidthSpec,
                               double &outMinGlueWidth,
                               double &outMaxGlueWidth,
                               double &outMaxAvgGlueWidth)
{
    atl::String string1;
    atl::String string2;
    atl::String string3;
    RegionJudgeState regionJudgeState1;
    atl::Conditional< avl::Region > region1;
    avl::Region region2;
    avl::Region region3;
    atl::Conditional< avl::Region > region4;
    atl::Conditional< avl::Region > region5;
    WidthJudgeState widthJudgeState1;

    avl::Image image3;
    avl::Image image4;
    avl::Image image5;
    bool bool1;

    atl::String file1;
    atl::String file2;
    file1 = L"C:\\Users\\emil\\Desktop\\Test\\glueInspectionTest\\2.3\\2.3\\Image\\before_2\\1.jpg";
    file2 = L"C:\\Users\\emil\\Desktop\\Test\\glueInspectionTest\\2.3\\2.3\\Image\\after_2\\1_d.jpg";

    try {
        avl::Image image1;
        imageBefore = imageBefore.convertToFormat(QImage::Format_RGB888);
        avl::Image imageInput(imageBefore.width(), imageBefore.height(), imageBefore.bytesPerLine(), avl::PlainType::Type::UInt8, imageBefore.depth() / 8, imageBefore.bits());
        avl::RealignImagePitch(imageInput, image1);

        avl::Image image2;
        imageAfter = imageAfter.convertToFormat(QImage::Format_RGB888);
        avl::Image imageInput2(imageAfter.width(), imageAfter.height(), imageAfter.bytesPerLine(), avl::PlainType::Type::UInt8, imageAfter.depth() / 8, imageAfter.bits());
        avl::RealignImagePitch(imageInput2, image2);

        //avl::LoadImage( file1, false, image1 );
        //avl::LoadImage( file2, false, image2 );

        avl::SubtractImages( image1, image2, atl::NIL, 5.0f, image3 );
        avl::ThresholdImage( image3, atl::NIL, 80.0f, atl::NIL, 0.0f, image4 );
        avl::SubtractImages( image2, image1, atl::NIL, 5.0f, image1 );
        avl::ThresholdImage( image1, atl::NIL, 80.0f, atl::NIL, 0.0f, image5 );
        avl::AddImages( image4, image5, atl::NIL, 1.0f, image4 );
        QString sub = "sub.jpg";
        avl::SaveImageToJpeg( image4 , sub.toStdString().c_str(), atl::NIL, false );
        RegionJudge( regionJudgeState1, image4, image2, bool1, region1, region2, region3, region4 );

        qInfo("CP1");
        if (region1 != atl::NIL)
        {

            qInfo("CP2");
            region5.AssignNonNil();

            avl::OpenRegion( region1.Get(), avl::KernelShape::Box, 1, atl::NIL, region5.Get() );
        }
        else
        {

            qInfo("CP3");
            region5 = atl::NIL;
        }
        bool outResultOK = true;
        atl::Conditional<float> outMaxWidth = 0; atl::Conditional<float> outMinWidth = 0; atl::Conditional<float> outAveWidth = 0;

        WidthJudge( widthJudgeState1, bool1, region5, resolution, inMinGlueWidthSpec, inMaxGlueWidthSpec, image2, inAvgGlueWidthSpec, outResultOK, image5, outMaxWidth, outMinWidth, outAveWidth );

        if (outMaxWidth != atl::NIL && outMaxWidth.HasValue()) outMaxGlueWidth = outMaxWidth.Get();
        if (outMinWidth != atl::NIL && outMinWidth.HasValue()) outMinGlueWidth = outMinWidth.Get();
        if (outAveWidth != atl::NIL && outAveWidth.HasValue()) outMaxAvgGlueWidth = outAveWidth.Get();
        qDebug("Glue Inspection result: %d outMaxWidth: %f outMinWidth: %f outAvgWidth: %f", outResultOK, outMaxGlueWidth,  outMinGlueWidth, outMaxAvgGlueWidth);
        QString result = "result.jpg";
        avl::SaveImageToJpeg( image5, result.toStdString().c_str(), atl::NIL, false );
        QImage testImage((uchar*)image5.Data(), image5.Width(), image5.Height(), image5.Pitch(), QImage::Format_RGB888);
        resultImage = testImage.copy();
    }catch(const atl::Error& error) {
        qWarning(error.Message());
        qWarning("Glue inspection fail");
        return false;
    }
    return true;
}


void glueInspection::RegionJudge( RegionJudgeState& state, const avl::Image& inSubtractImage, const avl::Image&, bool& outRegionOk, atl::Conditional< avl::Region >& outRegion, avl::Region& outRegion1, avl::Region& outRegion2, atl::Conditional< avl::Region >& outRegion3 )
{
    int GlueLineMinArea = 8000;
    int GlueInnerFrameMinArea = 18000;

    atl::Conditional< bool > bool1;

    avl::ThresholdToRegion_Dynamic( inSubtractImage, atl::NIL, atl::NIL, 5, 5, 5.0f, atl::NIL, 0.0f, state.region1, atl::Dummy<avl::Image>().Get() );
    avl::SplitRegionIntoBlobs( state.region1, avl::RegionConnectivity::EightDirections, 2000, atl::NIL, true, state.regionArray1, atl::Dummy< atl::Array< int > >().Get() );

    state.regionArray2.Resize(state.regionArray1.Size());

    for( int i = 0; i < state.regionArray1.Size(); ++i )
    {
        avl::DilateRegion( state.regionArray1[i], avl::KernelShape::Box, 2, atl::NIL, state.regionArray2[i] );
    }

    avl::RegionUnion_OfArray( state.regionArray2, outRegion1 );
    avl::FillRegionHoles( outRegion1, avl::RegionConnectivity::EightDirections, 0, GlueInnerFrameMinArea, outRegion2 );
    avl::SplitRegionIntoBlobs( outRegion2, avl::RegionConnectivity::EightDirections, GlueLineMinArea, atl::NIL, true, state.regionArray3, atl::Dummy< atl::Array< int > >().Get() );
    avl::GetMaximumRegion_OrNil( state.regionArray3, avl::RegionFeature::Area, outRegion, atl::Dummy< atl::Conditional< float > >().Get(), atl::Dummy< atl::Conditional< int > >().Get() );

    if (outRegion != atl::NIL)
    {
        int integer1;

        bool1.AssignNonNil();

        avl::RegionNumberOfHoles( outRegion.Get(), avl::RegionConnectivity::EightDirections, GlueInnerFrameMinArea, atl::NIL, integer1 );
        bool1.Get() = integer1 == 1;
    }
    else
    {
        bool1 = atl::NIL;
    }

    avl::MergeDefault< bool >( bool1, false, outRegionOk );

    outRegion3 = outRegion;
}

void glueInspection::WidthJudge( WidthJudgeState& state, bool inRegionOk, atl::Conditional< const avl::Region& > inRegion, float inResolution, float inMinWidth, float inMaxWidth, const avl::Image& inAfterImage, float inMaxAveWidth, bool& outResultOK, avl::Image& outResultImage, atl::Conditional< float >& outMaxWidth, atl::Conditional< float >& outMinWidth, atl::Conditional< float >& outAveWidth )
{
   atl::Array<atl::Conditional<atl::String>> g_constData1;
   atl::Array<atl::Conditional<avl::Location>> g_constData2;
   atl::Array<atl::Conditional<atl::String>> g_constData3;
   atl::String g_constData4;
   atl::String g_constData5;
   atl::String g_constData6;
   atl::String g_constData7;
   atl::Array<atl::Conditional<avl::Location>> g_constData8;

   outResultOK = false;
   g_constData1.Reset(1);
   g_constData1[0] = "胶线破损严重";

   g_constData2.Reset(1);
   g_constData2[0] = avl::Location(323, 66);

   g_constData3.Reset(1);
   g_constData3[0] = "胶线宽度问题";

   g_constData4 = "Result:";

   g_constData5 = " Max:";

   g_constData6 = " Min:";

   g_constData7 = " Avg:";

   g_constData8.Reset(1);
   g_constData8[0] = avl::Location(313, 23);


   if (inResolution < 0.0f)
       throw atl::DomainError("Argument \"inResolution\" of function \"WidthJudge\" is out of the range of valid values.");

   if (inRegionOk == false)
   {
       avs::DrawStrings_SingleColor( inAfterImage, g_constData1, g_constData2, atl::NIL, avl::Anchor2D::MiddleCenter, avl::Pixel(255.0f, 0.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 1.0f), 32.0f, 0.0f, true, atl::NIL, state.image1 );

       if (inRegion != atl::NIL)
       {
           state.pathArray1.AssignNonNil();

           avl::RegionContours( inRegion.Get(), avl::RegionContourMode::PixelCenters, avl::RegionConnectivity::EightDirections, state.pathArray2 );
           avl::SelectClosedPaths( state.pathArray2, state.pathArray3 );

           state.pathArray1.Get().Resize(state.pathArray3.Size());

           for( int i = 0; i < state.pathArray3.Size(); ++i )
           {
               state.pathArray1.Get()[i].AssignNonNil();
               state.pathArray1.Get()[i].Get() = state.pathArray3[i];
           }
       }
       else
       {
           state.pathArray1 = atl::NIL;
       }

       avs::DrawPaths_SingleColor( state.image1, state.pathArray1, atl::NIL, avl::Pixel(255.0f, 0.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 1.0f), true, outResultImage );
       //avl::SaveImageToJpeg(outResultImage , imageName.toStdString().c_str(), atl::NIL, false);
       outMaxWidth = atl::NIL;
       outMinWidth = atl::NIL;
       outAveWidth = atl::NIL;
   }
   else if (inRegionOk == true)
   {
       atl::Conditional< bool > bool1;

       if (inRegion != atl::NIL)
       {
           avl::RegionContours( inRegion.Get(), avl::RegionContourMode::PixelCenters, avl::RegionConnectivity::EightDirections, state.pathArray4 );
           avl::SelectClosedPaths( state.pathArray4, state.pathArray5 );

           state.pathArray6.Resize(state.pathArray5.Size());

           for( int i = 0; i < state.pathArray5.Size(); ++i )
           {
               avl::SmoothPath_Gauss( state.pathArray5[i], 0.6f, 3.0f, state.pathArray6[i] );
           }

           avl::GetMinimumPath_OrNil( state.pathArray6, avl::PathFeature::Length, state.path1, atl::Dummy< atl::Conditional< float > >().Get(), atl::Dummy< atl::Conditional< int > >().Get() );
           avl::GetMaximumPath_OrNil( state.pathArray6, avl::PathFeature::Length, state.path2, atl::Dummy< atl::Conditional< float > >().Get(), atl::Dummy< atl::Conditional< int > >().Get() );

           if (state.path1 != atl::NIL)
           {
               state.path3.AssignNonNil();

               avl::ConvertToEquidistantPath( state.path1.Get(), 3.0f, avl::EquidistanceType::OutputPathEquidistance, state.path3.Get() );

               // Function AvsFilter_AccessPath is intended for generated code only. Consider use of Path accessors instead.
               avs::AvsFilter_AccessPath( state.path3.Get(), state.point2DArray1, atl::Dummy<bool>().Get() );
           }
           else
           {
               state.path3 = atl::NIL;
           }

           if (state.path2 != atl::NIL)
           {
               state.path4.AssignNonNil();

               avl::ConvertToEquidistantPath( state.path2.Get(), 3.0f, avl::EquidistanceType::OutputPathEquidistance, state.path4.Get() );

               if (state.path3 != atl::NIL)
               {
                   atl::Conditional< bool > bool2;

                   bool1.AssignNonNil();
                   state.segment2DArray1.AssignNonNil();

                   avl::PathToPathDistanceProfile( state.path3.Get(), state.path4.Get(), avl::PathDistanceMode::PointToSegment, inResolution, state.profile1, state.realArray1, state.segment2DArray2 );
                   avl::MaximumReal_OfArray_OrNil( state.realArray1, outMaxWidth, atl::NIL );
                   avl::MinimumReal_OfArray_OrNil( state.realArray1, outMinWidth, atl::NIL );
                   avl::AverageReals_OfArray_OrNil( state.realArray1, outAveWidth );

                   if (outAveWidth != atl::NIL)
                   {
                       bool2.AssignNonNil();
                       state.string1.AssignNonNil();

                       avl::TestRealLessOrEqual( outAveWidth.Get(), inMaxAveWidth, bool2.Get() );
                       avl::RealToString( outAveWidth.Get(), state.string1.Get() );
                   }
                   else
                   {
                       bool2 = atl::NIL;
                       state.string1 = atl::NIL;
                   }

                   state.boolArray1.Resize(state.realArray1.Size());
                   state.boolArray2.Resize(state.realArray1.Size());

                   for( int i = 0; i < state.realArray1.Size(); ++i )
                   {
                       avl::TestRealGreaterOrEqual( state.realArray1[i], inMinWidth, state.boolArray1[i] );

                       // AvsFilter_MergeAnd is intended for generated code only. In regular programs builtin operator&& should be used.
                       avs::AvsFilter_MergeAnd( state.boolArray1[i], bool2, true, true, true, true, true, true, false, state.boolArray2[i] );
                   }

                   // AvsFilter_And_OfArray is intended for generated code only. In regular programs builtin operator&& should be used.
                   avs::AvsFilter_And_OfArray( state.boolArray2, bool1.Get() );
                   avl::ClassifyByPredicate< avl::Segment2D >( state.segment2DArray2, state.boolArray1, state.segment2DArray3, state.segment2DArray4 );

                   state.segment2DArray1.Get().Resize(state.segment2DArray4.Size());

                   for( int i = 0; i < state.segment2DArray4.Size(); ++i )
                   {
                       state.segment2DArray1.Get()[i].AssignNonNil();
                       state.segment2DArray1.Get()[i].Get() = state.segment2DArray4[i];
                   }

                   if (outMinWidth != atl::NIL)
                   {
                       state.string2.AssignNonNil();

                       avl::RealToString( outMinWidth.Get(), state.string2.Get() );
                   }
                   else
                   {
                       state.string2 = atl::NIL;
                   }
               }
               else
               {
                   bool1 = atl::NIL;
                   state.segment2DArray1 = atl::NIL;
                   outMaxWidth = atl::NIL;
                   outMinWidth = atl::NIL;
                   outAveWidth = atl::NIL;
                   state.string1 = atl::NIL;
                   state.string2 = atl::NIL;
               }
           }
           else
           {
               state.path4 = atl::NIL;
               outMaxWidth = atl::NIL;
               outMinWidth = atl::NIL;
               outAveWidth = atl::NIL;
               state.string1 = atl::NIL;
               bool1 = atl::NIL;
               state.segment2DArray1 = atl::NIL;
               state.string2 = atl::NIL;
           }
       }
       else
       {
           state.path3 = atl::NIL;
           state.path4 = atl::NIL;
           outMaxWidth = atl::NIL;
           outMinWidth = atl::NIL;
           outAveWidth = atl::NIL;
           state.string1 = atl::NIL;
           bool1 = atl::NIL;
           state.segment2DArray1 = atl::NIL;
           state.string2 = atl::NIL;
       }

       avl::MergeDefault< bool >( bool1, false, outResultOK );
       state.pathArray7.Resize(1);
       state.pathArray7[0] = state.path3;
       avs::DrawPaths_SingleColor( inAfterImage, state.pathArray7, atl::NIL, avl::Pixel(0.0f, 255.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 1.0f), true, state.image2 );
       state.pathArray8.Resize(1);
       state.pathArray8[0] = state.path4;
       avs::DrawPaths_SingleColor( state.image2, state.pathArray8, atl::NIL, avl::Pixel(0.0f, 0.0f, 255.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 1.0f), true, state.image3 );
       avs::DrawSegments_SingleColor( state.image3, state.segment2DArray1, atl::NIL, avl::Pixel(255.0f, 0.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 5.0f, false, atl::NIL, 1.0f), avl::MarkerType::None, 5.0f, true, state.image4 );
       state.stringArray1 = atl::NIL;
       state.stringArray2 = g_constData3;
       state.stringArray3 = outResultOK ? state.stringArray1 : state.stringArray2;
       avs::DrawStrings_TwoColors( state.image4, state.stringArray3, g_constData2, atl::NIL, avl::Anchor2D::MiddleCenter, avl::Pixel(0.0f, 200.0f, 0.0f, 0.0f), avl::Pixel(200.0f, 0.0f, 0.0f, 0.0f), atl::ToArray< atl::Conditional< bool > >(outResultOK), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 1.0f), 32.0f, 0.0f, true, atl::NIL, state.image5 );

       if (outMaxWidth != atl::NIL && state.string2 != atl::NIL && state.string1 != atl::NIL)
       {
           if (outMinWidth.Get() < inMinWidth || outMaxWidth.Get() > inMaxWidth || outAveWidth.Get() > inMaxAveWidth) outResultOK = false;
           else outResultOK = true;
           state.string3.AssignNonNil();

           avl::BoolToString( outResultOK, state.string4 );
           avl::RealToString( outMaxWidth.Get(), state.string5 );

           // AvsFilter_ConcatenateStrings is intended for generated code only. In regular programs  String::operator+() or String:Append() member function should be used.
           avs::AvsFilter_ConcatenateStrings( g_constData4, state.string4, g_constData6, state.string2.Get(), g_constData5, state.string5, g_constData7, state.string1.Get(), state.string3.Get() );
       }
       else
       {
           state.string3 = atl::NIL;
       }

       state.stringArray4.Resize(1);
       state.stringArray4[0] = state.string3;
       if (outResultOK)
           avs::DrawStrings_SingleColor( state.image5, state.stringArray4, g_constData8, atl::NIL, avl::Anchor2D::MiddleCenter, avl::Pixel(0.0f, 0.0f, 255.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 1.0f), 22.0f, 0.0f, true, atl::NIL, outResultImage );
       else {
           avs::DrawStrings_SingleColor( state.image5, state.stringArray4, g_constData8, atl::NIL, avl::Anchor2D::MiddleCenter, avl::Pixel(255.0f, 0.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 1.0f), 22.0f, 0.0f, true, atl::NIL, outResultImage );
       }
       //avl::SaveImageToJpeg(outResultImage , imageName.toStdString().c_str(), atl::NIL, false);
   }
   else
   {
       throw atl::RuntimeError("Unsupported fork value encountered in variantable macro function \"WidthJudge\".");
   }
}

void glueInspection::diffenenceImage(QImage image1, QImage image2)
{
    avl::Image output;
    avl::Image in1(image1.width(), image1.height(), image1.bytesPerLine(), avl::PlainType::Type::UInt8, image1.depth() / 8, image1.bits());
    avl::Image in2(image2.width(), image2.height(), image2.bytesPerLine(), avl::PlainType::Type::UInt8, image2.depth() / 8, image2.bits());
    avl::DifferenceImage(in1, in2, atl::NIL, output);
}
