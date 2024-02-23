#pragma once

#include "EventEmitter.hpp"
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

    class Obj: public EventEmitter {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
        
        static lv_event_dsc_t * findEventDsc(lv_obj_t* obj, lv_event_code_t e) ;
        static void eventCallback(lv_event_t * event) ;

    protected:
        lv_obj_t * _lvobj = nullptr ;
        Obj(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;

    public:
        Obj(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static Obj * wrap(JSContext * ctx, lv_obj_t * lvobj) ;
        ~Obj() ;

        lv_obj_t * lvobj() const ;

        static JSValue invalidGetter(JSContext *ctx, JSValueConst this_val) ;
        static JSValue invalidSetter(JSContext *ctx, JSValueConst this_val, JSValueConst value) ;

        static JSValue style(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue center(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue enableEvent(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

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
        static JSValue addFlag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue removeFlag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue updateFlag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue addState(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue removeState(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setState(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue hasFlag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue hasFlagAny(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue hasState(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue allocateSpecAttr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue checkType(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue hasClass(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isValid(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue classCreateObj(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue classInitObj(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isEditable(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isGroupDef(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue initDrawRectDsc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue initDrawLabelDsc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue initDrawImageDsc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue initDrawLineDsc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue initDrawArcDsc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue calculateExtDrawSize(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue refreshExtDrawSize(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue sendEvent(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue eventBase(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue addEventCb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getEventDsc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue removeEvent(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue removeEventCb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue removeEventCbWithUserData(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setPos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setSize(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue refrSize(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isLayoutPositioned(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue markLayoutAsDirty(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue updateLayout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue alignTo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getCoords(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getContentCoords(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue refreshSelfSize(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue refrPos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue moveTo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue moveChildrenBy(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue transformPoint(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getTransformedArea(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue invalidateArea(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue invalidate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue areaIsVisible(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isVisible(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getClickArea(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue hitTest(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setProperty(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setProperties(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getProperty(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getScrollEnd(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue scrollBy(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue scrollByBounded(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue scrollTo(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue scrollToX(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue scrollToY(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue scrollToView(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue scrollToViewRecursive(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue isScrolling(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue updateSnap(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getScrollbarArea(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue scrollbarInvalidate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue readjustScroll(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue addStyle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue replaceStyle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue removeStyle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue removeStyleAll(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue reportStyleChange(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue refreshStyle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue enableStyleRefresh(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getStyleProp(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue hasStyleProp(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setLocalStyleProp(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getLocalStyleProp(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue removeLocalStyleProp(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue fadeIn(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue fadeOut(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue calculateStyleTextAlign(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getStyleOpaRecursive(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleWidth(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleMinWidth(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleMaxWidth(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleHeight(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleMinHeight(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleMaxHeight(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleLength(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleX(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleY(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleAlign(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleTransformWidth(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleTransformHeight(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleTranslateX(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleTranslateY(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleTransformScaleX(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleTransformScaleY(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleTransformRotation(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleTransformPivotX(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleTransformPivotY(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleTransformSkewX(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleTransformSkewY(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStylePadTop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStylePadBottom(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStylePadLeft(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStylePadRight(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStylePadRow(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStylePadColumn(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleMarginTop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleMarginBottom(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleMarginLeft(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleMarginRight(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleBgColor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleBgOpa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleBgGradColor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleBgGradDir(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleBgMainStop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleBgGradStop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleBgMainOpa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleBgGradOpa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleBgGrad(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleBgImageSrc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleBgImageOpa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleBgImageRecolor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleBgImageRecolorOpa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleBgImageTiled(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleBorderColor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleBorderOpa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleBorderWidth(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleBorderSide(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleBorderPost(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleOutlineWidth(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleOutlineColor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleOutlineOpa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleOutlinePad(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleShadowWidth(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleShadowOffsetX(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleShadowOffsetY(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleShadowSpread(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleShadowColor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleShadowOpa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleImageOpa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleImageRecolor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleImageRecolorOpa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleLineWidth(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleLineDashWidth(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleLineDashGap(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleLineRounded(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleLineColor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleLineOpa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleArcWidth(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleArcRounded(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleArcColor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleArcOpa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleArcImageSrc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleTextColor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleTextOpa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleTextFont(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleTextLetterSpace(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleTextLineSpace(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleTextDecor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleTextAlign(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleRadius(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleClipCorner(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleOpa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleOpaLayered(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleColorFilterDsc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleColorFilterOpa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleAnim(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleAnimDuration(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleTransition(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleBlendMode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleLayout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleBaseDir(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleFlexFlow(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleFlexMainPlace(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleFlexCrossPlace(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleFlexTrackPlace(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleFlexGrow(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleGridColumnDscArray(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleGridColumnAlign(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleGridRowDscArray(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleGridRowAlign(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleGridCellColumnPos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleGridCellXAlign(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleGridCellColumnSpan(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleGridCellRowPos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleGridCellYAlign(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setStyleGridCellRowSpan(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue _delete(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue clean(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue deleteDelayed(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue deleteAnimCompletedCb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue deleteAsync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue swap(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue moveToIndex(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getChild(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getChildByType(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getSibling(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getSiblingByType(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getChildCountByType(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue getIndexByType(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue treeWalk(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue dumpTree(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue redraw(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setFlexAlign(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setGridDscArray(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setGridAlign(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setGridCell(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue bindFlagIfEq(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue bindFlagIfNotEq(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue bindStateIfEq(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue bindStateIfNotEq(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------
    } ;

}
