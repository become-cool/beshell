exports.appjs = 
`lv = require("lv")

// 该文件由 Workspace 自动生成，所有修改都可能在下次保存时被还原
module.exports = {
    var: {}, part: {}, ui: {} ,

    setup () {
        let screen = new lv.CleanObj()
        screen.fromJson(JSON.load(__dirname+"/ui.json"), this.ui)
        lv.loadScreen(screen)

%{codeSetup}
    } ,
    
    main () {
%{codeEvents}
%{codeMain}
    } ,
}
`


exports.indexjs = 
`const app = require("./app.js")
global.app = app

app.setup()

app.main()
`