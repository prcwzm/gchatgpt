package main

import (
	"fmt"
	"gchatgpt/chat"
	"gchatgpt/gws"
	"gchatgpt/trtc/swing"
	"github.com/gin-gonic/gin"
	"net/http"
)

import "C"

func main() {
	//client := openai.NewClient("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")
	client := chat.GetChatClient()
	swing.Create()
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
