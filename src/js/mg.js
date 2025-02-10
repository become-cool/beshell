import { exportValue } from "loader"
import * as mg from "mg"

;[get, download].map(func => exportValue(mg, func.name, func))


function request(url, handle) {
  let t = Date.now()
  let reason=null
  return new Promise((resolve, reject) => {
    let conn = mg.connect(url, (event, data) => {
      switch (event) {
        case 'poll':
          let n = Date.now()
          let s = n - t
          if(s>4320000000) { //50年，可能修改了系统时间
            t=n
            s=0
          }
          else if(s>15000) {
            conn.close()
            reason = 'timeout'
          }
          break
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
          reject(reason||event)
          break
        default:
          handle(conn,event,data,resolve,reject)
          break
      }
    })
  })
}

function get(url) {
  return request(url, (conn,event,data,resolve,reject)=>{
    if(event=='http.msg'){
      resolve(data.body())
    }
  })
}


async function download(url, localPath, progress_cb) {
  let fhandle = fs.open(localPath, "w")
  let wroten = 0
  try{
    await request(url, (conn,event,msg,resolve,reject)=>{
      if(event=='http.msg'||event=='http.chunk'){

        let data = (event == 'http.msg') ? msg.body() : msg.chunk()
        // let n = data.byteLength - wroten
        let chunk = data.slice(wroten, data.byteLength)

        fs.write(fhandle, chunk)
        wroten += chunk.byteLength

        progress_cb && progress_cb(msg.bodyLength(), wroten)

        if (wroten == msg.bodyLength()) {
          fs.close(fhandle)
          fhandle = 0
          resolve()
        }
      }
    })
  }catch(e) {
    if(fhandle) {
      fs.close(fhandle)
      fhandle = 0
    }
    throw e
  }
}

function download2(url, localPath, progress_cb) {

  let fhandle = fs.open(localPath, "w")
  let wroten = 0

  let t = Date.now()
  let reason=undefined
  return new Promise((resolve, reject) => {
    let conn = mg.connect(url, (event, msg) => {
      switch (event) {
        case 'poll':
          let n = Date.now()
          let s = n - t
          if(s>4320000000) { //50年，可能修改了系统时间
            t=n
            s=0
          }
          else if(s>15000) {
            // console.log("timeout")
            conn.close()
            reason = 'timeout'
          }
          break
        case 'connect':
          connect(conn,url)
          break
        case 'http.msg':
        case 'http.chunk':
          t = Date.now()

          let data = (event == 'http.msg') ? msg.body() : msg.chunk()
          // let n = data.byteLength - wroten
          let chunk = data.slice(wroten, data.byteLength)

          fs.write(fhandle, chunk)
          wroten += chunk.byteLength

          progress_cb && progress_cb(msg.bodyLength(), wroten)

          if (wroten == msg.bodyLength()) {
            fs.close(fhandle)
            resolve()
          }

          break
        case 'timeout':
        case 'error':
          fs.close(fhandle)
          reject(reason||event)
          break
      }
    })
  })
}
