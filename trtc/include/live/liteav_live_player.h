// Copyright (c) 2021 Tencent. All rights reserved.

#ifndef SDK_SERVER_INCLUDE_LIVE_LITEAV_LIVE_PLAYER_H_
#define SDK_SERVER_INCLUDE_LIVE_LITEAV_LIVE_PLAYER_H_

#include "liteav_live_defines.h"  // NOLINT(build/include_directory)

namespace liteav {
namespace live {

struct TRTC_API PlayerOption {
  // 期望的音频采样率，单位 Hz，取值 16000，48000.
  // 默认： 48000
  int audio_samplerate = 48000;

  // 期望的音频声道数
  // - 1: 单声道
  // - 2: 立体声, 左右声道数据交错布局
  // 默认：单声道
  int audio_channels = 1;

  // 期望的视频格式
  //  - 1: H264
  //  - 2: YUV420p
  // 默认：YUV420p
  int video_type = 2;
};

class TRTC_API V2TXLivePlayerDelegate {
 public:
  // 错误 回调
  virtual void OnError(Error error) = 0;

  // 音频 PCM 数据回调
  virtual void OnRemoteAudioReceived(const AudioFrame& frame) = 0;

  // 视频 H264 数据回调
  virtual void OnRemoteVideoReceived(const VideoFrame& frame) = 0;

  // 视频 YUV420p 数据回调
  virtual void OnRemoteVideoReceived(const PixelFrame& frame) = 0;

  // 视频 SEI 消息回调
  virtual void OnSeiMessageReceived(int message_type, const uint8_t* message, size_t size) = 0;

  // 播放网络质量回调
  virtual void OnNetworkQuality(NetworkQuality quality) = 0;

 protected:
  virtual ~V2TXLivePlayerDelegate() {}
};

class TRTC_API V2TXLivePlayer {
 public:
  // 创建 V2TXLivePlayer， 注意不需要的时候， 请调用 Destroy() 释放资源
  static V2TXLivePlayer* Create(V2TXLivePlayerDelegate* delegate);

  // 销毁 V2TXLivePlayer
  static void Destroy(V2TXLivePlayer* player);

  // 开始播放
  //
  // |url| - trtc 播放地址
  // |option| - 播放选项，控制数据回调的内容
  //
  // 返回值
  // - ERR_OK: 成功
  // - ERR_INVALID_PARAMETER: 参数无效
  virtual int StartPlay(const char* url, const PlayerOption& option) = 0;

  // 结束播放
  // 返回值
  // - ERR_OK: 成功
  virtual int StopPlay() = 0;

  // 是否播放中
  // - true: 播放中
  // - false: 未播放
  virtual bool IsPlaying() = 0;

 protected:
  virtual ~V2TXLivePlayer() {}
};

}  // namespace live
}  // namespace liteav

#endif  // SDK_SERVER_INCLUDE_LIVE_LITEAV_LIVE_PLAYER_H_
