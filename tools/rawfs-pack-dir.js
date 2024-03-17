const fs = require("fs")

const TYPE_DIR = 1
const TYPE_FILE = 2

function mknode(name, type) {
    return {
        name, type, children: [] ,
        offset: 0, size:0
    }
}

function packdir(target, tar, parent) {

    parent.size = 0

    for(let item of fs.readdirSync(target)) {
        if(item=='.'||item=='..'){
            continue
        }

        let itemFull = target +"/"+ item
        console.log(itemFull)

        let stat = fs.statSync(itemFull)

        let node = mknode(item)
        parent.size ++

        if (stat.isDirectory()) {
            node.type = TYPE_DIR
            tar = packdir(itemFull, tar, node)
        }
        else if(stat.isFile()) {
            let raw = fs.readFileSync(itemFull)
            node.type = TYPE_FILE
            node.offset = tar.length
            node.size = raw.length
            tar = Buffer.concat([tar, raw])
        }
        else {
            console.error("unknow type", itemFull)
            continue
        }

        parent.children.push(node)
    }

    return tar
}



function write_node(node, fd){
    let buflen = 1 + node.name.length + 1 + 4
    if(node.type == TYPE_FILE) {
        buflen+= 4  // for offset
    }
    // console.log(buflen)
    let buf = Buffer.alloc(buflen, 0)

    let offset = 0
    
    // type
    buf.writeUint8(node.type, offset)
    offset++

    // name + \0
    buf.write(node.name,offset)
    offset+= node.name.length

    buf.write(String.fromCharCode(0),offset)
    offset+=1

    // size
    buf.writeUInt32LE(node.size,offset)
    offset+= 4

    // offset
    if(node.type == TYPE_FILE) {
        buf.writeUInt32LE(node.offset,offset)
        offset+= 4
    }

    // console.log(node, buf)

    fs.writeSync(fd, buf)

    for(let child of node.children) {
        write_node(child, fd)
    }
}


let root = mknode("/", TYPE_DIR)
console.log(root)

let tar = packdir(__dirname+'/tmp/root', Buffer.from([]), root)
console.log("tar size:", tar.length)

console.log("write to",__dirname+"/img/fs-root.img")

let fd = fs.openSync(__dirname+"/img/fs-root.img", 'w')

write_node(root, fd)
fs.writeSync(fd, tar)

fs.close(fd,console.log)
// 