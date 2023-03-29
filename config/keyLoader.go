package config

import (
	"fmt"
	"io"
	"os"
)

func LoadKey(key string) string {
	file, err := os.Open(key + ".key")
	if err != nil {
		fmt.Println("无法打开文件:", err)
		return ""
	}
	defer func(file *os.File) {
		err := file.Close()
		if err != nil {
			fmt.Println("errors:", err)
		}
	}(file)

	// 读取文件内容
	contents, err := io.ReadAll(file)
	if err != nil {
		fmt.Println("无法读取文件:", err)
		return ""
	}

	// 输出文件内容
	fmt.Println("文件内容:")
	fmt.Println(string(contents))
	return string(contents)
}
