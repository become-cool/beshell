function NormalizeOpacity (value) {
    if (isNaN(value) || value > 1) return 255
    if (value <= 0) return 0
    return Math.floor(value * 255)
}

export function OpacityStyle (style, result, compName) {
    if (style['opacity'] !== void 0) {
        if (compName === "Image") {
            result['img-opacity'] = NormalizeOpacity(style['opacity'])
        } else {
            result['opacity'] = NormalizeOpacity(style['opacity'])
        }
    }
    if (style['background-opacity'] !== void 0) {
        result['background-opacity'] = NormalizeOpacity(style['background-opacity'])
    }
    if (style['border-opacity'] !== void 0) {
        result['border-opacity'] = NormalizeOpacity(style['border-opacity'])
    }
    if (style['outline-opacity'] !== void 0) {
        result['outline-opacity'] = NormalizeOpacity(style['outline-opacity'])
    } 
    if (style['recolor-opacity'] !== void 0 && compName === 'Image') {
        result['recolor-opacity'] = NormalizeOpacity(style['recolor-opacity'])
    }
    if (style['shadow-opacity'] !== void 0) {
        result['shadow-opacity'] = NormalizeOpacity(style['shadow-opacity'])
    }

    return result
}