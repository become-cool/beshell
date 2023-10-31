const base = require("./cards/CardBase")
const basic = require("./cards/basic")
const utils = require("../comm/utils")
const appTpl = require("../app_tpl")

class VM extends beapi.EventEmitter {

    cards = {}

    constructor() {
        super()
    }

    addCard(card) {
        if(!card.uuid){
            card.uuid = utils.newUUID(this.cards)
        }
        if(this.cards[card.uuid]) {
            throw new Error("uuid of card has exsisted")
        }
        this.cards[card.uuid] = card
    }

    generate() {

        let codeSetup = ''
        let codeEvents = ''
        let codeMain = ''

        for(let uuid in this.cards) {
            let card = this.cards[uuid]
            if(!card.isTop()) {
                continue
            }
            if(card instanceof base.CardEvent) {
                if(card instanceof basic.Setup) {
                    codeSetup+= card.generate(2) + "\r\n\r\n"
                }
                else {
                    codeEvents+= card.generate(2) + "\r\n\r\n"
                }
            }
            else if(card instanceof base.CardStatement) {
                if(card.prev || card.parent() instanceof base.CardStatement) {
                    continue
                }
                codeMain+= card.generateQueue(2) + "\r\n\r\n"
            }
        }

        return appTpl.appjs
                    .replace("%{codeSetup}",codeSetup)
                    .replace("%{codeEvents}",codeEvents)
                    .replace("%{codeMain}",codeMain)
    }

    serialize() {
        let json = {}
        for(let uuid in this.cards) {
            json[uuid] = this.cards[uuid].serialize()
            if(this.cards[uuid].constructor.pkgname) {
                json[uuid].class = this.cards[uuid].constructor.pkgname + '.' + json[uuid].class
            }
        }
        return json
    }

    unserialize(json, libCardClass, workspace) {
        // 创建所有 card
        for(let uuid in json) {
            let cardjson = json[uuid]
            if(!cardjson.class) {
                throw new Error("missing class for card")
            }
            if(!libCardClass[cardjson.class]){
                throw new Error("unknow card class:"+cardjson.class)
            }
            let card = new libCardClass[cardjson.class](workspace.program, workspace.program)
            card.uuid = uuid
            this.addCard(card)
        }
        // 反序列化所有 card
        for(let uuid in json) {
            this.cards[uuid].unserialize(json[uuid], this)
        }

    }
}

module.exports = VM