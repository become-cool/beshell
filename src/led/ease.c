#include "ease.h"
#include <math.h>
#include <stdint.h>
#include <stdbool.h>


#define PI 3.1415926f
const float EPSINON = 0.000001f;
#define EQUAL_ZERO(x) ((x >= - EPSINON) && (x <= EPSINON)) ? true : false
 
// Linear
float easeLinear(float t, float b, float c, float d)
{
	return c * t / d + b;
}
 
// Quadratic
float easeInQuad(float t, float b, float c, float d)
{
    t /= d ;
	return c * t * t + b;
}
 
float easeOutQuad(float t, float b, float c, float d)
{
    t /= d ;
	return -c * t * (t - 2) + b;
}
 
float easeInOutQuad(float t, float b, float c, float d)
{
    t /= d ;
	if ((t / 2) < 1)
		return c / 2 * t * t + b;
    --t ;
	return -c / 2 * (t * (t - 2) - 1) + b;
}
 
// Cubic
float easeInCubic(float t, float b, float c, float d)
{
    t /= d ;
	return c * t * t * t + b;
}
 
float easeOutCubic(float t, float b, float c, float d)
{
    t = t / d - 1 ;
	return c * (t * t * t + 1) + b;
}
 
float easeInOutCubic(float t, float b, float c, float d)
{
    t /= d / 2 ;
	if (t < 1)
		return c / 2 * t * t * t + b;
    
    t -= 2 ;
	return c / 2 * (t * t * t + 2) + b;
}
 
// Quartic
float easeInQuart(float t, float b, float c, float d)
{
    t /= d ;
	return c * t * t * t * t + b;
}
 
float easeOutQuart(float t, float b, float c, float d)
{
    t = t / d - 1 ;
	return -c * (t * t * t * t - 1) + b;
}
 
float easeInOutQuart(float t, float b, float c, float d)
{
    t /= d / 2 ;
	if (t < 1)
		return c / 2 * t * t * t * t + b;
    t -= 2 ;
	return -c / 2 * (t * t * t * t - 2) + b;
}
 
// Quintic
float easeInQuint(float t, float b, float c, float d)
{
    t /= d ;
	return c * t * t * t * t * t + b;
}
 
float easeOutQuint(float t, float b, float c, float d)
{
    t = t / d - 1 ;
	return c * (t* t * t * t * t + 1) + b;
}
 
float easeInOutQuint(float t, float b, float c, float d)
{
    t /= d / 2 ;
	if (t < 1)
		return c / 2 * t * t * t * t * t + b;
    t -= 2 ;
	return c / 2 * (t * t * t * t * t + 2) + b;
}
 
// Sinusoidal
float easeInSine(float t, float b, float c, float d)
{
	return -c * cos(t / d * (PI / 2)) + c + b;
}
 
float easeOutSine(float t, float b, float c, float d)
{
	return c * sin(t / d * (PI / 2)) + b;
}
 
float easeInOutSine(float t, float b, float c, float d)
{
	return -c / 2 * (cos(PI * t/ d) - 1) + b;
}
 
// Exponential
float easeInExpo(float t, float b, float c, float d)
{
	return (t == 0) ? b : c * pow(2, 10 * (t / d - 1)) + b;
}
 
float easeOutExpo(float t, float b, float c, float d)
{
	return (t == d) ? b + c : c * (-pow(2, -10 * t / d) + 1) + b;
}
 
float easeInOutExpo(float t, float b, float c, float d)
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
float easeInCirc(float t, float b, float c, float d)
{
    t /= d ;
	return -c * (sqrt(1 - t * t) - 1) + b;
}
 
float easeOutCirc(float t, float b, float c, float d)
{
    t = t / d - 1 ;
	return c * sqrt(1 - t * t) + b;
}
 
float easeInOutCirc(float t, float b, float c, float d)
{
    t /= d / 2 ;
	if (t < 1)
		return -c / 2 * (sqrt(1 - t * t) - 1) + b;
    t -= 2 ;
	return c / 2 * (sqrt(1 - t * t) + 1) + b;
}
 
// Elastic
float easeInElastic(float t, float b, float c, float d, float s)
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
 
float easeOutElastic(float t, float b, float c, float d, float s)
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
 
float easeInOutElastic(float t, float b, float c, float d, float s)
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
float easeInBack(float t, float b, float c, float d, float s)
{
	// float s = 1.70158f;
    t /= d ;
	return c * t * t * ((s + 1) * t - s) + b;
}
 
float easeOutBack(float t, float b, float c, float d, float s)
{
	// float s = 1.70158f;
    t = t / d - 1 ;
	return c * (t * t * ((s + 1) * t + s) + 1) + b;
}
 
float easeInOutBack(float t, float b, float c, float d, float s)
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
float easeOutBounce(float t, float b, float c, float d)
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
 
float easeInBounce(float t, float b, float c, float d)
{
	return c - easeOutBounce(d - t, 0, c, d) + b;
}
 
float easeInOutBounce(float t, float b, float c, float d)
{
	if (t < d / 2)
		return easeInBounce(t * 2, 0, c, d) * .5f + b;
	else return easeOutBounce(t * 2 - d, 0, c, d) * .5f + c * .5f + b;
}


float (*libEaseFuncs[])(float t, float b, float c, float d) = {

    easeLinear ,

    easeInSine,
    easeOutSine,
    easeInOutSine ,

    easeInQuad ,
    easeOutQuad ,
    easeInOutQuad ,
    
    easeInCubic ,
    easeOutCubic ,
    easeInOutCubic ,
    
    easeInQuart ,
    easeOutQuart ,
    easeInOutQuart ,
    
    easeInQuint ,
    easeOutQuint ,
    easeInOutQuint ,

    easeInExpo ,
    easeOutExpo ,
    easeInOutExpo ,

    easeInCirc ,
    easeOutCirc ,
    easeInOutCirc ,

    easeInBounce ,
    easeOutBounce ,
    easeInOutBounce ,
};

float (*libEaseExFuncs[])(float t, float b, float c, float d, float s) = {
    easeInBack ,
    easeOutBack ,
    easeInOutBack ,

    easeInElastic ,
    easeOutElastic ,
    easeInOutElastic ,
};
