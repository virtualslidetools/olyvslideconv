/*******************************************************************************
Copyright (c) 2005-2016, Paul F. Richards

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*******************************************************************************/
#ifndef INISCANSUPPORT_H
#define INISCANSUPPORT_H

#include <string>
#include <vector>
#include <iostream>
#include <cstdint>
#include "imagesupport.h"
#ifndef USE_MAGICK
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#else
namespace Magick
{
#include <MagickCore/MagickCore.h>
#include <MagickWand/MagickWand.h>
}
const char* getMagickCoreCoderPath();
const char* getMagickCoreFilterPath();
const char* getMagickCoreLibraryPath();
#endif 
#include "safebmp.h"

#if !defined(BLENDBKGD_H)
#include "blendbkgd.h"
#endif

inline char separator()
{
#if defined _WIN32 || defined __CYGWIN__ || defined _WIN64
  return '\\';
#else
  return '/';
#endif
}

struct Pt
{
  int64_t x, y;
};

class JpgXY
{
public:
  int64_t mxPixel, myPixel;
};

class JpgXYSortForX
{
public:
  bool operator() (const JpgXY& jpgXY1, const JpgXY& jpgXY2) 
  {
    if (jpgXY1.mxPixel==jpgXY2.mxPixel)
    {
      return jpgXY1.myPixel<jpgXY2.myPixel;
    }
    else
    {
      return jpgXY1.mxPixel<jpgXY2.mxPixel;
    }
  }
};


class JpgFileXY
{
public:
  int64_t mx, my;
  int64_t mxPixel, myPixel;
  int64_t mxSortedIndex;
  std::string mFileName[2][4];
  std::string mBaseFileName;
  std::vector<Pt> border;
  bool operator < (const JpgFileXY& jpgFile) const
  {
    if (jpgFile.myPixel==myPixel)
    {
      return mxPixel<jpgFile.mxPixel;
    }
    else
    {
      return myPixel<jpgFile.myPixel;
    }
  }
  bool mzStack[2][4];
};

class JpgFileXYSortForX
{
public:
  bool operator() (const JpgFileXY& jpgFile1, const JpgFileXY& jpgFile2) 
  {
    if (jpgFile1.mx==jpgFile2.mx)
    {
      return jpgFile1.my<jpgFile2.my;
    }
    else
    {
      return jpgFile1.mx<jpgFile2.mx;
    }
  }
};

class JpgFileXYSortForY
{
public:
  bool operator() (const JpgFileXY& jpgFile1, const JpgFileXY& jpgFile2) 
  {
    if (jpgFile2.my==jpgFile1.my)
    {
      return jpgFile1.mx<jpgFile2.mx;
    }
    else
    {
      return jpgFile1.my<jpgFile2.my;
    }
  }
};


class IniConf 
{
public:
  const char* mname;
  bool mFound;
  int64_t mxMin, mxMax, myMin, myMax;
  int64_t mxDiffMin, myDiffMin;
  int64_t mxStepSize, myStepSize;
  int64_t mPixelWidth, mPixelHeight;
  double mxAdj, myAdj;
  int64_t mTotalTiles;
  int64_t mxAxis, myAxis;
  bool mxKnowStepSize, myKnowStepSize, mKnowStepSizes;
  int64_t mTotalWidth, mTotalHeight;
  int64_t mDetailedWidth, mDetailedHeight;
  int64_t mOrgDetailedWidth, mOrgDetailedHeight;
  bool mIsPreviewSlide;
  int mQuality;
  std::vector<JpgFileXY> mxyArr;
  std::vector<JpgXY> mxSortedArr;
  bool mzStackExists[2][4];
public:
  IniConf();
};

class JpgFileXYSortForXAdj
{
public:
  bool operator() (const IniConf *iniConf1, const IniConf *iniConf2)
  {
    return (iniConf1->mxAdj < iniConf2->mxAdj);
  }
};

bool drawXHighlight(BYTE *pBmp, int samplesPerPixel, int64_t y1, int64_t x1, int64_t x2, int64_t width, int64_t height, int thickness, int position);
bool drawYHighlight(BYTE *pBmp, int samplesPerPixel, int64_t x1, int64_t y1, int64_t y2, int64_t width, int64_t height, int thickness, int position);

class CompositeSlide {
protected:
  bool mValidObject;
  uint8_t mbkgColor;
  std::vector<IniConf*> mConf; 
  static const char* miniNames[4][4];
  double mxStart, myStart;
  int mlevel;
  int64_t mbaseWidth, mbaseHeight;
  int64_t mxMax, mxMin, myMax, myMin;
  bool mGrayScale;
  int mmagnification;
  int mTotalZLevels, mTotalTopZLevels, mTotalBottomZLevels;
  int64_t mBestXOffset, mBestYOffset;
  bool mOptBorder;
  int mOrientation;
  int mOptDebug;
public:
  CompositeSlide(); 
  ~CompositeSlide() { close(); }
  bool isValidObject() { return mValidObject; }
  void initialize();
  void close();
  bool open(const std::string& inputDir, int options, int orientation, int debugLevel = 0, int64_t bestXOffset = -1, int64_t bestYOffset = -1, safeBmp **pImageL2 = NULL); 
  bool read(int64_t x, int64_t y, int64_t width, int64_t height, bool setGrayScale = false);
  bool read(BYTE *pBmp, int level, int direction, int zLevel, int64_t x, int64_t y, int64_t width, int64_t height, bool setGrayScale, int64_t *readWidth, int64_t *readHeight);
  bool allocate(safeBmp* pBmp, int level, int64_t x, int64_t y, int64_t width, int64_t height, bool setGrayScale = false);

  #ifndef USE_MAGICK
  bool findXYOffset(int lowerLevel, int higherLevel, int64_t *bestXOffset0, int64_t *bestYOffset0, int64_t *bestXOffset1, int64_t *bestYOffset1, int optUseCustomOffset, int debugLevel, std::fstream& logFile);
  #endif

  #ifndef USE_MAGICK
  bool loadFullImage(int level, safeBmp **ptpFullImage, cv::Mat **ptpMatImage, int orientation, double xZoomOut, double yZoomOut, bool useZoom, int optDebug, std::fstream& logFile);
  #else
  bool loadFullImage(int level, safeBmp **ptpImageL2, void **ptpMatImage, int orientation, double xZoomOut, double yZoomOut, bool useZoom, int optDebug, std::fstream& logFile);
  #endif
  
  bool checkLevel(int level);
  bool checkZLevel(int level, int direction, int zLevel); 
  int getTotalZLevels() { return mValidObject == true ? mTotalZLevels : 0; }
  int getTotalBottomZLevels() { return mValidObject == true ? mTotalBottomZLevels : 0; }
  int getTotalTopZLevels() { return mValidObject == true ? mTotalTopZLevels : 0; }
  static bool testHeader(BYTE*, int64_t);
  bool isPreviewSlide(size_t level);
  int getMagnification() { return (mValidObject == true ? mmagnification : 0); }
  int getQuality(size_t level) { if (mValidObject == true && level < mConf.size() && mConf[level]->mFound) { return mConf[level]->mQuality; } else { return 0; } }
  long long getBaseWidth() { return (mValidObject == true ? mbaseWidth : 0); }
  long long getBaseHeight() { return (mValidObject == true ? mbaseHeight : 0); }
  int64_t getPixelWidth(size_t level) { return (mValidObject == true && level < mConf.size() && mConf[level]->mFound ? mConf[level]->mPixelWidth : 0); }
  int64_t getPixelHeight(size_t level) { return (mValidObject == true && level < mConf.size() && mConf[level]->mFound ? mConf[level]->mPixelHeight : 0); }
  int64_t getActualWidth(size_t level) { return (mValidObject == true && level < mConf.size() && mConf[level]->mFound ? mConf[level]->mTotalWidth : 0); }
  int64_t getActualHeight(size_t level) { return (mValidObject == true && level < mConf.size() && mConf[level]->mFound ? mConf[level]->mTotalHeight : 0); }
  double getXAdj(size_t level) { if (mValidObject && level < mConf.size() && mConf[level]->mFound) { return mConf[level]->mxAdj; } else { return 1; }}
  double getYAdj(size_t level) { if (mValidObject && level < mConf.size() && mConf[level]->mFound) { return mConf[level]->myAdj; } else { return 1; }}
  int64_t getTotalTiles(size_t level) { if (mValidObject && level < mConf.size() && mConf[level]->mFound) { return mConf[level]->mTotalTiles; } else { return 0; }}
  bool drawBorder(BYTE *pBuff, int samplesPerPixel, int64_t x, int64_t y, int64_t width, int64_t height, int level);
  void blendLevelsRegionScan(BlendSection** yFreeMap, int64_t ySize, int orientation);
  std::vector<JpgFileXY>* getTileXYArray(size_t level) { if (mValidObject && level < mConf.size() && mConf[level]->mFound) { return &mConf[level]->mxyArr; } else { return NULL; }}
  bool setOrientation(int orientation, std::fstream& logFile);
};

#ifndef USE_MAGICK
class CVMatchCompare 
{
public:
  bool operator() (const cv::DMatch& match1, const cv::DMatch& match2) 
  {
    return match1.distance < match2.distance;
  }
};
#endif

#endif

