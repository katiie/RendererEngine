#pragma once
#include <Rendering/Buffers/GraphicBuffer.h>
#include <Hardwares/VulkanDevice.h>

namespace ZEngine::Rendering::Buffers
{

    class VertexBuffer : public IGraphicBuffer
    {
    public:
        explicit VertexBuffer() : IGraphicBuffer() {}

        void SetData(const void* data, size_t byte_size)
        {
            if (byte_size == 0)
            {
                return;
            }

            if (this->m_byte_size < byte_size)
            {
                /*
                 * Tracking the size change..
                 */
                m_last_byte_size = m_byte_size;

                CleanUpMemory();
                this->m_byte_size = byte_size;
                m_staging_buffer = Hardwares::VulkanDevice::CreateBuffer(
                static_cast<VkDeviceSize>(this->m_byte_size), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
                m_vertex_buffer   = Hardwares::VulkanDevice::CreateBuffer(
                    static_cast<VkDeviceSize>(this->m_byte_size), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
            }

            if (!data)
            {
                return;
            }

            auto  device = Hardwares::VulkanDevice::GetNativeDeviceHandle();
            void* memory_region;
            ZENGINE_VALIDATE_ASSERT(vkMapMemory(device, m_staging_buffer.Memory, 0, this->m_byte_size, 0, &memory_region) == VK_SUCCESS, "Failed to map the memory")
            std::memcpy(memory_region, data, this->m_byte_size);
            vkUnmapMemory(device, m_staging_buffer.Memory);

            Hardwares::VulkanDevice::CopyBuffer(m_staging_buffer, m_vertex_buffer, static_cast<VkDeviceSize>(this->m_byte_size));
        }

        template <typename T>
        inline void SetData(const std::vector<T>& content)
        {
            size_t byte_size = sizeof(T) * content.size();
            this->SetData(content.data(), byte_size);
        }

        ~VertexBuffer()
        {
            CleanUpMemory();
        }

        void* GetNativeBufferHandle() const override
        {
            return reinterpret_cast<void*>(m_vertex_buffer.Handle);
        }

        void Dispose()
        {
            CleanUpMemory();
        }

    private:
        void CleanUpMemory()
        {
            if (m_staging_buffer)
            {
                Hardwares::VulkanDevice::EnqueueForDeletion(Rendering::DeviceResourceType::BUFFER, m_staging_buffer.Handle);
                Hardwares::VulkanDevice::EnqueueForDeletion(Rendering::DeviceResourceType::BUFFERMEMORY, m_staging_buffer.Memory);
                m_staging_buffer = {};
            }

            if (m_vertex_buffer)
            {
                Hardwares::VulkanDevice::EnqueueForDeletion(Rendering::DeviceResourceType::BUFFER, m_vertex_buffer.Handle);
                Hardwares::VulkanDevice::EnqueueForDeletion(Rendering::DeviceResourceType::BUFFERMEMORY, m_vertex_buffer.Memory);
                m_vertex_buffer = {};
            }
        }

    private:
        Hardwares::BufferView m_staging_buffer;
        Hardwares::BufferView m_vertex_buffer;
    };
} // namespace ZEngine::Rendering::Buffers
