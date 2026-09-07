// BeShell PC 冒烟脚本
console.log("[beshell-pc] main.js running")
console.log("[beshell-pc] platform =", process.platform)

setTimeout(()=>{
    console.log("[beshell-pc] setTimeout works")
}, 10)

Promise.resolve().then(()=>{
    console.log("[beshell-pc] promise works")
})
