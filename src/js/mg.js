import { exportValue } from "loader"
import * as mg from "mg"

;[get, download].map(func => exportValue(mg, func.name, func))

function connect(conn, url) {
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
  conn.send(`Content-Type: application/json\r\n\r\n`)
}

function get(url) {
  return new Promise((resolve, reject) => {
    let conn = mg.connect(url, (event, data) => {
      switch (event) {
        case 'connect':
          connect(conn, url)
          break
        case 'http.msg':
          // console.log("msg")
          resolve(data.body())
          break
        case 'http.chunk':
          // console.log("chunk")
          // resolve(data.chunk())
          break
        case 'timeout':
        case 'error':
          reject(event)
          break
      }
    })
  })
}

function download(url, localPath, progress_cb) {

  let fhandle = fs.open(localPath, "w")
  let wroten = 0

  return new Promise((resolve, reject) => {
    let conn = mg.connect(url, (event, msg) => {
      switch (event) {
        case 'connect':
          connect(conn, url)
          break
        case 'http.msg':
        case 'http.chunk':

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
          reject()
          break
      }
    })
  })
}
