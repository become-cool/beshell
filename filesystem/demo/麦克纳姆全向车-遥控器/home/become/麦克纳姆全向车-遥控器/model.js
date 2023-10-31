
/**
 * 此文件由 BeConsole 自动生成，任何改动都会被 BeConsole 覆盖
 */
require("besdk")

// 在初始化 BeConsole "硬件" 视图中创建并配置的零件
exports.setup = function () {




	be.part["switch1"] = new (require('besdk/driver/switch.js')) ()

	be.part["switch2"] = new (require('besdk/driver/switch.js')) ()

	be.part["switch3"] = new (require('besdk/driver/switch.js')) ()

	be.part["switch4"] = new (require('besdk/driver/switch.js')) ()

	be.part["switch5"] = new (require('besdk/driver/switch.js')) ()

	be.part["switch6"] = new (require('besdk/driver/switch.js')) ()

	be.part["switch7"] = new (require('besdk/driver/switch.js')) ()

	be.part["switch8"] = new (require('besdk/driver/switch.js')) ()



	be.part["switch1"].begin( 18, 19 )

	be.part["switch2"].begin( 16, 17 )

	be.part["switch3"].begin( 26, 'ground' )

	be.part["switch4"].begin( 25, 27 )

	be.part["switch5"].begin( 21, 'vcc' )

	be.part["switch6"].begin( 12, 'vcc' )

	be.part["switch7"].begin( 32, 33 )

	be.part["switch8"].begin( 13, 15 )


}

// 根据 BeConsole "编程" 视图中的程序卡片生成的JS代码
exports.runCards = async function(){
	be.part["switch3"].on( "on", async ()=>{
		var _pkg = utils.pack(...[
  			('forward') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)


	})

	be.part["switch7"].on( "on", async ()=>{
		var _pkg = utils.pack(...[
  			('roll-cw') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)


	})

	be.part["switch8"].on( "on", async ()=>{
		var _pkg = utils.pack(...[
  			('roll-ccw') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)


	})

	be.part["switch1"].on( "on", async ()=>{
		var _pkg = utils.pack(...[
  			('leftward') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)


	})

	be.part["switch2"].on( "on", async ()=>{
		var _pkg = utils.pack(...[
  			('rightward') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)


	})

	be.part["switch5"].on( "on", async ()=>{
		var _pkg = utils.pack(...[
  			('turnleft') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)


	})

	be.part["switch6"].on( "on", async ()=>{
		var _pkg = utils.pack(...[
  			('turnright') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)


	})

	be.part["switch4"].on( "on", async ()=>{
		var _pkg = utils.pack(...[
  			('backward') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)


	})

	be.part["switch1"].on( "off", async ()=>{
		var _pkg = utils.pack(...[
  			('stop') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)

		var _pkg = utils.pack(...[
  			('stop') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)


	})

	be.part["switch2"].on( "off", async ()=>{
		var _pkg = utils.pack(...[
  			('stop') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)

		var _pkg = utils.pack(...[
  			('stop') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)


	})

	be.part["switch3"].on( "off", async ()=>{
		var _pkg = utils.pack(...[
  			('stop') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)

		var _pkg = utils.pack(...[
  			('stop') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)


	})

	be.part["switch4"].on( "off", async ()=>{
		var _pkg = utils.pack(...[
  			('stop') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)

		var _pkg = utils.pack(...[
  			('stop') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)


	})

	be.part["switch5"].on( "off", async ()=>{
		var _pkg = utils.pack(...[
  			('stop') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)

		var _pkg = utils.pack(...[
  			('stop') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)


	})

	be.part["switch6"].on( "off", async ()=>{
		var _pkg = utils.pack(...[
  			('stop') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)

		var _pkg = utils.pack(...[
  			('stop') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)


	})

	be.part["switch7"].on( "off", async ()=>{
		var _pkg = utils.pack(...[
  			('stop') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)

		var _pkg = utils.pack(...[
  			('stop') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)


	})

	be.part["switch8"].on( "off", async ()=>{
		var _pkg = utils.pack(...[
  			('stop') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)

		var _pkg = utils.pack(...[
  			('stop') ,
		])
		socks.udp.broadcase(null, 6666, _pkg)


	})

}
