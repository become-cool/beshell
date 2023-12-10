#! node

const fs = require("fs")

// nvs,      data, nvs,      0x9000,      0x6000
// phy_init, data, phy,      0xf000,      0x1000
// factory,  app,  factory,  0x10000,   0x170000
// fsroot,       data, fat,      0x180000,  0x280000
// fshome,       data, fat,      0x180000,  0x280000

function mallocFlash(flashsize, appSize, rootSize) {

    flashsize*= 1024 * 1024
    appSize = Math.floor(appSize * 1024 * 1024) ;
    rootSize = Math.floor(rootSize * 1024 * 1024) ;

    let appAddr = 0x10000
    let rootAddr = appAddr + appSize
    let homeAddr = rootAddr + rootSize
    let homeSize = flashsize - homeAddr

    let json = [
        {
            name: "nvs",
            type: "data",
            subtype: "nvs",
            offset: 0x9000,
            size: 24*1024,
        } ,
        {
            name: "phy_init",
            type: "data",
            subtype: "phy",
            size: 4*1024,
        } ,
        {
            name: "factory",
            type: "app",
            subtype: "factory",
            addr: appAddr ,
            size: appSize,
        }
    ] ;
    if(rootSize>0) {
        json.push({
            name: "fsroot",
            type: "data",
            subtype: "fat",
            addr: rootAddr,
            size: rootSize,
        })
    }
    
    json.push({
        name: "fshome",
        type: "data",
        subtype: "fat",
        addr: homeAddr,
        size: homeSize,
    })

    return json
}

function generatePartitionsTable(parts) {

    let output = ''

    output+= (`# Name,   Type, SubType, Offset,  Size, Flags\n`)
    output+= (`# Note: if you change the phy_init or app partition offset, make sure to change the offset in Kconfig.projbuild\n`)

    let offset = 0
    for(let partinfo of parts) {
        if(!partinfo.size) {
            continue
        }
        if(partinfo.offset!=undefined) {
            offset = partinfo.offset
        }
        let size = partinfo.size
        
        output+= `${partinfo.name}, ${partinfo.type}, ${partinfo.subtype}, 0x${offset.toString(16).toUpperCase()}, 0x${size.toString(16).toUpperCase()} \n`

        // console.log(partinfo.name, "0x"+(offset.toString(16)), "+", "0x"+(size.toString(16)), "=", "0x"+(offset+size).toString(16))

        offset+= size
    }

    output+=(`\n\n`)

    return output
}

; (function main() {

    if(process.argv[2]=='help' || process.argv[2]=='?') {
        console.log(`node mk-partitions.js [appsize=2.5] [rootsize=0]`)
        return
    }

    let appsize = parseFloat(process.argv[2])
    let rootsize = parseFloat(process.argv[3])

    if(isNaN(appsize)) {
        appsize = 2.5
    }
    if(isNaN(rootsize)) {
        rootsize = 0
    }

    console.log(`app partition size: ${appsize}MB`)
    console.log(`root partition size: ${rootsize}MB\n`)

    let parttables4MB = mallocFlash(4, appsize, rootsize)
    let parttables8MB = mallocFlash(8, appsize, rootsize)
    let parttables16MB = mallocFlash(16, appsize, rootsize)

    
    let csv4MB = generatePartitionsTable(parttables4MB)
    let csv8MB = generatePartitionsTable(parttables8MB)
    let csv16MB = generatePartitionsTable(parttables16MB)
    
    console.log(csv4MB)
    console.log(csv8MB)
    console.log(csv16MB)
    
    fs.writeFileSync(__dirname+'/partitions-4MB.csv',csv4MB)
    fs.writeFileSync(__dirname+'/partitions-8MB.csv',csv8MB)
    fs.writeFileSync(__dirname+'/partitions-16MB.csv',csv16MB)

}) ()
