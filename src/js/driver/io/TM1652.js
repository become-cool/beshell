import * as driver from 'driver'
import { exportValue } from "loader"
import * as serial from "serial"

const fonts = {
" ": 0b00000000,
"!": 0b10000110,
'"': 0b00100010,
"#": 0b01111110,
"$": 0b01101101,
"%": 0b00000000,
"&": 0b00000000,
"'": 0b00000010,
"(": 0b00110000,
")": 0b00000110,
"*": 0b01100011,
"+": 0b00000000,
",": 0b00000100,
"-": 0b01000000,
".": 0b10000000,
"/": 0b01010010,
"0": 0b00111111,
"1": 0b00000110,
"2": 0b01011011,
"3": 0b01001111,
"4": 0b01100110,
"5": 0b01101101,
"6": 0b01111101,
"7": 0b00100111,
"8": 0b01111111,
"9": 0b01101111,
":": 0b00000000,
";": 0b00000000,
"<": 0b00000000,
"=": 0b01001000,
">": 0b00000000,
"?": 0b01010011,
"@": 0b01011111,
"A": 0b01110111,
"B": 0b01111111,
"C": 0b00111001,
"D": 0b00111111,
"E": 0b01111001,
"F": 0b01110001,
"G": 0b00111101,
"H": 0b01110110,
"I": 0b00000110,
"J": 0b00011110,
"K": 0b01101001,
"L": 0b00111000,
"M": 0b00010101,
"N": 0b00110111,
"O": 0b00111111,
"P": 0b01110011,
"Q": 0b01100111,
"R": 0b00110001,
"S": 0b01101101,
"T": 0b01111000,
"U": 0b00111110,
"V": 0b00101010,
"W": 0b00011101,
"X": 0b01110110,
"Y": 0b01101110,
"Z": 0b01011011,
"[": 0b00111001,
"\\": 0b01100100,
"]": 0b00001111,
"^": 0b00000000,
"_": 0b00001000,
"`": 0b00100000,
"a": 0b01011111,
"b": 0b01111100,
"c": 0b01011000,
"d": 0b01011110,
"e": 0b01111011,
"f": 0b00110001,
"g": 0b01101111,
"h": 0b01110100,
"i": 0b00000100,
"j": 0b00001110,
"k": 0b01110101,
"l": 0b00110000,
"m": 0b01010101,
"n": 0b01010100,
"o": 0b01011100,
"p": 0b01110011,
"q": 0b01100111,
"r": 0b01010000,
"s": 0b01101101,
"t": 0b01111000,
"u": 0b00011100,
"v": 0b00101010,
"w": 0b00011101,
"x": 0b01110110,
"y": 0b01101110,
"z": 0b01000111,
"{": 0b01000110,
"|": 0b00000110,
"}": 0b01110000,
"~": 0b00000001,
}
class TM1652 {
    setup(options) {
        if(!options) {
            throw new Error("missing arg options")
        }
        this.uart = serial["uart"+options.uart]
        if(!this.uart){
            throw new Error("missing options property uart, or invalid")
        }
        if(options.pin!==undefined) {
            try{
                this.uart.setup({tx:options.pin,rx:-1, baudrate:19200,parity:3})
            }catch(e) {
                console.log("install uart failed")
            }
        }
    }
    setNumber(v, dot) {
        if(!this.uart) {
            throw new Error("uart not setup")
        }
        dot = 5-dot
        let s = v.toString()
        let bytes = [0x08, 0,0,0,0,0]
        for(let i=0;i<s.length;i++) {
            let c = s.charAt(i)
            bytes[i+1] = fonts[c]
            if(i==dot) {
                bytes[i+1] |= 0x80
            }
        }
        this.uart.write(bytes)
        this.uart.write([0x18,0x18])
    }
}

exportValue(driver, "TM1652", TM1652)
