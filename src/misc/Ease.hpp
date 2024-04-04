#pragma once



namespace be::misc {

    typedef float (*EaseFunc)(float, float, float, float, float) ;

    // https://www.xuanfengge.com/easeing/easeing/

    class Ease {
    public:

        static float Linear(float t, float b, float c, float d, float s) ;
        static float InSine(float t, float b, float c, float d, float s) ;
        static float OutSine(float t, float b, float c, float d, float s) ;
        static float InOutSine(float t, float b, float c, float d, float s) ;

        static float InQuad(float t, float b, float c, float d, float s) ;
        static float OutQuad(float t, float b, float c, float d, float s) ;
        static float InOutQuad(float t, float b, float c, float d, float s) ;

        static float InCubic(float t, float b, float c, float d, float s) ;
        static float OutCubic(float t, float b, float c, float d, float s) ;
        static float InOutCubic(float t, float b, float c, float d, float s) ;

        static float InQuart(float t, float b, float c, float d, float s) ;
        static float OutQuart(float t, float b, float c, float d, float s) ;
        static float InOutQuart(float t, float b, float c, float d, float s) ;

        static float InQuint(float t, float b, float c, float d, float s) ;
        static float OutQuint(float t, float b, float c, float d, float s) ;
        static float InOutQuint(float t, float b, float c, float d, float s) ;

        static float InExpo(float t, float b, float c, float d, float s) ;
        static float OutExpo(float t, float b, float c, float d, float s) ;
        static float InOutExpo(float t, float b, float c, float d, float s) ;

        static float InCirc(float t, float b, float c, float d, float s) ;
        static float OutCirc(float t, float b, float c, float d, float s) ;
        static float InOutCirc(float t, float b, float c, float d, float s) ;

        static float InBounce(float t, float b, float c, float d, float s) ;
        static float OutBounce(float t, float b, float c, float d, float s) ;
        static float InOutBounce(float t, float b, float c, float d, float s) ;

        static float InBack(float t, float b, float c, float d, float s) ;
        static float OutBack(float t, float b, float c, float d, float s) ;
        static float InOutBack(float t, float b, float c, float d, float s) ;

        static float InElastic(float t, float b, float c, float d, float s) ;
        static float OutElastic(float t, float b, float c, float d, float s) ;
        static float InOutElastic(float t, float b, float c, float d, float s) ;

        static EaseFunc map_name_to_func(const char* name) ;
    };
}
