const fs = require("fs")

let align = 32
let appstat = fs.statSync(__dirname + "/img/beshell.bin")
let appsize = Math.ceil(appstat.size/1024/align) *1024*align
console.log("appsize:", appstat.size, '->', appsize)

try{
    let appstat_s3 = fs.statSync(__dirname + "/img/beshell-s3.bin")
    let appsize_s3 = Math.ceil(appstat_s3.size/1024/align) *1024*align
    console.log("appsize(s3):", appstat_s3.size, '->', appsize_s3)
    if(appsize_s3>appsize) {
        appsize = appsize_s3
    }
}catch(e) {
    // console.log(e)
}

// appsize = 0x320000

align = 32
let rootstat = fs.statSync(__dirname + "/img/fs-root.img")
let rootsize = Math.ceil(rootstat.size/1024/align) *1024*align

console.log("rootsize:", rootstat.size, '->', rootsize)

module.exports = [appsize, rootsize]