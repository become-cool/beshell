const fs = require('fs')

const parts = [
    {
        name: 'BeCore16-001' ,
        id: 1 ,
        vers: [1] ,
    } ,
    {
        name: 'BeCore32-002' ,
        id: 2 ,
        vers: [1] ,
    } ,
    {
        name: 'BePad-003' ,
        id: 3 ,
        vers: [1] ,
    } ,
    {
        name: 'BeCamera-019' ,
        id: 19 ,
        vers: [1] ,
    }
]

const METAFIELD_PART_ID = 1
const METAFIELD_PART_VER = 2

const VERIFY_BASE = 55


for(let partmeta of parts) {
    for(let version of partmeta.vers) {

        let data = new Uint8Array(32)

        data[0] = METAFIELD_PART_ID | METAFIELD_PART_VER
        data[1] = partmeta.id
        data[2] = version
        data[3] = VERIFY_BASE ^ data[0] ^ data[1] ^ data[2]

        let filepath = `${__dirname}/meta-bins/${partmeta.name}-v${version}.bin`
        fs.writeFileSync(filepath,data)
    }
}

console.log("")
console.log("python <esp-idf path>/components/esptool_py/esptool/espefuse.py --port COM18 burn_block_data BLOCK3 <bin_file_path>")
console.log("")
