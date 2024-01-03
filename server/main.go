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

	r.POST("/fine-tune", func(c *gin.Context) {
		fineTunedText := c.PostForm("text")
		// 在这里处理 fine-tuned 文本，可以调用 chat.GetCompletion() 等方法进行处理
		// 示例：response := chat.GetCompletion(client, fineTunedText)
		c.JSON(http.StatusOK, gin.H{
			"message": "Fine-tuned text received and processed.",
			"text":    fineTunedText,
			// 可根据需要添加其他返回信息
		})
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
