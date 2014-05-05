package main

import (
	"errors"
	"fmt"
	"io"
	"os"
	"strconv"

	"github.com/huin/goserial"
)

func main() {
	if len(os.Args) != 4 {
		fmt.Fprintf(os.Stderr, "%v <device> <baud> <data>\n", os.Args[0])
		os.Exit(1)
	}

	baud, err := strconv.Atoi(os.Args[2])
	if err != nil {
		fmt.Fprintf(os.Stderr, "baud must be numerical\n")
		os.Exit(1)
	}

	c := &goserial.Config{Name: os.Args[1], Baud: baud}
	s, err := goserial.OpenPort(c)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error connecting to device: %v\n", err)
		os.Exit(2)
	}

	err = sendBytes(s, []byte(os.Args[3]))
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error writing data: %v\n", err)
		os.Exit(3)
	}
}

func sendBytes(w io.Writer, buf []byte) error {
	var b [81]byte

	if len(buf) > 80 {
		return errors.New("Data longer than 80 bytes")
	}

	copy(b[:], buf)
	_, err := w.Write(b[:len(buf)+1])
	return err
}
