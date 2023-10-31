const fs = require("fs")


// 返回 esp32 或者 esp32s3
function getEsp32Target() {
    let file = (fs.readFileSync( __dirname+'/../dependencies.lock' ) || '').toString()

    let targetChip = (file.match(/target: ([^\r\n\s$]+)/)||[]) [1]
    if(!targetChip) {
        throw new Error("unknow idf build target (missing dependencies.lock file?)")
    }

    return targetChip
}

console.log(getEsp32Target())

exports.getEsp32Target = getEsp32Target