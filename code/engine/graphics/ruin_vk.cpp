#include "ruin_vk.h"
#include "ruin_file.h"

#include "vulkan.h"
#include <cstdio>

using namespace Ruin;

void initVulkan()
{
//#if defined(VK_USE_PLATFORM_WIN32_KHR)
    rcLibrary* VulkanLibrary = rLoadLibrary("vulkan-1.dll");
//#elif defined(VK_USE_PLATFORM_XCB_KHR) || defined(VK_USE_PLATFORM_XLIB_KHR)
//   rcLibrary VulkanLibrary = rLoadLibrary("libvulkan.so");
//#endif

    if (!VulkanLibrary)
    {
        printf("Could not load Vulkan Library!\n");
    }
}