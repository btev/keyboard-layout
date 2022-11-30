#keyboard-layout  
An attempt at finding the best keyboard layout

# Evolution
    Through random mutation and fitness selection I can quicky find a local minimum



# Weights
## Singles
### Key Weights / Travel Distance
    My algorithm is weighted a bit differently from a lot of the popular layouts today, I don't think that minimum-travel-distance and the the keys position need to be weighed so heavily.
    To me avoiding the lower->upper row combos, same-finger-bigrams and redirects are much more important. When I pay attention to myself typing on Qwerty, the time-loss and
        discomfort to hit keys like 'q' or 'p' is barely there. Especially relative to same-finger-bigrams and the other metrics I mentioned earlier. Of course I would rather
        hit a key on the home-row with my pointer finger over some pinky-finger bottom row key, but if it increases the same-finger-bigram rate significantly, I'll choose the pinky.
    Anyways that's the reason I barely debuff the corner pinky spots and other hard to reach locations. 

    1.1, 1.07, 1.05, 1.05, 1.07,  1.2, 1.05, 1.05, 1.07,  1.1,
    1.05, 1.02,   1,    1,  1.1,  1.1,    1,    1, 1.02, 1.05,
    1.1,  1.1, 1.07, 1.05,  1.3,  1.1, 1.05, 1.07,  1.1,  1.1

    This isn't really a proper travel distance measurement, but I think it's fine for now. Maybe I'll implement a proper one in the future
## Capitalization
### General
        Well it is hitting two-keys, but the capilization is very easy to recognize and the shift key is set, so I wouldn't value it as high as a normal two-key combo like "fj" (valued at 2.0)
        Base : 1.5x

### Left-Pinky
        I use the left-pinky for shift so this debuff is based on that. zebra quesiton kddkdkA
        I actually don't think this is that bad, not much worse than base and definitely not close to bigrams like "yn" yet... future combos like "Aw" will be though.

        Upper : 1.7x
        Home  : 1.6x
        Lower : 1.75x
            
## Bigrams
    - Evalutate both characters and add them up
    - Add any modifiers from the two character perspective
### Same Finger
    If true, don't bother checking the other buffs

    Same-Key   : 1.4x
    Up-Or-Down : 2.2x
    Top-Bottom : 3x

### Capitals
    Things like "Aw" that aren't normally counted as same-finger-bigrams should be added here.
    That being said, I don't think it's as bad as a normal SFB.
    Might re-work this to also consider same-hand maneuvers after typing a capital with pinky, definitely can mess up your hand-position.

    Same-Row   : 1.1x  (1.3x -> 1.43x)    // A Normal SFB gets this low of a debuff because it's just hitting the same key-twice, no movement. This gets a low value because generally my index finger would already be in the correct position for key two.
    Up-Down    : 1.15x (1.36x -> 1.58x)    // I find this to not be too terrible for similar reasons to the first one, it's just a little more awkward.
    Top-Bottom : 1.8x  (1.41x -> 2.53x)    // This one is pretty close to normal Top-Bottom bigrams
    
### Single row jump (same hand)
    I initially thought the default key position weights aleady did this, but I've decided these should be added

    Home-Upper : 1.1x  (1.07x -> 1.17x)
    Upper-Home : 1.05x (1.07x -> 1.12x)
    Home       : 1.0x  (1x -> 1.0x)
    Lower-Home : 1.05x (1.1x -> 1.15x)
    Home-Lower : 1.1x  (1.1x -> 1.21x)
    
### Double row jump (same hand)
    Pointer: 
    Pointer is lower       : Good   1x
    Pointer-Higher-> pinky : Ok     1.25x
    Pointer is higher      : Trash  2x

    Middle:
    Middle is upper        : Good   1x
    Middle-Lower->Pinky    : Ok     1.5x
    Middle is lower        : Trash  2x

    Ring -> Pinky:
    Lower Pinky            : Ok     1.25x
    Lower Ring             : Trash  2x

### Rolls
    At this stage it's really just alternation vs. same-hand,  everything on the same hand is already a roll
    It's all summed up pretty well in the single/double row jumps, no need to add any buff/debuffs

## Trigrams
    - Evaluate both bigrams and add them together
    - Add any modifiers from the three character perspective
    - Multiply by 3/4 as we had added up four characters worth of value instead of three

### Same-Finger (The beginning and end of Trigram)
    Bigram values shifted

    Same-Key   : 1x
    Up-Or-Down : 1.4x
    Top-Bottom : 2.2x

### Capitals
    Things like "Aus"
    I don't know why but this one almost feels worse than the bigram version,
    like with the bigram version my brain is ready to hit the next key even though my hand it out of position.
    With this, my brain switches back to normal, but before I can re-center myself I'm thrown back into this weird hand position game
    Usually I end up using the pinky for both ends of the trigram, but I'm going to assume you could learn to use the other finger instead

    Same-Row   : 1x  (1.3x)
    Up-Down    : 1x  (1.36x)  
    Top-Bottom : 1.41x  (1.41x -> 2x)
    
### General
    Does consider inward rolls with "trash" combos in them, but they will be severely debuffed from the bigram calculation anyways
    Feels like Up-Or-Down same-finger is a similar debuff to a redirect.
    I keep going back and forth on outward-rolls, very difficult to pin down. Sometimes I feel like I can do them very well, other times it messes with my brain
    #Re-Evaluate Outward-Rollss

    Inside-Redirect  : 2x    (1x to 2x)     // Feels super awkward, messes with my brain
    Pinky-Outward    : 1.5x  (1x to 1.5x)   // Outward roll that used the pinky
    Center-Outward   : 1.3x  (1x to 1.3x)   // Outward roll with the pointer finger on the center column
    Redirect         : 1.2x  (1x to 1.2x)
    Roll-Alternate   : 0.8x  (1x to 0.8x)
    Full-Outward     : 0.8x  (1x to 0.8x)   // With neither of the mentioned above
    Full-Inward      : 0.65x (1x to 0.65x)
    // Full-Alternating : 1.0x the two bigrams should calulate this correctly

## Quadgrams
    - Evaluate both trigrams and add them together
    - Add any modifiers from the four character perspective
    - Multiply by 4/6 as we added up six characters instead of four

    ###Same-Finger (The beginning and end of Trigram)
    Trigam values shifted

    Same-Key   : 1x
    Up-Or-Down : 1x
    Top-Bottom : 1.4x

### Capitals
    Things like "Ques"
    Still not able to re-center hands unless it's same-row. But overall not that bad.

    Same-Key   : 1x
    Up-Or-Down : 1x
    Top-Bottom : 1.1x (1.41x -> 1.551x)
    
### General
    // Inside-Redirect  : 1.5x should already be calculated from trigrams
    // Redirect         : 1.2x should already be calculated from trigrams
    Full-Alternating : 1.0x
    Full-Outward     : 0.8x  (0.8x -> 0.64x)
    Roll-Alternate   : 0.85x  (0.725x -> 0.616x inward, 0.8x -> 0.68x outward) 
    Full-Inward      : 0.8x   (0.65x -> 0.52x)
        
## Overall
### Finger Usage / Hand Usage
    I think this is represented fairly well with the key weights, but I might as-well list some buffs
    The values are really small cause I'll be multiplying the entire fitness
    All values within 1%

    Pinky 8%     : 0.99x
    Ring  11%    : 0.99x
    Middle 16%   : 0.99x
    Pointer 15%  : 0.99x

    Hand 50% each: 0.99x

    Figured home-row usage being higher than the other rows was probably guarenteed, so I didn't include that

## Process
    I've been reading a ton, taking in everyone's keyboard ideals and opinions from layouts like
    
    Colemak, Halmak, Dvorak, Workman, Norman, Kaehi, Ornate, Asset, Capewell, ect ect

    The layout I've found to agree most with so far is the MTGAP layouts, been reading some old
        blogish/discussion posts that have been talking a lot about the ideas I've been thinking about myself.
        Here's a link to some cool info https://mathematicalmulticore.wordpress.com/2011/11/27/typing-data-preliminary-analysis/#comments
    
    A lot of the other layouts seem to highly focus on key-positions and distance traveled. Which is nice because it's a clean and easy to calculate statistic, but I just
        don't think it's #that#important.
        
        
    Probably the biggest thing though has just been me typing away in this document, trying different finger/key maneuvers and comparing there speed/comfort to others.
## Results so far
    First result with the new weights. I did skip out on a few metrics because they were annoying to implement, for example capitals haven't been implemented yet.
    3599605  
    d r t s c k n i e h  
    g l m f w , . a o y  
    x j v z q ; b u / p  
    This does actually look pretty good but I'm not sure why it put the period on the home row, especially under the pointer finger. Maybe it's because I'm using reddit data and the period key is often used in combination
        like "...", that in combination of debuffing same-key combos might have caused the fitness for this combo to be REALLY bad. I will run it again but loosen the debuff on same-key combos.


    3568801
    s d t r m ; / o u f
    g c p l w j y i e .
    z v k x q b n a , h

    I changed it so same-key hits don't give any debuff, but still the period is on the home-row. At least it's in a more feasable location. Even though periods are fairly common, there is just no way it should be on the home-row.
    

    3567017
    g s t h d ; x o e r
    l c w m f , . a i y
    q v p k z u n / j b

    I thought for sure this would do it, I removed any characters that appear more than twice in a row in succession. But still it persists, like why not just swap the '.' and 'n'?
    The letter 'n' appears like 4 times as much as the period, 


    3303743
    h r s t g ; n i e y
    l m c d w , . a o p
    q x v k z j b u / f

    Just added buffs for being close to an even finger/hand distribution amount.

    3356659
    . l s t c k h a e g
    ; r n w m , y i u p
    / v x z q b d o j f

    Added debuff for capitals near the left-pinky... clearly weighed a bit too much. At least the period is off the home row lol


# Other Methods/Ideas
## Weights based on typing data
    So far I've been using made-up values based on subjective ideals for the weights of the algorithm. Obviously it's hard to be confident that I have the best layout if the weights could be changed to whatever feels right.
    So I created an in-terminal typing-test thing that collects your speed in bigrams and uses your average time per bigram as weights instead. There are like 900 different bi-grams and most of them you'll never type with qwerty, so I had to just randomly generate nonsense words. After an annoying amount of time typing, I ran the algorithm and got this...

    q b o n h ; u a e c  
    i t d m v w j . l ,  
    k g y f p s r / z x  

    Yeah really not what I was hoping for. Unfortunately because I'm used to typing on qwerty, all the weights are biased on those movements. Qwerty is known to be heavy on the top row, and because I sub-consiously learned to be quick with those keys it placed a lot of common letters on the upper rows. Look at the j key for example, definitely shouldn't be in the exact same position as qwerty.

    This probably isn't the way to find the optimal keyboard weights. It is just too biased to your layout and the layouts common moves. Even if I learned a layout like Colemak where the most common letters are closer to ideal positions, I would be biased to the manuevers on the Colemake Layout, probably missing a plethra of better layouts in the process. The only way I could see this method being valuable would be if the data was collected from a ton of different people and layouts, with partnership with monkeytype or something.