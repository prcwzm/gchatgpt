package chat

import (
	"context"
	"fmt"
	"github.com/sashabaranov/go-openai"
	"net/http"
	"net/url"
)

func GetChatClient() *openai.Client {
	conf := openai.DefaultConfig("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")
	proxyAddress, _ := url.Parse("http://localhost:33210")
	//proxyAddress, _ := url.Parse("https://openapi.ssiic.com")
	conf.HTTPClient = &http.Client{
		Transport: &http.Transport{
			Proxy: http.ProxyURL(proxyAddress),
		},
	}

	return openai.NewClientWithConfig(conf)
}

func Chat(client *openai.Client, b []byte) string {
	resp, err := client.CreateChatCompletion(
		context.Background(),
		openai.ChatCompletionRequest{
			Model:     openai.GPT3Dot5Turbo,
			MaxTokens: 2048,
			Messages: []openai.ChatCompletionMessage{
				{
					Role:    openai.ChatMessageRoleUser,
					Content: string(b),
				},
			},
		},
	)

	if err != nil {
		fmt.Println("Error creating chat completion :", err)
		return ""
	}
	return resp.Choices[0].Message.Content

}
