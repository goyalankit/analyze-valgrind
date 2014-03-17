
**Valgrind trace macpo analyze**

```
[macpo] Analyzing logs created from the binary /work/0268 at Tue Mar 11 22:51:33 2014

[macpo] Analyzing records for latency.

[macpo] Reuse distances:
var: b: 1 (89 times) 0 (7 times) 2 (4 times).
var: a: 2 (5 times) inf. (2 times).
var: c: 1 (59 times) 3 (43 times) inf. (5 times).

[macpo] Cache conflicts:
var: b, conflict ratio: 0%.
var: a, conflict ratio: 0%.
var: c, conflict ratio: 0%.

[macpo] Analyzing records for stride values.
var: b: 0 (88 times) 127 (16 times) 3 (1 times).
var: a: 127 (6 times).
var: c: 127 (98 times) 89 (1 times) 90 (1 times).

[macpo] Analyzing records for vector stride values.
```

Macpo Only benchmarks

```
[macpo] Analyzing logs created from the binary /work/02681/ankitg/workspace/analyze-valgrind/analysis-files/new_valgrind/macpo at Tue Mar 11 22:56:03 2014

[macpo] Analyzing records for latency.

[macpo] Reuse distances:
var: b: 0 (6 times) inf. (2 times) 2 (2 times).
var: a: 2 (4 times) 0 (2 times).

[macpo] Cache conflicts:
var: b, conflict ratio: 0%.
var: a, conflict ratio: 0%.
var: c, conflict ratio: 0%.

[macpo] Analyzing records for stride values.
var: b: 127 (6 times) 0 (5 times).
var: a: 127 (5 times).

[macpo] Analyzing records for vector stride values.
```
