let _scrSplash = null

function createSplash(disp) {

    _scrSplash = disp.activeScreen()
    _scrSplash.splash = true
    _scrSplash.setStyle("bg-color", beapi.lvgl.rgb(80))
    let logo = new beapi.lvgl.Label(_scrSplash)
    logo.setText("BECOME")
    logo.setStyle("text-color", beapi.lvgl.rgb(255))
    logo.setFont("m24")
    logo.center()

    let loading = new beapi.lvgl.Bar(_scrSplash)
    loading.setAlign("bottom-mid")
    loading.setY(-50)
    loading.setWidth(200)
    loading.setHeight(5)
    loading.setValue(0)
    // loading.addFlag("hidden")
    _scrSplash.loading = loading
}

async function load(lst) {
    let total = 0
    for(let [files,] of lst) {
        total+= files.length
    }
    
    let loaded = 0 

    for(let [files,req] of lst) {
        await prequire(files,req,function (l,t,p) {
            if(l==t) { return }
            _scrSplash && _scrSplash.loading.setValue((loaded++)*100/total)
        })
    }

    _scrSplash && _scrSplash.loading.setValue(100)
}

module.exports = function(scripts) {
    if(!be.dev?.disp?.length) {
        return
    }
    createSplash(be.dev.disp[0])
    return load([ require("lv/prequire"), ... scripts])
}