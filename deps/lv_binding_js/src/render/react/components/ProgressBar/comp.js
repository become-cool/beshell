import { setStyle, handleEvent, styleGetterProp, STYLE_TYPE } from '../config'
import { CommonComponentApi } from '../common/index'

const bridge = globalThis.SJSJSBridge
const NativeProgressBar = bridge.NativeRender.NativeComponents.ProgressBar

function setProgressBarProps(comp, newProps, oldProps) {
    const setter = {
        ...CommonComponentApi({ compName: 'ProgressBar', comp, newProps, oldProps }),
        style(styleSheet) {
            if (newProps.animationTime) {
                styleSheet['style-transition-time'] = newProps.animationTime
            }
            setStyle({ comp, styleSheet, compName: 'ProgressBar', styleType: STYLE_TYPE.PART_MAIN, oldStyleSheet: oldProps.style });
        },
        indicatorStyle (styleSheet) {
            setStyle({ comp, styleSheet, compName: 'ProgressBar', styleType: STYLE_TYPE.PART_INDICATOR, oldStyleSheet: oldProps.style });
        },
        value (value) {
            if (value !== oldProps.value) {
                comp.setValue(value, !!newProps.useAnimation)
            }
        },
        range (arr) {
            if (arr?.[0] !== oldProps?.arr?.[0] || arr?.[1] !== oldProps?.arr?.[1]) {
                comp.setRange(arr[0], arr[1])
            }
        },
        
    }
    Object.keys(setter).forEach(key => {
        if (newProps.hasOwnProperty(key)) {
            setter[key](newProps[key])
        }
    })
    comp.dataset = {}
    Object.keys(newProps).forEach(prop => {
        const index = prop.indexOf('data-')
        if (index === 0) {
            comp.dataset[prop.substring(5)] = newProps[prop]
        }
    })
}
  
export class ProgressBarComp extends NativeProgressBar {
    constructor ({ uid }) {
        super({ uid })
        this.uid = uid

        const style = super.style
        const that = this
        this.style = new Proxy(this, {
            get (obj, prop) {
                if (styleGetterProp.includes(prop)) {
                    return style[prop].call(that)
                }
            }
        })
    }
    setProps(newProps, oldProps) {
        setProgressBarProps(this, newProps, oldProps);
    }
    insertBefore(child, beforeChild) {
    }
    static tagName = "ProgressBar";
    appendInitialChild(child) {
    }
    appendChild(child) {
    }
    removeChild(child) {
    }
    close () {
        super.close()
    }
    setStyle (style, type = 0x0000) {
        setStyle({ comp: this, styleSheet: style, compName: "ProgressBar", styleType: type, oldStyleSheet: null, isInit: false })
    }
    moveToFront () {
        super.moveToFront()
    }
    moveToBackground () {
        super.moveToBackground()
    }
    scrollIntoView () {
        super.scrollIntoView()
    }
}