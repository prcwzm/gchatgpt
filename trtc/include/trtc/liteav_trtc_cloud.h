// Copyright (c) 2021 Tencent. All rights reserved.

#ifndef SDK_SERVER_INCLUDE_TRTC_LITEAV_TRTC_CLOUD_H_
#define SDK_SERVER_INCLUDE_TRTC_LITEAV_TRTC_CLOUD_H_

#include "liteav_trtc_defines.h"  // NOLINT(build/include_directory)

namespace liteav {
namespace trtc {

// TRTC 应用场景
enum TRTCScene {
  // 实时视频通话
  TRTC_SCENE_VIDEO_CALL,

  // 实时语音通话
  TRTC_SCENE_AUDIO_CALL,

  // 视频互动直播
  TRTC_SCENE_LIVE,

  // 语音互动直播
  TRTC_SCENE_VOICE_CHAT,

  // 录制模式，用于服务器端音视频录制场景
  TRTC_SCENE_RECORD,
};

// TRTC 用户角色
enum TRTCRole {
  // 主播
  TRTC_ROLE_ANCHOR,
  // 观众
  TRTC_ROLE_AUDIENCE,
};

// 连接状态
enum ConnectionState {
  // 初始状态
  CONNECTION_STATE_INIT = 0,

  // 连接断开
  CONNECTION_STATE_DISCONNECTED = 1,

  // 正在连接中
  // 网络断开时，SDK 内部会做自动重连
  CONNECTION_STATE_CONNECTING = 2,

  // 已连接上
  CONNECTION_STATE_CONNECTED = 3,
};

// 用户信息
struct TRTC_API UserInfo {
  // 用户字符串 ID，由用户在进房时设置
  TrtcString user_id;
};

// TRTC_SCENE_RECORD 录制模式配置参数
struct TRTC_API RecordConfig {
  RecordConfig()
      : enable_remote_audio_mix(false),
        output_sample_rate(16000),
        output_channels(1),
        output_frame_length_ms(20),
        output_audio_codec_type(AUDIO_CODEC_TYPE_PCM) {}

  // 是否将远端音频混音后通过 TRTCCloudDelegate::OnRemoteMixedAudioReceived()
  // 输出；
  // 默认是 false，每一路远端音频通过 TRTCCloudDelegate::OnRemoteAudioReceived()
  // 输出
  bool enable_remote_audio_mix;

  // 输出采样率，默认是 16khz
  int output_sample_rate;

  // 输出声道数，默认是单声道
  int output_channels;

  // 输出的音频帧长，要求是 20 的整数倍，默认是 20ms，取值范围 [20, 1000]
  // 若输出的音频是用于语音识别，推荐帧长为 100 ms
  // 仅当 |output_audio_codec_type| == AUDIO_CODEC_TYPE_PCM 时有效。
  int output_frame_length_ms;

  // 输出的音频编码格式，默认 AUDIO_CODEC_TYPE_PCM
  AudioCodecType output_audio_codec_type;
};

// 进房参数
struct TRTC_API EnterRoomParams {
  EnterRoomParams();
  ~EnterRoomParams();

  // 进房信息
  RoomParams room;

  // TRTC 应用场景, 默认 TRTC_SCENE_VIDEO_CALL
  TRTCScene scene;

  // TRTC 用户角色， 默认 TRTC_ROLE_ANCHOR
  TRTCRole role;

  // 房间签名。当您不希望所有用户都能进入房间时，
  // 通过设置房间签名的方式，只允许有相同签名的用户进入房间
  // 详细说明:https://cloud.tencent.com/document/product/647/32240
  // 默认 ""
  TrtcString private_map_key;

  // 录制配置参数，|scene| 为 TRTC_SCENE_RECORD 时有效
  RecordConfig record_config;

  // 指定视频格式 |use_pixel_frame_input| 为 true 则视频发送为 YUV 数据。
  // 发送接口是 SendVideoFrame(StreamType type, const PixelFrame& frame)
  // 默认 false
  bool use_pixel_frame_input;

  // 指定视频格式 |use_pixel_frame_input| 为 true 则视频回调为 YUV 数据。
  // 回调接口是 OnRemoteVideoReceived(const char* user_id,
  //                                   StreamType type,
  //                                   const PixelFrame& frame)
  // 默认 false
  bool use_pixel_frame_output;
};

// 音频编码参数
struct TRTC_API AudioEncodeParams {
  AudioEncodeParams()
      : channels(1),
        sample_rate(48000),
        frame_length_ms(20),
        bitrate_bps(51200),
        need_encode(true) {}

  // 声道个数
  int channels;

  // 采样率 Hz
  int sample_rate;

  // 帧长 ms
  int frame_length_ms;

  // 码率
  int bitrate_bps;

  // 是否需要 Opus 编码
  // 当发送 PCM 数据时，|need_encode| 为 true，SDK 内部会进行 Opus 编码
  // 当发送 Opus 数据时，需将 |need_encode| 设置为 false，SDK 内部将直接打包发送
  // opus 数据
  bool need_encode;
};

// TRTCCloudDelegate 所有回调方法均在 SDK 内部线程上执行，
// 请勿在回调方法中执行任何可能会引起阻塞的操作。
class TRTC_API TRTCCloudDelegate {
 public:
  // SDK 内部不可恢复错误通知，需要 App 干预或提示用户。
  virtual void OnError(Error error) = 0;

  // 连接状态通知
  virtual void OnConnectionStateChanged(ConnectionState old_state, ConnectionState new_state) = 0;

  // 本地用户进房成功通知
  virtual void OnEnterRoom() = 0;

  // 本地用户退房成功通知
  virtual void OnExitRoom() = 0;

  ///////////////////////////////////////////////////////////////////////
  //                   本地音视频发送相关回调                         //
  /////////////////////////////////////////////////////////////////////

  // 本地音频发送通道创建成功，此后可以调用 TRTCCloud::SendAudioFrame()
  // 发送音频数据
  virtual void OnLocalAudioChannelCreated() = 0;

  // 本地音频发送通道已销毁
  virtual void OnLocalAudioChannelDestroyed() = 0;

  // 视频流类型为 |type| 的发送通道创建成功，此后可以调用
  // TRTCCloud::SendVideoFrame() 发送视频编码数据
  virtual void OnLocalVideoChannelCreated(StreamType type) = 0;

  // 视频流类型为 |type| 的发送通道已销毁
  virtual void OnLocalVideoChannelDestroyed(StreamType type) = 0;

  // 请求更改视频编码码率
  // SDK 会根据当前网络状况要求视频发送方调整编码码率。
  virtual void OnRequestChangeVideoEncodeBitrate(StreamType type, int bitrate_bps) = 0;

  ///////////////////////////////////////////////////////////////////////
  //                     远端音视频接收相关回调                       //
  /////////////////////////////////////////////////////////////////////

  // 远端用户进房通知
  virtual void OnRemoteUserEnterRoom(const UserInfo& info) = 0;

  // 远端用户退房通知
  virtual void OnRemoteUserExitRoom(const UserInfo& info) = 0;

  // 远端用户是否有音频
  // |user_id| - 远端用户 ID
  // |available|：
  //   - true： 远端用户开启音频
  //   - false：远端用户关闭音频
  virtual void OnRemoteAudioAvailable(const char* user_id, bool available) = 0;

  // 远端用户是否有视频
  // |user_id| - 远端用户 ID
  // |available|：
  //   - true： 远端用户开启视频
  //   - false：远端用户关闭视频
  // |type|：视频流类型
  virtual void OnRemoteVideoAvailable(const char* user_id, bool available, StreamType type) = 0;

  // 远端视频数据接收回调
  // 默认情况下，SDK 内部已做好音画同步，App 在收到视频帧后可立即解码播放。
  //
  // |user_id| - 视频流所属的远端用户 ID
  // |type| - 视频流类型
  // |frame| - 视频编码数据
  virtual void OnRemoteVideoReceived(const char* user_id,
                                     StreamType type,
                                     const VideoFrame& frame) = 0;

  // 远端视频数据接收回调
  // 默认情况下，SDK 内部已做好音画同步，App 在收到视频帧后可立即播放。
  //
  // |user_id| - 视频流所属的远端用户 ID
  // |type| - 视频流类型
  // |frame| - 视频 YUV 数据
  virtual void OnRemoteVideoReceived(const char* user_id,
                                     StreamType type,
                                     const PixelFrame& frame) = 0;

  // TRTC_SCENE_RECORD 录制场景下，单路远端音频 PCM 数据回调接口
  //
  // |user_id| - 远端用户 ID
  // |frame| - 音频 PCM 数据
  //   帧长度为 20 ms，采样率和声道数由进房参数 RecordConfig 控制
  virtual void OnRemoteAudioReceived(const char* user_id, const AudioFrame& frame) = 0;

  // TRTC_SCENE_RECORD 录制场景下，远端音频混流后的 PCM 数据回调接口
  //
  // |frame| - 音频 PCM 数据
  //   帧长度为 20 ms，采样率和声道数由进房参数 RecordConfig 控制
  virtual void OnRemoteMixedAudioReceived(const AudioFrame& frame) = 0;

  // 远端 SEI 数据回调接口
  //
  // |user_id| - 远端用户 ID
  // |stream_type| - 流类型
  // |message_type| - 消息类型
  // |message| - SEI 消息体
  // |length| - SEI 消息长度
  virtual void OnSeiMessageReceived(const char* user_id,
                                    StreamType stream_type,
                                    int message_type,
                                    const uint8_t* message,
                                    int length) {}

 protected:
  virtual ~TRTCCloudDelegate() {}
};

// TRTCCloud 用于连接腾讯云 TRTC 服务器，提供实时音视频流的发送和接收服务。
// 它支持音频 PCM 数据的发送和接收，以及视频 H264 编码数据的发送和接收。
// - 对于接收到的音视频数据，SDK 内部已做好抖动缓冲处理和音画同步，使用者只需要
//   对音频数据进行播放和视频解码渲染。
// - 对于视频发送，SDK 会根据当前网络状况指导发送方进行码率调控。

class TRTC_API TRTCCloud {
 public:
  // 创建一个 TRTCCloud 实例
  // |delegate| - TRTCCloudDelegate 委托实现。
  //
  // TRTCCloud 支持多实例.
  static TRTCCloud* Create(TRTCCloudDelegate* delegate);

  // 销毁 TRTCCloud 实例.
  static void Destroy(TRTCCloud* cloud);

  // 进入 TRTC 房间
  // 进房成功会触发 TRTCCloudDelegate::OnEnterRoom() 回调。
  //
  // |param| - 进房参数
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error 定义。
  virtual int EnterRoom(const EnterRoomParams& params) = 0;

  // 退出 TRTC 房间
  // 退出房间成功会触发 TRTCCloudDelegate::OnExitRoom() 回调。
  //
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error 定义。
  virtual int ExitRoom() = 0;

  ///////////////////////////////////////////////////////////////////////////
  //                                                                      //
  // 以下接口必须在 TRTCCloudDelegate::OnEnterRoom() 进房回调触发后再调用 //
  //                                                                      //
  /////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  //                      远端音视频流控制接口                        //
  /////////////////////////////////////////////////////////////////////

  // 订阅指定远端用户的音频或视频流
  //
  // |user_id| - 远端用户 ID
  // |type| - 音频或视频流类型
  //
  // 默认情况下，用户进房后 SDK 会自动订阅所有远端用户的音视频流。
  //
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error。
  virtual int Subscribe(const char* user_id, StreamType type) = 0;

  // 取消订阅指定远端用户的音频或视频流
  // 取消订阅后，服务器将会停止下发该远端用户的音频或视频数据。App 可通过调用
  // Subscribe() 方法重新订阅。
  //
  // |user_id| - 远端用户 ID
  // |type| - 音频或视频流类型
  //
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error 定义。
  virtual int Unsubscribe(const char* user_id, StreamType type) = 0;

  // 读取指定远端用户的音频 PCM 数据，默认每次读取 20ms 数据。
  // 为了最小化音频播放延迟，请通过系统高优先级线程定时调用该接口读取数据进行播放。
  //
  // 注意：该接口仅在非录制模式下生效，即进房参数 |scene| 不等于
  // TRTC_SCENE_RECORD 时。
  //
  // |user_id| - 远端用户 ID
  // |frame| - 音频帧
  //
  // 返回值：
  //  - >0：为成功读取到的字节数
  //  - <0：为读取失败，具体错误码参见 Error 定义
  virtual int GetAudioFrame(const char* user_id, AudioFrame* frame) = 0;

  ///////////////////////////////////////////////////////////////////////
  //                     本地音频发送控制接口                         //
  /////////////////////////////////////////////////////////////////////

  // 创建本地音频发送通道
  // 音频发送通道创建成功后会触发
  // TRTCCloudDelegate::OnLocalAudioChannelCreated()，此时可以调用
  // SendAudioFrame() 发送数据。
  //
  // |params| - 音频编码参数
  // 若需要发送 opus 编码数据，需将 |params.need_encode| 设置为 false。
  //
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error 定义。
  virtual int CreateLocalAudioChannel(const AudioEncodeParams& params) = 0;

  // 在 TRTCCloudDelegate::OnLocalAudioChannelCreated()
  // 回调触发后开始调用该接口发送音频数据包。
  //
  // - 若 CreateLocalAudioChannel() 时传入的 AudioEncodeParams::need_encode 为
  //   true 时，发送 PCM 数据包。建议每次发送 20MS 长度的 PCM 数据包。
  // - 若 CreateLocalAudioChannel() 时传入的 AudioEncodeParams::need_encode 为
  //   false 时，发送 Opus 编码数据。
  //
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error 定义。
  virtual int SendAudioFrame(const AudioFrame& frame) = 0;

  // 销毁本地音频发送通道
  // 销毁成功后会触发 TRTCCloudDelegate::OnLocalAudioChannelDestroyed() 回调
  //
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error 定义。
  virtual int DestroyLocalAudioChannel() = 0;

  ///////////////////////////////////////////////////////////////////////
  //                     本地视频发送控制接口                         //
  /////////////////////////////////////////////////////////////////////

  // 创建本地视频发送通道
  // |type|:
  // - STREAM_TYPE_VIDEO_HIGH：大流 - 高分辨率高码率视频流
  // - STREAM_TYPE_VIDEO_LOW：小流 - 低分辨率低码率视频流
  // - STREAM_TYPE_VIDEO_AUX：辅流 - 通常用于屏幕分享，一个房间内只能有一个辅流
  //
  // 视频发送通道创建成功后会触发
  // TRTCCloudDelegate::OnLocalVideoChannelCreated() 回调。 此时可以调用
  // SendVideoFrame() 发送视频编码数据。
  //
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error 定义。
  virtual int CreateLocalVideoChannel(StreamType type) = 0;

  // 发送视频编码数据
  // 在 TRTCCloudDelegate::OnVideoChannelCreated()
  // 回调成功后调用该接口进行发送视频编码数据
  //
  // |type| - 在指定视频流发送通道上发送。
  //
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error 定义。
  virtual int SendVideoFrame(StreamType type, const VideoFrame& frame) = 0;

  // 发送视频 YUV 数据
  // 在 TRTCCloudDelegate::OnVideoChannelCreated()
  // 回调成功后调用该接口进行发送视频 YUV 数据
  //
  // |type| - 在指定视频流发送通道上发送。
  //
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error 定义。
  virtual int SendVideoFrame(StreamType type, const PixelFrame& frame) = 0;

  // 发送视频 SEI 数据
  // 在 TRTCCloudDelegate::OnVideoChannelCreated()
  // 回调成功后调用该接口进行发送视频 SEI 数据
  //
  // |message_type| - 可取值 5 和 242， 推荐 242，和其它端取值要相同才可以互通。
  // |message| - 消息体
  // |length| - 消息字节长度
  //
  // 返回值：
  // - 0: 方法调用成功
  // - <0: 方法调用失败，具体错误码参见 Error 定义。
  //
  // 注意（任何一条不满足，返回失败）：
  // - 1: 单条消息数据量限制，不超过 1000 字节
  // - 2: 发送频率限制，每秒不超过 30 条
  // - 3: 发送码率限制，每秒不超过 8000 字节
  virtual int SendSeiMessage(int message_type, const uint8_t* message, int length) = 0;

  // 销毁本地视频发送通道
  // 成功销毁后会触发 TRTCCloudDelegate::OnLocalVideoChannelDestroyed() 回调。
  // |type| 为指定的视频流通道类型
  //
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error 定义。
  virtual int DestroyLocalVideoChannel(StreamType type) = 0;

 protected:
  virtual ~TRTCCloud() {}
};

// C API: 目的满足本库的动态加载，采用 dlopen() / dlclose() 的使用场景。
// 便于 dlopen() 调用成功后找到 TRTCCloud 的创建和销毁函数。
extern "C" {
// 创建一个 TRTCCloud 实例
// |delegate| - TRTCCloudDelegate 委托实现。
//
// TRTCCloud 支持多实例. 功能和 TRTCCloud::Create() 相同
TRTCCloud* TRTC_API CreateTRTCCloud(TRTCCloudDelegate* delegate);

// // 销毁 TRTCCloud 实例. 功能和 TRTCCloud::Destroy() 相同
void TRTC_API DestroyTRTCCloud(TRTCCloud* cloud);
}

}  // namespace trtc
}  // namespace liteav

#endif  // SDK_SERVER_INCLUDE_TRTC_LITEAV_TRTC_CLOUD_H_
