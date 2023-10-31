const lv = require("lv")

class ToolBar extends lv.Obj {

    workspace = null
    viewTools = null

    constructor(workspace) {
        super(workspace)

        this.workspace = workspace

        this.fromJson({
            clear: true ,
            width: 20 ,
            height: "100%" ,
            style:{
                "border-width": 1 ,
                "border-color": lv.palette("blue") ,
                "bg-color": lv.palette("blue") ,
                "bg-opa": 128 ,
                "align": "left-mid" ,
            } ,
            children: [
                {
                    class: "Column" ,
                    width: 17 ,
                    height: -1 ,
                    align: "bottom-right" ,
                    style: {
                        "pad-row": 5 ,
                        "flex-cross-place": "center" ,
                    }, 
                    children: [
                        {
                            class: "Img" ,
                            src: '/lib/icon/16/plugins.png' ,
                            clicked() {
                                console.log("graph")
                                workspace.setActiveView(workspace.graph)
                            }
                        } ,
                        {
                            class: "Img" ,
                            src: '/lib/icon/16/ui.png' ,
                            clicked() {
                                console.log("ui")
                                workspace.setActiveView(workspace.ui)
                            }
                        } ,
                        {
                            class: "Img" ,
                            src: '/lib/icon/16/program.png' ,
                            clicked() {
                                console.log("program")
                                workspace.setActiveView(workspace.program)
                            }
                        } ,
                        {
                            class: "Label" ,
                            text: lv.symbol.settings ,
                            ref: "setting" ,
                            clicked: ()=>{
                                this.menu.popup()
                            }
                        } ,
                    ]
                } ,
            ]

        }, this)
        
        workspace.on("ws-active-view-changed", (view)=>{
            let tools = view.viewTools(this)
            if(this.viewTools == tools)
                return
            if(this.viewTools) {
                this.viewTools.hide()
            }
            if(tools) {
                tools.show()
            }
            this.viewTools = tools
        })

        this.menu = new lv.Menu(null, {
            items: [
                { value:"设置", font:"msyh", callback:()=>{
                }} ,
                { value:"保存", font:"msyh", callback:()=>{
                    workspace.loader.save()
                }} ,
                { value:"运行", font:"msyh", callback:()=>{
                    process.reset(3, workspace.model.folderPath)
                }} ,
                { value:"退出", font:"msyh", callback:()=>{
                    process.reboot()
                }} ,
            ]
        })
    }
}

module.exports = ToolBar