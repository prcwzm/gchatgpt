package main

import (
	"fmt"
	"gchatgpt/chat"
	"gchatgpt/gws"
	"github.com/gin-gonic/gin"
	"net/http"
)

/*
#cgo CXXFLAGS: -std=c++11 -stdlib=libc++
#cgo CPPFLAGS: -I /usr/local/include -I /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/14.0.3/include -I /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include -I /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -I /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/
#cgo CFLAGS: -I /Users/wuziming/GolandProjects/gchatgpt/trtc/include/trtc/
#cgo LDFLAGS: -L/Users/wuziming/GolandProjects/gchatgpt/trtc/trtclibs/x86_64 -lliteav -lc++ -lstdc++
#include <liteav_trtc_cloud.h>
#include <liteav_trtc_defines.h>
#include <liteav_trtc_recorder.h>
*/
import "C"

func main() {
	//client := openai.NewClient("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")
	client := chat.GetChatClient()
	var codec C.enum_AudioCodecType
	codec = C.AUDIO_CODEC_TYPE_OPUS
	fmt.Println(codec)
	r := gin.Default()
	r.LoadHTMLGlob("static/*")
	// 处理 HTTP 请求
	r.GET("/st", func(c *gin.Context) {
		c.HTML(http.StatusOK, "index.html", nil)
	})

	r.GET("/bt", func(c *gin.Context) {
		c.HTML(http.StatusOK, "beautify.html", nil)
	})

	// 处理 WebSocket 请求
	r.GET("/ws", func(c *gin.Context) {
		gws.Ws(c, client)
	})

	// 静态文件处理
	r.Static("/static", "./static")

	err := r.Run("0.0.0.0:8080")
	if err != nil {
		fmt.Println("Error:", err)
	}
}
