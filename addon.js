const addon = require('bindings')('addon')

if (process.argv.length !== 3) {
  console.error('Usage: native.exe <pid|name>')
  process.exit(1)
}

let name = process.argv[2]
let pid = parseInt(name)

if (Number.isNaN(pid)) {
  let start = process.hrtime()
  let result = addon.byName(name)
  let diff = process.hrtime(start)
  printResult('byName', result, diff)
} else {
  let start = process.hrtime()
  let result = addon.byId(pid)
  let diff = process.hrtime(start)
  printResult('byId', result, diff)
}

function printResult(type, result, diff) {
  let nanos = diff[0] * 1e9 + diff[1]
  let micros = Math.floor(nanos / 1e3)
  let millis = Math.floor(micros / 1e3)
  console.log(`addon,${type},${result},${millis},${micros},${nanos}\n`)
}
