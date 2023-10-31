const lv = require('lv')
const appTpl = require("./app_tpl")

module.exports = class NewAppDialog extends lv.Obj {
    constructor(parent) {
        super(parent)

        this.fromJson({
            width: "100%" ,
            height: 220 ,
            center: true ,
            
            children:[{
                class: "Column" ,
                width: "100%" ,
                height: "100%" ,
                style: {
                    "pad-row": 12
                } ,
                children: [
                    {
                        class: "Label" ,
                        text: "创建应用" ,
                        font: "msyh" ,
                    } ,
                    {
                        class: "Row" ,
                        style: {
                            "flex-cross-place": "center" ,
                            "pad-column": 10 ,
                        } ,
                        children: [
                            {
                                class: "Label" ,
                                text: "App Name:" ,
                                font: "m12" ,
                                width: -1 ,
                            } ,
                            {
                                class: "TextArea" ,
                                ref: "appName" ,
                                text: "unname app" ,
                                font: "m12" ,
                                oneLine: true ,
                                grow: true ,
                                clicked (){
                                    lv.keyboard().popup(this)
                                }
                            } ,
                        ]
                    } ,
                    {
                        class: "Row" ,
                        grow: true ,
                        ref: "iconMenu" ,
                        style: {
                            "pad-column": 16 ,
                            "flex-cross-place": "center" ,
                        }
                    } ,
                    {
                        class: "Row" ,
                        style: {
                            "flex-main-place": "end" ,
                            "pad-column": 10 ,
                        } ,
                        children: [
                            {
                                class: lv.Checkbox ,
                                text: "进入工作台" ,
                                font: "msyh" , 
                                style: {
                                    "text-color": lv.rgb(160)
                                } ,
                                state: ["checked"] ,
                                ref: "startWorkspace" ,
                            } ,
                            {
                                class: lv.CleanObj ,
                                grow: true ,
                                height: 10 ,
                            } ,
                            {
                                class: "Btn" ,
                                font: "m12" ,
                                text: "Cancel" ,
                                clicked: ()=>{ this.cancel() }
                            } ,
                            {
                                class: "Btn" ,
                                font: "m12" ,
                                text: "OK" ,
                                clicked: ()=>{ this.ok() }
                            }
                        ]
                    }
                ]
            }]
        }, this)

        this.selectedIcon = null

        for(let filename of beapi.fs.readdirSync("/lib/icon/32")) {
            if(filename.substr(-4)!='.png') {
                continue
            }
            let img = new lv.Img(this.iconMenu)
            img.src = "/lib/icon/32/"+filename
            img.fromJson({
                src: img.src ,
                style: {
                    "pad-top": 2,
                    "pad-bottom": 2,
                    "pad-left": 2,
                    "pad-right": 2,
                    "radius": 3,
                    "border-width": 2,
                    "border-opa": 0 ,
                    "border-color": lv.palette("blue-grey")
                } ,
                clicked:(e, icon)=>{
                    this.setSelectedIcon(icon)
                } ,
            })
            if(!this.selectedIcon) {
                this.setSelectedIcon(img)
            }
        }
    }

    setSelectedIcon(icon){
        if(this.selectedIcon) {
            this.selectedIcon.setStyle("border-opa", 0)
        }
        this.selectedIcon = icon
        this.selectedIcon.setStyle("border-opa", 255)
    }

    start() {
        this.show()
    }

    ok() {
        lv.keyboard().hide()
        this.hide()

        let path = createApp(this.appName.text(), this.selectedIcon.src)
        if(!path){
            return
        }
        
        this.emit("new-app", path, this.startWorkspace.hasState("checked"))
        // lv.msg.sucess("suuccccc!")
    }

    cancel() {
        lv.keyboard().hide()
        this.hide()
    }

    
}

function makePkgName(parentFolder, title, uuid) {
    let folderName = title.replace(/\s/g, "_").replace(/[^\w\-_\.]/g, "")
    let path = parentFolder + '/' + folderName
    if( !beapi.fs.existsSync(path) ){
        return folderName
    }
    for(let len=3; len<12; len++) {
        let tmpname = folderName +'-'+uuid.substr(-len)
        if(!beapi.fs.existsSync(parentFolder + "/" + tmpname)) {
            return tmpname
        }
    }
    return null
}

function mkDir(path){
    if(!beapi.fs.mkdirSync(path)){
        throw {title:"无法创建目录", desc: path}
    }
}
function writeFile(path, data){
    if(!beapi.fs.writeFileSync(path, data)){
        throw {title:`无法写入文件, (size:${(data||"").length})`, desc: path}
    }
}

function createApp(title, iconpath) {
    let uuid = beapi.utils.genUUID()
    let folderName = makePkgName(process.env.HOME, title, uuid)
    // console.log(title, iconpath, uuid, folderName)
    if(!folderName) {
        lv.msg.error("无法为APP创建目录")
        return null
    }
    try{
        let folderPath = process.env.HOME + '/' + folderName

        mkDir(folderPath)

        let pkgjson = JSON.stringify({
            name: folderName ,
            version: '1.0.0',
            title ,
            uuid ,
            icon: iconpath ,
            main: 'index.js' ,
        },null,4)
        writeFile(folderPath+"/package.json", pkgjson)

        let wsjson = JSON.stringify({
            graph: {
                parts: {
                    "0000": {
                        "class": "BePad"
                    },
                }
            }
        },null,4)
        writeFile(folderPath+"/workspace.json", wsjson)
        writeFile(folderPath+"/ui.json", '[]')
        
        writeFile(folderPath+"/app.js", appTpl.appjs.replace(/%\{\w+\}/g, ''))
        writeFile(folderPath+"/index.js", appTpl.indexjs)

        return folderPath
    }catch(e){
        if(e.title||e.desc){
            lv.msg.error(e.title, e.desc)
        } else {
            console.log(e)
        }
    }
    return null
}