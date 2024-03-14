import * as fs from 'fs'
JSON.loadSync = function(path, defaultValue) {
    try {
        return JSON.parse(fs.readFileSync(path).asString())
    }catch(e) {
        if(defaultValue===undefined) {
            throw e
        }
        return defaultValue
    }
}
JSON.saveSync = function(path, obj, indent) {
    fs.writeFileSync(path, JSON.stringify(obj, null, indent))
}