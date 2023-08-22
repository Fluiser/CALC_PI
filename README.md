# CALC_PI
- Use boost/multiprecision.<br/>
If I can calc PI - I calc pi.<br/>
<img src="https://i.imgur.com/Naug4cx.png"><br/>
> **Compile**:<br/>

`g++ -DMAX_INDEX=(100 or another(для большого числа лучше начинать с 18000)) -DTHREADS_SIZE=TN main.cpp -o main.exe -std=c++11`<br/>
`MAX_INDEX` - Size of dec. number. (`boost::multiprecision::cpp_dec_float<MAX_INDEX>`)<br/>
`THREADS_SIZE` - Count use threads for calculating.
