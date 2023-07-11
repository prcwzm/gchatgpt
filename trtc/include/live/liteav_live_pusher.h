// Copyright (c) 2021 Tencent. All rights reserved.

#ifndef SDK_SERVER_INCLUDE_LIVE_LITEAV_LIVE_PUSHER_H_
#define SDK_SERVER_INCLUDE_LIVE_LITEAV_LIVE_PUSHER_H_

#include "liteav_live_defines.h"  // NOLINT(build/include_directory)

namespace liteav {
namespace live {

struct TRTC_API PushOption {
  // 视频格式
  //  - 1: H264
  //  - 2: YUV420p
  // 默认：YUV420p
  int video_type = 2;
};

struct TRTC_API LivePusherStatistics {
  // 发送帧率 fps
  uint32_t video_frame_rate_sent = 0;
  // 服务器视频接收帧率 fps
  uint32_t video_frame_rate_received = 0;
  // 上行丢包率，百分比
  uint32_t loss = 0;
  // 上行 rtt 单位 ms
  uint32_t rtt = 0;
};

class TRTC_API V2TXLivePusherDelegate {
 public:
  // 错误 回调
  virtual void OnError(Error error) = 0;

  // 推流网络质量回调
  virtual void OnNetworkQuality(NetworkQuality quality) = 0;

  // 推流状态回调 2s 一次
  virtual void OnStatisticsUpdate(const LivePusherStatistics& stats) = 0;

 protected:
  virtual ~V2TXLivePusherDelegate() {}
};

class TRTC_API V2TXLivePusher {
 public:
  // 创建 V2TXLivePusher， 注意不需要的时候， 请调用 Destroy() 释放资源
  static V2TXLivePusher* Create(V2TXLivePusherDelegate* delegate);

  // 销毁 V2TXLivePusher
  static void Destroy(V2TXLivePusher* pusher);

  // 开始推流
  //
  // |url| - trtc 推流地址
  // |option| - 推流选项
  //
  // 返回值
  // - ERR_OK: 成功
  // - ERR_INVALID_PARAMETER: 参数无效
  //
  // 收到 OnReady() 回调前, SendVideoFrame(), SendAudioFrame()
  // 调用将返回失败
  virtual int StartPush(const char* url, const PushOption& option) = 0;

  // 停止推流
  virtual int StopPush() = 0;

  // 是否在推流中
  // - true: 推流中
  // - false: 未推流
  virtual bool IsPushing() = 0;

  // 发送 PCM 音频
  //
  // |frame| - 音频帧长要求是 20ms，采样率 16000Hz 或 48000Hz，声道数 1 或 2，
  //           每个采样点 16 位有符号表示。
  //
  // 返回值：
  // - ERR_OK: 成功
  // - ERR_INVALID_PARAMETER: 参数无效
  virtual int SendAudioFrame(const AudioFrame& frame) = 0;

  // 发送 H264 视频
  //
  // |frame| - 视频编码后的 H264 格式的视频帧
  //
  // 返回值：
  // - ERR_OK: 成功
  // - ERR_INVALID_PARAMETER: 参数无效
  virtual int SendVideoFrame(const VideoFrame& frame) = 0;

  // 发送 YUV420p 视频
  //
  // |frame| - YUV420p 数据帧。
  //
  // 返回值：
  // - ERR_OK: 成功
  // - ERR_INVALID_PARAMETER: 参数无效
  virtual int SendVideoFrame(const PixelFrame& frame) = 0;

  // 发送 视频 SEI 消息
  //
  // |message_type| - 取值 5 或 242，推荐 242
  // |message| - SEI 消息体
  // |size| - SEI 消息长度，单位字节
  //
  // 注意（任何一条不满足，返回失败）：
  // - 1: 单条消息数据量限制，不超过 1000 字节
  // - 2: 发送频率限制，每秒不超过 30 条
  // - 3: 发送码率限制，每秒不超过 8000 字节
  //
  // 返回值：
  // - ERR_OK: 成功
  // - ERR_INVALID_PARAMETER: 参数无效
  // - ERR_INVALID_OPERATION: 操作无效
  virtual int SendSeiMessage(int message_type, const uint8_t* message, size_t size) = 0;

 protected:
  virtual ~V2TXLivePusher() {}
};
}  // namespace live

}  // namespace liteav

#endif  // SDK_SERVER_INCLUDE_LIVE_LITEAV_LIVE_PUSHER_H_
