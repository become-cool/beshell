import { setStyle, handleEvent, EVENTTYPE_MAP, styleGetterProp, STYLE_TYPE } from '../config'
import { CommonComponentApi } from '../common/index'

const bridge = globalThis.SJSJSBridge;
const NativeText = bridge.NativeRender.NativeComponents.Text

function setTextProps(comp, newProps, oldProps) {
    const setter = {
        ...CommonComponentApi({ compName: 'Text', comp, newProps, oldProps }),
        children(str) {
            const type = typeof str
            if ((type == 'string' || type == 'number') && oldProps.children !== str) {
                comp.setText(String(str));
            } else if (Array.isArray(str)) {
                const isStringArr = str.every(item => (typeof item === 'string' || typeof item === 'number'))
                if (isStringArr) {
                    comp.setText(str.join(""))
                }
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
  
export class TextComp extends NativeText {
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
        setTextProps(this, newProps, oldProps);
    }
    insertBefore(child, beforeChild) {
    }
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
        setStyle({ comp: this, styleSheet: style, compName: "Text", styleType: type, oldStyleSheet: {}, isInit: false })
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