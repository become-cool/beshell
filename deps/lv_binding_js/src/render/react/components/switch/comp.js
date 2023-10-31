import { setStyle, handleEvent, EVENTTYPE_MAP, styleGetterProp, STYLE_TYPE } from '../config'
import { CommonComponentApi } from '../common/index'

const bridge = globalThis.SJSJSBridge;
const NativeComp = bridge.NativeRender.NativeComponents.Switch

function setSwitchProps(comp, newProps, oldProps) {
    const setter = {
        ...CommonComponentApi({ compName: 'Switch', comp, newProps, oldProps }),
        checkedStyle (styleSheet) {
            setStyle({comp, styleSheet, compName: "Switch", styleType: STYLE_TYPE.STATE_CHECKED, oldStyleSheet: oldProps.checkedStyle});
        },
        onChange (fn) {
            handleEvent(comp, fn, EVENTTYPE_MAP.EVENT_VALUE_CHANGED);
        },
        checked (val) {
            if (isNaN(val)) return
            if (val == oldProps.value) return
            comp.setChecked(val)
        },
        disabled (val) {
            if (val !== oldProps.disabled) {
                comp.setDisabled(val)
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
  
export class SwitchComp extends NativeComp {
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
        setSwitchProps(this, newProps, oldProps);
    }
    insertBefore(child, beforeChild) {
        this.insertChildBefore(child, beforeChild);
    }
    appendInitialChild(child) {
        this.appendChild(child);
    }
    appendChild(child) {
        super.appendChild(child);
    }
    removeChild(child) {
        super.removeChild(child);
    }
    close () {
        super.close()
    }
    setStyle (style, type = 0x0000) {
        setStyle({ comp: this, styleSheet: style, compName: "Switch", styleType: type, oldStyleSheet: {}, isInit: false })
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