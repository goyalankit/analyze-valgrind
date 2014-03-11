\n--------------------\n

#### Valgrind trace analysis

[macpo] Analyzing logs created from the binary /work/0268 at Mon Mar 10 20:25:34 2014

[macpo] Analyzing records for latency.

[macpo] Reuse distances:
var: a: 0 (5010000 times) 26 (108150 times) 8 (108113 times).
var: b: 0 (6000000 times).
var: c: 0 (4000000 times).

[macpo] Cache conflicts:
var: a, conflict ratio: 0%.
var: b, conflict ratio: 0%.
var: c, conflict ratio: 0%.

[macpo] Analyzing records for stride values.
var: a: 0 (4010000 times) 127 (1009948 times) 1 (1000000 times).
var: b: 0 (4000000 times) 1 (1000000 times) 127 (999949 times).
var: c: 0 (3000000 times) 1 (990099 times) 127 (9900 times).

[macpo] Analyzing records for vector stride values.

---

#### Macpo benchmarks

[macpo] Analyzing logs created from the binary /work/02681/ankitg/workspace/analyze-valgrind/benchmarks/macpo at Mon Mar 10 20:36:21 2014

[macpo] Analyzing records for latency.

[macpo] Reuse distances:
var: c: 2 (11 times) inf. (2 times) 8 (2 times).
var: a: 2 (12 times) 8 (2 times) inf. (1 times).
var: b: 2 (11 times) inf. (2 times) 8 (2 times).

[macpo] Cache conflicts:
var: c, conflict ratio: 0%.
var: a, conflict ratio: 0%.
var: b, conflict ratio: 0%.

[macpo] Analyzing records for stride values.
var: c: 1 (14 times).
var: a: 0 (14 times).
var: b: 1 (14 times).

[macpo] Analyzing records for vector stride values.

