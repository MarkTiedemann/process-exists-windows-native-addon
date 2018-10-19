const path = require('path')
const child_process = require('child_process')

if (process.argv.length !== 3) {
  console.error('Usage: native.exe <pid|name>')
  process.exit(1)
}

let bin = path.join(__dirname, 'wrap.exe')

let name = process.argv[2]
let pid = parseInt(name)

~(async function() {
  if (Number.isNaN(pid)) {
    let start = process.hrtime()
    let result = await exec(name)
    let diff = process.hrtime(start)
    printResult('byName', result, diff)
  } else {
    let start = process.hrtime()
    let result = await exec(pid)
    let diff = process.hrtime(start)
    printResult('byId', result, diff)
  }
})()

function exec(nameOrPid) {
  return new Promise((resolve, reject) => {
    child_process.execFile(bin, [nameOrPid], (err, stdout, stderr) => {
      if (err || stderr) reject(stderr)
      else resolve(JSON.parse(stdout))
    })
  })
}

function printResult(type, result, diff) {
  let nanos = diff[0] * 1e9 + diff[1]
  let micros = Math.floor(nanos / 1e3)
  let millis = Math.floor(micros / 1e3)
  console.log(`wrap,${type},${result},${millis},${micros},${nanos}\n`)
}
