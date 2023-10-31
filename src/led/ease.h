#ifndef ONCE_H_ease
#define ONCE_H_ease



typedef enum {

    Linear ,

    EaseInSine,
    EaseOutSine,
    EaseInOutSine ,

    EaseInQuad ,
    EaseOutQuad ,
    EaseInOutQuad ,
    
    EaseInCubic ,
    EaseOutCubic ,
    EaseInOutCubic ,
    
    EaseInQuart ,
    EaseOutQuart ,
    EaseInOutQuart ,
    
    EaseInQuint ,
    EaseOutQuint ,
    EaseInOutQuint ,

    EaseInExpo ,
    EaseOutExpo ,
    EaseInOutExpo ,

    EaseInCirc ,
    EaseOutCirc ,
    EaseInOutCirc ,

    EaseInBounce ,
    EaseOutBounce ,
    EaseInOutBounce ,
    
    EaseInBack ,
    EaseOutBack ,
    EaseInOutBack ,

    EaseInElastic ,
    EaseOutElastic ,
    EaseInOutElastic ,

} easing_func_t ;

float easeLinear(float t, float b, float c, float d) ;
float easeInSine(float t, float b, float c, float d) ;
float easeOutSine(float t, float b, float c, float d) ;
float easeInOutSine(float t, float b, float c, float d) ;

float easeInQuad(float t, float b, float c, float d) ;
float easeOutQuad(float t, float b, float c, float d) ;
float easeInOutQuad(float t, float b, float c, float d) ;

float easeInCubic(float t, float b, float c, float d) ;
float easeOutCubic(float t, float b, float c, float d) ;
float easeInOutCubic(float t, float b, float c, float d) ;

float easeInQuart(float t, float b, float c, float d) ;
float easeOutQuart(float t, float b, float c, float d) ;
float easeInOutQuart(float t, float b, float c, float d) ;

float easeInQuint(float t, float b, float c, float d) ;
float easeOutQuint(float t, float b, float c, float d) ;
float easeInOutQuint(float t, float b, float c, float d) ;

float easeInExpo(float t, float b, float c, float d) ;
float easeOutExpo(float t, float b, float c, float d) ;
float easeInOutExpo(float t, float b, float c, float d) ;

float easeInCirc(float t, float b, float c, float d) ;
float easeOutCirc(float t, float b, float c, float d) ;
float easeInOutCirc(float t, float b, float c, float d) ;

float easeInBounce(float t, float b, float c, float d) ;
float easeOutBounce(float t, float b, float c, float d) ;
float easeInOutBounce(float t, float b, float c, float d) ;

float easeInBack(float t, float b, float c, float d, float s) ;
float easeOutBack(float t, float b, float c, float d, float s) ;
float easeInOutBack(float t, float b, float c, float d, float s) ;

float easeInElastic(float t, float b, float c, float d, float s) ;
float easeOutElastic(float t, float b, float c, float d, float s) ;
float easeInOutElastic(float t, float b, float c, float d, float s) ;

extern float (*libEaseFuncs[])(float t, float b, float c, float d) ;
extern float (*libEaseExFuncs[])(float t, float b, float c, float d, float s) ;

#endif