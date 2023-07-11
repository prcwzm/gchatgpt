// Copyright (c) 2021 Tencent. All rights reserved.

#ifndef SDK_SERVER_INCLUDE_LIVE_LITEAV_LIVE_DEFINES_H_
#define SDK_SERVER_INCLUDE_LIVE_LITEAV_LIVE_DEFINES_H_

#include <stdint.h>

#include <cstddef>

namespace liteav {
namespace live {

#if defined(_WIN32)

#if defined(NOT_BUILD_TRTC_SDK)
#define TRTC_API
#else
#if defined(BUILD_TRTC_SDK)
#define TRTC_API __declspec(dllexport)
#else
#define TRTC_API __declspec(dllimport)
#endif  // defined(BUILD_TRTC_SDK)
#endif

#else  // defined(_WIN32)
#if defined(BUILD_TRTC_SDK)
#define TRTC_API __attribute__((visibility("default")))
#else
#define TRTC_API
#endif
#endif

// 错误码
enum Error {
  // 无错误
  ERR_OK = 0,

  // 通用错误，可根据日志文件排查问题
  ERR_FAILED = -1,

  // 业务层逻辑错误：非法参数
  ERR_INVALID_PARAMETER = -2,

  // 业务层逻辑错误：非法调用
  ERR_INVALID_OPERATION = -3,

  // 当前 API 不支持调用
  ERR_NOT_SUPPORTED = -4,

  // 无法连接网络
  ERR_NETWORK_UNREACHABLE = -103,

  // 进房失败
  ERR_ENTER_ROOM_FAILED = -3301,

  // 进房参数 user_sig 无效
  ERR_INVALID_USER_SIG = -3320,

  // 进房参数 房间号格式错误
  ERR_INVALID_ROOM_ID = -3318,

  // 进房参数 sdk_app_id 无效
  ERR_INVALID_SDK_APP_ID = -3317,

  // 服务不可用。请检查：套餐包剩余分钟数是否大于0，腾讯云账号是否欠费
  ERR_SERVICE_SUSPENDED = -100013,
};

// 网络质量
enum NetworkQuality {
  // 未定义
  NETWORK_QUALITY_UNKNOWN = 0,

  // 当前网络非常好
  NETWORK_QUALITY_EXCELLENT = 1,

  // 当前网络比较好
  NETWORK_QUALITY_GOOD = 2,

  // 当前网络一般
  NETWORK_QUALITY_POOR = 3,

  // 当前网络较差
  NETWORK_QUALITY_BAD = 4,

  // 当前网络很差
  NETWORK_QUALITY_VERY_BAD = 5,

  // 当前网络不满足 TRTC 的最低要求
  NETWORK_QUALITY_DOWN = 6,
};
// 音频 codec 类型
enum AudioCodecType {
  // 音频裸数据 PCM
  AUDIO_CODEC_TYPE_PCM,

  // OPUS 编码
  AUDIO_CODEC_TYPE_OPUS,
};

// 音频帧
class TRTC_API AudioFrame {
 public:
  AudioFrame();
  AudioFrame(const AudioFrame& other);
  AudioFrame& operator=(const AudioFrame& other);
  virtual ~AudioFrame();

  // 设置音频帧数据
  // |data| - 音频数据缓冲起始地址
  // |size| - 音频数据长度，单位 bytes
  // 主要用于 SendAudioFrame(const AudioFrame& frame) 前写入音频数据。
  void SetData(const uint8_t* data, size_t size);

  // 音频数据缓冲起始地址 （只读）
  const uint8_t* data() const;
  // 音频数据长度，单位 bytes
  size_t size() const;

  // 采样率
  int sample_rate;

  // 声道个数
  int channels;

  // 每个采样的位宽，目前仅支持 16bits
  int bits_per_sample;

  // 音频编码类型
  AudioCodecType codec;

  // PTS，单位：毫秒
  uint32_t pts;

  // SDK internal use only
  const void* handle() const { return handle_; }

 private:
  void* handle_;
};

// 顺时针旋转角度
enum VideoRotation {
  VIDEO_ROTATION_0 = 0,
  VIDEO_ROTATION_90 = 1,
  VIDEO_ROTATION_180 = 2,
  VIDEO_ROTATION_270 = 3,
};

// 视频编码类型
enum VideoCodecType {
  // H264/AVC 编码
  VIDEO_CODEC_TYPE_H264,

  // H265/HEVC 编码
  VIDEO_CODEC_TYPE_H265,
};

// 视频帧
class TRTC_API VideoFrame {
 public:
  VideoFrame();
  VideoFrame(const VideoFrame& other);
  VideoFrame& operator=(const VideoFrame& other);
  virtual ~VideoFrame();

  // 设置视频帧数据
  // |data| - 视频数据缓冲起始地址
  // |size| - 视频数据长度，单位 bytes
  // 主要用于 SendVideoFrame(const VideoFrame& frame) 前写入视频数据。
  void SetData(const uint8_t* data, size_t size);

  // 视频数据缓冲起始地址 (只读)
  const uint8_t* data() const;
  // 视频帧长度，单位 bytes
  size_t size() const;

  // PTS 单位：毫秒
  uint32_t pts;

  // DTS 单位：毫秒
  uint32_t dts;

  // 是否为关键帧
  bool is_key_frame;

  VideoCodecType codec;

  // 视频顺时针旋转角度
  VideoRotation rotation;

  // SDK internal use only
  const void* handle() const { return handle_; }

 private:
  void* handle_;
};

// YUV类型
enum VideoPixelFormat {
  // yuv420p
  VIDEO_PIXEL_FORMAT_YUV420p,
};

class TRTC_API PixelFrame {
 public:
  PixelFrame();
  PixelFrame(const PixelFrame& other);
  PixelFrame& operator=(const PixelFrame& other);
  virtual ~PixelFrame();

  // 设置视频帧数据
  // |data| - 视频数据缓冲起始地址
  // |size| - 视频数据长度，单位 bytes
  // 主要用于 SendVideoFrame(const PixelFrame& frame) 前写入视频数据。
  void SetData(const uint8_t* data, size_t size);

  // 视频数据缓冲起始地址 (只读)
  const uint8_t* data() const;
  // 视频帧长度，单位 bytes
  size_t size() const;

  // PTS 单位：毫秒
  uint32_t pts;

  // 视频宽度 单位：像素
  uint32_t width;

  // 视频高度 单位：像素
  uint32_t height;

  VideoPixelFormat format;

  // 视频顺时针旋转角度
  VideoRotation rotation;

  // SDK internal use only
  const void* handle() const { return handle_; }

 private:
  void* handle_;
};

}  // namespace live
}  // namespace liteav

#endif  // SDK_SERVER_INCLUDE_LIVE_LITEAV_LIVE_DEFINES_H_
