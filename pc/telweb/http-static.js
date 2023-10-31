module.exports = function(server, uriPattern, rootdir){
    uriPattern = uriPattern.replace(/\/+$/, "")
    let func = function (req){
        let uri = req.getPath()
        let path = uri.substr(uriPattern.length)

        if(path[0]!='/')
            path = '/' + path
        path = rootdir + path

        if(!beapi.fs.existsSync(path)) {
            req.rspnStatus("404 Not Found")
            req.end()
        }

        if(beapi.fs.isDirSync(path)) {
            if(path[path.length-1]!='/')
                path+= '/'
            let indexpath = path+"index.html"
            if(beapi.fs.isFileSync(indexpath)) {
                req.rspnFile(indexpath)
                return
            }
            indexpath = path+"index.htm"
            if(beapi.fs.isFileSync(indexpath)) {
                req.rspnFile(indexpath)
                return
            }
        }
        
        req.rspnFile(path)
    }
    server.route(uriPattern, func)
    server.route(uriPattern+"/*", func)
}