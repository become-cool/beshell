const fs = require("fs")
const exec = require("child_process").execSync
const beconsolePath = __dirname + "/../../beconsole.next/platform/nw.js"

const version = require("./version")
const getEsp32Target = require("./target").getEsp32Target

function mkdir(path) {
    try{ 
        fs.mkdirSync(path)
    }catch(e){
        // console.log(e)
    }
}
function cp(from, to) {
    let cmd = `cp ${from} ${to}`
    console.log(cmd)
    exec(cmd)
}

function dist(firmware, targetDir) {

    let path = targetDir + "/last"
    // let path = targetDir + "/beshell-"+version
    console.log("\r\ndispense to", path, firmware)
    mkdir(path)

    let postfix = getEsp32Target()=='esp32s3'? '-s3': ''

    let dispenseJson = false
    let dispenseBeshell = false
    let dispenseFSHome = false
    let dispenseFSRoot = false
    let dispensePartition = false
    let dispenseBootloader = false

    if(firmware=='all') {
        dispenseJson = true
        dispenseBeshell = true
        dispenseFSHome = true
        dispenseFSRoot = true
        dispensePartition = true
        dispenseBootloader = true
    }
    else if(firmware=='beshell') {
        dispenseBeshell = true
    }
    else if(firmware=='fs') {
        dispenseJson = true
        dispenseFSHome = true
        dispenseFSRoot = true
    }
    else if(firmware=='fs-root') {
        dispenseFSRoot = true
    }
    else if(firmware=='fs-home') {
        dispenseFSHome = true
    }
    else {
        throw new Error("unknow task:", firmware)
    }

    if(dispenseJson) {
        cp(__dirname+'/img/firmware.json', path+'/firmware.json')
    }

    if(dispenseBootloader) {
        cp(__dirname+"/img/bootloader-4MB.bin", path+"/bootloader-4MB.bin")
        cp(__dirname+"/img/img/bootloader-16MB.bin", path+"/bootloader-16MB.bin")
        cp(__dirname+"/img/img/bootloader-s3-8MB.bin", path+"/bootloader-s3-8MB.bin")
        cp(__dirname+"/img/img/bootloader-s3-16MB.bin", path+"/bootloader-s3-16MB.bin")
    }
    
    if(dispensePartition) {
        cp(__dirname+"/img/img/partitions-4MB.bin", path+"/")
        cp(__dirname+"/img/img/partitions-8MB.bin", path+"/")
        cp(__dirname+"/img/img/partitions-16MB.bin", path+"/")
    }

    if(dispenseBeshell) {
        cp("build/beshell.bin", path+"/beshell"+postfix+".bin")
    }

    if(dispenseFSRoot) {
        cp(__dirname+"/img/img/fs-root.img", path+"/")
    }

    if(dispenseFSHome) {
        cp(__dirname+"/img/img/fs-home.img", path+"/")
    }
}

let firmware = "all"
for(let t of ['beshell', 'fs', 'fs-home', 'fs-root']) {
    if( process.argv.includes(t) ) {
        firmware = t
    }
}

// 本地
dist(firmware, __dirname+"/firmware/")

// 远程
dist(firmware, beconsolePath+'/tmp/assets/firmware')
dist(firmware, beconsolePath+'/assets/firmware')