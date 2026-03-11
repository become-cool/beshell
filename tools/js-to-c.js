const fs = require('fs')
const path = require('path')

function js2c (src, cArrayName) {
    let cArray = `const unsigned char ${cArrayName}[] = {\n`;

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

function genFile(jsFilePath, cFilePath) {
    const cArrayName = path.basename(jsFilePath, '.js').replace(/\./g, '_')
    const jsDir = path.dirname(jsFilePath)
    if(!cFilePath) {
        cFilePath = path.join(jsDir, `${cArrayName}.c`);
    }

    const data = fs.readFileSync(jsFilePath, 'utf8');
    fs.writeFileSync(cFilePath, js2c(data, `embeded_js_src_${cArrayName}`), 'utf8');

    console.log(`File ${cFilePath} has been generated`);
}

function genDir(jsDir) {

    const files = fs.readdirSync(jsDir)

    files.forEach(file => {
        if (path.extname(file) === '.js') {
            const jsFilePath = path.join(jsDir, file)
            genFile(jsFilePath)
        }
    });
}

function genDirToDist(srcDir, distDir) {
    // 确保输出目录存在
    if (!fs.existsSync(distDir)) {
        fs.mkdirSync(distDir, { recursive: true });
    }

    const files = fs.readdirSync(srcDir);

    files.forEach(file => {
        const srcFilePath = path.join(srcDir, file);
        const stat = fs.statSync(srcFilePath);

        if (stat.isDirectory()) {
            // 递归处理子目录
            const subDistDir = path.join(distDir, file);
            genDirToDist(srcFilePath, subDistDir);
        } else if (path.extname(file) === '.js') {
            // 生成对应的 .c 文件
            const cArrayName = path.basename(file, '.js').replace(/\./g, '_');
            const cFilePath = path.join(distDir, `${cArrayName}.c`);

            const data = fs.readFileSync(srcFilePath, 'utf8');
            fs.writeFileSync(cFilePath, js2c(data, `embeded_js_src_${cArrayName}`), 'utf8');

            console.log(`File ${cFilePath} has been generated`);
        }
    });
}

function genBeShellJS() {

    genDir(__dirname+'/../src/js')
    genDir(__dirname+'/../src/js/bt')

    const jsDriverDef = [
        ["PCA9557", "be::driver::io", __dirname+"/../src/js/driver/io/PCA9557.js", __dirname+"/../src/driver/io/PCA9557.cpp"],
        ["TM1652", "be::driver::io", __dirname+"/../src/js/driver/io/TM1652.js", __dirname+"/../src/driver/io/TM1652.cpp"]
    ]
    
    for(let [name, namespace, jspath, cpppath] of jsDriverDef) {
        genJSDriver(name, namespace, jspath, cpppath)
    }
}

; (function main() {

    if( process.argv.length<=2 ) {
        genBeShellJS()
        return
    }
    
    let srcPath = path.resolve(process.cwd(), process.argv[2])
    let outPath
    if(process.argv[3]) {
        outPath = path.resolve(process.cwd(), process.argv[3])
    }

    // 如果两个参数都是目录，则批量转换
    const srcStat = fs.existsSync(srcPath) ? fs.statSync(srcPath) : null;
    const outStat = outPath && fs.existsSync(outPath) ? fs.statSync(outPath) : null;

    if (srcStat && srcStat.isDirectory() && outPath && (!fs.existsSync(outPath) || outStat && outStat.isDirectory())) {
        // 两个目录模式：src 目录下的所有 .js 文件输出到 dist 目录
        genDirToDist(srcPath, outPath);
    } else {
        // 单文件模式
        genFile(srcPath, outPath);
    }
    
})()