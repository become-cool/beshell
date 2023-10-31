const lv = require('lv')

exports["130"] = {
    title: "1.3寸屏幕" ,
    width: 32 ,
    height: 32 ,
    "bg-color": lv.rgb(80,80,80) ,
    onlyFemale:true ,
    ports: [
        { title: "VCC", x:44, y:12 } ,
        { title: "GND", x:52, y:12 } ,
        { title: "MOSI", x:52, y:12 } ,
        { title: "DC", x:52, y:12 } ,
        { title: "SCK", x:52, y:12 } ,
        { title: "CS", x:52, y:12 } ,
    ]
}

exports["096"] = {
    title: "0.96寸屏幕" ,
    width: 16 ,
    height: 32 ,
    "bg-color": lv.rgb(80,80,80) ,
    __proto__: exports.motor
}