#ifndef __LISENCEKEY_H
#define __LISENCEKEY_H

#include <string>

#ifdef VSN_OS_WIN
    static const std::string strKey = "C3D_Vision_Samples.2020010120201231.[cnv][mdl][slv][vsn]";
    static const std::string strSignature = "aAsXgt9PCGLz/BU/8PGg033fYFcvlKcosIYOm/q3B9YLnzSFnl/D8ITuwAg9tEiWqaMtYeOv9Y05BezO35MPpg==";
#else
    static const std::string strKey = "C3D_Vision_Samples_Linux.2020010120201231.[cnv][mdl][slv][vsn]";
    static const std::string strSignature = "JXtCYgkCU5izGK/g7eQnnvw+lNrUMUqmOTwTCXDgfgI2sWpfM/3tpgzCay+b5Mx9KFKDfFryo0mdWRvfRdSsIA==";
#endif

#endif // __LISENCEKEY_H
