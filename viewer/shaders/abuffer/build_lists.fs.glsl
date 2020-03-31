#version 420 core

// Turn on early fragment testing
layout (early_fragment_tests) in;

layout (binding = 0, r32ui) uniform uimage2D head_pointer_image;
layout (binding = 1, rgba32ui) uniform writeonly uimageBuffer list_buffer;

layout (binding = 0, offset = 0) uniform atomic_uint index_counter;

uniform vec4 input_color;

void main(void)
{
        vec4 frag_color = input_color;

	uint index = atomicCounterIncrement(index_counter);
	uint old_head = imageAtomicExchange(head_pointer_image, ivec2(gl_FragCoord.xy), uint(index));

	// Now assemble the fragment into the buffer.
        uvec4 item;
	item.x = old_head;
	item.y = packUnorm4x8(frag_color);
        item.z = floatBitsToUint(gl_FragCoord.z);
        item.w = 0;

	// Write the data into the buffer at the right location
	imageStore(list_buffer, int(index), item);
}
