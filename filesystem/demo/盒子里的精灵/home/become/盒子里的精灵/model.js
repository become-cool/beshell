
/**
 * 此文件由 BeConsole 自动生成，任何改动都会被 BeConsole 覆盖
 */
require("besdk")

// 在初始化 BeConsole "硬件" 视图中创建并配置的零件
exports.setup = function () {




	be.part["servo1"] = new (require("besdk/driver/pwmservo")) (undefined, 360)

	be.part["servo2"] = new (require("besdk/driver/pwmservo")) (undefined, 295)

	be.part["switch1"] = new (require('besdk/driver/switch.js')) ()



	be.part["servo1"].begin(13, 360)

	be.part["servo2"].begin(15, 295)

	be.part["switch1"].begin( 25, 27 )


}

// 根据 BeConsole "编程" 视图中的程序卡片生成的JS代码
exports.runCards = async function(){
	be.part["switch1"].on( "off", async ()=>{
		be.part["servo2"].setValue(35)
		await sleep(500)
		be.part["servo1"].setValue(200)
		await sleep(600)
		be.part["servo1"].setValue(70)
		await sleep(500)
		be.part["servo2"].setValue(8)
		await sleep(500)

	})

}
