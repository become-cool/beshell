const lv = require('lv')

module.exports = {
    title: "BePad" ,
    width: 14 * 8 ,
    height: 8 * 8 ,
    "bg-color": lv.rgb(80,80,80) ,
    ports: [
        { title: "0", x:-52, y:-28 } ,
        { title: "2", x:-44, y:-28 } ,
        
        { title: "VCC", x:-52, y:-20 } ,
        { title: "GND", x:-44, y:-20 } ,
        
        { title: "34", x:-52, y:-12 } ,
        { title: "35", x:-44, y:-12 } ,
        
        { title: "4", x:-52, y:-4 } ,
        { title: "5", x:-44, y:-4 } ,
        
        { title: "1", x:-52, y:4 } ,
        { title: "3", x:-44, y:4 } ,
        
        { title: "VCC", x:-52, y:12 } ,
        { title: "GND", x:-44, y:12 } ,
        
        { title: "12", x:-52, y:20 } ,
        { title: "13", x:-44, y:20 } ,
        
        { title: "14", x:-52, y:28 } ,
        { title: "15", x:-44, y:28 } ,

        
        { title: "22", x:44, y:-28 } ,
        { title: "23", x:52, y:-28 } ,
        
        { title: "VCC", x:44, y:-20 } ,
        { title: "GND", x:52, y:-20 } ,
        
        { title: "38", x:44, y:-12 } ,
        { title: "26", x:52, y:-12 } ,
        
        { title: "36", x:44, y:-4 } ,
        { title: "37", x:52, y:-4 } ,
        
        { title: "GND", x:44, y:4 } ,
        { title: "VCC", x:52, y:4 } ,
        
        { title: "VCC", x:44, y:12 } ,
        { title: "GND", x:52, y:12 } ,
        
        { title: "32", x:44, y:20 } ,
        { title: "33", x:52, y:20 } ,
        
        { title: "25", x:44, y:28 } ,
        { title: "27", x:52, y:28 } ,
    ]
}
