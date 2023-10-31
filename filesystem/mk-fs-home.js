const { exec } = require('child_process');

let sourceDir = __dirname + '/tmp/home/'

if( process.argv.length>2 ) {
    sourceDir = process.argv[2]
}
console.log("sourceDir =",sourceDir)

let imgsize = 0
if( process.argv.length>3 ) {
    imgsize = parseInt(process.argv[3])
}

const align = 256

function execAsync(cmd) {
    return new Promise((resolve, reject) => {
        exec(cmd, (error, stdout, stderr) => {
            if(stderr) {
                console.error(stderr)
            }
            if(error) {
                reject(error)
                return
            }
            resolve(stdout)
        })
    })
}

async function main() {

    if(!imgsize) {
        let stdout = await execAsync(`du -sb ${sourceDir}`)
        
        const output = stdout.trim()
        let res = output.match(/(\d+)/)
        if(!res && !res[1]) {
            console.error("")
        }

        let size = parseInt(res[1])
        imgsize = size + 20480

        if(imgsize<align) {
            imgsize = align
        }
        else {
            imgsize = Math.ceil(imgsize/align) * align
        }

        console.log(size, '->', imgsize)
    }

    console.log("imgsize =", imgsize)
    
    let cmd = `${__dirname}/../bin/mklittlefs -c ${sourceDir} -s ${imgsize} ${__dirname}/img/fs-home.img -d 5`
    console.log(cmd)
    let stdout = await execAsync(cmd)
    console.log(stdout)
}

main()
