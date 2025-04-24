import {exportValue} from 'loader'
import * as ota from 'ota'
import * as flash from 'flash'
import * as mg from 'mg'
import * as nvs from 'nvs'

;[start].map(func => exportValue(ota, func.name, func))

function downloadFirmware(p, opt, type, onProgress, onComplete, step) {

    if(!opt.url) {
        throw new Error("url not found")
    }

    console.log("erase partition ...")
    p.erase(0, p.size)

    return new Promise(async (resolve,reject)=>{
        try{
            let t = Date.now()
            let writer = 0
            let printProg = 0
            console.log("upgrade from url", opt.url)
            console.log("write to partition", p.label, p.size, "bytes")
            console.log("start download ...")

            let totalBytes = 0
            await mg.download(opt.url, null, (total,wrote,chunk)=>{
                let prog = Math.round(wrote*100/total)
                if(printProg==prog) {
                    console.log(prog+"%,", ((Date.now()-t)/1000)+"sec")
                    printProg+= step
                }
                p.write(writer, chunk)
                writer+= chunk.byteLength

                onProgress && onProgress(type, total, wrote)

                totalBytes = total
            })
            console.log("download and flash", (Date.now()-t)+"ms")

            if( Number.isInteger(opt.checksum) ){
                console.log("calculate checksum ...", opt.checksum.toString(16).toUpperCase())
                let flashChecksum = p.checksum(totalBytes)
                if( opt.checksum != flashChecksum ) {
                    throw new Error("Checksum error, expected "+opt.checksum+", got "+flashChecksum)
                }
            }

            onComplete && onComplete(type, null)

            resolve()

        } catch(e) {
            reject(e)
            onComplete && onComplete(type, e)
        }
    })
}

function findOTAPartitions(){
    let arr = []
    for(let p of Object.values(flash.allPartitions())) {
        if( p.type == 'app' && (p.subtype==16 || p.subtype==17) ){
            arr.push(p)
        }
    }
    return arr
}

/*
{
    bin: {
        url: "" ,
        size: n ,
        crc: n ,
        partitions: ['ota_0', 'ota_1'] ,
        step
    } ,
    fs: {
        url: "" ,
        size: n ,
        crc: n ,
        partitions: ['fsroot_0', 'fsroot_1']
    } ,
    onProgress(type, total, wrote) {},
    onComplete(type, error) {},
}
*/
export async function start(opt) {

    let bootBinPart = null
    let bootFSPart = null

    if(opt.bin) {
        let parts = opt.bin.partitions || findOTAPartitions()
        bootBinPart = ota.getBootPartition()
        console.log("boot bin partition:", bootBinPart.label)

        parts = parts.filter(p => p.label !== bootBinPart.label);
        if(parts.length<1){
            throw new Error("Aalternative OTA partition not found")
        }
        bootBinPart = parts[0]
        console.log("write bin firmware to partition:", bootBinPart.label)
        await downloadFirmware(bootBinPart, opt.bin, "bin", opt.onProgress, opt.onComplete, opt.step||5)
    }

    if(opt.fs) {
        if(!opt.fs.partitions) {
            opt.fs.partitions = []
            for(let p of Object.values(flash.allPartitions())) {
                if(p.name=="fsroot_0" || p.name=="fsroot_1") {
                    opt.fs.partitions.push(p)
                }
            }
        }

        console.log("fs partitions:", opt.fs.partitions)
        
        if(opt.fs.partitions.length<2) {
            throw new Error("Missing alternative OTA partition for fs")
        }

        bootFSPart = ota.getRunningPartition("fs")
        console.log("boot fs partition:", bootFSPart.label)

        let parts = opt.fs.partitions.filter(p => p.label !== bootFSPart.label)
        console.log("alternative fs partitions:", parts.map(p=>p.label))

        if(parts.length<1){
            throw new Error("Aalternative OTA FS partition not found")
        }
        bootFSPart = parts[0]
        console.log("write bin firmware to partition:", bootFSPart.label)
        await downloadFirmware(bootFSPart, opt.fs, "fs", opt.onProgress, opt.onComplete, opt.step||5)
    }

    if(bootBinPart) {
        ota.setBootPartition(bootBinPart)
    }
    if(bootFSPart) {
        nvs.writeString("fsroot.boot", bootFSPart.label)
    }
}