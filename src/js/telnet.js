import { exportValue } from 'loader'
import * as telnet from 'telnet'
import * as mg from 'mg'

function connect(url) {

  let channel = new telnet.TelnetChannel()

  let conn = mg.connect(url, (ev, data, isBinary) => {
    if(ev=="ws.msg") {
      channel && channel.process(data)
    }
    else if(['error','close'].includes(ev)){
      channel = null
    }
  })

  channel.on("output-stream", (data) => {
    if (conn && conn.isConnected()) {
      conn.send(data)
    }
  })
  
  return conn
}

exportValue(telnet, "ws", {
  connect
})