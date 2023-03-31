package chat

import (
	"context"
	"errors"
	"fmt"
	"gchatgpt/config"
	"github.com/sashabaranov/go-openai"
	"io"
	"net/http"
	"net/url"
)

func GetChatClient() *openai.Client {
	conf := openai.DefaultConfig(config.LoadKey("openai"))
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
			Model:     openai.GPT3Dot5Turbo0301,
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

func StreamChat(client *openai.Client, b []byte, dataSource chan string) {

	req := openai.ChatCompletionRequest{
		Model:     openai.GPT3Dot5Turbo0301,
		MaxTokens: 2048,
		Messages: []openai.ChatCompletionMessage{
			{
				Role:    openai.ChatMessageRoleUser,
				Content: string(b),
			},
		},
		Stream: true,
	}
	fmt.Println(req)
	streamSend(client, req, dataSource)
}

func StreamChatContent(client *openai.Client, b []byte, dataSource chan string, content []openai.ChatCompletionMessage, contentChan chan *openai.ChatCompletionMessage) []openai.ChatCompletionMessage {
	completionMessage := new(openai.ChatCompletionMessage)
	completionMessage = &openai.ChatCompletionMessage{Role: openai.ChatMessageRoleUser, Content: string(b)}
	content = StitchContent(content, completionMessage)
	requestContent := InterfaceStitch(content)
	req := openai.ChatCompletionRequest{
		Model:     openai.GPT3Dot5Turbo0301,
		MaxTokens: 2048,
		Messages:  requestContent,
		Stream:    true,
	}
	go streamSendContent(client, req, dataSource, contentChan)
	return content
}

func streamSendContent(client *openai.Client, req openai.ChatCompletionRequest, dataSource chan string, contentChan chan *openai.ChatCompletionMessage) {
	ctx := context.Background()
	stream, err := client.CreateChatCompletionStream(ctx, req)
	if err != nil {
		fmt.Printf("ChatCompletionStream error: %v\n", err)
	}
	defer stream.Close()
	responseContent := ""
	for {
		response, err := stream.Recv()
		if errors.Is(err, io.EOF) {
			fmt.Println("\nStream finished")
			dataSource <- "EOF"
			p := new(openai.ChatCompletionMessage)
			p = &openai.ChatCompletionMessage{Role: openai.ChatMessageRoleAssistant, Content: responseContent}
			contentChan <- p
			return
		}

		if err != nil {
			fmt.Printf("\nStream error: %v\n", err)
			dataSource <- "NETWORK_ERROR"
		}
		responseContent = responseContent + response.Choices[0].Delta.Content

		dataSource <- response.Choices[0].Delta.Content
	}
}
func streamSend(client *openai.Client, req openai.ChatCompletionRequest, dataSource chan string) {
	ctx := context.Background()
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
