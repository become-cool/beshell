class ST77XX {
    constructor() {
        this.spi = -1
        this.dc = 12
        this.cs = 15
        this._ox = 0  // offset x
        this._oy = 0  // offset y
        this.width = 0
        this.height = 0
    }

    setup(bus, cs, dc, freq) {
        this.spi = beapi.spi.addDevice(bus, cs, freq||26000000, 0)
        this.dc = dc
        pinMode(dc, "output")
    }

    async wakeup() {
        this.sendCommand(0x11);     //Sleep out
        await sleep(120);           //Delay 120ms
    }

    rgb565(r,g,b) {
        return ((r & 31) << 11) | ((g & 63)<<5) | (b & 31)
    }

    sendFillCommand(x1,y1, x2,y2) {
        x1+= this._ox
        x2+= this._ox
        y1+= this._oy
        y2+= this._oy

        // x1, x2
        this.sendCommand(0x2A)
        this.sendData([
            (x1>>8)&255, x1&255 ,
            (x2>>8)&255, x2&255 ,
        ])
        // y1, y2
        this.sendCommand(0x2B)
        this.sendData([
            (y1>>8)&255, y1&255 ,
            (y2>>8)&255, y2&255 ,
        ])

        this.sendCommand(0x2C)
        
        beapi.gpio.digitalWrite(this.dc, 1)
    }

    fillRect (x1,y1, x2,y2, red, green, blue) {

        this.sendFillCommand(x1,y1, x2,y2)

        let rgb565 = this.rgb565(red, green, blue)
        rgb565 = ((rgb565&0xFF)<<8) | ((rgb565>>8)&0xFF)    // 改变字节顺序

        // 填充数组
        let buff = new Uint16Array(10240)
        buff.fill(rgb565)

        let n = (x2-x1+1) * (y2-y1+1) * 2
        for( ; n>0; n-=20480) {
            beapi.spi.send(this.spi, buff.buffer, 0, (n>=20480)? 20480: n)
        }
    }
    
    sendCommand(cmd) {
        if(this.spi<0)
            return
        beapi.gpio.digitalWrite(this.dc, 0)
        beapi.spi.send(this.spi, new Uint8Array([cmd]).buffer)
    }
    sendCmdSeq(seq){
        for(let i=0;i<seq.length;i++) {
            this.sendCommand(seq[i++])
            // await sleep(1)
            if(seq[i]) {
                this.sendData(seq[i])
                // await sleep(1)
            }
        }
    }
    sendData(data) {
        beapi.gpio.digitalWrite(this.dc, 1)
        if(data instanceof Array)
            data = new Uint8Array(data).buffer
        beapi.spi.send(this.spi, data)
    }
}


// ST7735
class BeScreen096 extends ST77XX {

    constructor(){
        super()
        this._ox = 26
        this._oy = 1
        this.width = 80
        this.height = 160
    }

    async begin(bus, cs, dc, freq) {
        
        super.begin(bus,cs,dc, freq)
        
        await this.wakeup()

        this.sendCmdSeq([
            // Normal mode
            0xB1, [0x05, 0x3C, 0x3C], 

            // Idle mode
            0xB2, [0x05, 0x3C, 0x3C],      
            
            // Partial mode
            0xB3, [0x05,0x3C,0x3C,0x05,0x3C,0x3C],      
            
            // Dot inversion
            0xB4, [0x03],      
            
            // AVDD GVDD
            0xC0, [0xAB,0x0B,0x04],   
            
            // VGH VGL
            0xC1, [0xC5],   
            
            // Normal Mode
            0xC2, [0x0D,0x00],   
            
            // Idle
            0xC3, [0x8D,0x6A],   
            
            // Partial+Full
            0xC4, [0x8D,0xEE],   
            
            // VCOM
            0xC5, [0x0F],   
            
            //positive gamma
            0xE0, [0x07,0x0E,0x08,0x07,0x10,0x07,0x02,0x07,0x09,0x0F,0x25,0x36,0x00,0x08,0x04,0x10],  
            
            //negative gamma
            0xE1, [0x0A,0x0D,0x08,0x07,0x0F,0x07,0x02,0x07,0x09,0x0F,0x25,0x35,0x00,0x09,0x04,0x10],   

            // 
            0xFC, [0x80],   
            // 
            0x3A, [0x05],   

            // 转动方向
            0x36, [0x08],   

            ///Display inversion
            0x21, null,   

            //Display on
            0x29, null,   

            // //Set Column Address 26 - 105
            // 0x2A, [0x00,this._ox,0x00,this._ox+this.width-1],   

            // //Set Page Address 1 - 160
            // 0x2B, [0x00, this._oy, 0x00, this._oy+this.height-1],   
        ])
    }
}


// ST7789
class BeScreen130 extends ST77XX {
    constructor(){
        super()
        this._ox = 0
        this._oy = 0
        this.width = 240
        this.height = 240
    }

    async begin(spiBus, cs, dc, freq) {

        super.begin(spiBus, cs, dc, freq)

        await this.wakeup()

        this.sendCmdSeq([

            // 方向
            0x36, [0x00], 

            // 颜色模式： RGB565 (16Bit)
            0x3A, [0x05],      
            
            // 帧率: 60hz
            0xC6, [0x0F] ,
            
            // power control
            0xD0, [0xA4, 0xA1] ,
            
            // 反色
            0x21, null ,

            // 打开屏幕
            0x29, null ,
        ])

    }
}

// ST7789
class BeScreen320 extends ST77XX {
    constructor(){
        super()
        this._ox = 0
        this._oy = 0
        this.width = 320
        this.height = 240
    }

    async begin(spiBus, cs, dc, freq) {

        super.begin(spiBus, cs, dc, freq)

        await this.wakeup()

        this.sendCmdSeq([

            // 方向
            0x36, [0x00], 

            // 颜色模式： RGB565 (16Bit)
            0x3A, [0x05],      
            
            // 帧率: 60hz
            0xC6, [0x0F] ,
            
            // power control
            0xD0, [0xA4, 0xA1] ,
            
            // 反色
            0x21, null ,

            // 打开屏幕
            0x29, null ,
        ])

    }
}

// ST7796
class BeScreen400 extends ST77XX {
    constructor(spi, cs, dc, res){
        super(spi, dc, cs, res)
        this._ox = 1
        this._oy = 1
        this.width = 480
        this.height = 320
    }

    async begin(spiBus, cs, dc, freq) {

        super.begin(spiBus, cs, dc, freq)

        await this.wakeup()

        this.sendCmdSeq([
            0Xf0, [0xc3],
            0Xf0, [0x96],

            // 方向
            0x36, [0x48],
            
            // 颜色模式： RGB565 (16Bit)
            0x3A, [0x05],

            0xE6, [0x0f,0xf2,0x3f,0x4f,0x4f,0x28,0x0e,0x00],

            // VCOM
            0xC5, [0x2A] ,

            //positive gamma
            0xE0, [0xf0,0x03,0x0a,0x11,0x14,0x1c,0x3b,0x55,0x4a,0x0a,0x13,0x14,0x1c,0x1f],  

            //negative gamma
            0xE1, [0xf0,0x03,0x0a,0x0c,0x0c,0x09,0x36,0x54,0x49,0x0f,0x1b,0x18,0x1b,0x1f],

            0Xf0, [0x3c] ,
            0Xf0, [0x69] ,
            0X29,
        ])
    }
}

module.exports = {
    ST77XX, BeScreen096, BeScreen130, BeScreen320, BeScreen400
}