#ifndef CFileUtil_H
#define CFileUtil_H

namespace CFileUtil {
  inline CFileType getImageTypeFromName(const std::string &filename) {
    std::string suffix;

    size_t pos = filename.rfind('.');

    if (pos != std::string::npos)
      suffix = filename.substr(pos + 1);

    if      (CStrUtil::casecmp(suffix, "bmp") == 0)
      return CFILE_TYPE_IMAGE_BMP;
    else if (CStrUtil::casecmp(suffix, "eps") == 0)
      return CFILE_TYPE_APP_EPS;
    else if (CStrUtil::casecmp(suffix, "gif") == 0)
      return CFILE_TYPE_IMAGE_GIF;
    else if (CStrUtil::casecmp(suffix, "ico") == 0)
      return CFILE_TYPE_IMAGE_ICO;
    else if (CStrUtil::casecmp(suffix, "iff") == 0)
      return CFILE_TYPE_IMAGE_IFF;
    else if (CStrUtil::casecmp(suffix, "jpg" ) == 0 || CStrUtil::casecmp(suffix, "jpeg") == 0)
      return CFILE_TYPE_IMAGE_JPG;
    else if (CStrUtil::casecmp(suffix, "pcx") == 0)
      return CFILE_TYPE_IMAGE_PCX;
    else if (CStrUtil::casecmp(suffix, "png") == 0)
      return CFILE_TYPE_IMAGE_PNG;
    else if (CStrUtil::casecmp(suffix, "ppm") == 0)
      return CFILE_TYPE_IMAGE_PPM;
    else if (CStrUtil::casecmp(suffix, "psp") == 0)
      return CFILE_TYPE_IMAGE_PSP;
    else if (CStrUtil::casecmp(suffix, "ps") == 0)
      return CFILE_TYPE_IMAGE_PS;
    else if (CStrUtil::casecmp(suffix, "rgb") == 0 || CStrUtil::casecmp(suffix, "sgi") == 0)
      return CFILE_TYPE_IMAGE_SGI;
    else if (CStrUtil::casecmp(suffix, "tga") == 0)
      return CFILE_TYPE_IMAGE_TGA;
    else if (CStrUtil::casecmp(suffix, "tif") == 0)
      return CFILE_TYPE_IMAGE_TIF;
    else if (CStrUtil::casecmp(suffix, "xbm") == 0)
      return CFILE_TYPE_IMAGE_XBM;
    else if (CStrUtil::casecmp(suffix, "xpm") == 0)
      return CFILE_TYPE_IMAGE_XPM;
    else if (CStrUtil::casecmp(suffix, "xwd") == 0)
      return CFILE_TYPE_IMAGE_XWD;
    else
      return CFILE_TYPE_NONE;
  }
};

#endif
