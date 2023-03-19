package gws

import (
	"fmt"
	"gchatgpt/chat"
	"github.com/gin-gonic/gin"
	"github.com/gorilla/websocket"
	"github.com/sashabaranov/go-openai"
)

var upgrader = websocket.Upgrader{
	ReadBufferSize:  1024,
	WriteBufferSize: 1024,
}

func Ws(c *gin.Context, client *openai.Client) {
	conn, err := upgrader.Upgrade(c.Writer, c.Request, nil)
	if err != nil {
		fmt.Println("Error upgrading to WebSocket:", err)
		return
	}

	// 在此处处理 WebSocket 连接
	for {
		// 读取客户端发送的消息
		_, message, err := conn.ReadMessage()
		if err != nil {
			fmt.Println("Error reading message from WebSocket:", err)
			break
		}
		var answer string
		if string(message) == "exit" {
			answer = "欢迎再来"
			err = conn.WriteMessage(websocket.TextMessage, []byte(answer))
			if err != nil {
				fmt.Println("Error sending message:", err.Error())
				break
			}
			break
		} else {
			//answer := chat.Chat(client, message)
			dataSource := make(chan string)
			go chat.ChatStream(client, message, dataSource)
			for {
				answer = <-dataSource
				if answer == "EOF" {
					break
				} else {
					// 将消息返回给客户端
					err = conn.WriteMessage(websocket.TextMessage, []byte(answer))
				}
			}

		}
		if err != nil {
			fmt.Println("Error writing message to WebSocket:", err)
			break
		}
	}

	// 关闭 WebSocket 连接
	if err := conn.Close(); err != nil {
		fmt.Println("Error closing WebSocket connection:", err)
	}
}
