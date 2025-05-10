import { exportValue } from "loader"
import * as mg from "mg"

;[request, get, download, post].map(func => exportValue(mg, func.name, func))


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
        case 'timeout':
        case 'error':
          reject(event)
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
          handle(conn,event,data,timeTick)
          break
      }
    })
    handle && handle(conn,'new',null,timeTick)
  })
}

async function get(url,handle) {
  let body = null
  let received = false
  await request(url, (conn,event,msg)=>{
    if(event=='http.msg'){
      body = msg.body()
      received = true
    }
    else if(event=='connect') {
      let info = mg.parseUrl(url)
      conn.send(`GET ${info.uri} HTTP/1.0\r\n`)
      conn.send(`Host: ${info.host}\r\n`)
      conn.send(`User-Agent: BeShell\r\n`)
      conn.send(`\r\n`)
    }
    try{
      handle && handle(conn,event,msg)
    }catch(e) {
      console.error(e)
    }
  })
  if(!received) {
    throw new Error("the connection was unexpectedly disconnected.")
  }
  return body
}


async function download(url, localPath, progress_cb) {
  let fhandle = null
  if(localPath) {
    fhandle = fs.open(localPath, "w")
  }
  let wroten = 0
  let done = false
  try{
    let t = Date.now()
    let total = 0
    await request(url, (conn,event,data)=>{
      if(event=='connect') {
        let info = mg.parseUrl(url)
        conn.send(`GET ${info.uri} HTTP/1.0\r\n`)
        conn.send(`Host: ${info.host}\r\n`)
        conn.send(`User-Agent: BeShell\r\n`)
        conn.send(`\r\n`)
      }
      else if (event=='http.head'){
        total = data.bodyLength() || 0
      }
      else if(event=='http.chunk'){
        if(!data) {
          return
        }
        if(fhandle) {
          fs.write(fhandle, data)
        }
        wroten += data.byteLength
        
        progress_cb && progress_cb(total, wroten, data)

        if (wroten == total) {
          fhandle && fs.close(fhandle)
          fhandle = null
          done = true
          conn.close()
        }
      }
      else if(event=="new") {
        conn.enableChunkEvent()
      }
    })
    if(!done) {
        fhandle && fs.close(fhandle)
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

async function post(url, ab) {
  let body = null
  let received = false
  await request(url, (conn,event,msg)=>{
    if(event=='http.msg'){
      body = msg.body()
      received = true
    }
    else if(event=='connect') {
      let info = mg.parseUrl(url)
      conn.send(`POST ${info.uri} HTTP/1.0\r\n`)
      conn.send(`Host: ${info.host}\r\n`)
      conn.send(`User-Agent: BeShell\r\n`)
      conn.send(`Content-Type: application/octet-stream\r\n`)
      conn.send(`Content-Length: ${ab.byteLength}\r\n`)
      conn.send(`\r\n`)

      // @todo use timeTick
      conn.send(ab)
    }
  })
  if(!received) {
    throw new Error("the connection was unexpectedly disconnected.")
  }
  return body
}
