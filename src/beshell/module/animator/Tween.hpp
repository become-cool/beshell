#pragma once

// 缓动函数集，C/C++ 兼容（C++ 侧位于 be::animator::tween 命名空间，C 侧为纯 C 符号）
// https://www.xuanfengge.com/easeing/easeing/

#ifdef __cplusplus
namespace be::animator::tween {
extern "C" {
#endif

typedef float (*EaseFunc)(float t, float b, float c, float d, float s) ;

// 缓动函数枚举常量
// C++ 名(如 InQuad)仅是编译期符号，不出现在字符串接口中；
// JS 侧传入 lowerCamelCase 字符串(如 "inQuad")，经 map_name_to_id() 转为以下常量
typedef enum {
    TWEEN_LINEAR = 0,
    TWEEN_IN_SINE, TWEEN_OUT_SINE, TWEEN_IN_OUT_SINE,
    TWEEN_IN_QUAD, TWEEN_OUT_QUAD, TWEEN_IN_OUT_QUAD,
    TWEEN_IN_CUBIC, TWEEN_OUT_CUBIC, TWEEN_IN_OUT_CUBIC,
    TWEEN_IN_QUART, TWEEN_OUT_QUART, TWEEN_IN_OUT_QUART,
    TWEEN_IN_QUINT, TWEEN_OUT_QUINT, TWEEN_IN_OUT_QUINT,
    TWEEN_IN_EXPO, TWEEN_OUT_EXPO, TWEEN_IN_OUT_EXPO,
    TWEEN_IN_CIRC, TWEEN_OUT_CIRC, TWEEN_IN_OUT_CIRC,
    TWEEN_IN_BOUNCE, TWEEN_OUT_BOUNCE, TWEEN_IN_OUT_BOUNCE,
    TWEEN_IN_BACK, TWEEN_OUT_BACK, TWEEN_IN_OUT_BACK,
    TWEEN_IN_ELASTIC, TWEEN_OUT_ELASTIC, TWEEN_IN_OUT_ELASTIC,
    TWEEN_EASE_COUNT,
    TWEEN_EASE_UNKNOWN = -1
} TweenEaseType ;

// JS 命名风格(lowerCamelCase)名字表，顺序与 TweenEaseType 一致，定义在 Animator-js.cpp
extern const char * const jsNames[] ;

// 缓动函数
float Linear(float t, float b, float c, float d, float s) ;
float InSine(float t, float b, float c, float d, float s) ;
float OutSine(float t, float b, float c, float d, float s) ;
float InOutSine(float t, float b, float c, float d, float s) ;

float InQuad(float t, float b, float c, float d, float s) ;
float OutQuad(float t, float b, float c, float d, float s) ;
float InOutQuad(float t, float b, float c, float d, float s) ;

float InCubic(float t, float b, float c, float d, float s) ;
float OutCubic(float t, float b, float c, float d, float s) ;
float InOutCubic(float t, float b, float c, float d, float s) ;

float InQuart(float t, float b, float c, float d, float s) ;
float OutQuart(float t, float b, float c, float d, float s) ;
float InOutQuart(float t, float b, float c, float d, float s) ;

float InQuint(float t, float b, float c, float d, float s) ;
float OutQuint(float t, float b, float c, float d, float s) ;
float InOutQuint(float t, float b, float c, float d, float s) ;

float InExpo(float t, float b, float c, float d, float s) ;
float OutExpo(float t, float b, float c, float d, float s) ;
float InOutExpo(float t, float b, float c, float d, float s) ;

float InCirc(float t, float b, float c, float d, float s) ;
float OutCirc(float t, float b, float c, float d, float s) ;
float InOutCirc(float t, float b, float c, float d, float s) ;

float InBounce(float t, float b, float c, float d, float s) ;
float OutBounce(float t, float b, float c, float d, float s) ;
float InOutBounce(float t, float b, float c, float d, float s) ;

float InBack(float t, float b, float c, float d, float s) ;
float OutBack(float t, float b, float c, float d, float s) ;
float InOutBack(float t, float b, float c, float d, float s) ;

float InElastic(float t, float b, float c, float d, float s) ;
float OutElastic(float t, float b, float c, float d, float s) ;
float InOutElastic(float t, float b, float c, float d, float s) ;

// 枚举常量 -> switch 直接调用对应缓动函数（C/C++ 侧调用走此路径，无字符串转换）
float ease(TweenEaseType id, float t, float b, float c, float d, float s) ;

// 枚举常量 -> 函数指针（供需要保存函数指针的调用方使用）
EaseFunc map_id_to_func(TweenEaseType id) ;

// JS 命名风格字符串(如 "inQuad") -> 枚举常量
// 仅 JS 侧传入字符串时才发生此转换，未知名称返回 TWEEN_EASE_UNKNOWN
TweenEaseType map_name_to_id(const char* name) ;

// JS 命名风格字符串 -> 函数指针，未知名称回退 Linear
EaseFunc map_name_to_func(const char* name) ;

#ifdef __cplusplus
} // extern "C"
} // namespace be::animator::tween
#endif
