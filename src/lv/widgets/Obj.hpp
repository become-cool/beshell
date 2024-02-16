#pragma once

#include "NativeClass.hpp"
#include "lvgl.h"
#include "../const.hpp"


#define JSVALUE_TO_LVOBJ(jsval, cvar)               \
    lv_obj_t * cvar = nullptr ;                     \
    {                                               \
        JSVALUE_TO_NCLASS(be::lv::Obj, jsval, tmp)  \
        cvar = tmp->lvobj() ;                       \
    }

#define JSVALUE_TO_LVOBJ_VAR(jsval, cvar)           \
    {                                               \
        JSVALUE_TO_NCLASS(be::lv::Obj, jsval, tmp)  \
        cvar = tmp->lvobj() ;                       \
    }

namespace be::lv {

    class Obj: public NativeClass {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    protected:
        lv_obj_t * _lvobj = nullptr ;
        Obj(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;

    public:
        Obj(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static Obj * wrap(JSContext * ctx, lv_obj_t * lvobj) ;

        lv_obj_t * lvobj() const ;

        static JSValue invalidGetter(JSContext *ctx, JSValueConst this_val) ;
        static JSValue invalidSetter(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;

// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue getState(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getEventCount(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getX(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setX(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getY(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setY(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getWidth(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setWidth(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getHeight(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setHeight(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getContentWidth(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setContentWidth(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getContentHeight(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setContentHeight(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setLayout(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setAlign(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getXAligned(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getYAligned(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getSelfWidth(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getSelfHeight(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setExtClickArea(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getScrollbarMode(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setScrollbarMode(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getScrollDir(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setScrollDir(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getScrollSnapX(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setScrollSnapX(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getScrollSnapY(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setScrollSnapY(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getScrollX(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getScrollY(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getScrollTop(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getScrollBottom(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getScrollLeft(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getScrollRight(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getParent(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setParent(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue getScreen(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getChildCount(JSContext *ctx, JSValueConst this_val) ;
        static JSValue getIndex(JSContext *ctx, JSValueConst this_val) ;
        static JSValue setFlexFlow(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
        static JSValue setFlexGrow(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue jsAddFlag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsRemoveFlag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsUpdateFlag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsAddState(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsRemoveState(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsHasFlag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsHasFlagAny(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsHasState(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsAllocateSpecAttr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsCheckType(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsHasClass(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsIsValid(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsClassCreateObj(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsClassInitObj(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsIsEditable(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsIsGroupDef(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsInitDrawRectDsc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsInitDrawLabelDsc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsInitDrawImageDsc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsInitDrawLineDsc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsInitDrawArcDsc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsCalculateExtDrawSize(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsRefreshExtDrawSize(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsSendEvent(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsEventBase(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsAddEventCb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsRemoveEvent(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsRemoveEventCb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsRemoveEventCbWithUserData(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsRefrSize(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsIsLayoutPositioned(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsMarkLayoutAsDirty(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsUpdateLayout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsAlignTo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsRefreshSelfSize(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsRefrPos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsMoveTo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsMoveChildrenBy(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsTransformPoint(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsInvalidateArea(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsInvalidate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsAreaIsVisible(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsIsVisible(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsHitTest(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsScrollBy(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsScrollByBounded(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsScrollTo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsScrollToX(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsScrollToY(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsScrollToView(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsScrollToViewRecursive(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsIsScrolling(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsUpdateSnap(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsScrollbarInvalidate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsReadjustScroll(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsAddStyle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsReplaceStyle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsRemoveStyle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsRemoveStyleAll(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsReportStyleChange(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsRefreshStyle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsEnableStyleRefresh(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsHasStyleProp(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsRemoveLocalStyleProp(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsFadeIn(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsFadeOut(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsCalculateStyleTextAlign(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsDelete(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsClean(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsDeleteDelayed(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsDeleteAnimCompletedCb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsDeleteAsync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsSwap(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsMoveToIndex(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsTreeWalk(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsDumpTree(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsRedraw(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsBindFlagIfEq(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsBindFlagIfNotEq(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsBindStateIfEq(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue jsBindStateIfNotEq(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------
    } ;

}
