exports.start = function(folder) {
    let loader = new lv.Obj()
    loader.fromJson({
        children: [
            {
                class: lv.Column ,
                width: -1 ,
                height: -1 ,
                center: true ,
                style: {
                    "pad-row": 10 ,
                    "flex-cross-place": "center" ,
                    // "border-width": 1
                }, 
                ref: "column" ,
                children: [
                    {
                        class: lv.Label ,
                        text: "正在启动工作台" ,
                        font: "msyh" ,
                    } ,
                    
                    {
                        class: lv.Bar ,
                        width: 100 ,
                        ref: "bar" ,
                        // style: {
                        //     "border-width": 1
                        // }, 
                    }
                ]
            }
        ]
    },loader)

    lv.loadScreen(loader)
    prequire( ... require("./prequire"), function(loaded, total, path){
        console.log(loaded, total, path)
        loader.bar.setValue(loaded*100/total)
        if(loaded==total) {
            loader.del()
            require('./Workspace').singleton().start(folder)
        }
    } )
}