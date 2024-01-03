package fineTune

import (
	"fmt"
	"github.com/gin-gonic/gin"
	"github.com/sashabaranov/go-openai"
)

func FineTune(client *openai.Client, ctx *gin.Context) {
	file, err := client.CreateFile(ctx, openai.FileRequest{
		FilePath: "training_prepared.jsonl",
		Purpose:  "fine-tune",
	})
	if err != nil {
		fmt.Printf("Upload JSONL file error: %v\n", err)
		return
	}

	tune, err := client.CreateFineTune(ctx, openai.FineTuneRequest{
		TrainingFile: file.ID,
		Model:        "ada", // babbage, curie, davinci, or a fine-tuned model created after 2022-04-21.
	})

	if err != nil {
		fmt.Printf("Creating new fine tune model error: %v\n", err)
		return
	}

	getTune, err := client.GetFineTune(ctx, tune.ID)
	if err != nil {
		fmt.Printf("Getting fine tune model error: %v\n", err)
		return
	}
	fmt.Println(getTune.FineTunedModel)

}
