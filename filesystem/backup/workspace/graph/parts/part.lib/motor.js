
exports.motor = {
    title: "调速电机" ,
    ports: [
        { title: "VCC", x:44, y:12 } ,
        { title: "GND", x:52, y:12 } ,
        { title: "PWM", x:52, y:12 } ,
    ]
}

exports.servo = {
    title: "舵机" ,
    __proto__: exports.motor
}