// Copyright (c) 2021 Tencent. All rights reserved.

#ifndef SDK_SERVER_INCLUDE_LIVE_LITEAV_LIVE_PREMIER_H_
#define SDK_SERVER_INCLUDE_LIVE_LITEAV_LIVE_PREMIER_H_

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

namespace liteav {
namespace live {

class TRTC_API TXLiveSDK {
 public:
  // 设置日志目录路径
  //
  // |path| - 期望存放日志的路径
  //
  // 返回值
  // - true : 成功
  // - false : 失败
  //
  // 若未设置，日志路径默认是 /data/home/liteav.
  // 请于 所有不在 TXLiveSDK::XXX 下的接口之前调用。运行中途请不要调用本接口。
  static bool SetLogPath(const char* path);

  // 设置日志级别
  // |level| - 0 : 输出 debug, info, warning, error 和 fatal 级别的日志
  //           1 : 输出 info, warning, error 和 fatal 级别的日志
  //           2 : 输出 warning, error 和 fatal 级别的日志
  //           3 : 输出 error 和 fatal 级别的日志
  //           4 : 输出 fatal 级别的日志
  //           5 : 不输出日志
  // 注： 本地日志文件和控制台日志同时生效。
  static void SetLogLevel(int level);

  // 开启控制台日志 - 默认开启
  static void EnableConsoleLog();

  // 关闭控制台日志
  static void DisableConsoleLog();

  // 设置 Socks5 代理
  //
  // |ip| 代理服务的 ip地址 比如 “10.1.2.3”
  // |port| 代理服务的端口
  // |user_name| 代理服务的用户名，传 "" 或 nullptr 表示没有用户名
  // |password| 代理服务的密码，传 "" 或 nullptr 表示没有密码
  //
  // 请于 所有不在 TXLiveSDK::XXX 下的接口之前调用。运行中途请不要调用本接口。
  static bool SetSocks5Proxy(const char* ip,
                             unsigned short port,
                             const char* username,
                             const char* password);

  // 设置 SDK 接入环境
  //
  // |env| - 目前仅支持 default
  //   - default: 默认环境，全球最优接入
  //   - pstn: 公共电话系统专有环境（仅腾讯内部使用）
  //   - dev: 测试开发环境（仅腾讯内部使用）
  //   - {json}: 私有化环境、通用代理设置，采用 json 格式。
  /*     私有化环境设置：
          {
              "trtc_env":{
                  "access_ip":["ip1", "ip2"],
                  "access_host":"",
                  "anycast_domain":"",
                  "access_public_key":"",
                  "end_report":"",
                  "speed_test":{
                      "req_url":"",
                      "report_url":""
                  }
              },
              "log_env":{
                  "sign_url":"",
                  "cos_url":"",
                  "cos_upload_type":0(v4上传方式) or 1(v5上传方式),
                  "report_url":""
              },
              "server_config":{
                  "path":"",
                  "dec_key":""
              },
              "kibana_env":{
                  "domain":"",
                  "anycast_ip":"",
                  "access_ip":["ip1", "ip2"]
              }
          }
         通用代理设置：
          {
              "proxy_env":{
                  "domain":"",
                  "sdk_appid":number_id
              }
          }
  */
  // 请于 所有不在 TXLiveSDK::XXX 下的接口之前调用。运行中途请不要调用本接口。
  static bool SetEnvironment(const char* env);

 protected:
  virtual ~TXLiveSDK() {}
};
}  // namespace live
}  // namespace liteav

#endif  // SDK_SERVER_INCLUDE_LIVE_LITEAV_LIVE_PREMIER_H_
