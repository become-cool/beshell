import { setStyle, handleEvent, styleGetterProp, EVENTTYPE_MAP } from '../config'
import { CommonComponentApi } from '../common/index'

const bridge = globalThis.SJSJSBridge;
const NativeComp = bridge.NativeRender.NativeComponents.Window

function setWindowProps(comp, newProps, oldProps) {
    const setter = {
        ...CommonComponentApi({ compName: 'Window', comp, newProps, oldProps }),
        title (title) {
            if (oldProps.title != title) {
                comp.setTitle(title);
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

export class Window extends NativeComp {
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
        setWindowProps(this, newProps, oldProps);
    }
    insertBefore(child, beforeChild) {
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
        setStyle({ comp: this, styleSheet: style, compName: "Window", styleType: type, oldStyleSheet: {}, isInit: false })
    }
}