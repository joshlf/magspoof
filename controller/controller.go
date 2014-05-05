package main

import (
	"errors"
	"fmt"
	"io"
	"os"
)

func main() {
	if len(os.Args) != 3 {
		fmt.Fprintf(os.Stderr, "%v <device> <data>\n", os.Args[0])
		os.Exit(1)
	}

	f, err := os.Open(os.Args[1])
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error opening device: %v\n", err)
		os.Exit(2)
	}

	err = sendBytes(f, []byte(os.Args[2]))
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
	b[80] = byte(len(buf))
	_, err := w.Write(b[:])
	return err
}
