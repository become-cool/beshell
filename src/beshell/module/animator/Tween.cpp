#include "Tween.hpp"
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>


#define PI 3.1415926f
const float EPSINON = 0.000001f;
#define EQUAL_ZERO(x) ((x >= - EPSINON) && (x <= EPSINON)) ? true : false

#ifdef __cplusplus
namespace be::animator::tween {
#endif

// Linear
float Linear(float t, float b, float c, float d, float s)
{
	return c * t / d + b;
}
 
// Quadratic
float InQuad(float t, float b, float c, float d, float s)
{
    t /= d ;
	return c * t * t + b;
}
 
float OutQuad(float t, float b, float c, float d, float s)
{
    t /= d ;
	return -c * t * (t - 2) + b;
}
 
float InOutQuad(float t, float b, float c, float d, float s)
{
    t /= d ;
	if ((t / 2) < 1)
		return c / 2 * t * t + b;
    --t ;
	return -c / 2 * (t * (t - 2) - 1) + b;
}
 
// Cubic
float InCubic(float t, float b, float c, float d, float s)
{
    t /= d ;
	return c * t * t * t + b;
}
 
float OutCubic(float t, float b, float c, float d, float s)
{
    t = t / d - 1 ;
	return c * (t * t * t + 1) + b;
}
 
float InOutCubic(float t, float b, float c, float d, float s)
{
    t /= d / 2 ;
	if (t < 1)
		return c / 2 * t * t * t + b;
    
    t -= 2 ;
	return c / 2 * (t * t * t + 2) + b;
}
 
// Quartic
float InQuart(float t, float b, float c, float d, float s)
{
    t /= d ;
	return c * t * t * t * t + b;
}
 
float OutQuart(float t, float b, float c, float d, float s)
{
    t = t / d - 1 ;
	return -c * (t * t * t * t - 1) + b;
}
 
float InOutQuart(float t, float b, float c, float d, float s)
{
    t /= d / 2 ;
	if (t < 1)
		return c / 2 * t * t * t * t + b;
    t -= 2 ;
	return -c / 2 * (t * t * t * t - 2) + b;
}
 
// Quintic
float InQuint(float t, float b, float c, float d, float s)
{
    t /= d ;
	return c * t * t * t * t * t + b;
}
 
float OutQuint(float t, float b, float c, float d, float s)
{
    t = t / d - 1 ;
	return c * (t* t * t * t * t + 1) + b;
}
 
float InOutQuint(float t, float b, float c, float d, float s)
{
    t /= d / 2 ;
	if (t < 1)
		return c / 2 * t * t * t * t * t + b;
    t -= 2 ;
	return c / 2 * (t * t * t * t * t + 2) + b;
}
 
// Sinusoidal
float InSine(float t, float b, float c, float d, float s)
{
	return -c * cos(t / d * (PI / 2)) + c + b;
}
 
float OutSine(float t, float b, float c, float d, float s)
{
	return c * sin(t / d * (PI / 2)) + b;
}
 
float InOutSine(float t, float b, float c, float d, float s)
{
	return -c / 2 * (cos(PI * t/ d) - 1) + b;
}
 
// Exponential
float InExpo(float t, float b, float c, float d, float s)
{
	return (t == 0) ? b : c * pow(2, 10 * (t / d - 1)) + b;
}
 
float OutExpo(float t, float b, float c, float d, float s)
{
	return (t == d) ? b + c : c * (-pow(2, -10 * t / d) + 1) + b;
}
 
float InOutExpo(float t, float b, float c, float d, float s)
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
float InCirc(float t, float b, float c, float d, float s)
{
    t /= d ;
	return -c * (sqrt(1 - t * t) - 1) + b;
}
 
float OutCirc(float t, float b, float c, float d, float s)
{
    t = t / d - 1 ;
	return c * sqrt(1 - t * t) + b;
}
 
float InOutCirc(float t, float b, float c, float d, float s)
{
    t /= d / 2 ;
	if (t < 1)
		return -c / 2 * (sqrt(1 - t * t) - 1) + b;
    t -= 2 ;
	return c / 2 * (sqrt(1 - t * t) + 1) + b;
}
 
// Elastic
float InElastic(float t, float b, float c, float d, float s)
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
 
float OutElastic(float t, float b, float c, float d, float s)
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
 
float InOutElastic(float t, float b, float c, float d, float s)
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
float InBack(float t, float b, float c, float d, float s)
{
	// float s = 1.70158f;
    t /= d ;
	return c * t * t * ((s + 1) * t - s) + b;
}
 
float OutBack(float t, float b, float c, float d, float s)
{
	// float s = 1.70158f;
    t = t / d - 1 ;
	return c * (t * t * ((s + 1) * t + s) + 1) + b;
}
 
float InOutBack(float t, float b, float c, float d, float s)
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
float OutBounce(float t, float b, float c, float d, float s)
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
 
float InBounce(float t, float b, float c, float d, float s)
{
	return c - OutBounce(d - t, 0, c, d, 0.0f) + b;
}
 
float InOutBounce(float t, float b, float c, float d, float s)
{
	if (t < d / 2)
		return InBounce(t * 2, 0, c, d, 0.0f) * .5f + b;
	else return OutBounce(t * 2 - d, 0, c, d, 0.0f) * .5f + c * .5f + b;
}

// 枚举常量 -> switch 直接调用对应缓动函数，未知 id 回退 Linear
float ease(TweenEaseType id, float t, float b, float c, float d, float s) {
	switch(id) {
		case TWEEN_IN_SINE:       return InSine(t, b, c, d, s) ;
		case TWEEN_OUT_SINE:      return OutSine(t, b, c, d, s) ;
		case TWEEN_IN_OUT_SINE:   return InOutSine(t, b, c, d, s) ;
		case TWEEN_IN_QUAD:       return InQuad(t, b, c, d, s) ;
		case TWEEN_OUT_QUAD:      return OutQuad(t, b, c, d, s) ;
		case TWEEN_IN_OUT_QUAD:   return InOutQuad(t, b, c, d, s) ;
		case TWEEN_IN_CUBIC:      return InCubic(t, b, c, d, s) ;
		case TWEEN_OUT_CUBIC:     return OutCubic(t, b, c, d, s) ;
		case TWEEN_IN_OUT_CUBIC:  return InOutCubic(t, b, c, d, s) ;
		case TWEEN_IN_QUART:      return InQuart(t, b, c, d, s) ;
		case TWEEN_OUT_QUART:     return OutQuart(t, b, c, d, s) ;
		case TWEEN_IN_OUT_QUART:  return InOutQuart(t, b, c, d, s) ;
		case TWEEN_IN_QUINT:      return InQuint(t, b, c, d, s) ;
		case TWEEN_OUT_QUINT:     return OutQuint(t, b, c, d, s) ;
		case TWEEN_IN_OUT_QUINT:  return InOutQuint(t, b, c, d, s) ;
		case TWEEN_IN_EXPO:       return InExpo(t, b, c, d, s) ;
		case TWEEN_OUT_EXPO:      return OutExpo(t, b, c, d, s) ;
		case TWEEN_IN_OUT_EXPO:   return InOutExpo(t, b, c, d, s) ;
		case TWEEN_IN_CIRC:       return InCirc(t, b, c, d, s) ;
		case TWEEN_OUT_CIRC:      return OutCirc(t, b, c, d, s) ;
		case TWEEN_IN_OUT_CIRC:   return InOutCirc(t, b, c, d, s) ;
		case TWEEN_IN_BOUNCE:     return InBounce(t, b, c, d, s) ;
		case TWEEN_OUT_BOUNCE:    return OutBounce(t, b, c, d, s) ;
		case TWEEN_IN_OUT_BOUNCE: return InOutBounce(t, b, c, d, s) ;
		case TWEEN_IN_BACK:       return InBack(t, b, c, d, s) ;
		case TWEEN_OUT_BACK:      return OutBack(t, b, c, d, s) ;
		case TWEEN_IN_OUT_BACK:   return InOutBack(t, b, c, d, s) ;
		case TWEEN_IN_ELASTIC:    return InElastic(t, b, c, d, s) ;
		case TWEEN_OUT_ELASTIC:   return OutElastic(t, b, c, d, s) ;
		case TWEEN_IN_OUT_ELASTIC:return InOutElastic(t, b, c, d, s) ;
		case TWEEN_LINEAR:
		default:                  return Linear(t, b, c, d, s) ;
	}
}

// 枚举常量 -> 缓动函数指针
EaseFunc map_id_to_func(TweenEaseType id) {
	switch(id) {
		case TWEEN_IN_SINE:       return InSine ;
		case TWEEN_OUT_SINE:      return OutSine ;
		case TWEEN_IN_OUT_SINE:   return InOutSine ;
		case TWEEN_IN_QUAD:       return InQuad ;
		case TWEEN_OUT_QUAD:      return OutQuad ;
		case TWEEN_IN_OUT_QUAD:   return InOutQuad ;
		case TWEEN_IN_CUBIC:      return InCubic ;
		case TWEEN_OUT_CUBIC:     return OutCubic ;
		case TWEEN_IN_OUT_CUBIC:  return InOutCubic ;
		case TWEEN_IN_QUART:      return InQuart ;
		case TWEEN_OUT_QUART:     return OutQuart ;
		case TWEEN_IN_OUT_QUART:  return InOutQuart ;
		case TWEEN_IN_QUINT:      return InQuint ;
		case TWEEN_OUT_QUINT:     return OutQuint ;
		case TWEEN_IN_OUT_QUINT:  return InOutQuint ;
		case TWEEN_IN_EXPO:       return InExpo ;
		case TWEEN_OUT_EXPO:      return OutExpo ;
		case TWEEN_IN_OUT_EXPO:   return InOutExpo ;
		case TWEEN_IN_CIRC:       return InCirc ;
		case TWEEN_OUT_CIRC:      return OutCirc ;
		case TWEEN_IN_OUT_CIRC:   return InOutCirc ;
		case TWEEN_IN_BOUNCE:     return InBounce ;
		case TWEEN_OUT_BOUNCE:    return OutBounce ;
		case TWEEN_IN_OUT_BOUNCE: return InOutBounce ;
		case TWEEN_IN_BACK:       return InBack ;
		case TWEEN_OUT_BACK:      return OutBack ;
		case TWEEN_IN_OUT_BACK:   return InOutBack ;
		case TWEEN_IN_ELASTIC:    return InElastic ;
		case TWEEN_OUT_ELASTIC:   return OutElastic ;
		case TWEEN_IN_OUT_ELASTIC:return InOutElastic ;
		case TWEEN_LINEAR:
		default:                  return Linear ;
	}
}

// JS 命名风格字符串 -> 枚举常量（仅 JS 侧传入字符串时才发生此转换）
TweenEaseType map_name_to_id(const char* name) {
	for(int i=0; i<TWEEN_EASE_COUNT; i++) {
		if (strcmp(name, jsNames[i]) == 0) {
			return (TweenEaseType) i ;
		}
	}
	return TWEEN_EASE_UNKNOWN ;
}

// JS 命名风格字符串 -> 缓动函数指针，未知名称回退 Linear
EaseFunc map_name_to_func(const char* name) {
	return map_id_to_func(map_name_to_id(name)) ;
}

#ifdef __cplusplus
} // namespace be::animator::tween
#endif

