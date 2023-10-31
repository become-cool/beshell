const lv = require('lv')
const CardStack = require("../comm/CardStack")

const cardlib = [
    {
        name: 'basic' ,
        icon: 'If' ,
        cards: require("./cards/basic.js")
    } ,
    {
        name: 'parts' ,
        icon: 'H' ,
        cards: [] ,
    } ,
    {
        name: 'UI' ,
        icon: 'ui' ,
        cards: require("./cards/ui.js") ,
    } ,
]

class ProgramTools extends lv.Column{
    
    libCardClass = {}
    categories = {}
    activeCategory = null
    

    constructor(parent, workspace) {
        super(parent)

        this.refs = this.fromJson({
            width: "100%" ,
            height: -1 ,
            align: "top-mid" ,
            style: {
                "pad-row": 5 ,
                "flex-cross-place": "center" ,
            }, 
            children: []
        })

        let self = this
        this.libCardClass = {}
        for(let catconf of cardlib) {
            let category = new lv.Label(this, {
                text: catconf.icon ,
                font: 'm12' ,
                clicked() {
                    if(this.stack.toggle()) {
                        if(self.activeCategory && self.activeCategory!=this) {
                            self.activeCategory.stack.hide()
                        }
                        self.activeCategory = this
                    }
                    else {
                        self.activeCategory = null
                    }
                }
            })
            category.stack = new CardStack(workspace)
            category.stack.dragCenter = false
            for(let name in catconf.cards) {
                let clz = catconf.cards[name]
                let card = new clz(category.stack, null)
                category.stack.addCard(card, (clz)=>{
                    self.activeCategory = null
                    let card = new clz(workspace.program, workspace.program)
                    workspace?.model?.vm?.addCard(card)
                    return card
                }, clz)
            }
            // category.stack.createCard(catconf.name)s
            category.stack.setStyle("pad-left",10)

            for(let className in catconf.cards) {
                let clazz = catconf.cards[className]
                if(clazz.pkgname) {
                    className = clazz.pkgname + '.' + className
                }
                this.libCardClass[className] = clazz
            }

            this.categories[catconf.name] = category
        }
    }

    close() {
        if(this.activeCategory) {
            this.activeCategory.stack.hide()
        }
    }
}

module.exports = ProgramTools