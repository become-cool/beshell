const net = require('net')

//创建TCP客户端
const client = new net.Socket()


function find_pkg_header(buff) {

    for(let i=0; i<buff.length; i++) {
        if(buff[i]==5 && buff[i+1]==19 && buff[i+3]==8 ) {
            if(i+5>=buff.length) {
                console.log("i+5<buff.length", i,buff.length)
                return [-1, 0, 0, buff.subarray(i)]
            }
            if(i!=0) {
                console.log("drop data", i)
            }
            let id = buff[i+2]
            let vsum = buff[i+4]

            let len = buff[i+5] & 0x7F
            if(buff[i+5]<0x80) {
                return [len, id, vsum, buff.subarray(i+6)]
            }

            if(i+6>=buff.length) {
                return [-1, 0, 0, buff.subarray(i)]
            }

            len|= (buff[i+6] & 0x7F) << 7
            return [len, id, vsum, buff.subarray(i+7)]
        }
    }

    console.log("drop all", i)
    return [-1, 0, 0, null]
}


let recvResolve = null
function recv() {
    return new Promise(function(resolve) {
        recvResolve = resolve
    })
}

async function recvAppend(buff) {
    let data = await recv()
    return buff? Buffer.concat([buff,data]): data
}

async function recvUtil(data, length) {
    while( !data || data.length<length) {
        data = await recvAppend(data)
    }
    return data
}

//监听data事件
client.on("data", function (data) {
    if(recvResolve) {
        recvResolve(data)
        recvResolve = null
    }
    else {
        // console.log("drop")
    }
    // console.log("recv: " + data.length);
    

    // let [len, id, vsum] = wait_pkg_header(data)

    // console.log(data[0], data[1], data[2], data[3], data[4], data[5], data[6])
    // console.log(len, id, vsum)


    // process.exit()
})

//设置连接的服务器
client.connect(8018, '192.168.199.210', function () {
    console.log("connected");
    loop()
    // client.write('Hello, server! Love, Client.');
})

client.on("close",()=>{
    console.log('close')
})

function checksum(buff, length) {
    let sum = 0
    for(let i=0;i<length; i++) {
        sum^= buff[i] ;
    }
    return sum
}


async function loop() {
    
    let data = null
    let len = 0 , id=0, vsum=0

    while(1) {
        data = await recvUtil(data, 20) ;

        [len, id, vsum, data] = find_pkg_header(data)

        data = await recvUtil(data, len)

        let vsum2 = checksum(data, len)
        console.log(len, data.length, vsum, vsum2)

        data = data.subarray(len)

        // process.exit()
    }
}