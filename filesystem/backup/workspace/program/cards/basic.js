const base = require('./CardBase')

const bgColor = lv.palette("orange")
const pkgname = 'basic'


class Compare extends base.CardCompare {
    static pkgname = pkgname
    constructor(parent, program) {
        super(parent, program)
        this.addSlot("left").input.numeric = true
        this.addMenu(base.shareMenu("compare-op", ["<","<=","==","=>",">"]), "op")
        this.addSlot("right").input.numeric = true
    }
    bgColor() {
        return bgColor
    }
}

class IsTrue extends base.CardCompare {
    static pkgname = pkgname
    constructor(parent, program) {
        super(parent, program)
        this.addSlot("expr", true)
        this.addLabel("is true")
    }
    bgColor() {
        return bgColor
    }
}
class IsFalse extends base.CardCompare {
    static pkgname = pkgname
    constructor(parent, program) {
        super(parent, program)
        this.addSlot("expr", true)
        this.addLabel("is false")
    }
    bgColor() {
        return bgColor
    }
}

class OperatorTwo extends base.CardExpression {
    static pkgname = pkgname
    constructor(parent, program) {
        super(parent, program)
        this.addSlot("left").input.numeric = true
        this.addMenu(base.shareMenu("operators", ["+","-","*","/","**","^","|","&"]), "op")
        this.addSlot("right").input.numeric = true
    }
    evaluate() {
        let left = new Number(this.slots.left.evaluate())
        let right = new Number(this.slots.right.evaluate())
        let op = this.slots.op.value
        switch (op) {
            case '+': {
                return left + right
            }
            case '-': {
                return left - right
            }
            case '*': {
                return left * right
            }
            case '/': {
                return left / right
            }
            case '**': {
                return left ** right
            }
            case '^': {
                return left ^ right
            }
            case '|': {
                return left | right
            }
            case '&': {
                return left & right
            }
        }
        return NaN
    }
    generate() {
        let left = this.slots.left.generate()
        let right = this.slots.right.generate()
        let op = this.slots.op.value
        return `Number(${left}) ${op} Number(${right})`
    }
}

class MathFunctions extends base.CardExpression {
    static pkgname = pkgname
    constructor(parent, program) {
        super(parent, program)
        this.addLabel("call math function")
        this.addMenu(base.shareMenu("mathf", [
            "sin","cos","tan","asin","acos","atan",
            "cbrt", "sqrt",
            "abs", "floor", "ceil", "round", "random", "trunc" ,
            "log", "log10", "log2",

        ]), "func")
        this.addSlot("argv").input.numeric = true
    }
    evaluate() {
        let argv = new Number(this.slots.argv.evaluate())
        let func = this.slots.func.value
        if(Math[func]) {
            return Math[func](argv)
        }
        return NaN
    }
}

class If extends base.CardControl {
    static pkgname = pkgname
    constructor(parent, program) {
        super(parent, program)
        this.expr.addLabel("If")
        this.expr.addCompareSlot("what")
    }
    run() {
        let result = this.expr.slots.what.evaluate()
        if(result) {
            this.proc1.run()
        }
        this.runNext()
    }
    generate(indent) {
        let what = this.expr.slots.what.generate()
        let body = this.proc1.first? this.proc1.first.generateQueue(indent+1): "" ;
        indent = " ".repeat(indent*4)
        return `${indent}if(${what}){
${body}
${indent}}`
    }
}
class IfElse extends base.CardControl {
    static pkgname = pkgname
    constructor(parent, program) {
        super(parent, program)
        this.expr.addLabel("If-Else")
        this.expr.addCompareSlot("what")

        this.proc2 = this.createProcess()
    }
    run() {
        let result = this.expr.slots.what.evaluate()
        if(result) {
            this.proc1.run()
        }
        else {
            this.proc2.run()
        }
        this.runNext()
    }
    generate() {
        let what = this.expr.slots.what.generate()
        let body1 = this.proc1.first? this.proc1.first.generateQueue(indent+1): "" ;
        let body12 = this.proc2?.first? this.proc2.first.generateQueue(indent+1): "" ;
        indent = " ".repeat(indent*4)
        return `${indent}if(${what}){
${body1}
${indent}} else {
${body2}
${indent}}`
    }
}

class IsNumber extends base.CardCompare {
    static pkgname = pkgname
    constructor(parent, program) {
        super(parent, program)
        this.addSlot("what")
        this.addMenu(base.shareMenu("isornot", ["is","is not"]), "op")
        this.addLabel("a number")
    }
    evaluate() {
        let what = this.slots.what.evaluate()
        if(this.slots.op.value=='is') {
            return ! isNaN(what)
        }
        else if(this.slots.op.value=='is not') {
            return isNaN(what)
        }
        return false
    }
    
    generate() {
        let is = this.slots.op.value=='is'? "!": ""
        return `${is}isNaN(${this.slots.what.generate()})`
    }
}

class Setup extends base.CardEvent {
    static pkgname = pkgname
    constructor(parent, program) {
        super(parent, program)
        this.expr.addLabel("When app setup")
    }
}

module.exports = {
    OperatorTwo ,
    If ,
    IfElse ,
    IsNumber ,
    Compare ,
    IsTrue ,
    IsFalse ,
    MathFunctions ,
    Setup ,
}