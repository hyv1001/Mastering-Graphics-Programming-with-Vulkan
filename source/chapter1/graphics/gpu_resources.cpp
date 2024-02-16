#include "gpu_resources.hpp"

namespace raptor {


// BlendState  /////////////////////////////////////////////
BlendState& BlendState::set_color( VkBlendFactor source, VkBlendFactor destination, VkBlendOp operation ) {
    source_color = source;
    destination_color = destination;
    color_operation = operation;
    blend_enabled = 1;

    return *this;
}

BlendState& BlendState::set_alpha( VkBlendFactor source, VkBlendFactor destination, VkBlendOp operation ) {
    source_alpha = source;
    destination_alpha = destination;
    alpha_operation = operation;
    separate_blend = 1;

    return *this;
}

BlendState& BlendState::set_color_write_mask( ColorWriteEnabled::Mask value ) {
    color_write_mask = value;

    return *this;
}

// BlendStateCreation //////////////////////////////////////
BlendStateCreation& BlendStateCreation::reset() {
    active_states = 0;

    return *this;
}

BlendState& BlendStateCreation::add_blend_state() {
    return blend_states[active_states++];
}


// ShaderStateCreation /////////////////////////////////////
ShaderStateCreation& ShaderStateCreation::reset() {
    stages_count = 0;

    return *this;
}

ShaderStateCreation& ShaderStateCreation::set_name( const char* name_ ) {
    name = name_;

    return *this;
}

ShaderStateCreation& ShaderStateCreation::add_stage( const char* code, u32 code_size, VkShaderStageFlagBits type ) {
    stages[stages_count].code = code;
    stages[stages_count].code_size = code_size;
    stages[stages_count].type = type;
    ++stages_count;

    return *this;
}

ShaderStateCreation& ShaderStateCreation::set_spv_input( bool value ) {
    spv_input = value;
    return *this;
}

// DescriptorSetLayoutCreation ////////////////////////////////////////////
DescriptorSetLayoutCreation& DescriptorSetLayoutCreation::reset() {
    num_bindings = 0;
    set_index = 0;
    return *this;
}

DescriptorSetLayoutCreation& DescriptorSetLayoutCreation::add_binding( const Binding& binding ) {
    bindings[num_bindings++] = binding;
    return *this;
}

DescriptorSetLayoutCreation& DescriptorSetLayoutCreation::set_name( cstring name_ ) {
    name = name_;
    return *this;
}


DescriptorSetLayoutCreation& DescriptorSetLayoutCreation::set_set_index( u32 index ) {
    set_index = index;
    return *this;
}

// DescriptorSetCreation //////////////////////////////////////////////////
DescriptorSetCreation& DescriptorSetCreation::reset() {
    num_resources = 0;
    return *this;
}

DescriptorSetCreation& DescriptorSetCreation::texture( TextureHandle texture, u16 binding ) {
    // Set a default sampler
    samplers[ num_resources ] = k_invalid_sampler;
    bindings[ num_resources ] = binding;
    resources[ num_resources++ ] = texture.index;
    return *this;
}

DescriptorSetCreation& DescriptorSetCreation::buffer( BufferHandle buffer, u16 binding ) {
    samplers[ num_resources ] = k_invalid_sampler;
    bindings[ num_resources ] = binding;
    resources[ num_resources++ ] = buffer.index;
    return *this;
}

DescriptorSetCreation& DescriptorSetCreation::texture_sampler( TextureHandle texture, SamplerHandle sampler, u16 binding ) {
    bindings[ num_resources ] = binding;
    resources[ num_resources ] = texture.index;
    samplers[ num_resources++ ] = sampler;
    return *this;
}

// VertexInputCreation /////////////////////////////////////
VertexInputCreation& VertexInputCreation::reset() {
    num_vertex_streams = num_vertex_attributes = 0;
    return *this;
}

VertexInputCreation& VertexInputCreation::add_vertex_stream( const VertexStream& stream ) {
    vertex_streams[num_vertex_streams++] = stream;
    return *this;
}

VertexInputCreation& VertexInputCreation::add_vertex_attribute( const VertexAttribute& attribute ) {
    vertex_attributes[num_vertex_attributes++] = attribute;
    return *this;
}

// RenderPassOutput ////////////////////////////////////////
RenderPassOutput& RenderPassOutput::reset() {
    num_color_formats = 0;
    for ( u32 i = 0; i < k_max_image_outputs; ++i) {
        color_formats[i] = VK_FORMAT_UNDEFINED;
    }
    depth_stencil_format = VK_FORMAT_UNDEFINED;
    color_operation = depth_operation = stencil_operation = RenderPassOperation::DontCare;
    return *this;
}

RenderPassOutput& RenderPassOutput::color( VkFormat format ) {
    color_formats[ num_color_formats++ ] = format;
    return *this;
}

RenderPassOutput& RenderPassOutput::depth( VkFormat format ) {
    depth_stencil_format = format;
    return *this;
}

RenderPassOutput& RenderPassOutput::set_operations( RenderPassOperation::Enum color_, RenderPassOperation::Enum depth_, RenderPassOperation::Enum stencil_ ) {
    color_operation = color_;
    depth_operation = depth_;
    stencil_operation = stencil_;

    return *this;
}


RenderPassOutput& PipelineCreation::render_pass_output() {
    return render_pass;
}

// RenderPassCreation //////////////////////////////////////
RenderPassCreation& RenderPassCreation::reset() {
    num_render_targets = 0;
    depth_stencil_texture = k_invalid_texture;
    resize = 0;
    scale_x = 1.f;
    scale_y = 1.f;
    color_operation = depth_operation = stencil_operation = RenderPassOperation::DontCare;

    return *this;
}

RenderPassCreation& RenderPassCreation::add_render_texture( TextureHandle texture ) {
    output_textures[num_render_targets++] = texture;

    return *this;
}

RenderPassCreation& RenderPassCreation::set_scaling( f32 scale_x_, f32 scale_y_, u8 resize_ ) {
    scale_x = scale_x_;
    scale_y = scale_y_;
    resize = resize_;

    return *this;
}

RenderPassCreation& RenderPassCreation::set_depth_stencil_texture( TextureHandle texture ) {
    depth_stencil_texture = texture;

    return *this;
}

RenderPassCreation& RenderPassCreation::set_name( const char* name_ ) {
    name = name_;

    return *this;
}

RenderPassCreation& RenderPassCreation::set_type( RenderPassType::Enum type_ ) {
    type = type_;

    return *this;
}

RenderPassCreation& RenderPassCreation::set_operations( RenderPassOperation::Enum color_, RenderPassOperation::Enum depth_, RenderPassOperation::Enum stencil_ ) {
    color_operation = color_;
    depth_operation = depth_;
    stencil_operation = stencil_;

    return *this;
}

// ExecutionBarrier ////////////////////////////////////////
ExecutionBarrier& ExecutionBarrier::reset() {
    num_image_barriers = num_memory_barriers = 0;
    source_pipeline_stage = PipelineStage::DrawIndirect;
    destination_pipeline_stage = PipelineStage::DrawIndirect;
    return *this;
}

ExecutionBarrier& ExecutionBarrier::set( PipelineStage::Enum source, PipelineStage::Enum destination ) {
    source_pipeline_stage = source;
    destination_pipeline_stage = destination;

    return *this;
}

ExecutionBarrier& ExecutionBarrier::add_image_barrier( const ImageBarrier& image_barrier ) {
    image_barriers[num_image_barriers++] = image_barrier;

    return *this;
}

ExecutionBarrier& ExecutionBarrier::add_memory_barrier( const MemoryBarrier& memory_barrier ) {
    memory_barriers[ num_memory_barriers++ ] = memory_barrier;

    return *this;
}

} // namespace raptor
