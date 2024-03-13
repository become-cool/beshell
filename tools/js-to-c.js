const fs = require('fs');
const path = require('path');

const jsDir = __dirname+'/../src/js'; // 指定的目录

const files = fs.readdirSync(jsDir);

files.forEach(file => {
    if (path.extname(file) === '.js') {
        const jsFilePath = path.join(jsDir, file);
        const cArrayName = path.basename(file, '.js').replace(/\./g, '_');
        const cFilePath = path.join(jsDir, `${cArrayName}.c`);

        const data = fs.readFileSync(jsFilePath, 'utf8');

        let cArray = `unsigned char embeded_js_src_${cArrayName}[] = {\n`;

        for (let i = 0; i < data.length; i++) {
            if (i % 16 === 0 && i !== 0) {
                cArray += '\n';
            }
            const utf8Bytes = unescape(encodeURIComponent(data.charAt(i)))
                .split('')
                .map(char => char.charCodeAt(0).toString(16))
                .join('')
                .match(/.{1,2}/g)
                .map(byte => `0x${byte}`)
                .join(', ');
            cArray += `${utf8Bytes}, `;
        }

        cArray += "\n0x" + "\n".charCodeAt(0).toString(16) + ', 0x00\n};';

        fs.writeFileSync(cFilePath, cArray, 'utf8');
        console.log(`File ${cFilePath} has been generated`);
    }
});