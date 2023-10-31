const telweb = require("telweb")

const addr80 = "http://0.0.0.0:80"
be.server.http80 = telweb.start(addr80)

console.log("telweb start on addr:")
console.log(addr80)