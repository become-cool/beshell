const fs = require('fs');
const path = require('path');

const jsDir = __dirname+'/../src/js' // 指定的目录

const files = fs.readdirSync(jsDir)

files.forEach(file => {
    if (path.extname(file) === '.js') {
        const jsFilePath = path.join(jsDir, file);
        const cArrayName = path.basename(file, '.js').replace(/\./g, '_');
        const cFilePath = path.join(jsDir, `${cArrayName}.c`);

        const data = fs.readFileSync(jsFilePath, 'utf8');
        fs.writeFileSync(cFilePath, js2c(data, `embeded_js_src_${cArrayName}`), 'utf8');

        console.log(`File ${cFilePath} has been generated`);
    }
});


function js2c (src, cArrayName) {
    let cArray = `static unsigned char ${cArrayName}[] = {\n`;

    for (let i = 0; i < src.length; i++) {
        if (i % 16 === 0 && i !== 0) {
            cArray += '\n';
        }
        const utf8Bytes = unescape(encodeURIComponent(src.charAt(i)))
            .split('')
            .map(char => char.charCodeAt(0).toString(16))
            .join('')
            .match(/.{1,2}/g)
            .map(byte => `0x${byte}`)
            .join(', ');
        cArray += `${utf8Bytes}, `;
    }

    cArray += "\n0x" + "\n".charCodeAt(0).toString(16) + ', 0x00\n};';

    return cArray;
}


// driver
function genJSDriver(name, namespace, jspath, cpppath) {

    const src = fs.readFileSync(jspath, 'utf8');


    let cppcode = `#include "${name}.hpp"

using namespace std;
using namespace be;

namespace ${namespace} {

${js2c(src, 'embeded_js_src_'+name)}

std::string ${name}::name = "${name}" ;
    void ${name}::exportDriver(JSContext * ctx) {
        JSEval(ctx, (const char *)embeded_js_src_${name}, -1, "embeded://${name}.js", JS_EVAL_TYPE_MODULE);
        JSEngine::fromJSContext(ctx)->dumpError() ;
    }
}
`
    fs.writeFileSync(cpppath, cppcode, 'utf8');
    console.log(`Driver class ${namespace}::${name} has been generated`);
}

const jsDriverDef = [
    ["PCA9557", "be::driver::io", __dirname+"/../src/js/driver/io/PCA9557.js", __dirname+"/../src/driver/io/PCA9557.cpp"],
    ["TM1652", "be::driver::io", __dirname+"/../src/js/driver/io/TM1652.js", __dirname+"/../src/driver/io/TM1652.cpp"]
]

for(let [name, namespace, jspath, cpppath] of jsDriverDef) {
    genJSDriver(name, namespace, jspath, cpppath)
}