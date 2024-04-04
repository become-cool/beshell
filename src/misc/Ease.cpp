#include "Ease.hpp"
#include <math.h>
#include <stdint.h>
#include <cstring>
#include <stdbool.h>


#define PI 3.1415926f
const float EPSINON = 0.000001f;
#define EQUAL_ZERO(x) ((x >= - EPSINON) && (x <= EPSINON)) ? true : false
 
namespace be::misc {

// Linear
float Ease::Linear(float t, float b, float c, float d, float s)
{
	return c * t / d + b;
}
 
// Quadratic
float Ease::InQuad(float t, float b, float c, float d, float s)
{
    t /= d ;
	return c * t * t + b;
}
 
float Ease::OutQuad(float t, float b, float c, float d, float s)
{
    t /= d ;
	return -c * t * (t - 2) + b;
}
 
float Ease::InOutQuad(float t, float b, float c, float d, float s)
{
    t /= d ;
	if ((t / 2) < 1)
		return c / 2 * t * t + b;
    --t ;
	return -c / 2 * (t * (t - 2) - 1) + b;
}
 
// Cubic
float Ease::InCubic(float t, float b, float c, float d, float s)
{
    t /= d ;
	return c * t * t * t + b;
}
 
float Ease::OutCubic(float t, float b, float c, float d, float s)
{
    t = t / d - 1 ;
	return c * (t * t * t + 1) + b;
}
 
float Ease::InOutCubic(float t, float b, float c, float d, float s)
{
    t /= d / 2 ;
	if (t < 1)
		return c / 2 * t * t * t + b;
    
    t -= 2 ;
	return c / 2 * (t * t * t + 2) + b;
}
 
// Quartic
float Ease::InQuart(float t, float b, float c, float d, float s)
{
    t /= d ;
	return c * t * t * t * t + b;
}
 
float Ease::OutQuart(float t, float b, float c, float d, float s)
{
    t = t / d - 1 ;
	return -c * (t * t * t * t - 1) + b;
}
 
float Ease::InOutQuart(float t, float b, float c, float d, float s)
{
    t /= d / 2 ;
	if (t < 1)
		return c / 2 * t * t * t * t + b;
    t -= 2 ;
	return -c / 2 * (t * t * t * t - 2) + b;
}
 
// Quintic
float Ease::InQuint(float t, float b, float c, float d, float s)
{
    t /= d ;
	return c * t * t * t * t * t + b;
}
 
float Ease::OutQuint(float t, float b, float c, float d, float s)
{
    t = t / d - 1 ;
	return c * (t* t * t * t * t + 1) + b;
}
 
float Ease::InOutQuint(float t, float b, float c, float d, float s)
{
    t /= d / 2 ;
	if (t < 1)
		return c / 2 * t * t * t * t * t + b;
    t -= 2 ;
	return c / 2 * (t * t * t * t * t + 2) + b;
}
 
// Sinusoidal
float Ease::InSine(float t, float b, float c, float d, float s)
{
	return -c * cos(t / d * (PI / 2)) + c + b;
}
 
float Ease::OutSine(float t, float b, float c, float d, float s)
{
	return c * sin(t / d * (PI / 2)) + b;
}
 
float Ease::InOutSine(float t, float b, float c, float d, float s)
{
	return -c / 2 * (cos(PI * t/ d) - 1) + b;
}
 
// Exponential
float Ease::InExpo(float t, float b, float c, float d, float s)
{
	return (t == 0) ? b : c * pow(2, 10 * (t / d - 1)) + b;
}
 
float Ease::OutExpo(float t, float b, float c, float d, float s)
{
	return (t == d) ? b + c : c * (-pow(2, -10 * t / d) + 1) + b;
}
 
float Ease::InOutExpo(float t, float b, float c, float d, float s)
{
	if (t == 0)
		return b;
	if (t == d)
		return b+c;
    t /= d / 2 ;
	if (t < 1)
		return c / 2 * pow(2, 10 * (t - 1)) + b;
	return c / 2 * (-pow(2, -10 * --t) + 2) + b;
}
 
// Circular
float Ease::InCirc(float t, float b, float c, float d, float s)
{
    t /= d ;
	return -c * (sqrt(1 - t * t) - 1) + b;
}
 
float Ease::OutCirc(float t, float b, float c, float d, float s)
{
    t = t / d - 1 ;
	return c * sqrt(1 - t * t) + b;
}
 
float Ease::InOutCirc(float t, float b, float c, float d, float s)
{
    t /= d / 2 ;
	if (t < 1)
		return -c / 2 * (sqrt(1 - t * t) - 1) + b;
    t -= 2 ;
	return c / 2 * (sqrt(1 - t * t) + 1) + b;
}
 
// Elastic
float Ease::InElastic(float t, float b, float c, float d, float s)
{
	// float s = 0;
	if (t == 0)
		return b;
    t /= d ;
	if (t == 1)
		return b + c;
	float p = d * .3f;
    float a = 0.0 ;
	if (!a || a < fabsf(c))
	{
		a = c;
		s = p/4;
	}
	else
		s = p / (2 * PI) * asin (c / a);
    t -= 1 ;
	return -(a * pow(2, 10 * t) * sin((t * d - s) * (2 * PI) / p)) + b;
}
 
float Ease::OutElastic(float t, float b, float c, float d, float s)
{
	// float s = 0;
	if (t == 0)
		return b;
    t /= d ;
	if (t == 1)
		return (b + c);
	float p = d *.3f;
    float a = 0.0 ;
	if (!a || a < fabsf(c))
	{
		a = c;
		s = p / 4;
	}
	else
		s = p / (2 * PI) * sin((float)c / a);
 
	return (a * pow(2.0f, (int) - 10 * t) * sin((t * d - s) * (2 * PI) / p) + c + b);
}
 
float Ease::InOutElastic(float t, float b, float c, float d, float s)
{
	// float s = 0;
	if (t == 0)
		return b;
	if ((t /= d / 2) == 2)
		return b + c;
	float p = d * (.3f * 1.5f);
    float a = 0.0 ;
	if (!a || a < fabsf(c))
	{
		a = c;
		s = p/4;
	}
	else
		s = p / (2 * PI) * asin (c / a);
    
    t -= 1 ;
	if (t < 1)
		return -.5f * (a * pow(2, 10 * t) * sin( (t * d - s) * (2 * PI) / p )) + b;
	return a * pow(2, -10 * t) * sin((t * d - s) * (2 * PI) / p ) * .5f + c + b;
}
 
// Back
float Ease::InBack(float t, float b, float c, float d, float s)
{
	// float s = 1.70158f;
    t /= d ;
	return c * t * t * ((s + 1) * t - s) + b;
}
 
float Ease::OutBack(float t, float b, float c, float d, float s)
{
	// float s = 1.70158f;
    t = t / d - 1 ;
	return c * (t * t * ((s + 1) * t + s) + 1) + b;
}
 
float Ease::InOutBack(float t, float b, float c, float d, float s)
{
	// float s = 1.70158f;
    t /= d / 2 ;
    s *= (1.525f) ;
	if (t < 1) {
		return c / 2 * (t * t * ((s + 1) * t - s)) + b;
    }
    t -= 2 ;
	return c / 2 * (t * t * ((s + 1) * t + s) + 2) + b;
}
 
// Bounce
float Ease::OutBounce(float t, float b, float c, float d, float s)
{
	if ((t /= d) < (1 / 2.75))
	{
		return c * (7.5625f * t * t) + b;
	}
	else if (t < (2 / 2.75))
	{
        t -= (1.5f / 2.75f) ;
		return c * (7.5625f * t * t + .75f) + b;
	}
	else if (t < (2.5 / 2.75))
	{
        t -= (2.25f / 2.75f) ;
		return c * (7.5625f * t * t + .9375f) + b;
	}
	else
	{
        t -= (2.625f / 2.75f) ;
		return c * (7.5625f * t * t + .984375f) + b;
	}
}
 
float Ease::InBounce(float t, float b, float c, float d, float s)
{
	return c - OutBounce(d - t, 0, c, d, 0.0f) + b;
}
 
float Ease::InOutBounce(float t, float b, float c, float d, float s)
{
	if (t < d / 2)
		return InBounce(t * 2, 0, c, d, 0.0f) * .5f + b;
	else return OutBounce(t * 2 - d, 0, c, d, 0.0f) * .5f + c * .5f + b;
}

EaseFunc Ease::map_name_to_func(const char* name) {
	if (strcmp(name, "Linear") == 0) {
		return Linear;
	}
	else if (strcmp(name, "InQuad") == 0) {
		return InQuad;
	}
	else if (strcmp(name, "OutQuad") == 0) {
		return OutQuad;
	}
	else if (strcmp(name, "InOutQuad") == 0) {
		return InOutQuad;
	}
	else if (strcmp(name, "InCubic") == 0) {
		return InCubic;
	}
	else if (strcmp(name, "OutCubic") == 0) {
		return OutCubic;
	}
	else if (strcmp(name, "InOutCubic") == 0) {
		return InOutCubic;
	}
	else if (strcmp(name, "InQuart") == 0) {
		return InQuart;
	}
	else if (strcmp(name, "OutQuart") == 0) {
		return OutQuart;
	}
	else if (strcmp(name, "InOutQuart") == 0) {
		return InOutQuart;
	}
	else if (strcmp(name, "InQuint") == 0) {
		return InQuint;
	}
	else if (strcmp(name, "OutQuint") == 0) {
		return OutQuint;
	}
	else if (strcmp(name, "InOutQuint") == 0) {
		return InOutQuint;
	}
	else if (strcmp(name, "InSine") == 0) {
		return InSine;
	}
	else if (strcmp(name, "OutSine") == 0) {
		return OutSine;
	}	
	else if (strcmp(name, "InOutSine") == 0) {
		return InOutSine;
	}
	else if (strcmp(name, "InExpo") == 0) {
		return InExpo;
	}
	else if (strcmp(name, "OutExpo") == 0) {
		return OutExpo;
	}
	else if (strcmp(name, "InOutExpo") == 0) {
		return InOutExpo;
	}
	else if (strcmp(name, "InCirc") == 0) {
		return InCirc;
	}
	else if (strcmp(name, "OutCirc") == 0) {
		return OutCirc;
	}
	else if (strcmp(name, "InOutCirc") == 0) {
		return InOutCirc;
	}
	else if (strcmp(name, "InElastic") == 0) {
		return InElastic;
	}
	else if (strcmp(name, "OutElastic") == 0) {
		return OutElastic;
	}
	else if (strcmp(name, "InOutElastic") == 0) {
		return InOutElastic;
	}
	else if (strcmp(name, "InBack") == 0) {
		return InBack;
	}
	else if (strcmp(name, "OutBack") == 0) {
		return OutBack;
	}
	else if (strcmp(name, "InOutBack") == 0) {
		return InOutBack;
	}
	else if (strcmp(name, "InBounce") == 0) {
		return InBounce;
	}
	else if (strcmp(name, "OutBounce") == 0) {
		return OutBounce;
	}
	else if (strcmp(name, "InOutBounce") == 0) {
		return InOutBounce;
	}
	else {
		return Linear;
	}
	return nullptr ;
}

}