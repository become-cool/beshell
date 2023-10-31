const fs = require("fs")

const header = fs.readFileSync(__dirname + "/../main/beshell.h").toString()

let res = header.match(/#define BESHELL_VERSION "([^"]+)"/)
if(!res) {
    console.log("read beshell version failed")
    process.exit()
}
module.exports = res[1]
console.log(res[1])