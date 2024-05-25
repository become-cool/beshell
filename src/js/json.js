import('fs').then(fs=>{
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
}).catch(e=>{
    JSON.loadSync = function() {
        throw new Error('fs module is not available, maybe you forgot to call BeShell::use<FS>() in cpp code?')
    }
    JSON.saveSync = function() {
        throw new Error('fs module is not available, maybe you forgot to call BeShell::use<FS>() in cpp code?')
    }
})