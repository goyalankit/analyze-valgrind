Same compiler (Macpo) - default

```
"L,i,0,4" RW: i+0
"S,i,0,4"
"L,i,1,4" RW: i+4
"S,i,1,4"
"L,i,2,4" RW: i+8
"S,i,2,4"
"L,i,3,4" RW: i+12
"S,i,3,4"
"L,i,4,4" RW: i+16
"S,i,4,4"
"L,i,5,4" RW: i+20
"S,i,5,4" 
"L,i,6,4" RW: i+24
"S,i,6,4" 
"L,i,7,4" RW: i+28
"S,i,7,4" 
"L,i,8,4" RW: i+32
"S,i,8,4"
"L,i,9,4" RW: i+36
"S,i,9,4"
```

`icc -mno-sse` Without Vectorization

```
"M,i,0,4" RW: i+0
"M,i,1,4" RW: i+4
"M,i,2,4" RW: i+8
"M,i,3,4" RW: i+12
"M,i,4,4" RW: i+16
"M,i,5,4" RW: i+20
"M,i,6,4" RW: i+24
"M,i,7,4" RW: i+28
"M,i,8,4" RW: i+32
"M,i,9,4" RW: i+36
```

`icc` default with vectorization

```
"L,i,0,16" # 0 1 2 3
"L,i,0,16"
"L,i,0,16"
"L,i,0,16"
"L,i,0,16"
"L,i,4,16" # 4 5 6 7
"L,i,4,16"
"L,i,4,16"
"L,i,4,16"
"L,i,4,16"
"S,i,0,16" 
"S,i,0,16"
"S,i,0,16"
"S,i,0,16"
"S,i,0,16"
"S,i,4,16"
"S,i,4,16"
"S,i,4,16"
"S,i,4,16"
"S,i,4,16"
"M,i,8,4" # 8
"M,i,9,4" # 9
```

`gcc`

```
"L,i,0,4"
"S,i,0,4"
"L,i,1,4"
"S,i,1,4"
"L,i,2,4"
"S,i,2,4"
"L,i,3,4"
"S,i,3,4"
"L,i,4,4"
"S,i,4,4"
"L,i,5,4"
"S,i,5,4"
"L,i,6,4"
"S,i,6,4"
"L,i,7,4"
"S,i,7,4"
"L,i,8,4"
"S,i,8,4"
"L,i,9,4"
"S,i,9,4"
```
