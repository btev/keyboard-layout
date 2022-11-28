# keyboard-layout
An attempt at finding the best keyboard layout


1200064  
p l f d m q a u y b  
n r s t g . o e i h  
v j c w z / , ; x k  
1200062  
p c l d m q a u y f  
n s r t k . o e i h  
b v w g j / , ; x z  
1200023  
f c l d k q a u y b  
s r n t p . o e i h  
v w m g j ; , z / x  

Up until now I've been using made-up values based on some ideals for the weights of the algorithm. Obviously it's hard to be confident that I have the best layout if the weights could be changed to whatever feels right.
So I created an in-terminal typing-test thing that collects your speed in bigrams and uses the average time as weights instead. There are like 900 different bi-grams and most of them you'll never type with qwerty, so I had to just randomly generate nonsense words. After an annoying amount of time typing, I ran the algorithm and got this...

q b o n h ; u a e c  
i t d m v w j . l ,  
k g y f p s r / z x  

Yeah really not what I was hoping for. Unfortunately because I'm used to typing on qwerty, all the weights are biased on those movements. Qwerty is known to be heavy on the top row, and because I sub-consiously learned to be quick with those keys it placed a lot of common letters on the upper rows.  
  
Another example is the letter 'j'. Weirdly, the j in the exact same spot, a supposed high-value location on the home-row. I assume this is because I almost never hit that key with Qwerty and therefore I'm pretty slow there. 
  
In conclusion, this probably isn't the way to find the optimal keyboard weights. It is just too biased to your layout and the layouts common moves. Even if I learned a layout like Colemak where the most common letters are closer to ideal positions, I would be biased to the manuevers on the Colemake Layout, probably missing a plethra of better layouts in the process.
