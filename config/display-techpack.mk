include hardware/qcom/display/config/display-modules.mk
include vendor/qcom/opensource/mm-drivers/mm_driver_product.mk
include vendor/qcom/opensource/display-drivers/display_driver_product.mk

ifneq (,$(wildcard $(QCPATH)/display))
include $(QCPATH)/display/config/display-vendor-modules.mk
endif

.PHONY: display_tp display_tp_hal display_tp_dlkm

display_tp: display_tp_hal display_tp_dlkm

display_tp_hal: $(DISPLAY_MODULES_HARDWARE) $(DISPLAY_MODULES_VENDOR)

display_tp_dlkm: $(DISPLAY_MODULES_DRIVER) $(DISPLAY_MM_DRIVER)

$(warning "Display Techpack configuration TARGET_USES_QMAA  = $(TARGET_USES_QMAA)")
$(warning "Display Techpack configuration TARGET_USES_QMAA_OVERRIDE_DISPLAY  = $(TARGET_USES_QMAA_OVERRIDE_DISPLAY)")
$(warning "Display Techpack configuration TARGET_IS_HEADLESS  = $(TARGET_IS_HEADLESS)")