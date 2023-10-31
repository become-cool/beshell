const nativefs = globalThis.SJSJSBridge.fs
globalThis.SJSJSBridge.fs = null;

class Stat {
    constructor (stat) {
        this.stat = stat
    }

    isFile () {
        return (this.stat?.mode & nativefs.S_IFMT) === nativefs.S_IFREG
    }

    isDirectory() {
        return (this.stat?.mode & nativefs.S_IFMT) === nativefs.S_IFDIR
    }

    isCharacterDevice() {
        return (this.stat?.mode & nativefs.S_IFMT) === nativefs.S_IFCHR
    }

    isBlockDevice() {
        return (this.stat?.mode & nativefs.S_IFMT) === nativefs.S_IFBLK
    }

    isFIFO() {
        return (this.stat?.mode & nativefs.S_IFMT) === nativefs.S_IFIFO
    }

    isSocket() {
        return (this.stat?.mode & nativefs.S_IFMT) === nativefs.S_IFSOCK
    }

    isSymbolicLink() {
        return (this.stat?.mode & nativefs.S_IFMT) === nativefs.S_IFLNK
    }

    get dev () {
        return this.stat?.dev
    }

    get info () {
        return this.stat?.info
    }

    get mode () {
        return this.stat?.mode
    }

    get nlink () {
        return this.stat?.nlink
    }

    get uid () {
        return this.stat?.uid
    }

    get gid () {
        return this.stat?.gid
    }

    get rdev () {
        return this.stat?.rdev
    }

    get size () {
        return this.stat?.size
    }

    get blocks () {
        return this.stat?.blocks
    }

    get atime () {
        return this.stat?.atime
    }

    get mtime () {
        return this.stat?.mtime
    }

    get ctime () {
        return this.stat?.ctime
    }

    get size () {
        return this.stat?.size
    }
}

async function statAsync (path) {
    let stat
    try {
        stat = await nativefs.stat(path);
    } catch (e) {

    }
    return new Stat(stat)
}

function statSync (path) {
    const [stat, err] = nativefs.statSync(path);
    return new Stat(stat)
}

function realPathSync (path) {
    const [filename, err] = nativefs.realPathSync(path)
    if (err) throw err
    return filename
}

function realpath (path) {
    return nativefs.realPath(path)
}

function readFileSync (path, { encoding = 'utf-8' } = {}) {
    const data = nativefs.readFileSync(path, encoding)
    
    if (encoding !== 'utf-8') {
        return Buffer.from(data)
    }

    return data
}

async function readFile (path, { encoding = 'utf-8' } = {}) {
    const data = await nativefs.readFile(path, encoding)

    if (encoding !== 'utf-8') {
        return Buffer.from(data)
    }

    return data
}

async function writeFile (path, data) {
    data = Buffer.from(data)
    return await nativefs.writeFile(path, data.buffer)
}

function writeFileSync (path, data) {
    data = Buffer.from(data)
    return nativefs.writeFileSync(path, data.buffer)
}

async function unlink (path) {
    return await nativefs.unlink(path)
}

function unlinkSync (path) {
    return nativefs.unlinkSync(path)
}

async function mkdir (path) {
    return nativefs.mkdir (path)
}

function mkdirSync (path) {
    return nativefs.mkdirSync (path)
}

async function rmdir (path) {
    return nativefs.rmdir(path)
}

function rmdirSync (path) {
    return nativefs.rmdirSync (path)
}

module.exports = {
    statSync,
    stat: statAsync,
    realpath,
    realPathSync,
    readFileSync,
    readFile,
    writeFile,
    writeFileSync,
    unlink,
    unlinkSync,
    mkdirSync,
    mkdir,
    rmdir,
    rmdirSync
}