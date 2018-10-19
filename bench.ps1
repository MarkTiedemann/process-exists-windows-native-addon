$ErrorActionPreference = 'Stop'

$doesExistName = 'does-exist.exe'
Start-Process -FilePath $doesExistName -NoNewWindow
Start-Sleep -Milliseconds 100

$doesExistId = Get-Content 'bench.pid' -Raw
Remove-Item 'bench.pid'

$doesNotExistName = 'does-not-exist.exe'
$doestNotExistId = 99999

$header = 'bench', 'type', 'result', 'ms', 'us', 'ns'

function Benchmark($name, $type, $nameOrId) {
  Write-Host -Object "Benchmark $name $type" -ForegroundColor 'blue'
  $results = @()
  $results += .\native.exe $nameOrId | ConvertFrom-Csv -Header $header
  $results += node.exe addon.js $nameOrId | ConvertFrom-Csv -Header $header
  $results += node.exe wrap.js $nameOrId | ConvertFrom-Csv -Header $header
  $results | Format-Table
}

Benchmark $doesExistName 'byId' $doesExistId
Benchmark $doesNotExistName 'byId' $doestNotExistId

Benchmark $doesExistName 'byName' $doesExistName
Benchmark $doesNotExistName 'byName' $doesNotExistName

Stop-Process -Id $doesExistId
