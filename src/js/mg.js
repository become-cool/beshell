import { exportValue } from "loader"
import * as mg from "mg"

;[get, download].map(func => exportValue(mg, func.name, func))


function WatchDog (ms, cbTimeout){
  let timer = setTimeout(cbTimeout,ms)
  function tick() {
    if(timer>-1) {
      resetTimeout(timer, ms)
    }
  }
  tick.stop = ()=>{
    if(timer>-1) {
      clearTimeout(timer)
    }
    timer = -1
  }
  return tick
}

function request(url, handle) {
  return new Promise((resolve, reject) => {
    let conn = null
    let timeTick = WatchDog(10000,()=>{
      conn && conn.close()
      conn = null
      reject("timeout")
    })
    conn = mg.connect(url, (event, data) => {
      if(!conn) return
      switch (event) {
        case 'connect':
          
          let info = mg.parseUrl(url)
          try {
            if (url.match(/^https:\/\//i)) {
              conn.initTLS(info.host)
            }
          } catch (e) {
            console.log(e)
          }
          conn.send(`GET ${info.uri} HTTP/1.0\r\n`)
          conn.send(`Host: ${info.host}\r\n`)
          conn.send(`User-Agent: BeShell\r\n`)
          conn.send(`\r\n`)

          break
        case 'timeout':
        case 'error':
        console.log("mg timeout/error",reason)
          reject(reason||event)
          break
        case 'close':
          timeTick.stop()
          resolve(data);
          break
        case 'http.msg':
        case 'http.chunk':
          timeTick()
          // 这里没有break
        default:
          handle(conn,event,data)
          break
      }
    })
  })
}

async function get(url) {
  let body = null
  let received = false
  await request(url, (conn,event,msg)=>{
    if(event=='http.msg'){
      body = msg.body()
      received = true
    }
  })
  if(!received) {
    throw new Error("the connection was unexpectedly disconnected.")
  }
  return body
}


async function download(url, localPath, progress_cb) {
  let fhandle = fs.open(localPath, "w")
  let wroten = 0
  let done = false
  try{
    let t = Date.now()
    await request(url, (conn,event,msg)=>{
      if(event=='http.msg'||event=='http.chunk'){

        let data = (event == 'http.msg') ? msg.body() : msg.chunk()
        let chunk = data.slice(wroten, data.byteLength)
        fs.write(fhandle, chunk)
        wroten += chunk.byteLength

        progress_cb && progress_cb(msg.bodyLength(), wroten)

        if (wroten == msg.bodyLength()) {
          fs.close(fhandle)
          fhandle = 0
          done = true
          conn.close()
        }
      }
    })
    if(!done) {
        fs.close(fhandle)
        fhandle = 0
        throw new Error("the connection was unexpectedly disconnected.")
    }
  }catch(e) {
    if(fhandle) {
      fs.close(fhandle)
      fhandle = 0
    }
    throw e
  }
}
