/*
* A color class
*/
#include <array>
#pragma once

namespace snail
{
	class color
	{
	public:
		/*
		* Constructor
		*/
		color();

		/*
		* Constructor with values
		*/
		color(float red, float green, float blue, float alpha = 1.f);

        /*
        * Return the aray data
        */
        const float* data() const;

		/*
		* Return the red value
		*/
		float red() const;

		/*
		* Return the green value
		*/
		float green() const;

		/*
		* Return the blue value
		*/
		float blue() const;

		/*
		* Return the alpha value
		*/
		float alpha() const;

	private:
		/*
		* Return the values
		*/
		std::array<float, 4> m_values;
	};
}
