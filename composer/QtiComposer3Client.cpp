/*
 * Copyright (c) 2023 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include "QtiComposer3Client.h"
#include "AidlComposerClient.h"
#include "android/binder_status.h"
#include "android/binder_auto_utils.h"
#include <android/binder_ibinder_platform.h>

using ::aidl::android::hardware::common::NativeHandle;
using sdm::Locker;

namespace aidl {
namespace vendor {
namespace qti {
namespace hardware {
namespace display {
namespace composer3 {

QtiComposer3Client::QtiComposer3Client() : hwc_session_(HWCSession::GetInstance()) {
  if (!hwc_session_) {
    ALOGE("%s: hwc_session:%p is invalid", __FUNCTION__, hwc_session_);
  }
}

ScopedAStatus QtiComposer3Client::init(const std::weak_ptr<AidlComposerClient> &composer_client) {
  composer_client_ = composer_client;
  return ScopedAStatus::ok();
}

ScopedAStatus QtiComposer3Client::qtiExecuteCommands(
    const std::vector<DisplayCommand> &in_commands,
    const std::vector<QtiDisplayCommand> &in_qtiCommands,
    std::vector<CommandResultPayload> *_aidl_return) {
  std::vector<CommandResultPayload> qti_results;

  auto composer_client = composer_client_.lock();

  if (composer_client) {
    auto qti_status = composer_client->executeQtiCommands(in_qtiCommands, &qti_results);
    auto status = composer_client->executeCommands(in_commands, _aidl_return);

    for (auto &result : qti_results) {
      _aidl_return->push_back(std::move(result));
    }

    return (!qti_status.isOk() ? std::move(qti_status) : std::move(status));
  }
  return TO_BINDER_STATUS(INT32(Error::NoResources));
}

ScopedAStatus QtiComposer3Client::qtiTryDrawMethod(int64_t in_display,
                                                   QtiDrawMethod in_drawMethod) {
  if (hwc_session_) {
    auto error = hwc_session_->TryDrawMethod(in_display, in_drawMethod);
    return TO_BINDER_STATUS(INT32(error));
  }
  return ScopedAStatus::ok();
}

SpAIBinder QtiComposer3Client::createBinder() {
  auto binder = BnQtiComposer3Client::createBinder();
  AIBinder_setInheritRt(binder.get(), true);
  return binder;
}

}  // namespace composer3
}  // namespace display
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
}  // namespace aidl
