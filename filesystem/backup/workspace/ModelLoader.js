const lv = require('lv')
const fs = beapi.fs

class Saver extends lv.CleanObj{
    workspace = null
    constructor(workspace) {
        super(lv.topLayer())
        this.workspace = workspace
        this.fromJson({
            visible: false ,
            center: true ,
            width: "100%" ,
            height: "100%" ,
            flag: ["clickable"] ,
            children: [
                {
                    class: lv.CleanObj ,
                    width: "100%" ,
                    height: "100%" ,
                    center: true ,
                    style: {
                        "bg-color": lv.palette("grey") ,
                        "bg-opa": 200 ,
                    } ,
                } ,
                {
                    width: "80%" ,
                    height: 120 ,
                    center: true ,
                    children: [
                        {
                            class: lv.Column ,
                            center: true ,
                            width: "100%" ,
                            height: -1 ,
                            style: {
                                "pad-row": 10
                            } ,
                            children: [
                                
                                {
                                    class: lv.Label ,
                                    text: "" ,
                                    width: "100%" ,
                                    height: -1 ,
                                    font: "msyh" ,
                                    ref: "title" ,
                                    style: {
                                        "text-align": "center"
                                    }
                                } ,
                                
                                {
                                    class: lv.Label ,
                                    width: "100%" ,
                                    height: -1 ,
                                    font: "m10" ,
                                    ref: "desc" ,
                                    style: {
                                        "text-color": lv.palette("grey")
                                    }
                                }
                            ]
                        }
                    ]
                }
            ]
        }, this)
    }

    save() {
        this.title.setText("正在保存APP ...")
        this.show()

        try{

            let folderPath = this.workspace.model.folderPath
            console.log("save app to folder", folderPath)

            let wsjsonPath = folderPath + '/workspace.json'
            this.desc.setText(wsjsonPath)
            let json = this.workspace.model.serialize()
            let data = JSON.stringify(json,null,4)
            fs.writeFileSync(wsjsonPath, data)

            let appjsPath = folderPath + '/app.js'
            this.desc.setText(appjsPath)
            let code = this.workspace.model.vm.generate()
            fs.writeFileSync(appjsPath, code)

            let uijsonPath = folderPath + '/ui.json'
            this.desc.setText(uijsonPath)
            let uijson = this.workspace.ui.serialize()
            fs.writeFileSync(uijsonPath, JSON.stringify(uijson,null,4))

            // setTimeout(() =>this.hide(), 1000)
            this.hide()
            
        }catch(e){

            this.title.setText("保存APP时遇到错误")
            console.error(e)
            console.error(e.stack)
        }
    }

    load(folderPath, model) {
        
        this.title.setText("正在加载APP ...")
        this.show()
        
        try{
            let json = JSON.load(folderPath + '/workspace.json')
            model.unserialize(json)
            // setTimeout(() =>this.hide(), 1000)

            try{
                json = JSON.load(folderPath + '/ui.json')
                this.workspace.ui.unserialize(json)
            }catch(e) {
                console.error(e)
                console.error(e.stack)
            }

            model.folderPath = folderPath

            this.hide()
        }catch(e){

            this.title.setText("加载APP时遇到错误")
            console.error(e)
            console.error(e.stack)
        }
        
    }

}

module.exports = Saver