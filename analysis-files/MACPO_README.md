
Instructions to get trace from macpo

```
source /work/01174/ashay/apps/macpo-setup.sh
macpo.sh --macpo:gen-trace=foo bar.c -o macpo
./macpo
macpo-analyze macpo.out
```
