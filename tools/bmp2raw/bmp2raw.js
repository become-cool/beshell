var fs = require('fs');
var jpeg = require('jpeg-js');
var jpegData = fs.readFileSync('./frames/frame-000001.jpg');
var rawImageData = jpeg.decode(jpegData);
console.log(rawImageData.data.length);
