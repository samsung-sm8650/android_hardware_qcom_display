/*
* Copyright (c) 2016 - 2018, 2020 The Linux Foundation. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are
* met:
*  * Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*  * Redistributions in binary form must reproduce the above
*    copyright notice, this list of conditions and the following
*    disclaimer in the documentation and/or other materials provided
*    with the distribution.
*  * Neither the name of The Linux Foundation nor the names of its
*    contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
* ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * Changes from Qualcomm Innovation Center are provided under the following license:
 *
 * Copyright (c) 2022 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#ifndef __HWC_TONEMAPPER_H__
#define __HWC_TONEMAPPER_H__

#include <fcntl.h>
#include <sys/mman.h>

#include <core/layer_stack.h>
#include <utils/sys.h>
#include <utils/sync_task.h>
#include <vector>
#include "hwc_buffer_sync_handler.h"
#include "hwc_buffer_allocator.h"

class Tonemapper;

namespace sdm {

enum class ToneMapTaskCode : int32_t {
  kCodeGetInstance,
  kCodeBlit,
  kCodeDestroy,
};

struct ToneMapGetInstanceContext : public SyncTask<ToneMapTaskCode>::TaskContext {
  Layer *layer = nullptr;
};

struct ToneMapBlitContext : public SyncTask<ToneMapTaskCode>::TaskContext {
  Layer *layer = nullptr;
  shared_ptr<Fence> merged = nullptr;
  shared_ptr<Fence> fence = nullptr;
};

struct ToneMapConfig {
  int type = 0;
  PrimariesTransfer blend_cs = {};
  GammaTransfer transfer = Transfer_Max;
  LayerBufferFormat format = kFormatRGBA8888;
  bool secure = false;
};

class ToneMapSession : public SyncTask<ToneMapTaskCode>::TaskHandler {
 public:
  explicit ToneMapSession(HWCBufferAllocator *buffer_allocator);
  ~ToneMapSession();
  DisplayError AllocateIntermediateBuffers(const Layer *layer);
  void FreeIntermediateBuffers();
  void UpdateBuffer(const shared_ptr<Fence> &acquire_fence, LayerBuffer *buffer);
  void SetReleaseFence(const shared_ptr<Fence> &fd);
  void SetToneMapConfig(Layer *layer, PrimariesTransfer blend_cs);
  bool IsSameToneMapConfig(Layer *layer, PrimariesTransfer blend_cs);

  // TaskHandler methods implementation.
  virtual void OnTask(const ToneMapTaskCode &task_code,
                      SyncTask<ToneMapTaskCode>::TaskContext *task_context);

  static const uint8_t kNumIntermediateBuffers = 2;
  SyncTask<ToneMapTaskCode> tone_map_task_;
  Tonemapper *gpu_tone_mapper_ = nullptr;
  HWCBufferAllocator *buffer_allocator_ = nullptr;
  ToneMapConfig tone_map_config_ = {};
  uint8_t current_buffer_index_ = 0;
  std::vector<BufferInfo> buffer_info_ = {};
  shared_ptr<Fence> release_fence_[kNumIntermediateBuffers] = {nullptr, nullptr};
  bool acquired_ = false;
  int layer_index_ = -1;
};

class HWCToneMapper {
 public:
  explicit HWCToneMapper(HWCBufferAllocator *allocator) : buffer_allocator_(allocator) {}
  ~HWCToneMapper() {}

  int HandleToneMap(LayerStack *layer_stack);
  bool IsActive() { return !tone_map_sessions_.empty(); }
  void PostCommit(LayerStack *layer_stack);
  void SetFrameDumpConfig(uint32_t count);
  void Terminate();

 private:
  void ToneMap(Layer *layer, ToneMapSession *session);
  DisplayError AcquireToneMapSession(Layer *layer, uint32_t *sess_idx, PrimariesTransfer blend_cs);
  void DumpToneMapOutput(ToneMapSession *session, shared_ptr<sdm::Fence> acquire_fence);

  std::vector<ToneMapSession *> tone_map_sessions_;
  HWCBufferAllocator *buffer_allocator_ = nullptr;
  uint32_t dump_frame_count_ = 0;
  uint32_t dump_frame_index_ = 0;
  int fb_session_index_ = -1;
};

}  // namespace sdm
#endif  // __HWC_TONEMAPPER_H__
