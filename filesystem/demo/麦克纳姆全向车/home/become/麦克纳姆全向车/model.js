
/**
 * 此文件由 BeConsole 自动生成，任何改动都会被 BeConsole 覆盖
 */
require("besdk")

// 在初始化 BeConsole "硬件" 视图中创建并配置的零件
exports.setup = function () {




	be.part["FR"] = new (require("besdk/driver/pwmmotor")) ()

	be.part["FL"] = new (require("besdk/driver/pwmmotor")) ()

	be.part["BR"] = new (require("besdk/driver/pwmmotor")) ()

	be.part["BL"] = new (require("besdk/driver/pwmmotor")) ()



	be.part["FR"].begin(27)

	be.part["FL"].begin(25)

	be.part["BR"].begin(4)

	be.part["BL"].begin(2)


}

// 根据 BeConsole "编程" 视图中的程序卡片生成的JS代码
exports.runCards = async function(){
	require("besdk/socket").udp.onmessage(6666,async (_pkg_buff)=>{
		var _pkg_buff = _pkg_buff
		var _pkg_offset = 0

  		be.var["cmd"] = utils.unpackString(_pkg_buff, _pkg_offset)
  		_pkg_offset+= utils.stringBytes(be.var["cmd"])+1

		console.log(be.var["cmd"])
		if( be.var["cmd"] == 'forward' ) {
  			be.part["FR"].setValue(100/100,true)
  			be.part["FL"].setValue(100/100,false)
  			be.part["BR"].setValue(100/100,true)
  			be.part["BL"].setValue(100/100,false)
		}
		if( be.var["cmd"] == 'backward' ) {
  			be.part["FR"].setValue(100/100,false)
  			be.part["FL"].setValue(100/100,true)
  			be.part["BR"].setValue(100/100,false)
  			be.part["BL"].setValue(100/100,true)
		}
		if( be.var["cmd"] == 'leftward' ) {
  			be.part["FR"].setValue(100/100,true)
  			be.part["FL"].setValue(100/100,true)
  			be.part["BR"].setValue(100/100,false)
  			be.part["BL"].setValue(100/100,false)
		}
		if( be.var["cmd"] == 'rightward' ) {
  			be.part["FR"].setValue(100/100,false)
  			be.part["FL"].setValue(100/100,false)
  			be.part["BR"].setValue(100/100,true)
  			be.part["BL"].setValue(100/100,true)
		}
		if( be.var["cmd"] == 'turnleft' ) {
  			be.part["FR"].setValue(100/100,true)
  			be.part["FL"].setValue(100/100,true)
  			be.part["BR"].setValue(100/100,true)
  			be.part["BL"].setValue(100/100,true)
		}
		if( be.var["cmd"] == 'turnright' ) {
  			be.part["FR"].setValue(100/100,false)
  			be.part["FL"].setValue(100/100,false)
  			be.part["BR"].setValue(100/100,false)
  			be.part["BL"].setValue(100/100,false)
		}
		if( be.var["cmd"] == 'roll-cw' ) {
  			be.part["FR"].setValue(100/100,true)
  			be.part["FL"].setValue(100/100,true)
  			be.part["BR"].setValue(20/100,false)
  			be.part["BL"].setValue(20/100,false)
		}
		if( be.var["cmd"] == 'roll-ccw' ) {
  			be.part["FR"].setValue(100/100,false)
  			be.part["FL"].setValue(100/100,false)
  			be.part["BR"].setValue(20/100,true)
  			be.part["BL"].setValue(20/100,true)
		}
		if( be.var["cmd"] == 'stop' ) {
  			be.part["FR"].setValue(0/100,true)
  			be.part["FL"].setValue(0/100,true)
  			be.part["BR"].setValue(0/100,true)
  			be.part["BL"].setValue(0/100,true)
		}
	})
	var _pkg_buff = _pkg_buff
	var _pkg_offset = 0

  	be.var["cmd"] = utils.unpackString(_pkg_buff, _pkg_offset)
  	_pkg_offset+= utils.stringBytes(be.var["cmd"])+1

	console.log(be.var["cmd"])
	if( be.var["cmd"] == 'forward' ) {
  		be.part["FR"].setValue(100/100,true)
  		be.part["FL"].setValue(100/100,false)
  		be.part["BR"].setValue(100/100,true)
  		be.part["BL"].setValue(100/100,false)
	}
	if( be.var["cmd"] == 'backward' ) {
  		be.part["FR"].setValue(100/100,false)
  		be.part["FL"].setValue(100/100,true)
  		be.part["BR"].setValue(100/100,false)
  		be.part["BL"].setValue(100/100,true)
	}
	if( be.var["cmd"] == 'leftward' ) {
  		be.part["FR"].setValue(100/100,true)
  		be.part["FL"].setValue(100/100,true)
  		be.part["BR"].setValue(100/100,false)
  		be.part["BL"].setValue(100/100,false)
	}
	if( be.var["cmd"] == 'rightward' ) {
  		be.part["FR"].setValue(100/100,false)
  		be.part["FL"].setValue(100/100,false)
  		be.part["BR"].setValue(100/100,true)
  		be.part["BL"].setValue(100/100,true)
	}
	if( be.var["cmd"] == 'turnleft' ) {
  		be.part["FR"].setValue(100/100,true)
  		be.part["FL"].setValue(100/100,true)
  		be.part["BR"].setValue(100/100,true)
  		be.part["BL"].setValue(100/100,true)
	}
	if( be.var["cmd"] == 'turnright' ) {
  		be.part["FR"].setValue(100/100,false)
  		be.part["FL"].setValue(100/100,false)
  		be.part["BR"].setValue(100/100,false)
  		be.part["BL"].setValue(100/100,false)
	}
	if( be.var["cmd"] == 'roll-cw' ) {
  		be.part["FR"].setValue(100/100,true)
  		be.part["FL"].setValue(100/100,true)
  		be.part["BR"].setValue(20/100,false)
  		be.part["BL"].setValue(20/100,false)
	}
	if( be.var["cmd"] == 'roll-ccw' ) {
  		be.part["FR"].setValue(100/100,false)
  		be.part["FL"].setValue(100/100,false)
  		be.part["BR"].setValue(20/100,true)
  		be.part["BL"].setValue(20/100,true)
	}
	if( be.var["cmd"] == 'stop' ) {
  		be.part["FR"].setValue(0/100,true)
  		be.part["FL"].setValue(0/100,true)
  		be.part["BR"].setValue(0/100,true)
  		be.part["BL"].setValue(0/100,true)
	}

}
