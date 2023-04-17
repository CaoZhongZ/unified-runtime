// Copyright (C) 2023 Intel Corporation
// SPDX-License-Identifier: MIT

#include <uur/fixtures.h>

struct urKernelCreateWithNativeHandleTest : uur::urKernelTest {
    void SetUp() override {
        UUR_RETURN_ON_FATAL_FAILURE(urKernelTest::SetUp());
        ASSERT_SUCCESS(urKernelGetNativeHandle(kernel, &native_kernel_handle));
    }

    void TearDown() override {
        if (native_kernel) {
            EXPECT_SUCCESS(urKernelRelease(native_kernel));
        }
        UUR_RETURN_ON_FATAL_FAILURE(urKernelTest::TearDown());
    }

    ur_native_handle_t native_kernel_handle = nullptr;
    ur_kernel_handle_t native_kernel = nullptr;
    ur_kernel_native_properties_t properties = {
        UR_STRUCTURE_TYPE_KERNEL_NATIVE_PROPERTIES, /*sType*/
        nullptr,                                    /*pNext*/
        true                                        /*isNativeHandleOwned*/
    };
};
UUR_INSTANTIATE_DEVICE_TEST_SUITE_P(urKernelCreateWithNativeHandleTest);

TEST_P(urKernelCreateWithNativeHandleTest, Success) {
    ASSERT_SUCCESS(urKernelCreateWithNativeHandle(
        native_kernel_handle, context, program, &properties, &native_kernel));

    uint32_t ref_count = 0;
    ASSERT_SUCCESS(urKernelGetInfo(native_kernel,
                                   UR_KERNEL_INFO_REFERENCE_COUNT,
                                   sizeof(ref_count), &ref_count, nullptr));

    ASSERT_NE(ref_count, 0);
}

TEST_P(urKernelCreateWithNativeHandleTest, InvalidNullHandleContext) {
    ASSERT_EQ_RESULT(
        UR_RESULT_ERROR_INVALID_NULL_HANDLE,
        urKernelCreateWithNativeHandle(native_kernel_handle, nullptr, program,
                                       &properties, &native_kernel));
}

TEST_P(urKernelCreateWithNativeHandleTest, InvalidNullHandleProgram) {
    ASSERT_EQ_RESULT(
        UR_RESULT_ERROR_INVALID_NULL_HANDLE,
        urKernelCreateWithNativeHandle(native_kernel_handle, context, nullptr,
                                       &properties, &native_kernel));
}

TEST_P(urKernelCreateWithNativeHandleTest, InvalidNullHandleNativeKernel) {
    ASSERT_EQ_RESULT(UR_RESULT_ERROR_INVALID_NULL_HANDLE,
                     urKernelCreateWithNativeHandle(nullptr, context, program,
                                                    &properties,
                                                    &native_kernel));
}

TEST_P(urKernelCreateWithNativeHandleTest, InvalidNullPointerProperties) {
    ASSERT_EQ_RESULT(UR_RESULT_ERROR_INVALID_NULL_HANDLE,
                     urKernelCreateWithNativeHandle(native_kernel_handle,
                                                    context, program, nullptr,
                                                    &native_kernel));
}

TEST_P(urKernelCreateWithNativeHandleTest, InvalidNullPointerNativeKernel) {
    ASSERT_EQ_RESULT(UR_RESULT_ERROR_INVALID_NULL_HANDLE,
                     urKernelCreateWithNativeHandle(native_kernel_handle,
                                                    context, program,
                                                    &properties, nullptr));
}
