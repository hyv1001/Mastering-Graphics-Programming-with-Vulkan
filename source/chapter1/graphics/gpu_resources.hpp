#pragma once

#include "foundation/platform.hpp"

#include "graphics/gpu_enum.hpp"

#include <vulkan/vulkan.h>
#include "external/vk_mem_alloc.h"

namespace raptor {

struct Allocator;

static const u32                    k_invalid_index = 0xffffffff;

typedef u32                         ResourceHandle;

struct BufferHandle {
    ResourceHandle                  index;
}; // struct BufferHandle

struct TextureHandle {
    ResourceHandle                  index;
}; // struct TextureHandle

struct ShaderStateHandle {
    ResourceHandle                  index;
}; // struct ShaderStateHandle

struct SamplerHandle {
    ResourceHandle                  index;
}; // struct SamplerHandle

struct DescriptorSetLayoutHandle {
    ResourceHandle                  index;
}; // struct DescriptorSetLayoutHandle

struct DescriptorSetHandle {
    ResourceHandle                  index;
}; // struct DescriptorSetHandle

struct PipelineHandle {
    ResourceHandle                  index;
}; // struct PipelineHandle

struct RenderPassHandle {
	ResourceHandle                  index;
}; // struct RenderPassHandle

// Invalid handles
static BufferHandle                 k_invalid_buffer    { k_invalid_index };
static TextureHandle                k_invalid_texture   { k_invalid_index };
static ShaderStateHandle            k_invalid_shader    { k_invalid_index };
static SamplerHandle                k_invalid_sampler   { k_invalid_index };
static DescriptorSetLayoutHandle    k_invalid_layout    { k_invalid_index };
static DescriptorSetHandle          k_invalid_set       { k_invalid_index };
static PipelineHandle               k_invalid_pipeline  { k_invalid_index };
static RenderPassHandle             k_invalid_pass      { k_invalid_index };



// Consts /////////////////////////////////////////////////////////////////

static const u8                     k_max_image_outputs = 8;                // Maximum number of images/render_targets/fbo attachments usable.
static const u8                     k_max_descriptor_set_layouts = 8;       // Maximum number of layouts in the pipeline.
static const u8                     k_max_shader_stages = 5;                // Maximum simultaneous shader stages. Applicable to all different type of pipelines.
static const u8                     k_max_descriptors_per_set = 16;         // Maximum list elements for both descriptor set layout and descriptor sets.
static const u8                     k_max_vertex_streams = 16;
static const u8                     k_max_vertex_attributes = 16;

static const u32                    k_submit_header_sentinel = 0xfefeb7ba;
static const u32                    k_max_resource_deletions = 64;

// Resource creation structs //////////////////////////////////////////////

//
//
struct Rect2D {
    f32                             x = 0.0f;
    f32                             y = 0.0f;
    f32                             width = 0.0f;
    f32                             height = 0.0f;
}; // struct Rect2D

//
//
struct Rect2DInt {
    i16                             x = 0;
    i16                             y = 0;
    u16                             width = 0;
    u16                             height = 0;
}; // struct Rect2D

//
//
struct Viewport {
    Rect2DInt                       rect;
    f32                             min_depth = 0.0f;
    f32                             max_depth = 0.0f;
}; // struct Viewport

//
//
struct ViewportState {
    u32                             num_viewports = 0;
    u32                             num_scissors = 0;

    Viewport*                       viewport = nullptr;
    Rect2DInt*                      scissors = nullptr;
}; // struct ViewportState

//
//
struct StencilOperationState {

    VkStencilOp                     fail = VK_STENCIL_OP_KEEP;
    VkStencilOp                     pass = VK_STENCIL_OP_KEEP;
    VkStencilOp                     depth_fail = VK_STENCIL_OP_KEEP;
    VkCompareOp                     compare = VK_COMPARE_OP_ALWAYS;
    u32                             compare_mask = 0xff;
    u32                             write_mask = 0xff;
    u32                             reference = 0xff;

}; // struct StencilOperationState

//
//
struct DepthStencilCreation {

    StencilOperationState           front;
    StencilOperationState           back;
    VkCompareOp                     depth_comparison = VK_COMPARE_OP_ALWAYS;

    u8                              depth_enable        : 1;
    u8                              depth_write_enable  : 1;
    u8                              stencil_enable      : 1;
    u8                              pad                 : 5;

}; // struct DepthStencilCreation

struct BlendState {

    VkBlendFactor                   source_color        = VK_BLEND_FACTOR_ONE;
    VkBlendFactor                   destination_color   = VK_BLEND_FACTOR_ONE;
    VkBlendOp                       color_operation     = VK_BLEND_OP_ADD;

    VkBlendFactor                   source_alpha        = VK_BLEND_FACTOR_ONE;
    VkBlendFactor                   destination_alpha   = VK_BLEND_FACTOR_ONE;
    VkBlendOp                       alpha_operation     = VK_BLEND_OP_ADD;

    ColorWriteEnabled::Mask         color_write_mask    = ColorWriteEnabled::All_mask;

    u8                              blend_enabled   : 1;
    u8                              separate_blend  : 1;
    u8                              pad             : 6;


    BlendState() : blend_enabled( 0 ), separate_blend( 0 ) {
    }

    BlendState&                     set_color( VkBlendFactor source_color, VkBlendFactor destination_color, VkBlendOp color_operation );
    BlendState&                     set_alpha( VkBlendFactor source_color, VkBlendFactor destination_color, VkBlendOp color_operation );
    BlendState&                     set_color_write_mask( ColorWriteEnabled::Mask value );

}; // struct BlendState

struct BlendStateCreation {

    BlendState                      blend_states[ k_max_image_outputs ];
    u32                             active_states = 0;

    BlendStateCreation&             reset();
    BlendState&                     add_blend_state();

}; // BlendStateCreation

//
//
struct RasterizationCreation {

    VkCullModeFlagBits              cull_mode = VK_CULL_MODE_NONE;
    VkFrontFace                     front = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    FillMode::Enum                  fill = FillMode::Solid;
}; // struct RasterizationCreation

//
//
struct BufferCreation {

    VkBufferUsageFlags              type_flags = 0;
    ResourceUsageType::Enum         usage   = ResourceUsageType::Immutable;
    u32                             size    = 0;
    void*                           initial_data = nullptr;

    const char*                     name    = nullptr;
}; // struct BufferCreation

//
//
struct TextureCreation {

    void*                           initial_data    = nullptr;
    u16                             width           = 1;
    u16                             height          = 1;
    u16                             depth           = 1;
    u8                              mipmaps         = 1;
    u8                              flags           = 0;    // TextureFlags bitmasks

    VkFormat                        format          = VK_FORMAT_UNDEFINED;
    TextureType::Enum               type            = TextureType::Texture2D;

    const char*                     name            = nullptr;
}; // struct TextureCreation

//
//
struct SamplerCreation {

    VkFilter                        min_filter = VK_FILTER_NEAREST;
    VkFilter                        mag_filter = VK_FILTER_NEAREST;
    VkSamplerMipmapMode             mip_filter = VK_SAMPLER_MIPMAP_MODE_NEAREST;

    VkSamplerAddressMode            address_mode_u = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    VkSamplerAddressMode            address_mode_v = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    VkSamplerAddressMode            address_mode_w = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    const char*                     name = nullptr;
}; // struct SamplerCreation

//
//
struct ShaderStage {

    const char*                     code        = nullptr;
    u32                             code_size   = 0;
    VkShaderStageFlagBits           type        = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;

}; // struct ShaderStage

//
//
struct ShaderStateCreation {

    ShaderStage                     stages[ k_max_shader_stages ];

    const char*                     name            = nullptr;

    u32                             stages_count    = 0;
    u32                             spv_input       = 0;

    // Building helpers
    ShaderStateCreation&            reset();
    ShaderStateCreation&            set_name( const char* name );
    ShaderStateCreation&            add_stage( const char* code, u32 code_size, VkShaderStageFlagBits type );
    ShaderStateCreation&            set_spv_input( bool value );

}; // struct ShaderStateCreation

//
// 
struct DescriptorSetLayoutCreation {

    //
    // A single descriptor binding. It can be relative to one or more resources of the same type.
    //
    struct Binding {

        VkDescriptorType            type    = VK_DESCRIPTOR_TYPE_MAX_ENUM;
        u16                         start   = 0;
        u16                         count   = 0;
        cstring                     name    = nullptr;  // Comes from external memory.
    }; // struct Binding

    Binding                         bindings[ k_max_descriptors_per_set ];
    u32                             num_bindings    = 0;
    u32                             set_index       = 0;

    cstring                         name            = nullptr;

    // Building helpers
    DescriptorSetLayoutCreation&    reset();
    DescriptorSetLayoutCreation&    add_binding( const Binding& binding );
    DescriptorSetLayoutCreation&    set_name( cstring name );
    DescriptorSetLayoutCreation&    set_set_index( u32 index );

}; // struct DescriptorSetLayoutCreation

//
//
struct DescriptorSetCreation {

    ResourceHandle                  resources[ k_max_descriptors_per_set ];
    SamplerHandle                   samplers[ k_max_descriptors_per_set ];
    u16                             bindings[ k_max_descriptors_per_set ];

    DescriptorSetLayoutHandle       layout;
    u32                             num_resources   = 0;

    cstring                         name            = nullptr;

    // Building helpers
    DescriptorSetCreation&          reset();
    DescriptorSetCreation&          texture( TextureHandle texture, u16 binding );
    DescriptorSetCreation&          buffer( BufferHandle buffer, u16 binding );
    DescriptorSetCreation&          texture_sampler( TextureHandle texture, SamplerHandle sampler, u16 binding );   // TODO: separate samplers from textures

}; // struct DescriptorSetCreation

//
//
struct DescriptorSetUpdate {
    DescriptorSetHandle             descriptor_set;

    u32                             frame_issued = 0;
}; // DescriptorSetUpdate

//
//
struct VertexAttribute {

    u16                             location = 0;
    u16                             binding = 0;
    u32                             offset = 0;
    VertexComponentFormat::Enum     format = VertexComponentFormat::Count;

}; // struct VertexAttribute

//
//
struct VertexStream {

    u16                             binding = 0;
    u16                             stride = 0;
    VertexInputRate::Enum           input_rate = VertexInputRate::Count;

}; // struct VertexStream

//
//
struct VertexInputCreation {

    u32                             num_vertex_streams = 0;
    u32                             num_vertex_attributes = 0;

    VertexStream                    vertex_streams[ k_max_vertex_streams ];
    VertexAttribute                 vertex_attributes[ k_max_vertex_attributes ];

    VertexInputCreation&            reset();
    VertexInputCreation&            add_vertex_stream( const VertexStream& stream );
    VertexInputCreation&            add_vertex_attribute( const VertexAttribute& attribute );
}; // struct VertexInputCreation

//
//
struct RenderPassOutput {

    VkFormat                        color_formats[ k_max_image_outputs ];
    VkFormat                        depth_stencil_format;
    u32                             num_color_formats;

    RenderPassOperation::Enum       color_operation         = RenderPassOperation::DontCare;
    RenderPassOperation::Enum       depth_operation         = RenderPassOperation::DontCare;
    RenderPassOperation::Enum       stencil_operation       = RenderPassOperation::DontCare;

    RenderPassOutput&               reset();
    RenderPassOutput&               color( VkFormat format );
    RenderPassOutput&               depth( VkFormat format );
    RenderPassOutput&               set_operations( RenderPassOperation::Enum color, RenderPassOperation::Enum depth, RenderPassOperation::Enum stencil );

}; // struct RenderPassOutput

//
//
struct RenderPassCreation {

    u16                             num_render_targets  = 0;
    RenderPassType::Enum            type                = RenderPassType::Geometry;

    TextureHandle                   output_textures[ k_max_image_outputs ];
    TextureHandle                   depth_stencil_texture;

    f32                             scale_x             = 1.f;
    f32                             scale_y             = 1.f;
    u8                              resize              = 1;

    RenderPassOperation::Enum       color_operation         = RenderPassOperation::DontCare;
    RenderPassOperation::Enum       depth_operation         = RenderPassOperation::DontCare;
    RenderPassOperation::Enum       stencil_operation       = RenderPassOperation::DontCare;

    const char*                     name                = nullptr;

    RenderPassCreation&             reset();
    RenderPassCreation&             add_render_texture( TextureHandle texture );
    RenderPassCreation&             set_scaling( f32 scale_x, f32 scale_y, u8 resize );
    RenderPassCreation&             set_depth_stencil_texture( TextureHandle texture );
    RenderPassCreation&             set_name( const char* name );
    RenderPassCreation&             set_type( RenderPassType::Enum type );
    RenderPassCreation&             set_operations( RenderPassOperation::Enum color, RenderPassOperation::Enum depth, RenderPassOperation::Enum stencil );

}; // struct RenderPassCreation

//
//
struct PipelineCreation {

    RasterizationCreation           rasterization;
    DepthStencilCreation            depth_stencil;
    BlendStateCreation              blend_state;
    VertexInputCreation             vertex_input;
    ShaderStateCreation             shaders;

    RenderPassOutput                render_pass;
    DescriptorSetLayoutHandle       descriptor_set_layout[ k_max_descriptor_set_layouts ];
    const ViewportState*            viewport = nullptr;

    u32                             num_active_layouts = 0;

    const char*                     name = nullptr;

    RenderPassOutput&               render_pass_output();

}; // struct PipelineCreation

// API-agnostic structs ///////////////////////////////////////////////////

//
// Helper methods for texture formats
//
namespace TextureFormat {

    inline bool                     is_depth_stencil( VkFormat value ) {
        return value == VK_FORMAT_D16_UNORM_S8_UINT || value == VK_FORMAT_D24_UNORM_S8_UINT || value == VK_FORMAT_D32_SFLOAT_S8_UINT;
    }
    inline bool                     is_depth_only( VkFormat value ) {
        return value >= VK_FORMAT_D16_UNORM && value < VK_FORMAT_D32_SFLOAT;
    }
    inline bool                     is_stencil_only( VkFormat value ) {
        return value == VK_FORMAT_S8_UINT;
    }

    inline bool                     has_depth( VkFormat value ) {
        return (value >= VK_FORMAT_D16_UNORM && value < VK_FORMAT_S8_UINT ) || (value >= VK_FORMAT_D16_UNORM_S8_UINT && value <= VK_FORMAT_D32_SFLOAT_S8_UINT);
    }
    inline bool                     has_stencil( VkFormat value ) {
        return value >= VK_FORMAT_S8_UINT && value <= VK_FORMAT_D32_SFLOAT_S8_UINT;
    }
    inline bool                     has_depth_or_stencil( VkFormat value ) {
        return value >= VK_FORMAT_D16_UNORM && value <= VK_FORMAT_D32_SFLOAT_S8_UINT;
    }

} // namespace TextureFormat

struct ResourceData {

    void* data = nullptr;

}; // struct ResourceData

//
//
struct ResourceBinding {
    u16                             type = 0;    // ResourceType
    u16                             start = 0;
    u16                             count = 0;
    u16                             set = 0;

    const char*                     name = nullptr;
}; // struct ResourceBinding


// API-agnostic descriptions //////////////////////////////////////////////

//
//
struct ShaderStateDescription {

    void*                           native_handle = nullptr;
    cstring                         name        = nullptr;

}; // struct ShaderStateDescription

//
//
struct BufferDescription {

    void*                           native_handle = nullptr;
    cstring                         name        = nullptr;

    VkBufferUsageFlags              type_flags  = 0;
    ResourceUsageType::Enum         usage       = ResourceUsageType::Immutable;
    u32                             size        = 0;
    BufferHandle                    parent_handle;

}; // struct BufferDescription

//
//
struct TextureDescription {

    void*                           native_handle = nullptr;
    cstring                         name        = nullptr;

    u16                             width       = 1;
    u16                             height      = 1;
    u16                             depth       = 1;
    u8                              mipmaps     = 1;
    u8                              render_target = 0;
    u8                              compute_access = 0;

    VkFormat                        format = VK_FORMAT_UNDEFINED;
    TextureType::Enum               type = TextureType::Texture2D;

}; // struct Texture

//
//
struct SamplerDescription {

    cstring                         name        = nullptr;

    VkFilter                        min_filter = VK_FILTER_NEAREST;
    VkFilter                        mag_filter = VK_FILTER_NEAREST;
    VkSamplerMipmapMode             mip_filter = VK_SAMPLER_MIPMAP_MODE_NEAREST;

    VkSamplerAddressMode            address_mode_u = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    VkSamplerAddressMode            address_mode_v = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    VkSamplerAddressMode            address_mode_w = VK_SAMPLER_ADDRESS_MODE_REPEAT;

}; // struct SamplerDescription

//
//
struct DescriptorSetLayoutDescription {

    ResourceBinding                 bindings[ k_max_descriptors_per_set ];
    u32                             num_active_bindings = 0;

}; // struct DescriptorSetLayoutDescription

//
//
struct DesciptorSetDescription {

    ResourceData                    resources[ k_max_descriptors_per_set ];
    u32                             num_active_resources = 0;

}; // struct DesciptorSetDescription

//
//
struct PipelineDescription {

    ShaderStateHandle               shader;

}; // struct PipelineDescription


// API-agnostic resource modifications ////////////////////////////////////

struct MapBufferParameters {
    BufferHandle                    buffer;
    u32                             offset = 0;
    u32                             size = 0;

}; // struct MapBufferParameters

// Synchronization ////////////////////////////////////////////////////////

//
//
struct ImageBarrier {

    TextureHandle                   texture;

}; // struct ImageBarrier

//
//
struct MemoryBarrier {

    BufferHandle                    buffer;

}; // struct MemoryBarrier

//
//
struct ExecutionBarrier {

    PipelineStage::Enum             source_pipeline_stage;
    PipelineStage::Enum             destination_pipeline_stage;

    u32                             new_barrier_experimental  = u32_max;
    u32                             load_operation = 0;

    u32                             num_image_barriers;
    u32                             num_memory_barriers;

    ImageBarrier                    image_barriers[ 8 ];
    MemoryBarrier                   memory_barriers[ 8 ];

    ExecutionBarrier&               reset();
    ExecutionBarrier&               set( PipelineStage::Enum source, PipelineStage::Enum destination );
    ExecutionBarrier&               add_image_barrier( const ImageBarrier& image_barrier );
    ExecutionBarrier&               add_memory_barrier( const MemoryBarrier& memory_barrier );

}; // struct Barrier

//
//
struct ResourceUpdate {

    ResourceDeletionType::Enum      type;
    ResourceHandle                  handle;
    u32                             current_frame;
}; // struct ResourceUpdate

// Resources //////////////////////////////////////////////////////////////

static const u32            k_max_swapchain_images = 3;

struct DeviceStateVulkan;


//
//
struct Buffer {

    VkBuffer                        vk_buffer;
    VmaAllocation                   vma_allocation;
    VkDeviceMemory                  vk_device_memory;
    VkDeviceSize                    vk_device_size;

    VkBufferUsageFlags              type_flags      = 0;
    ResourceUsageType::Enum         usage           = ResourceUsageType::Immutable;
    u32                             size            = 0;
    u32                             global_offset   = 0;    // Offset into global constant, if dynamic

    BufferHandle                    handle;
    BufferHandle                    parent_buffer;

    const char* name                = nullptr;

}; // struct BufferVulkan


//
//
struct Sampler {

    VkSampler                       vk_sampler;

    VkFilter                        min_filter = VK_FILTER_NEAREST;
    VkFilter                        mag_filter = VK_FILTER_NEAREST;
    VkSamplerMipmapMode             mip_filter = VK_SAMPLER_MIPMAP_MODE_NEAREST;

    VkSamplerAddressMode            address_mode_u = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    VkSamplerAddressMode            address_mode_v = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    VkSamplerAddressMode            address_mode_w = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    const char*                     name    = nullptr;

}; // struct SamplerVulkan

//
//
struct Texture {

    VkImage                         vk_image;
    VkImageView                     vk_image_view;
    VkFormat                        vk_format;
    VkImageLayout                   vk_image_layout;
    VmaAllocation                   vma_allocation;

    u16                             width = 1;
    u16                             height = 1;
    u16                             depth = 1;
    u8                              mipmaps = 1;
    u8                              flags = 0;

    TextureHandle                   handle;
    TextureType::Enum               type    = TextureType::Texture2D;

    Sampler*                        sampler = nullptr;

    const char*                     name    = nullptr;
}; // struct TextureVulkan

//
//
struct ShaderState {

    VkPipelineShaderStageCreateInfo shader_stage_info[ k_max_shader_stages ];

    const char*                     name = nullptr;

    u32                             active_shaders = 0;
    bool                            graphics_pipeline = false;
}; // struct ShaderStateVulkan

//
//
struct DescriptorBinding {

    VkDescriptorType                type;
    u16                             start   = 0;
    u16                             count   = 0;
    u16                             set     = 0;

    const char*                     name    = nullptr;
}; // struct ResourceBindingVulkan

//
//
struct DesciptorSetLayout {

    VkDescriptorSetLayout           vk_descriptor_set_layout;

    VkDescriptorSetLayoutBinding*   vk_binding      = nullptr;
    DescriptorBinding*              bindings        = nullptr;
    u16                             num_bindings    = 0;
    u16                             set_index       = 0;

    DescriptorSetLayoutHandle       handle;

}; // struct DesciptorSetLayoutVulkan

//
//
struct DesciptorSet {

    VkDescriptorSet                 vk_descriptor_set;

    ResourceHandle*                 resources       = nullptr;
    SamplerHandle*                  samplers        = nullptr;
    u16*                            bindings        = nullptr;

    const DesciptorSetLayout*       layout          = nullptr;
    u32                             num_resources   = 0;
}; // struct DesciptorSetVulkan


//
//
struct Pipeline {

    VkPipeline                      vk_pipeline;
    VkPipelineLayout                vk_pipeline_layout;

    VkPipelineBindPoint             vk_bind_point;

    ShaderStateHandle               shader_state;

    const DesciptorSetLayout*       descriptor_set_layout[ k_max_descriptor_set_layouts ];
    DescriptorSetLayoutHandle       descriptor_set_layout_handle[ k_max_descriptor_set_layouts ];
    u32                             num_active_layouts = 0;

    DepthStencilCreation            depth_stencil;
    BlendStateCreation              blend_state;
    RasterizationCreation           rasterization;

    PipelineHandle                  handle;
    bool                            graphics_pipeline = true;

}; // struct PipelineVulkan


//
//
struct RenderPass {

    VkRenderPass                    vk_render_pass;
    VkFramebuffer                   vk_frame_buffer;

    RenderPassOutput                output;

    TextureHandle                   output_textures[ k_max_image_outputs ];
    TextureHandle                   output_depth;

    RenderPassType::Enum            type;

    f32                             scale_x     = 1.f;
    f32                             scale_y     = 1.f;
    u16                             width       = 0;
    u16                             height      = 0;
    u16                             dispatch_x  = 0;
    u16                             dispatch_y  = 0;
    u16                             dispatch_z  = 0;

    u8                              resize      = 0;
    u8                              num_render_targets = 0;

    const char*                     name        = nullptr;
}; // struct RenderPassVulkan


// Enum translations. Use tables or switches depending on the case. ///////
static cstring to_compiler_extension( VkShaderStageFlagBits value ) {
    switch ( value ) {
        case VK_SHADER_STAGE_VERTEX_BIT:
            return "vert";
        case VK_SHADER_STAGE_FRAGMENT_BIT:
            return "frag";
        case VK_SHADER_STAGE_COMPUTE_BIT:
            return "comp";
        default:
            return "";
    }
}

//
static cstring to_stage_defines( VkShaderStageFlagBits value ) {
    switch ( value ) {
        case VK_SHADER_STAGE_VERTEX_BIT:
            return "VERTEX";
        case VK_SHADER_STAGE_FRAGMENT_BIT:
            return "FRAGMENT";
        case VK_SHADER_STAGE_COMPUTE_BIT:
            return "COMPUTE";
        default:
            return "";
    }
}
//
//
static VkImageType to_vk_image_type( TextureType::Enum type ) {
    static VkImageType s_vk_target[ TextureType::Count ] = { VK_IMAGE_TYPE_1D, VK_IMAGE_TYPE_2D, VK_IMAGE_TYPE_3D, VK_IMAGE_TYPE_1D, VK_IMAGE_TYPE_2D, VK_IMAGE_TYPE_3D };
    return s_vk_target[ type ];
}

//
//
static VkImageViewType to_vk_image_view_type( TextureType::Enum type ) {
    static VkImageViewType s_vk_data[] = { VK_IMAGE_VIEW_TYPE_1D, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_VIEW_TYPE_3D, VK_IMAGE_VIEW_TYPE_1D_ARRAY, VK_IMAGE_VIEW_TYPE_2D_ARRAY, VK_IMAGE_VIEW_TYPE_CUBE_ARRAY };
    return s_vk_data[ type ];
}

//
//
static VkFormat to_vk_vertex_format( VertexComponentFormat::Enum value ) {
    // Float, Float2, Float3, Float4, Mat4, Byte, Byte4N, UByte, UByte4N, Short2, Short2N, Short4, Short4N, Uint, Uint2, Uint4, Count
    static VkFormat s_vk_vertex_formats[ VertexComponentFormat::Count ] = { VK_FORMAT_R32_SFLOAT, VK_FORMAT_R32G32_SFLOAT, VK_FORMAT_R32G32B32_SFLOAT, VK_FORMAT_R32G32B32A32_SFLOAT, /*MAT4 TODO*/VK_FORMAT_R32G32B32A32_SFLOAT,
                                                                          VK_FORMAT_R8_SINT, VK_FORMAT_R8G8B8A8_SNORM, VK_FORMAT_R8_UINT, VK_FORMAT_R8G8B8A8_UINT, VK_FORMAT_R16G16_SINT, VK_FORMAT_R16G16_SNORM,
                                                                          VK_FORMAT_R16G16B16A16_SINT, VK_FORMAT_R16G16B16A16_SNORM, VK_FORMAT_R32_UINT, VK_FORMAT_R32G32_UINT, VK_FORMAT_R32G32B32A32_UINT };

    return s_vk_vertex_formats[ value ];
}

//
//
static VkPipelineStageFlags to_vk_pipeline_stage( PipelineStage::Enum value ) {
    static VkPipelineStageFlags s_vk_values[] = { VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT };
    return s_vk_values[ value ];
}

//
//
static VkAccessFlags util_to_vk_access_flags( ResourceState state ) {
    VkAccessFlags ret = 0;
    if ( state & RESOURCE_STATE_COPY_SOURCE ) {
        ret |= VK_ACCESS_TRANSFER_READ_BIT;
    }
    if ( state & RESOURCE_STATE_COPY_DEST ) {
        ret |= VK_ACCESS_TRANSFER_WRITE_BIT;
    }
    if ( state & RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER ) {
        ret |= VK_ACCESS_UNIFORM_READ_BIT | VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
    }
    if ( state & RESOURCE_STATE_INDEX_BUFFER ) {
        ret |= VK_ACCESS_INDEX_READ_BIT;
    }
    if ( state & RESOURCE_STATE_UNORDERED_ACCESS ) {
        ret |= VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT;
    }
    if ( state & RESOURCE_STATE_INDIRECT_ARGUMENT ) {
        ret |= VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
    }
    if ( state & RESOURCE_STATE_RENDER_TARGET ) {
        ret |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    }
    if ( state & RESOURCE_STATE_DEPTH_WRITE ) {
        ret |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
    }
    if ( state & RESOURCE_STATE_SHADER_RESOURCE ) {
        ret |= VK_ACCESS_SHADER_READ_BIT;
    }
    if ( state & RESOURCE_STATE_PRESENT ) {
        ret |= VK_ACCESS_MEMORY_READ_BIT;
    }
#ifdef ENABLE_RAYTRACING
    if ( state & RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE ) {
        ret |= VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_NV | VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_NV;
    }
#endif

    return ret;
}

static VkImageLayout util_to_vk_image_layout( ResourceState usage ) {
    if ( usage & RESOURCE_STATE_COPY_SOURCE )
        return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

    if ( usage & RESOURCE_STATE_COPY_DEST )
        return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

    if ( usage & RESOURCE_STATE_RENDER_TARGET )
        return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    if ( usage & RESOURCE_STATE_DEPTH_WRITE )
        return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    if ( usage & RESOURCE_STATE_DEPTH_READ )
        return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

    if ( usage & RESOURCE_STATE_UNORDERED_ACCESS )
        return VK_IMAGE_LAYOUT_GENERAL;

    if ( usage & RESOURCE_STATE_SHADER_RESOURCE )
        return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    if ( usage & RESOURCE_STATE_PRESENT )
        return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    if ( usage == RESOURCE_STATE_COMMON )
        return VK_IMAGE_LAYOUT_GENERAL;

    return VK_IMAGE_LAYOUT_UNDEFINED;
}

// Determines pipeline stages involved for given accesses
static VkPipelineStageFlags util_determine_pipeline_stage_flags( VkAccessFlags accessFlags, QueueType::Enum queueType ) {
    VkPipelineStageFlags flags = 0;

    switch ( queueType ) {
        case QueueType::Graphics:
        {
            if ( ( accessFlags & ( VK_ACCESS_INDEX_READ_BIT | VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT ) ) != 0 )
                flags |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;

            if ( ( accessFlags & ( VK_ACCESS_UNIFORM_READ_BIT | VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT ) ) != 0 ) {
                flags |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
                flags |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                /*if ( pRenderer->pActiveGpuSettings->mGeometryShaderSupported ) {
                    flags |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
                }
                if ( pRenderer->pActiveGpuSettings->mTessellationSupported ) {
                    flags |= VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
                    flags |= VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
                }*/
                flags |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
    #ifdef ENABLE_RAYTRACING
                if ( pRenderer->mVulkan.mRaytracingExtension ) {
                    flags |= VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_NV;
                }
    #endif
            }

            if ( ( accessFlags & VK_ACCESS_INPUT_ATTACHMENT_READ_BIT ) != 0 )
                flags |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

            if ( ( accessFlags & ( VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT ) ) != 0 )
                flags |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

            if ( ( accessFlags & ( VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT ) ) != 0 )
                flags |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;

            break;
        }
        case QueueType::Compute:
        {
            if ( ( accessFlags & ( VK_ACCESS_INDEX_READ_BIT | VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT ) ) != 0 ||
                ( accessFlags & VK_ACCESS_INPUT_ATTACHMENT_READ_BIT ) != 0 ||
                ( accessFlags & ( VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT ) ) != 0 ||
                ( accessFlags & ( VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT ) ) != 0 )
                return VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

            if ( ( accessFlags & ( VK_ACCESS_UNIFORM_READ_BIT | VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT ) ) != 0 )
                flags |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;

            break;
        }
        case QueueType::CopyTransfer : return VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
        default: break;
    }

    // Compatible with both compute and graphics queues
    if ( ( accessFlags & VK_ACCESS_INDIRECT_COMMAND_READ_BIT ) != 0 )
        flags |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;

    if ( ( accessFlags & ( VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT ) ) != 0 )
        flags |= VK_PIPELINE_STAGE_TRANSFER_BIT;

    if ( ( accessFlags & ( VK_ACCESS_HOST_READ_BIT | VK_ACCESS_HOST_WRITE_BIT ) ) != 0 )
        flags |= VK_PIPELINE_STAGE_HOST_BIT;

    if ( flags == 0 )
        flags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

    return flags;
}

} // namespace raptor

