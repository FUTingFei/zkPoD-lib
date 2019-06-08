package plain

/*
#cgo CFLAGS: -I../..
#cgo LDFLAGS: -lpod_core

#include <stdlib.h>
#include "pod_core/capi/c_api.h"
*/
import "C"

import (
	"fmt"
	"unsafe"

	"github.com/sec-bit/zkPoD-lib/pod-go/types"
	"github.com/sec-bit/zkPoD-lib/pod-go/utils"
)

type (
	A struct {
		handle types.CHandle
	}

	B struct {
		handle types.CHandle
	}
)

// NewA provides the Go interface for E_PlainANew()
//
// XXX: ecc.Load() must have been called.
func NewA(publishPath string) (*A, error) {
	if err := utils.CheckDirReadPerm(publishPath); err != nil {
		return nil, err
	}

	publishPathCStr := C.CString(publishPath)
	defer C.free(unsafe.Pointer(publishPathCStr))

	handle := types.CHandle(C.E_PlainANew(publishPathCStr))
	if handle == nil {
		return nil, fmt.Errorf(
			"E_PlainANew(%s) failed", publishPath)
	}

	return &A{handle: handle}, nil
}

func (a *A) CHandle() types.CHandle {
	return a.handle
}

// Free provides the Go interface for E_PlainAFree()
func (a *A) Free() error {
	ret := bool(C.E_PlainAFree(C.handle_t(a.handle)))
	if !ret {
		return fmt.Errorf("E_PlainAFree(%v) failed", a.handle)
	}
	return nil
}

// NewB provides the Go interface for E_PlainBNew()
//
// XXX: ecc.Load() must have been called.
func NewB(bulletinFile, publicPath string) (*B, error) {
	if err := utils.CheckRegularFileReadPerm(bulletinFile); err != nil {
		return nil, err
	}
	if err := utils.CheckDirReadPerm(publicPath); err != nil {
		return nil, err
	}

	bulletinFileCStr := C.CString(bulletinFile)
	defer C.free(unsafe.Pointer(bulletinFileCStr))

	publicPathCStr := C.CString(publicPath)
	defer C.free(unsafe.Pointer(publicPathCStr))

	handle := types.CHandle(C.E_PlainBNew(bulletinFileCStr, publicPathCStr))
	if handle == nil {
		return nil, fmt.Errorf(
			"E_PlainBNew(%s, %s) failed", bulletinFile, publicPath)
	}

	return &B{handle: handle}, nil
}

func (b *B) CHandle() types.CHandle {
	return b.handle
}

// Free provides the Go interface for E_PlainBFree()
func (b *B) Free() error {
	ret := bool(C.E_PlainBFree(C.handle_t(b.handle)))
	if !ret {
		return fmt.Errorf("E_PlainBFree(%v) failed", b.handle)
	}
	return nil
}
