function newUUID(pool) {
    let uuid = beapi.utils.genUUID()
    uuid = uuid.replace(/\-/g, '')
    for(let l=4;l<uuid.length;l++) {
        let seg = uuid.substr(0,l)
        if(!pool[seg]) {
            return seg
        }
    }
    return newUUID()
}

exports.newUUID = newUUID
