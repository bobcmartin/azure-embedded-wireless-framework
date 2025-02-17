// Copyright (c) Microsoft Corporation. All rights reserved.

#include "ewf_platform.h"
#include "ewf_allocator_threadx.h"
#include "ewf_interface_win32_com.h"
#include "ewf_adapter_quectel_bg96.h"
#include "ewf_lib.h"
#include "ewf_example.config.h"

/* The thread */
static TX_THREAD thread;
static void thread_entry(ULONG thread_input);
#define THREAD_STACK_SIZE (1024 * 2)
static ULONG thread_stack[THREAD_STACK_SIZE / sizeof(ULONG)];

/**
 *  @brief The application entry point, initialize the hardware and start ThreadX
 */
int main(int argc, char ** argv)
{
    /* Enter the ThreadX kernel.  */
    tx_kernel_enter();

    /* We never get here, but we keep the compiler happy.  */
    return 0;
}

/**
 *  @brief Define what the initial ThreadX system looks like.
 */
void tx_application_define(void *first_unused_memory)
{
    UINT status;

    /* Create the example thread.  */
    status = tx_thread_create(
        &thread,
        "Thread",
        thread_entry, 0,
        thread_stack, THREAD_STACK_SIZE,
        1, 1,
        TX_NO_TIME_SLICE,
        TX_AUTO_START);
    if (status != TX_SUCCESS)
    {
        exit(status);
    }
}

/**
 *  @brief The thread entry point
 */
void thread_entry(ULONG param)
{
    ewf_result result;

    ewf_allocator* message_allocator_ptr = NULL;
    ewf_interface* interface_ptr = NULL;
    ewf_adapter* adapter_ptr = NULL;

    EWF_ALLOCATOR_THREADX_STATIC_DECLARE(message_allocator_ptr, message_allocator, 8, 1500);
    EWF_INTERFACE_WIN32_COM_STATIC_DECLARE(interface_ptr, com_port, 
        EWF_CONFIG_INTERFACE_WIN32_COM_PORT_FILE_NAME, 
        EWF_CONFIG_INTERFACE_WIN32_COM_PORT_BAUD_RATE,
        EWF_CONFIG_INTERFACE_WIN32_COM_PORT_BYTE_SIZE,
        EWF_CONFIG_INTERFACE_WIN32_COM_PORT_PARITY,
        EWF_CONFIG_INTERFACE_WIN32_COM_PORT_STOP_BITS,
        EWF_CONFIG_INTERFACE_WIN32_COM_PORT_BUFFER_SIZE);
    EWF_ADAPTER_QUECTEL_BG96_STATIC_DECLARE(adapter_ptr, quectel_bg96, message_allocator_ptr, NULL, interface_ptr);

    // Start the adapter
    if (ewf_result_failed(result = ewf_adapter_start(adapter_ptr)))
    {
        EWF_LOG_ERROR("Failed to start the adapter: return code 0x%08lx.", result);
        exit(result);
    }

    // Set the SIM PIN
    if (ewf_result_failed(result = ewf_adapter_modem_sim_pin_enter(adapter_ptr, EWF_CONFIG_SIM_PIN)))
    {
        EWF_LOG_ERROR("Failed to the SIM PIN: return code 0x%08lx.", result);
        exit(result);
    }

    // Call the certificate provisioning example
    if (ewf_result_failed(result = ewf_example_certs_basic_quectel_bg96(adapter_ptr)))
    {
        EWF_LOG_ERROR("The certificate provisioning example returned and error: return code 0x%08lx.", result);
        exit(result);
    }

    /* Wait forever  */
    while (1)
    {
        tx_thread_sleep(1);
    }
}
