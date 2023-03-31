package chat

import (
	"github.com/sashabaranov/go-openai"
)

func StitchContent(contentMessage []openai.ChatCompletionMessage, completionMessage *openai.ChatCompletionMessage) []openai.ChatCompletionMessage {
	nowMessage := completionMessage
	size := sizeOfContent(contentMessage)
	if size == 0 {
		contentMessage[0] = *nowMessage
	} else if size == len(contentMessage) {
		i := 0
		for ; i < size-2; i++ {
			contentMessage[i] = contentMessage[i+1]
		}
		contentMessage[i] = *nowMessage
	} else {
		contentMessage[size] = *nowMessage
	}

	return contentMessage
}

func InterfaceStitch(contentMessage []openai.ChatCompletionMessage) []openai.ChatCompletionMessage {
	content := make([]openai.ChatCompletionMessage, sizeOfContent(contentMessage))
	for i := 0; i < sizeOfContent(contentMessage); i++ {
		content[i] = contentMessage[i]
	}
	return content
}

func sizeOfContent(message []openai.ChatCompletionMessage) int {
	i := 0
	for i < len(message) {
		if message[i].Role != "" {
			i++
		} else {
			break
		}
	}
	return i
}
