/*
 * Copyright (c) 2011-2016,2018-2021, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of The Linux Foundation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
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
 *
 */

/*
 *
 * Changes from Qualcomm Innovation Center are provided under the following license:
 * Copyright (c) 2022-2023 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#ifndef __GR_UTILS_H__
#define __GR_UTILS_H__

#include <hardware/gralloc.h>
#include <QtiGralloc.h>
#include <QtiGrallocPriv.h>
#include <QtiGrallocDefs.h>
#include <utils/debug.h>
#include <android/hardware/graphics/common/1.2/types.h>
#include <aidl/android/hardware/graphics/common/PixelFormat.h>
#include <gralloctypes/Gralloc4.h>
#include <limits>
#include <vector>

#define SZ_2M 0x200000
#define SZ_1M 0x100000
#define SZ_4K 0x1000

#define SIZE_4K 4096
#define SIZE_8K 8192

#ifdef SLAVE_SIDE_CP
#define SECURE_ALIGN SZ_1M
#else  // MASTER_SIDE_CP
#define SECURE_ALIGN SZ_4K
#endif

#define INT(exp) static_cast<int>(exp)
#define UINT(exp) static_cast<unsigned int>(exp)

#define OVERFLOW(x, y) (((y) != 0) && ((x) > (UINT_MAX / (y))))

#define ROUND_UP_PAGESIZE(x) roundUpToPageSize(x)
inline size_t roundUpToPageSize(size_t x) {
  return (x + (getpagesize() - 1)) & ~(getpagesize() - 1);
}

using aidl::android::hardware::graphics::common::StandardMetadataType;
using android::hardware::graphics::common::V1_2::PixelFormat;
using android::hardware::graphics::common::V1_1::BufferUsage;
using private_handle_t = qtigralloc::private_handle_t;

namespace gralloc {
struct BufferInfo {
  BufferInfo(int w, int h, int f, uint64_t usage = 0)
      : width(w), height(h), format(f), layer_count(1), usage(usage) {}
  int width;
  int height;
  int format;
  int layer_count;
  uint64_t usage;
};

struct GrallocProperties {
  bool use_system_heap_for_sensors = true;
  bool ubwc_disable = false;
  bool ahardware_buffer_disable = false;
};

template <class Type1, class Type2>
inline Type1 ALIGN(Type1 x, Type2 align) {
  Type1 max_val = std::numeric_limits<Type1>::max();
  if (x > (max_val - (Type1)align)) {
    return x;
  }

  return (Type1)((x + (Type1)align - 1) & ~((Type1)align - 1));
}

enum class Error : int32_t {
  /**
   * No error.
   */
  NONE = 0,
  /**
   * Invalid BufferDescriptor.
   */
  BAD_DESCRIPTOR = 1,
  /**
   * Invalid buffer handle.
   */
  BAD_BUFFER = 2,
  /**
   * Invalid HardwareBufferDescription.
   */
  BAD_VALUE = 3,
  /**
   * Resource unavailable.
   */
  NO_RESOURCES = 5,
  /**
   * Permanent failure.
   */
  UNSUPPORTED = 7,
};

enum PlaneComponent {
  /* luma */
  PLANE_COMPONENT_Y = 1 << 0,
  /* chroma blue */
  PLANE_COMPONENT_Cb = 1 << 1,
  /* chroma red */
  PLANE_COMPONENT_Cr = 1 << 2,

  /* red */
  PLANE_COMPONENT_R = 1 << 10,
  /* green */
  PLANE_COMPONENT_G = 1 << 11,
  /* blue */
  PLANE_COMPONENT_B = 1 << 12,

  /* alpha */
  PLANE_COMPONENT_A = 1 << 20,

  /* raw data plane */
  PLANE_COMPONENT_RAW = 1 << 30,

  /* meta information plane */
  PLANE_COMPONENT_META = 1 << 31,
};

/* Flag to determine interlaced content
  * Value maps to Flags presents in types.hal of QtiMapperextensions
  */
enum {
  LAYOUT_INTERLACED_FLAG = 1 << 0,
};

struct PlaneLayoutInfo {
  /** Components represented the type of plane. */
  PlaneComponent component;

  /** horizontal subsampling. Must be a positive power of 2. */
  uint32_t h_subsampling;

  /** vertical subsampling. Must be a positive power of 2. */
  uint32_t v_subsampling;

  /** offset to the first byte of the top-left pixel of the plane
   *  and it is calculated from the start of the buffer.
   *  Add base of the handle with offset to get the first byte of the plane.
   */
  uint32_t offset;

  /** step is the distance in bytes from one pixel value to the next. */
  int32_t step;

  /** stride of the plane in pixels */
  int32_t stride;

  /** stride of the plane in in bytes */
  int32_t stride_bytes;

  /** plane height or vertical stride */
  int32_t scanlines;

  /** size of the plane in bytes */
  uint32_t size;
};

bool IsYuvFormat(int format);
bool IsCompressedRGBFormat(int format);
bool IsUncompressedRGBFormat(int format);
bool IsGpuDepthStencilFormat(int format);
uint32_t GetBppForUncompressedRGB(int format);
bool CpuCanAccess(uint64_t usage);
bool CpuCanRead(uint64_t usage);
bool CpuCanWrite(uint64_t usage);
int GetBpp(int format);
unsigned int GetSize(const BufferInfo &d, unsigned int alignedw, unsigned int alignedh);
int GetBufferSizeAndDimensions(const BufferInfo &d, unsigned int *size, unsigned int *alignedw,
                               unsigned int *alignedh);
int GetBufferSizeAndDimensions(const BufferInfo &d, unsigned int *size, unsigned int *alignedw,
                               unsigned int *alignedh, GraphicsMetadata *graphics_metadata);
int GetCustomDimensions(private_handle_t *hnd, int *stride, int *height);
void GetColorSpaceFromMetadata(private_handle_t *hnd, int *color_space);
int GetAlignedWidthAndHeight(const BufferInfo &d, unsigned int *aligned_w,
                              unsigned int *aligned_h);
int GetYUVPlaneInfo(const private_handle_t *hnd, struct android_ycbcr ycbcr[2]);
int GetYUVPlaneInfo(const BufferInfo &info, int32_t format, int32_t width, int32_t height,
                    int32_t interlaced, int *plane_count, PlaneLayoutInfo plane_info[8],
                    const private_handle_t *hnd = nullptr, struct android_ycbcr *ycbcr = nullptr);
void GetRGBPlaneInfo(const BufferInfo &info, int32_t format, int32_t width, int32_t height,
                     int32_t flags, int *plane_count, PlaneLayoutInfo *plane_info);
unsigned int GetRgbMetaSize(int format, uint32_t width, uint32_t height, uint64_t usage);
void GetYuvSubSamplingFactor(int32_t format, int *h_subsampling, int *v_subsampling);
void CopyPlaneLayoutInfotoAndroidYcbcr(uint64_t base, int plane_count, PlaneLayoutInfo *plane_info,
                                       struct android_ycbcr *ycbcr);
int GetRgbDataAddress(private_handle_t *hnd, void **rgb_data);
bool IsUBwcFormat(int format);
bool IsUBwcSupported(int format);
bool IsTileRendered(int format);
bool IsOnlyGpuUsage(uint64_t usage);
bool IsUBwcPISupported(int format, uint64_t usage);
bool IsUBwcEnabled(int format, uint64_t usage);
bool IsUBwcPEnabled(int format, uint64_t usage);
bool IsUBwcPFormat(int format);
void GetYuvUBwcPWidthAndHeight(int width, int height, int format, unsigned int *aligned_w,
                               unsigned int *aligned_h);
unsigned int GetLinearSizeUBWCP(const BufferInfo &);
bool IsCameraCustomFormat(int format, uint64_t usage);
void GetYuvUBwcWidthAndHeight(int width, int height, int format, unsigned int *aligned_w,
                              unsigned int *aligned_h);
void GetYuvSPPlaneInfo(const BufferInfo &info, int format, uint32_t width, uint32_t height,
                       uint32_t bpp, PlaneLayoutInfo *plane_info);
void GetYuvUbwcSPPlaneInfo(uint32_t width, uint32_t height, int color_format,
                           PlaneLayoutInfo *plane_info);
void GetYuvUbwcInterlacedSPPlaneInfo(uint32_t width, uint32_t height,
                                     PlaneLayoutInfo plane_info[8]);
void GetRgbUBwcBlockSize(uint32_t bpp, int *block_width, int *block_height);
unsigned int GetRgbUBwcMetaBufferSize(int width, int height, uint32_t bpp);
unsigned int GetUBwcSize(int width, int height, int format, unsigned int alignedw,
                         unsigned int alignedh);
int GetBufferLayout(private_handle_t *hnd, uint32_t stride[4], uint32_t offset[4],
                    uint32_t *num_planes);
uint32_t GetDataAlignment(int format, uint64_t usage);
int GetGpuResourceSizeAndDimensions(const BufferInfo &info, unsigned int *size,
                                    unsigned int *alignedw, unsigned int *alignedh,
                                    GraphicsMetadata *graphics_metadata);
bool CanUseAdrenoForSize(int buffer_type, uint64_t usage);
bool GetAdrenoSizeAPIStatus();
bool UseUncached(int format, uint64_t usage);
uint64_t GetHandleFlags(int format, uint64_t usage);
int GetImplDefinedFormat(uint64_t usage, int format);
int GetCustomFormatFlags(int format, uint64_t usage, int *custom_format, uint64_t *priv_flags);
int GetBufferType(int inputFormat);
bool IsGPUFlagSupported(uint64_t usage);
bool HasAlphaComponent(int32_t format);
bool isTargetSupportUBwcP();

void GetDRMFormat(uint32_t format, uint32_t flags, uint32_t *drm_format,
                  uint64_t *drm_format_modifier);
bool CanAllocateZSLForSecureCamera();

uint64_t GetCustomContentMetadataSize(int format, uint64_t usage);
uint64_t GetMetaDataSize(uint64_t reserved_region_size, uint64_t custom_content_md_region_size = 0);
void UnmapAndReset(private_handle_t *handle);
int ValidateAndMap(private_handle_t *handle);
Error GetColorSpaceFromColorMetaData(ColorMetaData color_metadata, uint32_t *color_space);
Error GetMetaDataByReference(void *buffer, int64_t type, void **out);
Error GetMetaDataValue(void *buffer, int64_t type, void *in);
Error GetMetaDataInternal(void *buffer, int64_t type, void *in, void **out);
Error ColorMetadataToDataspace(ColorMetaData color_metadata,
                               aidl::android::hardware::graphics::common::Dataspace *dataspace);
Error GetPlaneLayout(private_handle_t *handle,
                     std::vector<aidl::android::hardware::graphics::common::PlaneLayout> *out);
Error SetMetaData(private_handle_t *handle, uint64_t paramType, void *param);
}  // namespace gralloc

#endif  // __GR_UTILS_H__
