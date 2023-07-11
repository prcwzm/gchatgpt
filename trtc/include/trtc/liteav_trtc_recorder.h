// Copyright (c) 2022 Tencent. All rights reserved.
// Author: yaobohua

#ifndef SDK_SERVER_INCLUDE_TRTC_LITEAV_TRTC_RECORDER_H_
#define SDK_SERVER_INCLUDE_TRTC_LITEAV_TRTC_RECORDER_H_

#include "liteav_trtc_defines.h"  // NOLINT(build/include_directory)

namespace liteav {
namespace trtc {

// 录制封装格式
enum FileFormat {
  // 音频 AAC 编码，视频 H264 编码，采用 FLV 格式封装
  kFlv = 1,

  // 音频 AAC 编码，视频 H264 编码，采用 MP4 格式封装
  kMp4 = 2,

  // 仅限于纯音频录制，音频 MP3 编码，采用 MP3 格式封装
  kMp3 = 3,
};

enum RecordMode {
  // 单流录制
  kRecordSingleStream = 1,
  // 多流录制
  kRecordMultiStreams = 2,
};

enum RecordType {
  // 仅录制音频数据
  kAudioOnly = 1,

  // 仅录制视频数据
  kVideoOnly = 2,

  // 录制音视频数据，从有视频数据开始录制。
  kAudioAndVideo = 3,
};

struct TRTC_API SingleRecordParams {
  SingleRecordParams();
  SingleRecordParams(const SingleRecordParams& other);
  ~SingleRecordParams();

  // 需要录制的远端用户 ID
  TrtcString user_id;

  // 流类型(仅填视频类型)
  StreamType stream_type = StreamType::STREAM_TYPE_VIDEO_HIGH;

  TrtcString ToString() const;
};

// 布局模式
enum LayoutMode {
  // 自定义布局
  // 使用 UpdateLayout() 接口实现布局
  KManual = 0,

  // 九宫格
  // 指定宫格数 N
  // 举例 N = 5
  // ----------------------
  // |      |      |      |
  // |   1  |   2  |   3  |
  // |      |      |      |
  // |--------------------|
  // |      |      |      |
  // |   4  |   5  |      |
  // |      |      |      |
  // ----------------------
  kSpeedDial = 1,

  // 线性布局，水平方向，从左往右
  // 指定格数 N， 当前就2个用户在房间
  // 举例 N = 5
  // ------------------------------------
  // |      |      |      |      |      |
  // |   1  |   2  |      |      |      |
  // |      |      |      |      |      |
  // ------------------------------------
  kLinearHorizontal = 2,

  // 线性布局，垂直方向，从上往下
  // 指定格数 N， 当前就2个用户在房间
  // 举例 N = 3
  // --------
  // |      |
  // |   1  |
  // |      |
  // --------
  // |      |
  // |   2  |
  // |      |
  // --------
  // |      |
  // |      |
  // |      |
  // --------
  kLinearVertical = 3,
};

// 画面填充模式
enum FillMode {
  // 示例：宽高比不一致的情况
  //            待填充的区域宽高比（1:1）           画面宽高比（2：1）
  //             --------------           **************************
  //             |            |           *                        *
  //             |            |           *                        *
  //             |            |      +    *                        *
  //             |            |           *                        *
  //             |            |           *                        *
  //             --------------           **************************
  //

  // 占满显示区域，填充适应居中显示。
  // 当待填充的区域的宽高比和填充内容的不一致时，可能部分画面无法显示。
  // 按示例：画面的 1 和 2 对应的区域将会被裁剪掉，不显示。
  //             ********-------------********
  //             *       |           |       *
  //             *       |           |       *
  //             *   1   |           |     2 *
  //             *       |           |       *
  //             *       |           |       *
  //             ********-------------********
  kFill = 0,

  // 自适应显示，可以完整显示画面。
  // 当待填充的区域的宽高比和填充内容的不一致时，可能画面无法填满待显示的区域。
  // 画面显示在区域的中间。
  // 按示例：画面的 1 和 2 对应的区域将会用背景色填充。
  //             --------------
  //             |      1     |
  //             **************
  //             *            *
  //             **************
  //             |      2     |
  //             --------------
  kFit = 1,
};

struct TRTC_API MultiRecordParams {
  MultiRecordParams();
  MultiRecordParams(const MultiRecordParams& other);
  ~MultiRecordParams();

  // 录制的画面宽度
  // |width| 取值范围 [16,4096]，单位像素
  uint32_t width = 16;

  // 录制的画面高度
  // |height| 取值范围 [16,4096]，单位像素
  uint32_t height = 16;

  // 录制的画面帧率
  // |video_frame_rate| 取值范围 [10, 50]
  // 建议 20
  uint32_t video_frame_rate = 20;

  // 画布的背景色(RGB)
  // eg. 0x00FF0000 表示红色
  int background_color = 0x00000000;

  // 自动布局模式
  // 当 |layout_mode| != LayoutMode::kManual 时，按照 |layout_mode|
  // 指定的模式自动布局画面。否则需要调用API进行布局。
  LayoutMode layout_mode = LayoutMode::kSpeedDial;

  // 固定人数布局
  // 当 |layout_mode| != LayoutMode::kManual 时有效
  // |max_layout_count| = 0，自动布局自动更加房间主播人数，动态划分子区域。
  // |max_layout_count| = N，N是一个固定值，布局就按照N，固定划分子区域。
  // 适合主播人数固定的应用场景。
  //
  // 具体划分子区域的方式 和 |layout_mode| 取值不同，可能有些差异。
  // 在 |max_layout_count| = 0
  // 的情况下，若自动布局模式是九宫，可能有些子区域没有填充主播画面。线性自动排布模式则会填满子区域。
  uint32_t max_layout_count = 0;

  TrtcString ToString() const;
};

struct TRTC_API RecordParams {
  RecordParams();
  RecordParams(const RecordParams& other);
  ~RecordParams();

  // 默认录制 FLV 格式
  FileFormat file_format = FileFormat::kFlv;

  // 默认录制音视频
  RecordType record_type = RecordType::kAudioAndVideo;

  // 录制文件以及内部产生的缓存文件存放的根目录
  // |storage_directory| 为空，则取当前目录
  // 请确保所指定的目录有读写权限
  //
  // 录制文件命名规则
  // out_${record_mode}_${sdk_app_id}_${room_id}_${remote_user_id}_f_${start_ts}_e_${stop_ts}.${file_format}
  // record_mode 录制模式，可以取的值 “audio”，“video”，和 “av”。
  // remote_user_id 若是单流录制，则是被录制的远端主播
  // ID，若是合流录制则是”mixed“。 start_ts 开始录制服务器 NTP 时间戳 单位毫秒
  // stop_ts 结束录制服务器 NTP 时间戳 单位毫秒
  // file_format 录制文件格式
  TrtcString storage_directory;

  // 分片录制时长
  // 单位 秒 0 表示不切片， 建议切片大小 [5, 60].
  // 注意：单流录制模式下，要求主播端采用 GOP 模式推流，gop 大小建议是1s。主播端通常通过 live
  // 场景进行推流。
  //      相邻的分片之间，会有短暂的间隔，通常小于一个gop。
  // 合流录制经过视频解码-合并-编码处理，无上述的要求。
  uint32_t segment_duration_in_seconds = 0;

  // 录制模式
  RecordMode record_mode = RecordMode::kRecordSingleStream;

  // 单流录制参数配置
  // ｜record_mode｜== RecordMode::kRecordSingleStream 时设置
  SingleRecordParams single_record_params;

  // 多流录制参数配置
  // ｜record_mode｜== RecordMode::kRecordMultiStreams 时设置
  MultiRecordParams multi_record_params;

  // 客户自定义数据
  // 为了方便客户业务层能和 Recorder 实例做映射，客户可以将一些相关的信息存放于此，
  // 之后可通过|Recorder::GetRecordParams()|获取到该信息
  void* custom_data = nullptr;

  TrtcString ToString() const;
};

// 布局相关信息
struct TRTC_API LayoutParams {
  LayoutParams();
  ~LayoutParams();

  // 需要录制的远端用户 ID
  TrtcString user_id;

  // 流类型
  StreamType stream_type = StreamType::STREAM_TYPE_VIDEO_HIGH;

  // 相对于画布左上角 水平偏移
  // 单位 像素
  uint32_t x = 0;

  // 相对于画布左上角 垂直偏移
  // 单位 像素
  uint32_t y = 0;

  // 占据画布的宽度
  // 单位 像素
  uint32_t width = 16;

  // 占据画布的高度
  // 单位像素
  uint32_t height = 16;

  // 深度层级
  // 取值范围 [1,100] 如果有重叠，|zorder| 较小者，可能被较大者遮挡。
  uint32_t zorder = 1;

  // 填充模式
  FillMode mode = FillMode::kFit;

  // 背景色
  int color = 0x00000000;

  TrtcString ToString() const;
};

enum RecordError {
  kErrorNone = 0,
  kErrorNotFoundRoom = -1,
  kErrorDiskIO = -2,
};

class RoomDelegate;
class RecordDelegate;

class TRTC_API Room {
 public:
  static Room* Create(const RoomParams& params, RoomDelegate* delegate);

  static void Destroy(Room* room);

  virtual void EnterRoom() = 0;

  virtual void ExitRoom() = 0;

  virtual const RoomParams& GetRoomParams() = 0;

 protected:
  virtual ~Room() {}
};

class TRTC_API Recorder {
 public:
  static Recorder* Create(Room* room, RecordDelegate* delegate);

  static void Destroy(Recorder* recorder);

  virtual void Start(const RecordParams& params) = 0;

  virtual void Stop() = 0;

  // 更新合流布局信息
  // |layouts| - 布局的数组
  // |layouts_count| - 数组的大小
  //
  // 全量更新
  //
  // 您可以通过 RoomDelegate::OnRemoteStreamAvailable() 获知房间中
  // 主播的变动情况，然后通过 UpdateLayout() 接口更新布局，UpdateLayout()
  // 支持以数组的形式指定每个用户的视图排版位置。
  // 本接口仅适用于 LayoutMode 为 KManual 的多路混流录制。
  virtual void UpdateLayout(const LayoutParams layouts[], size_t layouts_count) = 0;

  /**
   * 更新水印
   * |watermarks| - 水印信息
   * |watermark_count| - 水印个数
   *
   * 全量更新
   *
   * 混流录制生效，单流录制暂不支持
   */
  virtual void UpdateWatermark(const WatermarkConfig watermarks[], size_t watermark_count) = 0;

  virtual const RecordParams& GetRecordParams() = 0;

 protected:
  virtual ~Recorder() {}
};

// 房间相关回调
class TRTC_API RoomDelegate {
 public:
  // 进房成功通知
  virtual void OnEnterRoom(Room* room) = 0;
  // 退房成功通知
  virtual void OnExitRoom(Room* room) = 0;
  // 有新用户加入当前房间
  virtual void OnRemoteUserEnterRoom(Room* room, const TrtcString& remote_user_id) = 0;
  // 有用户离开当前房间
  virtual void OnRemoteUserLeaveRoom(Room* room, const TrtcString& remote_user_id) = 0;
  // 某远端用户发布了流
  // 远端用户开启/关闭、暂停/恢复流的上行时，都会触发此回调
  virtual void OnRemoteStreamAvailable(Room* room,
                                       const TrtcString& remote_user_id,
                                       StreamType type,
                                       bool available) = 0;
  // 房间错误回调
  virtual void OnRoomError(Room* room, Error error) = 0;

 protected:
  virtual ~RoomDelegate() {}
};

// 录制相关回调
class TRTC_API RecordDelegate {
 public:
  virtual void OnRecordStarted(Recorder* recorder) = 0;
  virtual void OnRecordProgress(Recorder* recorder, int file_size_bytes) = 0;
  virtual void OnRecordFinished(Recorder* recorder, const TrtcString& recorded_file_path) = 0;
  virtual void OnRecordError(Recorder* recorder, RecordError error) = 0;

 protected:
  virtual ~RecordDelegate() {}
};

}  // namespace trtc
}  // namespace liteav

#endif  // SDK_SERVER_INCLUDE_TRTC_LITEAV_TRTC_RECORDER_H_
