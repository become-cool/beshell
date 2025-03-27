import {exportValue} from 'loader'
import * as ota from 'ota'
import * as flash from 'flash'
import * as mg from 'mg'

;[start].map(func => exportValue(ota, func.name, func))

function downloadFirmware(p, opt) {

    console.log("erase partition ...")
    p.erase(0, p.size)

    return new Promise(async (resolve,reject)=>{
        try{
            let t = Date.now()
            let writer = 0
            let printProg = 0
            await mg.download(opt.url, null, (total,wrote,chunk)=>{
                let prog = Math.round(wrote*100/total)
                if(printProg==prog) {
                    console.log(prog+"%")
                    printProg+= 5
                }
                p.write(writer, chunk)
                writer+= chunk.byteLength
            })
            console.log("download and flash", (Date.now()-t)+"ms")
        } catch(e) {
            reject(e)
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
        partitions: ['ota_0', 'ota_1']
    } ,
    fs: {
        url: "" ,
        size: n ,
        crc: n ,
        partitions: ['fsroot_0', 'fsroot_1']
    } ,
    onProgress(type, total, writed) {},
    onComplete(error) {},
}
*/
export async function start(opt, progress) {

    if(opt.bin) {
        let parts = opt.bin.partitions || findOTAPartitions()
        let bootBinPart = ota.getBootPartition()
        parts = parts.filter(p => p.label !== bootBinPart.label);
        if(parts.length<1){
            throw new Error("Aalternative OTA partition not found")
        }
        console.log("write bin firmware to partition:", parts[0].label)
        await downloadFirmware(parts[0], opt.bin, progress)
    }

    if(opt.fs) {
        if(!opt.fs.partitions) {
            throw new Error("Missing alternative OTA partition for fs")
        }
    }
}