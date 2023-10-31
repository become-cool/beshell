const lv = require('lv')

const CardHeight = 30
const CardPad =5

class Card extends lv.Label {
    constructor(parent){
        super(parent)
        this.fromJson({
            width: 120 ,
            height: CardHeight ,
            font: "msyh" ,
            style: {
                "bg-opa": 180 ,
                "pad-left": 5 ,
                "bg-color": lv.rgb(220,220,220) ,
                "text-color": lv.rgb(120,120,120) ,
            } ,
            flag: ["clickable"] ,
        })
    }
}

class CardStack extends lv.CleanObj{
    constructor(parent) {
        super(parent)
        this.setWidth(120)

        this.fromJson({
            width: parent.width() - 20 ,
            height: "100%" ,
            x: 20, y: 0 , 
            autoHide: true ,
            style: {
                "pad-left": 8 ,
                "bg-opa": 255 ,
                "bg-color": lv.palette("grey") ,
            } ,
            flag: ["scrollable"] ,
            click: () =>{
                this.toggle()
            }
        })
        this.setScrollDir("ver")
        this.hide()
    }

    cards = []

    contentHeight = 0

    dragCenter = true

    createCard(title, cb, cbData) {
        let card = new Card(this)
        card.setText(title)
        this.addCard(card, cb, cbData)
    }

    addCard(card, cb, cbData) {

        card.setY(this.contentHeight)
        this.contentHeight+= card.height() + CardPad

        let startx, locx, locy
        let draggable = card.draggable()
        draggable.setStart(()=>{
            let {x,y} = lv.inputPoint()
            let [_x,_y] = card.coords()
            startx = _x
            locx = x - _x
            locy = y - _y
        })
        draggable.setDragging((pos)=>{
            if(pos.x<startx){
                pos.x = startx
            }
            pos.y = false
            if(pos.x>70) {
                return false
            }
        })
        draggable.setStop((cusStop)=>{
            card.setX(0)
            if(cusStop) {
                this.hide()
                let obj = cb && cb(cbData)
                if(!obj) {
                    return
                }
                let pos = lv.inputPoint()
                if(this.dragCenter) {
                    obj.updateLayout()
                    obj.setCoords(pos.x-obj.width()/2, pos.y-obj.height()/2)
                }
                else {
                    obj.setCoords(pos.x-locx, pos.y-locy)
                }
                
    
                setTimeout(()=>{        
                    try{
                        lv.fakeIndev(pos.x, pos.y, false)
                        lv.tickIndev()
                        lv.fakeIndev(pos.x, pos.y, true)
                        lv.tickIndev()
                    }catch(e){
                        console.log(e)
                    }
                }, 0)
            }
        })

        this.cards.push(card)
    }
}

module.exports = CardStack