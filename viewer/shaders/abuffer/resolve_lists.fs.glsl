#version 420 core

layout (binding = 0, r32ui) uniform uimage2D head_pointer_image;
layout (binding = 1, rgba32ui) uniform uimageBuffer list_buffer;

#define MAX_FRAGMENTS 40
uvec4 fragments[MAX_FRAGMENTS];

layout (location = 0) out vec4 output_color;

int build_local_fragment_list(void)
{
	uint current;
	int frag_count = 0;

	// Get the initial head pointer from the header-pointer image
        current = imageAtomicExchange(head_pointer_image, ivec2(gl_FragCoord.xy), uint(0));

	// While we haven’t reached the end of the list or exhausted
	// the storage available in fragments[]...
	while (current != 0 && frag_count < MAX_FRAGMENTS)
	{
		// Read an item from the linked list
                uvec4 item = imageLoad(list_buffer, int(current));

		// item.x contains the "next" pointer - update current
		current = item.x;

		// Store the fragment in the array
		fragments[frag_count] = item;

		// Update the fragment count
		frag_count++;
	}

	return frag_count;
}

void sort_fragment_list(int frag_count)
{
	int i;
	int j;

	for (i = 0; i < frag_count; i++)
	{
		for (j = i + 1; j < frag_count; j++)
		{
			// The depth of each fragment is bit-encoded into the
			// .z channel of the fragment array. Unpack it here.
			float depth_i = uintBitsToFloat(fragments[i].z);
			float depth_j = uintBitsToFloat(fragments[j].z);

			// Compare depth and if the comparison fails...
			if (depth_i < depth_j)
			{
				// Swap the fragments in the array
                                uvec4 temp = fragments[i];
				fragments[i] = fragments[j];
				fragments[j] = temp;
			}
		}
	}
}

vec4 blend(vec4 current_color, vec4 new_color)
{
	return mix(current_color, new_color, new_color.a);
}

vec4 calculate_final_color(int frag_count)
{
	// Initialize the final color output
	vec4 final_color = vec4(0.0);

	// For each fragment in the array...
	for (int i = 0; i < frag_count; i++)
	{
		// The color is stored packed into the .y channel of the
		// fragment vector. Unpack it here.
		vec4 frag_color = unpackUnorm4x8(fragments[i].y);

		// Now call the blending function.
		final_color = blend(final_color, frag_color);
	}

	// Done -- return the final color.
        /*if (frag_count > 40)
        {
            return vec4(1.f, 0.f, 0.f, 1.f);
        }
        else if (frag_count > 30)
        {
            return vec4(1.f, 1.f, 0.f, 1.f);
        }
        else if (frag_count > 20)
        {
            return vec4(0.f, 1.f, 0.f, 1.f);
        }
        else if (frag_count > 10)
        {
            return vec4(0.f, 1.f, 1.f, 1.f);
        }
        else if (frag_count > 1)
        {
            return vec4(0.f, 0.f, 1.f, 1.f);
        }
        else
        {
            return vec4(1.f, 1.f, 1.f, 1.f);
        }*/

	return final_color;
}

void main(void)
{
        int frag_count = build_local_fragment_list();
	sort_fragment_list(frag_count);
        output_color = calculate_final_color(frag_count);
}
