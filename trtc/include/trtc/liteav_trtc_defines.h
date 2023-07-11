// Copyright (c) 2021 Tencent. All rights reserved.

#ifndef SDK_SERVER_INCLUDE_TRTC_LITEAV_TRTC_DEFINES_H_
#define SDK_SERVER_INCLUDE_TRTC_LITEAV_TRTC_DEFINES_H_

#include <stdint.h>

#include <cstddef>

namespace liteav {
namespace trtc {

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

  // license 不合法，调用失败
  ERR_INVALID_LICENSE = -5,

  // 请求服务器超时
  ERR_REQUEST_SERVER_TIMEOUT = -6,

  // 服务器无法处理您的请求
  ERR_SERVER_PROCESS_FAILED = -7,

  // 当前无法读取，请重试
  // 比如：GetAudioFrame() 返回该值时表明当前没有数据可读，请在下一轮重试
  ERR_READ_TRY_AGAIN = -102,

  // 无法连接网络
  ERR_NETWORK_UNREACHABLE = -103,

  // 进房失败
  ERR_ENTER_ROOM_FAILED = -3301,

  // 进房参数 sdk_app_id 无效
  ERR_INVALID_SDK_APP_ID = -3317,

  // 进房参数 房间号格式错误
  ERR_INVALID_ROOM_ID = -3318,

  // 进房参数 user_id 格式错误
  ERR_INVALID_USER_ID = -3319,

  // 进房参数 user_sig 无效
  ERR_INVALID_USER_SIG = -3320,

  // 服务不可用。请检查：套餐包剩余分钟数是否大于0，腾讯云账号是否欠费
  ERR_SERVICE_SUSPENDED = -100013,

  // 当前用户被服务器踢出房间
  ERR_BANNED_BY_SERVER = -205,
};

class TRTC_API TrtcString {
 public:
  TrtcString();
  TrtcString(const char* str);
  TrtcString(const TrtcString& other);
  ~TrtcString();
  TrtcString& operator=(const char* str);
  TrtcString& operator=(const TrtcString& other);
  void SetValue(const char* str);
  const char* GetValue() const;

 private:
  void* str_;
};

class TRTC_API TrtcBuffer {
 public:
  TrtcBuffer();
  TrtcBuffer(const TrtcBuffer& other);
  TrtcBuffer& operator=(const TrtcBuffer& other);
  ~TrtcBuffer();

  // 拷贝 |data| 内部会自动分配不少于 |size| 字节的空间。
  void SetData(const uint8_t* data, size_t size);

  // 设置 有效数据 size
  // 如果此 size 超过当前 capacity，会造成重新分配内存，并复制数据
  //
  // 注意：data() / cdata() 之前获取的地址可能失效。
  void SetSize(size_t size);

  // 获取 buffer 的内存地址
  uint8_t* data();

  // 获取 buffer 的内存地址
  const uint8_t* cdata() const;

  // 获取 buffer 的内存 size
  size_t size() const;

  // SDK internal use only
  const void* handle() const { return handle_; }

 private:
  void* handle_;
};

// 音视频流类型
enum StreamType {
  // 未知类型
  STREAM_TYPE_UNKNOWN = -1,

  // 音频流
  STREAM_TYPE_AUDIO = 1,

  // 大流，即高分辨率高码率视频
  STREAM_TYPE_VIDEO_HIGH = 2,

  // 小流，即低分辨率低码率视频
  STREAM_TYPE_VIDEO_LOW = 3,

  // 辅流，主要用于屏幕分享。同一时刻，一个房间内只允许有一个辅流（屏幕分享）。
  STREAM_TYPE_VIDEO_AUX = 7,
};

struct TRTC_API EncryptionData {
  EncryptionData();
  ~EncryptionData();
  // 房间 ID 和进房参数保持一致
  int32_t room_id = 0;
  TrtcString str_room_id;
  // 远端用户 ID
  TrtcString user_id;
  // 音视频流类型
  StreamType stream_type;
  // 原始数据
  const TrtcBuffer* decrypted = nullptr;
  // 写回加密后的数据
  TrtcBuffer* encrypted = nullptr;
};

class TRTC_API EncryptionDelegate {
 public:
  virtual bool OnDataEncrypt(EncryptionData& data) = 0;

 protected:
  virtual ~EncryptionDelegate() {}
};

struct TRTC_API DecryptionData {
  DecryptionData();
  ~DecryptionData();
  // 房间 ID 和进房参数保持一致
  int32_t room_id = 0;
  TrtcString str_room_id;
  // 远端用户 ID
  TrtcString user_id;
  // 音视频流类型
  StreamType stream_type;
  // 原始数据
  const TrtcBuffer* encrypted = nullptr;
  // 写回解密后的数据
  TrtcBuffer* decrypted = nullptr;
};

class TRTC_API DecryptionDelegate {
 public:
  virtual bool OnDataDecrypt(DecryptionData& data) = 0;

 protected:
  virtual ~DecryptionDelegate() {}
};

// 进房参数
struct TRTC_API RoomParams {
  RoomParams();
  RoomParams(const RoomParams& other);
  ~RoomParams();

  // |sdk_app_id| 为从腾讯云官网申请的 sdkAppId，默认 0
  uint32_t sdk_app_id = 0;

  // |user_id| 用户 ID， 默认为空
  TrtcString user_id;

  // |user_sig| 用户签名
  // 如何计算UserSig: https://cloud.tencent.com/document/product/647/17275
  // 默认为空
  TrtcString user_sig;

  // 数字房间号，取值范围：[1, 0xfffffffe]
  // |room_id| 和 |str_room_id| 只能设置其中一个
  uint32_t room_id = 0;

  // 字符串房间号
  // 限制长度最大为64字节。以下为支持的字符集（共 89 个字符）:
  //   -大小写英文字母（a-zA-Z）；
  //   -数字（0-9）；
  //   -空格、"!"、"#"、"$"、"%"、"&"、"("、")"、"+"、"-"、":"、";"、"<"、"="、"."、">"、"?"、
  //    "@"、"["、"]"、"^"、"_"、"{"、"}"、"|"、"~"、","。
  // 各端都必须使用同类型房间号才能实现互通
  // |room_id| 和 |str_room_id| 只需设置其中一个
  // 默认为空
  TrtcString str_room_id;

  // 客户自定义数据
  // 为了方便客户业务层能和 RecordRoom 实例做映射，客户可以将一些相关的信息存放于此，
  // 之后可通过|RecordRoom::GetRoomParams()|获取到该信息
  void* custom_data = nullptr;

  // 自定义音视频数据解密回调, 默认为 nullptr
  DecryptionDelegate* decryption_delegate = nullptr;

  // 自定义音视频数据加密回调, 默认为 nullptr
  EncryptionDelegate* encryption_delegate = nullptr;
};

// 水印类型
enum WatermarkType {
  // 图片水印
  kWatermarkImage = 1,
  // 文本水印
  kWatermarkText = 2,
  // 时间戳水印 当前时间
  kWatermarkTimestamp = 3,
};

// 水印配置
struct TRTC_API WatermarkConfig {
  WatermarkConfig();
  WatermarkConfig(const WatermarkConfig& other);
  ~WatermarkConfig();
  WatermarkType type = WatermarkType::kWatermarkTimestamp;
  // 水印相对于 画布左上点，水平方向偏移，单位：像素
  int offset_x = 0;
  // 水印相对于 画布左上点，垂直方向偏移，单位：像素
  int offset_y = 0;
  // 水印宽度，单位：像素
  int width = 0;
  // 水印高度，单位：像素
  int height = 0;

  // 文本水印字体文件路径，要求是本地路径，UTF-8编码.
  // 本 SDK 不提供版权字体，使用者请确保对所使用的的字体拥有版权或使用权
  TrtcString path_to_font;
  // 文本水印 字段大小 默认 10
  size_t font_size = 10;
  // 水印内容，要求必须是UTF-8编码，
  // |type| = kWatermarkText, |content| 表示水印的文本内容
  // |type| = kWatermarkImage, |contect| 表示图片水印的文件，本地路径。
  TrtcString content;

  TrtcString ToString() const;
};

// 音频 codec 类型
enum AudioCodecType {
  // 音频裸数据 PCM
  AUDIO_CODEC_TYPE_PCM,

  // OPUS 编码
  AUDIO_CODEC_TYPE_OPUS,

  // AAC 编码
  AUDIO_CODEC_TYPE_AAC,
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

}  // namespace trtc
}  // namespace liteav

#endif  // SDK_SERVER_INCLUDE_TRTC_LITEAV_TRTC_DEFINES_H_
