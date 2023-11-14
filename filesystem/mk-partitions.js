
const fs = require("fs")

// 计算 beshell.bin 文件的大小，按 128 取整做为分区的尺寸
// const [appsize, rootsize] = require('./app-partition-size.js')
const appsize = 3*1024*1024
const rootsize = 0

// nvs,      data, nvs,      0x9000,      0x6000
// phy_init, data, phy,      0xf000,      0x1000
// factory,  app,  factory,  0x10000,   0x170000
// fsroot,       data, fat,      0x180000,  0x280000
// fshome,       data, fat,      0x180000,  0x280000
let parttables4MB = [
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
        addr: 0x10000 ,
        subtype: "factory",
        size: appsize,
    } ,
    {
        name: "fsroot",
        type: "data",
        subtype: "fat",
        size: rootsize,
    } ,
    {
        name: "fshome",
        type: "data",
        subtype: "fat",
    } ,
]

let parttables8MB = [
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
        addr: 0x10000 ,
        subtype: "factory",
        size: appsize,
    } ,
    {
        name: "fsroot",
        type: "data",
        subtype: "fat",
        size: rootsize,
    } ,
    {
        name: "fshome",
        type: "data",
        subtype: "fat",
    } ,
]

let parttables16MB = [
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
        addr: 0x10000 ,
        subtype: "factory",
        size: appsize,
    } ,
    {
        name: "fsroot",
        type: "data",
        subtype: "fat",
        size: rootsize,
    } ,
    {
        name: "fshome",
        type: "data",
        subtype: "fat",
    } ,
]


function generatePartitionsTable(parts, flashSize) {

    let output = ''

    output+= (`# FLASH Size: ${(flashSize/1024/1024).toFixed(2)}MB\n`)
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
        if(size==undefined) {
            size = flashSize - offset
        }
        
        output+= `${partinfo.name}, ${partinfo.type}, ${partinfo.subtype}, 0x${offset.toString(16).toUpperCase()}, 0x${size.toString(16).toUpperCase()} \n`

        console.log(partinfo.name, "0x"+(offset.toString(16)), "+", "0x"+(size.toString(16)), "=", "0x"+(offset+size).toString(16))

        offset+= size
    }

    output+=(`\n\n\n\n`)

    return output
}



let csv4MB = generatePartitionsTable(parttables4MB, 4*1024*1024)
let csv8MB = generatePartitionsTable(parttables16MB, 8*1024*1024)
let csv16MB = generatePartitionsTable(parttables16MB, 16*1024*1024)

console.log(csv4MB)
console.log(csv8MB)
console.log(csv16MB)

fs.writeFileSync(__dirname+'/partitions-4MB.csv',csv4MB)
fs.writeFileSync(__dirname+'/partitions-8MB.csv',csv8MB)
fs.writeFileSync(__dirname+'/partitions-16MB.csv',csv16MB)


// require(__dirname+"/mk-firmware-json.js")
