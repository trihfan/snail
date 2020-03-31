#include "color.h"

using namespace snail;

color::color()
	: color(1.f, 1.f, 1.f)
{
}

color::color(float red, float green, float blue, float alpha)
	: m_values({ { red, green, blue, alpha } })
{
}

 const float* color::data() const
 {
     return m_values.data();
 }

float color::red() const
{
	return m_values[0];
}

float color::green() const
{
	return m_values[1];
}

float color::blue() const
{
	return m_values[2];
}

float color::alpha() const
{
	return m_values[3];
}
