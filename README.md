# Data Communication Assignment 2563
## Frame :
### Fixed Size/ Stop and Wait ARQ/ Bit-Oriented<br />
1. **dataframe(16 bit):**<br />
-  flag{1} -seq{1}- control{2}  - data{8} - crc(x^3+x+1){3}  - flag{1}<br />
1.1) **flag{1}** -> 1<br />
1.2) **seq{1}** -> 0 and 1 (stop and wait)<br />
1.3) **control{2}** ->
  >- 00 -> DataFrame<br />
  > - 01 ->  ACK<br />
  > - 10 ->  U-Frame<br />

**data{8}**-><br />
  - 1). ข้อมูลสีสามมุม 3 เฟรม
          ->"0000"+{bit of color(xxxx)}
   - 2). ACK frame -> "00000000"
  -  3). ข้อมูล ความเข้มสี 20 จุด
          ->{bit of color 2^8}
 -  4). U-Frame
        >  -> "00000000" set/reset
          -> "00000001" get -45 degree
          -> "00000010" get 0 degree
          -> "00000011" get +45 degree
## 4-FSK Encoding Rule
- 00 -> 3 cycles/baud
- 01 -> 5 cycles/baud
- 10 -> 8 cycles/baud
- 11 -> 11 cycles/baud
