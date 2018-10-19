# process-exists-windows-native-addon

**Node native addon for improving [`process-exists`](https://github.com/sindresorhus/process-exists)' performance on Windows.**

> ⚠️ WORK_IN_PROGRESS
>
> Currently, this repo only contains preliminary benchmarks.

## Benchmark

```
Benchmark does-exist.exe byId

bench  type result ms us    ns
-----  ---- ------ -- --    --
native byId true   0  17    17139
addon  byId true   0  139   139669
wrap   byId true   53 53824 53824902


Benchmark does-not-exist.exe byId

bench  type result ms us    ns
-----  ---- ------ -- --    --
native byId false  0  5     5105
addon  byId false  0  159   159726
wrap   byId false  52 52594 52594862


Benchmark does-exist.exe byName

bench  type   result ms us    ns
-----  ----   ------ -- --    --
native byName true   8  8888  8888522
addon  byName true   6  6781  6781446
wrap   byName true   57 57582 57582121


Benchmark does-not-exist.exe byName

bench  type   result ms us    ns
-----  ----   ------ -- --    --
native byName false  7  7041  7041822
addon  byName false  6  6987  6987486
wrap   byName false  58 58453 58453688
```

## Development

- [`install`](install.cmd)
- [`configure`](configure.cmd)
- [`format`](format.cmd)
- [`build`](build.cmd)
- [`bench`](bench.cmd)

## License

MIT
