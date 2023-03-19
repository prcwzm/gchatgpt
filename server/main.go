package main

import (
	"fmt"
	"gchatgpt/chat"
	"gchatgpt/gws"
	"github.com/gin-gonic/gin"
	"net/http"
)

func main() {
	//client := openai.NewClient("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")
	client := chat.GetChatClient()

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

	err := r.Run(":8080")
	if err != nil {
		fmt.Println("Error:", err)
	}
}
