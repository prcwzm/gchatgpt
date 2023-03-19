package chat

import (
	"context"
	"errors"
	"fmt"
	"github.com/sashabaranov/go-openai"
	"io"
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

func ChatStream(client *openai.Client, b []byte, dataSource chan string) {
	ctx := context.Background()
	req := openai.ChatCompletionRequest{
		Model:     openai.GPT3Dot5Turbo,
		MaxTokens: 20,
		Messages: []openai.ChatCompletionMessage{
			{
				Role:    openai.ChatMessageRoleUser,
				Content: string(b),
			},
		},
		Stream: true,
	}

	stream, err := client.CreateChatCompletionStream(ctx, req)
	if err != nil {
		fmt.Printf("ChatCompletionStream error: %v\n", err)
	}
	defer stream.Close()

	for {
		response, err := stream.Recv()
		if errors.Is(err, io.EOF) {
			fmt.Println("\nStream finished")
			dataSource <- "EOF"
		}

		if err != nil {
			fmt.Printf("\nStream error: %v\n", err)
			dataSource <- "NETWORK_ERROR"
		}

		dataSource <- response.Choices[0].Delta.Content
	}
}
