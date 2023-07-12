//
// 功能说明：
//   SWIG 工具可以将c++接口形式的TRTCEngine转成其它语言 java 和 go 的接口封装。
//
// 2023-07-12 @ zenowu

%module(directors="1") trtcengine

// 虚类导出，可以用于目标语言的类继承。
%feature("director") TRTCCloudDelegate;
%feature("director") RoomDelegate;
%feature("director") RecordDelegate;
%feature("director") V2TXLivePlayerDelegate;


// "%{" 和 “}%” 的内容原样输出到转换后的 c++ 文件中
%{

#include <string>
#include <stdint.h>
#include "../include/trtc/liteav_trtc_cloud.h"
#include "../include/trtc/liteav_trtc_defines.h"
#include "../include/trtc/liteav_trtc_recorder.h"
#include "../include/live/liteav_live_defines.h"
#include "../include/live/liteav_live_player.h"
#include "../include/live/liteav_live_premier.h"
#include "../include/live/liteav_live_pusher.h"

%}

// swig 解析和转换
%include "std_string.i"
%include "stdint.i"
#include "../include/trtc/liteav_trtc_cloud.h"
#include "../include/trtc/liteav_trtc_defines.h"
#include "../include/trtc/liteav_trtc_recorder.h"
#include "../include/live/liteav_live_defines.h"
#include "../include/live/liteav_live_player.h"
#include "../include/live/liteav_live_premier.h"
#include "../include/live/liteav_live_pusher.h"

// 重命名接口首字母大写
%rename("%(firstuppercase)s", %$isfunction) "";

