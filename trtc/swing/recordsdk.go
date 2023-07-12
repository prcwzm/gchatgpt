package swing

// #cgo CFLAGS: -I ../include/live
// #cgo CFLAGS: -I ../include/trtc
// #cgo CXXFLAGS: -I ../include -std=c++11
// #cgo LDFLAGS: -L/Users/winnie/GolandProjects/gchatgpt/trtc/trtclibs/arm64 -lliteav -lz -ldl -lm
import "C"

const (
	recVersion   = "6.5.26"
	mixerVersion = "0.0.15"
)
