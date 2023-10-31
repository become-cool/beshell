import { ProcessEnum, ProcessPx, ProcessColor, ProcessBoolean } from "../util"

const obj = {
    'overflow': ProcessEnum({
        'hidden': 1,
        'scroll': 0,
        'auto': 0
    }),
    'overflow-scrolling': ProcessEnum({
        'auto': 0,
        'touch': 1
    }),
    'scroll-dir': ProcessEnum({
        'none': 0,
        'left': (1 << 0),
        'right': 1 << 1,
        'top': 1 << 2,
        'bottom': 1 << 3,
        'horizontal': 1 << 0 | 1 << 1,
        'vertical': 1 << 2 | 1 << 3,
        'all': 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3
    }),
    'scroll-snap-x': ProcessEnum({
        'none': 0,
        'snap_start': 1,
        'snap_end': 2,
        'snap_center': 3
    }),
    'scroll-snap-y': ProcessEnum({
        'none': 0,
        'snap_start': 1,
        'snap_end': 2,
        'snap_center': 3
    }),
    'scroll-enable-snap': ProcessBoolean
}
const keys = Object.keys(obj)

export function ScrollStyle (style, result, compName) {
    keys.forEach(key => {
        if (style[key] !== void 0) {
            obj[key](key, style[key], result)
        }
    })
}