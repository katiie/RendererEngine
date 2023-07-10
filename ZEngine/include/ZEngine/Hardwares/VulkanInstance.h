#pragma once
#include <string>
#include <vulkan/vulkan.h>
#include <Hardwares/VulkanLayer.h>
#include <Hardwares/VulkanDevice.h>

namespace ZEngine::Hardwares
{
    class VulkanInstance
    {
    public:
        VulkanInstance(std::string_view app_name);
        ~VulkanInstance();

        void CreateInstance(const std::vector<const char*>& additional_extension_layer_name_collection);

        void ConfigureDevices(const VkSurfaceKHR* surface = nullptr);

        const std::vector<VulkanDevice>& GetDevices() const;
        const VulkanDevice&              GetHighPerformantDevice() const;
        const VkInstance                 GetNativeHandle() const;

        VulkanDevice& GetHighPerformantDevice();

    private:
        std::string                m_application_name;
        VulkanLayer                m_layer;
        VkInstance                 m_vulkan_instance{VK_NULL_HANDLE};
        VkDebugUtilsMessengerEXT   m_debug_messenger{VK_NULL_HANDLE};
        std::vector<VulkanDevice>  m_device_collection;
        std::vector<LayerProperty> m_selected_layer_property_collection;

    private:
        PFN_vkCreateDebugUtilsMessengerEXT  __createDebugMessengerPtr{nullptr};
        PFN_vkDestroyDebugUtilsMessengerEXT __destroyDebugMessengerPtr{nullptr};

        static VKAPI_ATTR VkBool32 VKAPI_CALL __debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT             messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void*                                       pUserData);

        int GetHighPerformantDeviceIndex() const;
    };
} // namespace ZEngine::Hardwares
