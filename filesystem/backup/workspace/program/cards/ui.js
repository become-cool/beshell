const lv = require("lv")
const base = require('./CardBase')

const pkgname = 'ui'
const bgColor = lv.palette("green")

function lstWidgets (program){
    return program?.model?.widgetNames()
}

class UIEvent extends base.CardEvent {
    static pkgname = pkgname
    constructor(parent, program) {
        super(parent, program)
        this.expr.addLabel("UI")
        program?.model?.vm?.on("ui", (widgetName, eventName)=>{
            // console.log(widgetName, eventName, this.expr.slots.widget.value, this.expr.slots.event.value)
            if(widgetName!=this.expr.slots.widget.value || eventName!=this.expr.slots.event.value ) {
                return
            }
            this.run()
        })

        this.expr.addMenu(base.shareMenu("widgets"), "widget", lstWidgets)
        this.expr.addLabel("emit")
        this.expr.addMenu(base.shareMenu("events", ["clicked","pressed","pressing","released"]), "event")
    }
    bgColor() {
        return bgColor
    }
    generate(indent) {
        let wname = JSON.stringify(this.expr.slots.widget.value)
        let event = this.expr.slots.event.value
        let body = ""
        if(this.next) {
            body = this.next.generateQueue(indent+1)
        }
        indent = " ".repeat(indent*4)
        return `${indent}app.ui[${wname}].on("${event}",()=>{
${body}
${indent}})`
    }
}


class UIGetText extends base.CardExpression {
    static pkgname = pkgname
    constructor(parent, program) {
        super(parent, program)
        this.addLabel("text of")
        this.addMenu(base.shareMenu("widgets"), "widget", lstWidgets)
    }
    evaluate() {
        let value = this.slots.widget.value
        let widget = this.program.model.widgets[value]
        if(!widget) {
            return null
        }
        if(!widget.text) {
            return null
        }
        return widget.text()
    }
    bgColor() {
        return bgColor
    }
    generate() {
        let strname = JSON.stringify(this.slots.widget.value)
        return `this.ui[${strname}].text()`
    }
}
class UISetText extends base.CardStatement {
    static pkgname = pkgname
    constructor(parent, program) {
        super(parent, program)
        this.expr.addLabel("set")
        this.expr.addMenu(base.shareMenu("widgets"), "widget", lstWidgets)
        this.expr.addLabel("text to")
        this.expr.addSlot("what")
    }
    run() {
        let wname = this.expr.slots.widget.value
        let widget = this.program.model.widgets[wname]
        if(!widget) {
            this.runNext()
            return
        }
        let value = this.expr.slots.what.evaluate()
        console.log(value)
        widget.setText(value)

        this.runNext()
    }
    bgColor() {
        return bgColor
    }
    generate(indent) {
        let strname = JSON.stringify(this.expr.slots.widget.value)
        let what = this.expr.slots.what.generate()
        indent = " ".repeat(indent*4)
        return `${indent}this.ui[${strname}].setText(${what})`
    }
}

module.exports = {
    UIEvent ,
    UIGetText ,
    UISetText ,
}